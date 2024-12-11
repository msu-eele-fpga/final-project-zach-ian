#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/io.h>
#include <linux/mutex.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kstrtox.h>
#include <linux/sysfs.h>

#define RED_DUTY_OFFSET 0x0
#define GREEN_DUTY_OFFSET 0x4
#define BLUE_DUTY_OFFSET 0x8
#define PERIOD_OFFSET 0xc
#define SPAN 16

/**
 * struct rgbled_dev - Private rgbled device struct.
 * @base_addr: Pointer to the component's base address
 * @miscdev: miscdevice used to create a character device
 * @lock: mutex used to prevent concurrent writes to memory
 */
struct rgbled_dev {
    void __iomem *base_addr;
    struct miscdevice miscdev;
    struct mutex lock;
};

static ssize_t red_duty_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct rgbled_dev *priv = dev_get_drvdata(dev);
    u32 red_duty = ioread32(priv->base_addr + RED_DUTY_OFFSET);
    return scnprintf(buf, PAGE_SIZE, "%u\n", red_duty);
}

static ssize_t red_duty_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
    struct rgbled_dev *priv = dev_get_drvdata(dev);
    u32 red_duty;
    int ret = kstrtou32(buf, 0, &red_duty);
    if (ret < 0)
        return ret;
    iowrite32(red_duty, priv->base_addr + RED_DUTY_OFFSET);
    return size;
}

static ssize_t green_duty_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct rgbled_dev *priv = dev_get_drvdata(dev);
    u32 green_duty = ioread32(priv->base_addr + GREEN_DUTY_OFFSET);
    return scnprintf(buf, PAGE_SIZE, "%u\n", green_duty);
}

static ssize_t green_duty_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
    struct rgbled_dev *priv = dev_get_drvdata(dev);
    u32 green_duty;
    int ret = kstrtou32(buf, 0, &green_duty);
    if (ret < 0)
        return ret;
    iowrite32(green_duty, priv->base_addr + GREEN_DUTY_OFFSET);
    return size;
}

static ssize_t blue_duty_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct rgbled_dev *priv = dev_get_drvdata(dev);
    u32 blue_duty = ioread32(priv->base_addr + BLUE_DUTY_OFFSET);
    return scnprintf(buf, PAGE_SIZE, "%u\n", blue_duty);
}

static ssize_t blue_duty_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
    struct rgbled_dev *priv = dev_get_drvdata(dev);
    u32 blue_duty;
    int ret = kstrtou32(buf, 0, &blue_duty);
    if (ret < 0)
        return ret;
    iowrite32(blue_duty, priv->base_addr + BLUE_DUTY_OFFSET);
    return size;
}

static ssize_t period_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct rgbled_dev *priv = dev_get_drvdata(dev);
    u32 period = ioread32(priv->base_addr + PERIOD_OFFSET);
    return scnprintf(buf, PAGE_SIZE, "%u\n", period);
}

static ssize_t period_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
    struct rgbled_dev *priv = dev_get_drvdata(dev);
    u32 period;
    int ret = kstrtou32(buf, 0, &period);
    if (ret < 0)
        return ret;
    iowrite32(period, priv->base_addr + PERIOD_OFFSET);
    return size;
}

// Define sysfs attributes
static DEVICE_ATTR_RW(red_duty);
static DEVICE_ATTR_RW(green_duty);
static DEVICE_ATTR_RW(blue_duty);
static DEVICE_ATTR_RW(period);

static struct attribute *rgbled_attrs[] = {
    &dev_attr_red_duty.attr,
    &dev_attr_green_duty.attr,
    &dev_attr_blue_duty.attr,
    &dev_attr_period.attr,
    NULL,
};

ATTRIBUTE_GROUPS(rgbled);

static int rgbled_probe(struct platform_device *pdev)
{
    struct rgbled_dev *priv;
    priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
    if (!priv)
        return -ENOMEM;

    priv->base_addr = devm_platform_ioremap_resource(pdev, 0);
    if (IS_ERR(priv->base_addr))
        return PTR_ERR(priv->base_addr);

    mutex_init(&priv->lock);

    priv->miscdev.minor = MISC_DYNAMIC_MINOR;
    priv->miscdev.name = "rgbled";
    priv->miscdev.fops = NULL;
    priv->miscdev.parent = &pdev->dev;

    platform_set_drvdata(pdev, priv);
    return 0;
}

static int rgbled_remove(struct platform_device *pdev)
{
    struct rgbled_dev *priv = platform_get_drvdata(pdev);
    mutex_destroy(&priv->lock);
    return 0;
}

static const struct of_device_id rgbled_of_match[] = {
    { .compatible = "Ian,rgbled", },
    { }
};

MODULE_DEVICE_TABLE(of, rgbled_of_match);

static struct platform_driver rgbled_driver = {
    .probe = rgbled_probe,
    .remove = rgbled_remove,
    .driver = {
        .name = "rgbled",
        .of_match_table = rgbled_of_match,
        .dev_groups = rgbled_groups,
    },
};

module_platform_driver(rgbled_driver);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Ian Crittenden");
MODULE_DESCRIPTION("RGB LED Driver");
