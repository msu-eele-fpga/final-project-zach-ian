#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/io.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <kstrtox.h>

#define RED_DUTY_OFFSET 0x0
#define GREEN_DUTY_OFFSET 0x4
#define BLUE_DUTY_OFFSET 0x8
#define PERIOD_OFFSET 0xc

#define SPAN 16

