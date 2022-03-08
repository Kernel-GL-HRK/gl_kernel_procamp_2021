#define pr_fmt(fmt) "%s: " fmt, KBUILD_MODNAME
#define MOD_AUTHOR "Artem Shyshko"
#define MOD_LICENSE "GPL"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#define GPIO_NUMBER(port, bit) (32 * (port) + (bit))
#define LED_RED GPIO_NUMBER(0, 17)

static int irq, dev_id = 1;
static int ledr_gpio = -1;
static bool irq_count = false;

module_param(irq, int, S_IRUGO);

static irqreturn_t timer_interrupt(int irq, void* dev_id)
{
	irq_count = !irq_count;
	gpio_set_value(ledr_gpio, irq_count);
	return IRQ_NONE;
}

static int led_gpio_init(int gpio, int *led_gpio)
{
	int res;

	res = gpio_direction_output(gpio, 0);
	if (res != 0)
		return res;

	*led_gpio = gpio;
	return 0;
}

static int proc_init(void)
{
	int res = 0;
	if (request_irq(irq, timer_interrupt, IRQF_SHARED, "interrupt", &dev_id)) {
		res = -1;
		pr_info("Error while loading\n");
	} else {
		pr_info("Module loaded, %d\n", irq);
		res = led_gpio_init(LED_RED, &ledr_gpio);
		if (res != 0)
			pr_err("Can't set GPIO%d for output\n", LED_RED);
	}
	return res;
}

static void proc_exit(void)
{
	synchronize_irq(irq);
	free_irq(irq, &dev_id);
	gpio_set_value(ledr_gpio, 0);
	pr_info("Module exited\n");
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_AUTHOR(MOD_AUTHOR);
MODULE_LICENSE(MOD_LICENSE);
