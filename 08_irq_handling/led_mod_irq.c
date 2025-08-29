// SPDX-License-Identifier: GPL-2.0

#define pr_fmt(fmt) KBUILD_MODNAME ": %s: " fmt, __func__

#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/ktime.h>
#include <linux/interrupt.h>

#define GPIO_NUMBER(port, bit) (32 * (port) + (bit))

/*  
 * https://linux-sunxi.org/Xunlong_Orange_Pi_Zero#LEDs
 *  Board config for OPI-Zero:
 *	LED GREEN (PL10): GPIO_11_10 
 *	LED RED   (PA17): GPIO_0_17 
 *	BUTTON    (PG7) : GPIO_6_7
 */

#define LED_GREEN GPIO_NUMBER(11, 10)
#define LED_RED GPIO_NUMBER(0, 17)
#define BUTTON GPIO_NUMBER(6, 7)

static int ledg_gpio = -1;
static int ledr_gpio = -1;
static int button_gpio = -1;
static int button_irq = -1;
static int button_state = -1;
static int dev_id;

static irqreturn_t button_irq_handler(int irq, void *dev_id)
{
	gpio_set_value(ledg_gpio, button_state);
	gpio_set_value(ledr_gpio, button_state);
	button_state = !button_state;

	return IRQ_HANDLED;
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

static void button_gpio_deinit(void)
{
	if (button_gpio >= 0) {
		gpio_free(button_gpio);
		pr_info("[LED_IRQ_MOD] Deinit GPIO%d\n", button_gpio);
	}
}

static int button_gpio_init(int gpio)
{
	int res;

	res = gpio_request(gpio, "Onboard user button");
	if (res != 0)
		return res;
	
	button_gpio = gpio;

	res = gpio_direction_input(gpio);
	if (res != 0)
		return res;

	pr_info("Init GPIO%d OK\n", button_gpio);

	button_irq = gpio_to_irq(BUTTON);
	if (button_irq < 0) {
		pr_err("Can't find GPIO%d IRQ\n", BUTTON);
		return button_irq;
	}

	res = request_irq(button_irq, button_irq_handler, IRQF_SHARED, "my_button_irq", &dev_id);
	if (res < 0) {
		pr_err("Can't set IRQ %d for button\n", button_irq);
		return res;
	}

	button_state = 0;

	return 0;
}

/* Module entry/exit points */
static int __init led_mod_init(void)
{
	int res;
	pr_info("GPIO Init\n");

	res = button_gpio_init(BUTTON);
	if (res != 0) {
		pr_err("Can't set GPIO%d for button\n", BUTTON);
		goto err_led;
	}

	res = led_gpio_init(LED_GREEN, &ledg_gpio);
	if (res != 0) {
		pr_err("Can't set GPIO%d for output\n", LED_GREEN);
		goto err_led;
	}

	gpio_set_value(ledg_gpio, 0);

	res = led_gpio_init(LED_RED, &ledr_gpio);
	if (res != 0) {
		pr_err("Can't set GPIO%d for output\n", LED_RED);
		goto err_led;
	}
	gpio_set_value(ledr_gpio, 1);

	return 0;

err_led:
	button_gpio_deinit();
	return res;
}

static void __exit led_mod_exit(void)
{
	free_irq(button_irq, &dev_id);
	gpio_set_value(ledg_gpio, 0);
	gpio_set_value(ledr_gpio, 0);
	button_gpio_deinit();
}

module_init(led_mod_init);
module_exit(led_mod_exit);

MODULE_AUTHOR("Sergey Dubyna <serj.dub1na@gmail.com>");
MODULE_DESCRIPTION("LED IRQ Test");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
