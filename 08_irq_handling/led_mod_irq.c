#define pr_fmt(fmt) KBUILD_MODNAME ": %s: " fmt, __func__

#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/ktime.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>



#define GPIO_NUMBER(port, bit) (32 * (port) + (bit))

/*	https://linux-sunxi.org/Xunlong_Orange_Pi_PC#LEDs
 *  Board config for OPI-PC:
 *	LED GREEN (PL10): GPIO_11_10
 *	LED RED   (PA15): GPIO_0_15  
 *	BUTTON    (PG7) : GPIO_6_7
 *
 *  https://linux-sunxi.org/Xunlong_Orange_Pi_Zero#LEDs
 *  Board config for OPI-Zero:
 *	LED GREEN (PL10): GPIO_11_10 
 *	LED RED   (PA17): GPIO_0_17 
 *	BUTTON    (PA6) : GPIO_0_6
 *  
 */

#ifdef ORANGE_PI_PC
	#error "Implement the necessary port mappings"
#endif
#ifdef ORANGE_PI_ZERO
	#define LED_GREEN GPIO_NUMBER(11, 10)
	#define LED_RED GPIO_NUMBER(0, 17)
	#define BUTTON GPIO_NUMBER(0, 6)
#endif
//#define TIMER_ENABLE 1

static int ledg_gpio = -1;
static int ledr_gpio = -1;
static int button_gpio = -1;
static int button_state = -1;
static int button_cnt = -1;


// https://elixir.bootlin.com/linux/latest/source/drivers/iio/humidity/dht11.c#L310
// https://www.kernel.org/doc/Documentation/gpio/board.txt
// https://elixir.bootlin.com/linux/latest/source/drivers/input/touchscreen/ts4800-ts.c#L211

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

	res = gpio_request(gpio, "Onboard user button");
	if (res != 0)
		return res;

	res = gpio_direction_input(gpio);
	if (res != 0)
		goto err_input;

	button_gpio = gpio;
	pr_info("Init GPIO%d OK\n", button_gpio);
	button_state = gpio_get_value(button_gpio);
	pr_info("Got button value: %d\n",button_state);
	button_cnt = 0;

	return 0;

err_input:
	gpio_free(gpio);
	return res;
}

static irqreturn_t button_module_irq_handler(int irq, void* user_data)
{
    pr_info("Got interrupt. Jiffies: %d,!\n",jiffies);
    return IRQ_HANDLED;
}

static void button_gpio_deinit(void)
{
	if (button_gpio >= 0) {
		gpio_free(button_gpio);
		pr_info("Deinit GPIO%d\n", button_gpio);
	}
}

static void gpio_setup_irq(void)
{
    int irq;
    int rc;

    irq = gpio_to_irq(BUTTON);
    rc = request_irq(irq,button_module_irq_handler, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,"button_module_irq_handler",NULL);
}

static void gpio_deinit_irq(void)
{
    int irq = gpio_to_irq(BUTTON);
    free_irq(irq,NULL);
}

/* Module entry/exit points */
static int __init gpio_irq_init(void)
{
	int res;
	pr_info("GPIO Init\n");

	res = button_gpio_init(BUTTON);
	if (res != 0) {
		pr_err("Can't set GPIO%d for button\n", BUTTON);
		return res;
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

    gpio_setup_irq();
	return 0;

err_led:
	button_gpio_deinit();
	return res;
}

static void __exit gpio_irq_exit(void)
{
    gpio_deinit_irq();
	gpio_set_value(ledg_gpio, 1);
	gpio_set_value(ledr_gpio, 0);
	button_gpio_deinit();
}


module_init(gpio_irq_init);
module_exit(gpio_irq_exit);

MODULE_AUTHOR("Valentyn Korniienko valentyn.korniienko1@nure.ua>");
MODULE_DESCRIPTION("LED Test");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");