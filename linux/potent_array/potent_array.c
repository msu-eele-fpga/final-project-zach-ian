#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/io.h>
#include <linux/miscdevice.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <linux/kstrtox.h>
#include <linux/uaccess.h>

#define POTENT_OFFSET 0x4
#define STEP_OFFSET 0x0
#define SPAN 16

/**
 * struct potent_array_dev - Private device structure
 * @base_addr: Pointer to the device's base memory address
 * @potent: Pointer to the POTENT register
 * @step: Pointer to the STEP register
 * @miscdev: Miscdevice structure for character device interface
 * @lock: Mutex to synchronize access
 */
struct potent_array_dev {
    void __iomem *base_addr;
    void __iomem *potent;
    void __iomem *step;
    struct miscdevice miscdev;
    struct mutex lock;
};

/* Show and store functions for potent */
static ssize_t potent_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    u32 potent;
    struct potent_array_dev *priv = dev_get_drvdata(dev);
    potent = ioread32(priv->potent);
    return scnprintf(buf, PAGE_SIZE, "%u\n", potent);
}

static ssize_t potent_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
    u32 potent;
    int ret;
    struct potent_array_dev *priv = dev_get_drvdata(dev);

    ret = kstrtouint(buf, 0, &potent);
    if (ret)
        return ret;

    iowrite32(potent, priv->potent);
    return size;
}

/* Show and store functions for step */
static ssize_t step_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    u32 step;
    struct potent_array_dev *priv = dev_get_drvdata(dev);
    step = ioread32(priv->step);
    return scnprintf(buf, PAGE_SIZE, "%u\n", step);
}

static ssize_t step_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
    u32 step;
    int ret;
    struct potent_array_dev *priv = dev_get_drvdata(dev);

    ret = kstrtouint(buf, 0, &step);
    if (ret)
        return ret;

    iowrite32(step, priv->step);
    return size;
}

/* Define sysfs attributes */
static DEVICE_ATTR_RW(potent);
static DEVICE_ATTR_RW(step);

/* Attribute group for sysfs */
static struct attribute *potent_array_attrs[] = {
    &dev_attr_potent.attr,
    &dev_attr_step.attr,
    NULL,
};
ATTRIBUTE_GROUPS(potent_array);

/* Character device read function */
static ssize_t potent_array_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
    struct potent_array_dev *priv = container_of(file->private_data, struct potent_array_dev, miscdev);
    u32 value;
    size_t ret;

    if (*offset < 0 || *offset >= SPAN || (*offset % 4) != 0)
        return -EINVAL;

    value = ioread32(priv->base_addr + *offset);
    ret = copy_to_user(buf, &value, sizeof(value));
    if (ret)
        return -EFAULT;

    *offset += sizeof(value);
    return sizeof(value);
}

/* Character device write function */
static ssize_t potent_array_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
    struct potent_array_dev *priv = container_of(file->private_data, struct potent_array_dev, miscdev);
    u32 value;
    size_t ret;

    if (*offset < 0 || *offset >= SPAN || (*offset % 4) != 0)
        return -EINVAL;

    ret = copy_from_user(&value, buf, sizeof(value));
    if (ret)
        return -EFAULT;

    mutex_lock(&priv->lock);
    iowrite32(value, priv->base_addr + *offset);
    mutex_unlock(&priv->lock);

    *offset += sizeof(value);
    return sizeof(value);
}

/* File operations for character device */
static const struct file_operations potent_array_fops = {
    .owner = THIS_MODULE,
    .read = potent_array_read,
    .write = potent_array_write,
    .llseek = default_llseek,
};

/* Probe function */
static int potent_array_probe(struct platform_device *pdev)
{
    struct potent_array_dev *priv;
    int ret;

    priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
    if (!priv)
        return -ENOMEM;

    priv->base_addr = devm_platform_ioremap_resource(pdev, 0);
    if (IS_ERR(priv->base_addr))
        return PTR_ERR(priv->base_addr);

    priv->potent = priv->base_addr + POTENT_OFFSET;
    priv->step = priv->base_addr + STEP_OFFSET;

    mutex_init(&priv->lock);

    priv->miscdev.minor = MISC_DYNAMIC_MINOR;
    priv->miscdev.name = "potent_array";
    priv->miscdev.fops = &potent_array_fops;
    priv->miscdev.parent = &pdev->dev;

    ret = misc_register(&priv->miscdev);
    if (ret)
        return ret;

    platform_set_drvdata(pdev, priv);
    return 0;
}

/* Remove function */
static int potent_array_remove(struct platform_device *pdev)
{
    struct potent_array_dev *priv = platform_get_drvdata(pdev);
    misc_deregister(&priv->miscdev);
    return 0;
}

/* Match table for device tree */
static const struct of_device_id potent_array_of_match[] = {
    { .compatible = "Zach,potent_array" },
    { }
};
MODULE_DEVICE_TABLE(of, potent_array_of_match);

/* Platform driver structure */
static struct platform_driver potent_array_driver = {
    .probe = potent_array_probe,
    .remove = potent_array_remove,
    .driver = {
        .name = "potent_array",
        .of_match_table = potent_array_of_match,
        .dev_groups = potent_array_groups,
    },
};

module_platform_driver(potent_array_driver);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Ian Crittenden");
MODULE_DESCRIPTION("Potent Array Driver");
