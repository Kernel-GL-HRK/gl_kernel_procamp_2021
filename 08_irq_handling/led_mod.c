#define pr_fmt(fmt) KBUILD_MODNAME ": %s: " fmt, __func__

#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/ktime.h>
#include <linux/interrupt.h>

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
 *	BUTTON    (PG7) : GPIO_6_7
 *  
 */

#define LED_GREEN GPIO_NUMBER(11, 10)
#define LED_RED GPIO_NUMBER(0, 15)
#define BUTTON GPIO_NUMBER(6, 7)

//#define TIMER_ENABLE 1

static int ledg_gpio = -1;
static int ledr_gpio = -1;
static int button_gpio = -1;
static int button_state = -1;
static int button_cnt = -1;

#ifdef TIMER_ENABLE
static ktime_t timer_period;
struct hrtimer button_timer;

static enum hrtimer_restart timer_callback(struct hrtimer *timer)
{
	int cur_button_state;

	cur_button_state = gpio_get_value(button_gpio);
	button_cnt = (cur_button_state == button_state) ? (button_cnt + 1) : 0;
	button_state = cur_button_state;
	gpio_set_value(ledr_gpio, ((button_cnt == 20) ? 1 : 0));
	if (button_cnt >= 20)
		gpio_set_value(ledg_gpio, !button_state);
	hrtimer_forward(timer, timer->base->get_time(), timer_period);
	return HRTIMER_RESTART;  //restart timer
}
#endif

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
	button_cnt = 0;

	return 0;

err_input:
	gpio_free(gpio);
	return res;
}

static void button_gpio_deinit(void)
{
	if (button_gpio >= 0) {
		gpio_free(button_gpio);
		pr_info("Deinit GPIO%d\n", button_gpio);
	}
}

/* Module entry/exit points */
static int __init gpio_poll_init(void)
{
	int res;
	pr_info("GPIO Init\n");

	res = button_gpio_init(BUTTON);
	if (res != 0) {
		pr_err("Can't set GPIO%d for button\n", BUTTON);
		return res;
	}
#ifdef TIMER_ENABLE
	timer_period = ktime_set(0, 1000000);      /*1 msec*/
	hrtimer_init(&button_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	hrtimer_start(&button_timer, timer_period, HRTIMER_MODE_REL);
	button_timer.function = timer_callback;
#endif
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

static void __exit gpio_poll_exit(void)
{
	gpio_set_value(ledg_gpio, 0);
	gpio_set_value(ledr_gpio, 0);
	button_gpio_deinit();
#ifdef TIMER_ENABLE
	hrtimer_cancel(&button_timer);
#endif
}

module_init(gpio_poll_init);
module_exit(gpio_poll_exit);

MODULE_AUTHOR("Oleksandr Posukhov oleksandr.posukhov@gmail.com>");
MODULE_DESCRIPTION("LED Test");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");