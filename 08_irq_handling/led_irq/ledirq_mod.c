/*
 *  Simple LED irq handling test module
 */

#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

MODULE_AUTHOR("Volodymyr Kniazkyi <kniazkyi@gmail.com>");
MODULE_DESCRIPTION("LED irq handling test module");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");

#define GPIO_NUMBER(port, bit) (32 * (port) + (bit))

/*
 *  https://linux-sunxi.org/Xunlong_Orange_Pi_Zero#LEDs
 *  Board config for OPI-Zero:
 *	LED GREEN (PL10): GPIO_11_10
 *	LED RED   (PA17): GPIO_0_17
 *	BUTTON    (PG7) : GPIO_6_7
 */

#define LED_GREEN GPIO_NUMBER(11, 10)
#define LED_RED GPIO_NUMBER(0, 17)
#define BUTTON GPIO_NUMBER(6, 7)

static int ledg_gpio;
static int ledr_gpio;
static int button_gpio;
static int sw_button_state;
static int button_irq;
static int dev_id;


static int led_gpio_init(int gpio, int *led_gpio)
{
	int res;

	res = gpio_direction_output(gpio, 0);
	if (res != 0)
		return res;

	*led_gpio = gpio;
	return 0;
}

static int button_gpio_init(int gpio)
{
	int res;

	res = gpio_request(gpio, "user_button");
	if (res != 0)
		return res;

	res = gpio_direction_input(gpio);
	if (res != 0) {
		gpio_free(gpio);
		return res;
	}

	button_gpio = gpio;
	sw_button_state = 0;
	pr_info("Init GPIO%d OK\n", button_gpio);
	return 0;
}

static void button_gpio_deinit(void)
{
	if (button_gpio >= 0) {
		gpio_free(button_gpio);
		pr_info("Deinit GPIO%d\n", button_gpio);
	}
}

static irqreturn_t button_handler(int irq, void *devid)
{
	gpio_set_value(ledg_gpio, sw_button_state);
	gpio_set_value(ledr_gpio, !sw_button_state);
	sw_button_state = !sw_button_state;
	return IRQ_NONE;
}

/*
 *  Module entry/exit points
 */

static int __init gpio_irq_init(void)
{
	int res;

	pr_info("GPIO Init\n");

	dev_id = 0;

	res = button_gpio_init(BUTTON);
	if (res != 0) {
		pr_err("Can't set GPIO%d for button\n", BUTTON);
		return res;
	}

	res = led_gpio_init(LED_GREEN, &ledg_gpio);
	if (res != 0) {
		pr_err("Can't set GPIO%d for output\n", LED_GREEN);
		button_gpio_deinit();
		return res;
	}
	pr_info("Set GPIO%d for output\n", ledg_gpio);

	gpio_set_value(ledg_gpio, 0);

	res = led_gpio_init(LED_RED, &ledr_gpio);
	if (res != 0) {
		pr_err("Can't set GPIO%d for output\n", LED_RED);
		button_gpio_deinit();
		return res;
	}
	pr_info("Set GPIO%d for output\n", ledr_gpio);

	gpio_set_value(ledr_gpio, 0);

	button_irq = gpio_to_irq(BUTTON);
	if (button_irq < 0) {
		pr_err("Can't find GPIO%d IRQ\n", BUTTON);
		button_gpio_deinit();
	}

	res = request_irq(button_irq, button_handler, IRQF_SHARED, "user_button_irq", &dev_id);
	if (res < 0) {
		pr_err("Request for IRQ %d failed\n", button_irq);
		button_gpio_deinit();
	}
	return 0;
}

static void __exit gpio_irq_exit(void)
{
	free_irq(button_irq, &dev_id);
	gpio_set_value(ledg_gpio, 0);
	gpio_set_value(ledr_gpio, 0);
	button_gpio_deinit();
}

module_init(gpio_irq_init);
module_exit(gpio_irq_exit);
