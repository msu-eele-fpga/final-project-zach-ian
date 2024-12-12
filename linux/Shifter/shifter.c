#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/io.h>
#include <linux/mutex.h>
#include <linux/miscdevice.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/kstrtox.h>

// Register Offsets
#define PATTERN_OFFSET 0x0
#define SPAN 16

// Device struct
struct shifter_dev {
    void __iomem *base_addr;
    void __iomem *pattern;
    struct miscdevice miscdev;
    struct mutex lock;
};

// Show function for the pattern register
static ssize_t pattern_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    u32 pattern;
    struct shifter_dev *priv = dev_get_drvdata(dev);
    pattern = ioread32(priv->pattern);
    return scnprintf(buf, PAGE_SIZE, "%u\n", pattern);
}

// Store function for the pattern register
static ssize_t pattern_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
    u32 pattern;
    int ret;
    struct shifter_dev *priv = dev_get_drvdata(dev);

    ret = kstrtouint(buf, 0, &pattern);
    if (ret < 0)
        return ret;

    iowrite32(pattern, priv->pattern);
    return size;
}

// Define sysfs attributes
static DEVICE_ATTR_RW(pattern);

// Attribute group
static struct attribute *shifter_attrs[] = {
    &dev_attr_pattern.attr,
    NULL,
};
ATTRIBUTE_GROUPS(shifter);

// Read method for the char device
static ssize_t shifter_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
    size_t ret;
    u32 val;
    struct shifter_dev *priv = container_of(file->private_data, struct shifter_dev, miscdev);

    if (*offset < 0 || *offset >= SPAN || (*offset % 0x4) != 0)
        return -EINVAL;

    val = ioread32(priv->base_addr + *offset);
    ret = copy_to_user(buf, &val, sizeof(val));
    if (ret == sizeof(val))
        return -EFAULT;

    *offset += sizeof(val);
    return sizeof(val);
}

// Write method for the char device
static ssize_t shifter_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
    size_t ret;
    u32 val;
    struct shifter_dev *priv = container_of(file->private_data, struct shifter_dev, miscdev);

    if (*offset < 0 || *offset >= SPAN || (*offset % 0x4) != 0)
        return -EINVAL;

    mutex_lock(&priv->lock);
    ret = copy_from_user(&val, buf, sizeof(val));
    if (ret != 0) {
        mutex_unlock(&priv->lock);
        return -EFAULT;
    }

    iowrite32(val, priv->base_addr + *offset);
    *offset += sizeof(val);
    mutex_unlock(&priv->lock);
    return sizeof(val);
}

// File operations
static const struct file_operations shifter_fops = {
    .owner = THIS_MODULE,
    .read = shifter_read,
    .write = shifter_write,
    .llseek = default_llseek,
};

// Probe function
static int shifter_probe(struct platform_device *pdev)
{
    struct shifter_dev *priv;
    int ret;

    priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
    if (!priv)
        return -ENOMEM;

    priv->base_addr = devm_platform_ioremap_resource(pdev, 0);
    if (IS_ERR(priv->base_addr))
        return PTR_ERR(priv->base_addr);

    priv->pattern = priv->base_addr + PATTERN_OFFSET;
    iowrite32(0, priv->pattern);

    mutex_init(&priv->lock);

    priv->miscdev.minor = MISC_DYNAMIC_MINOR;
    priv->miscdev.name = "shifter";
    priv->miscdev.fops = &shifter_fops;
    priv->miscdev.parent = &pdev->dev;

    ret = misc_register(&priv->miscdev);
    if (ret)
        return ret;

    platform_set_drvdata(pdev, priv);
    dev_info(&pdev->dev, "shifter driver probed\n");
    return 0;
}

// Remove function
static int shifter_remove(struct platform_device *pdev)
{
    struct shifter_dev *priv = platform_get_drvdata(pdev);

    iowrite32(0, priv->pattern);
    misc_deregister(&priv->miscdev);
    dev_info(&pdev->dev, "shifter driver removed\n");
    return 0;
}

// Device tree match table
static const struct of_device_id shifter_of_match[] = {
    { .compatible = "Ian,shifter" },
    {}
};
MODULE_DEVICE_TABLE(of, shifter_of_match);

// Platform driver
static struct platform_driver shifter_driver = {
    .probe = shifter_probe,
    .remove = shifter_remove,
    .driver = {
        .name = "shifter",
        .of_match_table = shifter_of_match,
        .dev_groups = shifter_groups,
    },
};

module_platform_driver(shifter_driver);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Zach Guentherman");
MODULE_DESCRIPTION("Shifter device driver");
