// SPDX-License-Identifier: GPL
/*
 * Copyright (c) 2021 Vasyl Yosypenko
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": %s: " fmt, __func__

#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/ktime.h>
#include <linux/interrupt.h>

#define GPIO_NUMBER(port, bit) (32 * (port) + (bit))


/*  https://linux-sunxi.org/Xunlong_Orange_Pi_Zero#LEDs
 *  Board config for OPI-Zero:
 *  LED GREEN (PL10): GPIO_11_10
 *  LED RED   (PA17): GPIO_0_17
 *  BUTTON    (PG7) : GPIO_6_7
 *
 */

#define LED_GREEN GPIO_NUMBER(11, 10)
#define LED_RED GPIO_NUMBER(0, 17)
#define BUTTON GPIO_NUMBER(6, 7)

static int ledg_gpio = -1;
static int ledr_gpio = -1;
static int button_gpio = -1;
static int button_state = -1;
static int button_cnt = -1;

static int irq;
static int dev_id = 0;

typedef enum {
    ACTIVE,
    NOT_ACTIVE
}
led_latch_state_t;

static led_latch_state_t led_latch = ACTIVE;

static irqreturn_t my_gpio_interrupt(int irq, void * devid)
{
   /*
    * One button press:
    * enable green LED, disable red LED
    * press button one more time:
    * disable green LED, enable red LED
    *
    */

    if (ACTIVE == led_latch)
    {
        gpio_set_value(ledg_gpio, 1U);
        gpio_set_value(ledr_gpio, 0U);
        led_latch = NOT_ACTIVE;
    }
    else
    {
        gpio_set_value(ledg_gpio, 0U);
        gpio_set_value(ledr_gpio, 1U);
        led_latch = ACTIVE;
    }

    pr_info("my_gpio_interrupt\n");
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

    irq = gpio_to_irq(BUTTON);
    pr_info("Button irq: %d\n", irq);


    res = request_irq(irq, my_gpio_interrupt, IRQF_TRIGGER_FALLING, "my_gpio_interrupt", &dev_id);
    if (res != 0)
    {
        pr_err("Can't request interrupt %d\n", irq);
        gpio_free(button_gpio);
        return -1;
    }

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

    synchronize_irq(irq);
    free_irq(irq, &dev_id);
}

module_init(gpio_poll_init);
module_exit(gpio_poll_exit);

MODULE_AUTHOR("Vasyl Yosypenko");
MODULE_DESCRIPTION("LED Test with Interrupts");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
