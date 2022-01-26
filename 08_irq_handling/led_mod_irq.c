#define pr_fmt(fmt) KBUILD_MODNAME ": %s: " fmt, __func__

#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/ktime.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/ktime.h>

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

#define GREEN_LED_TOGGLE_TIMEOUT 400
#define RED_LED_TOGGLE_TIMEOUT 50

struct irq_module_data {
	int ledg_gpio;
	int ledr_gpio;
	int button_gpio;
	int button_state;
	int button_cnt;
	int ledr_gpio_state;
	struct delayed_work ledg_check_work;
	struct delayed_work ledr_toggle_work;
	spinlock_t lock;
};

struct irq_module_data *module_data = NULL;

static void check_green_delayed_handler(struct work_struct *work)
{
	pr_info("Delayed work handler\n");

	struct irq_module_data *user_data = container_of(
		work, struct irq_module_data, ledg_check_work.work);

	spin_lock(&user_data->lock);
	int button_gpio_state = gpio_get_value(user_data->button_gpio);
	int prev_gpio_value = user_data->button_state;

	user_data->button_state = button_gpio_state;
	spin_unlock(&user_data->lock);

	if (button_gpio_state == prev_gpio_value) {
		pr_info("Delayed work handler:Disabled green led\n");
		gpio_set_value(user_data->ledg_gpio, 0);
	} else {
		pr_info("Delayed work handler:Enabled green led\n");
		gpio_set_value(user_data->ledg_gpio, 1);
	}
}

static void toggle_red_delayed_handler(struct work_struct *work)
{
	struct irq_module_data *user_data = container_of(
		work, struct irq_module_data, ledr_toggle_work.work);
	spin_lock(&user_data->lock);
	gpio_set_value(user_data->ledr_gpio, !user_data->ledr_gpio_state);
	user_data->ledr_gpio_state = !user_data->ledr_gpio_state;
	spin_unlock(&user_data->lock);
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

static int button_gpio_init(int gpio)
{
	int res;

	res = gpio_request(gpio, "Onboard user button");
	if (res != 0)
		return res;

	res = gpio_direction_input(gpio);
	if (res != 0)
		goto err_input;

	module_data->button_gpio = gpio;
	pr_info("Init GPIO%d OK\n", module_data->button_gpio);
	module_data->button_state = gpio_get_value(module_data->button_gpio);
	pr_info("Got button value: %d\n", module_data->button_state);
	module_data->button_cnt = 0;

	return 0;

err_input:
	gpio_free(gpio);
	return res;
}

static irqreturn_t button_module_irq_handler(int irq, void *user_data)
{
	struct irq_module_data *irq_user_data =
		(struct irq_module_data *)user_data;
	unsigned long flags;
	uint8_t button_value;

	spin_lock_irqsave(&irq_user_data->lock, flags);

	button_value = gpio_get_value(irq_user_data->button_gpio);
	pr_info("Inside interrupt handler, button value:%d, prev_value:%d\n",
		button_value, irq_user_data->button_state);
	if (module_data->button_state == button_value)
		goto out;

	if (button_value == 0) {
		pr_info("Canceled delayed work because of the button_value==0\n");
		cancel_delayed_work(&irq_user_data->ledg_check_work);
		cancel_delayed_work(&irq_user_data->ledr_toggle_work);

		gpio_set_value(irq_user_data->ledr_gpio, 1);
		irq_user_data->ledr_gpio_state = 1;
		gpio_set_value(irq_user_data->ledg_gpio, 1);

		schedule_delayed_work(&irq_user_data->ledr_toggle_work,
				      msecs_to_jiffies(RED_LED_TOGGLE_TIMEOUT));

		module_data->button_state = button_value;
	} else {
		pr_info("Scheduled delayed work\n");

		module_data->button_state = button_value;

		gpio_set_value(irq_user_data->ledr_gpio, 1);
		irq_user_data->ledr_gpio_state = 1;
		schedule_delayed_work(
			&irq_user_data->ledg_check_work,
			msecs_to_jiffies(GREEN_LED_TOGGLE_TIMEOUT));
		schedule_delayed_work(&irq_user_data->ledr_toggle_work,
				      msecs_to_jiffies(RED_LED_TOGGLE_TIMEOUT));
	}
out:
	spin_unlock_irqrestore(&irq_user_data->lock, flags);
	return IRQ_HANDLED;
}

static void button_gpio_deinit(struct irq_module_data *module_data_arg)
{
	if (module_data_arg->button_gpio >= 0) {
		gpio_free(module_data_arg->button_gpio);
		pr_info("Deinit GPIO%d\n", module_data_arg->button_gpio);
	}
}

static void gpio_setup_irq(void)
{
	int irq;
	int rc;

	irq = gpio_to_irq(BUTTON);
	rc = request_irq(irq, button_module_irq_handler,
			 IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
			 "button_module_irq_handler", module_data);
}

static void gpio_deinit_irq(void)
{
	int irq = gpio_to_irq(BUTTON);

	free_irq(irq, module_data);
}

/* Module entry/exit points */
static int __init gpio_irq_init(void)
{
	module_data = kzalloc(sizeof(struct irq_module_data), GFP_KERNEL);

	if (!module_data)
		return -ENOMEM;

	module_data->button_cnt = -1;
	module_data->button_state = -1;

	int res;

	pr_info("GPIO Init\n");

	res = button_gpio_init(BUTTON);
	if (res != 0) {
		pr_err("Can't set GPIO%d for button\n", BUTTON);
		kfree(module_data);
		return res;
	}

	res = led_gpio_init(LED_GREEN, &module_data->ledg_gpio);
	if (res != 0) {
		pr_err("Can't set GPIO%d for output\n", LED_GREEN);
		goto err_led;
	}

	gpio_set_value(module_data->ledg_gpio, 0);

	res = led_gpio_init(LED_RED, &module_data->ledr_gpio);
	if (res != 0) {
		pr_err("Can't set GPIO%d for output\n", LED_RED);
		goto err_led;
	}
	gpio_set_value(module_data->ledr_gpio, 1);
	module_data->ledr_gpio_state = 1;

	INIT_DELAYED_WORK(&module_data->ledg_check_work,
			  check_green_delayed_handler);
	INIT_DELAYED_WORK(&module_data->ledr_toggle_work,
			  toggle_red_delayed_handler);
	spin_lock_init(&module_data->lock);

	gpio_setup_irq();
	return 0;

err_led:
	button_gpio_deinit(module_data);
	kfree(module_data);
	return res;
}

static void __exit gpio_irq_exit(void)
{
	gpio_deinit_irq();
	cancel_delayed_work_sync(&module_data->ledg_check_work);
	cancel_delayed_work_sync(&module_data->ledr_toggle_work);

	gpio_set_value(module_data->ledg_gpio, 1);
	gpio_set_value(module_data->ledr_gpio, 0);

	button_gpio_deinit(module_data);

	kfree(module_data);
}

module_init(gpio_irq_init);
module_exit(gpio_irq_exit);

MODULE_AUTHOR("Valentyn Korniienko valentyn.korniienko1@nure.ua>");
MODULE_DESCRIPTION("LED Test");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");