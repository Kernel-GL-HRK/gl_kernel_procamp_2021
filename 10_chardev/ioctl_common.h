#ifndef IOCTL_COMMON_H
#define IOCTL_COMMON_H

typedef enum led_clr
{
    RED,
    GREEN
} led_color_t;

typedef enum led_st
{
    LED_OFF,
    LED_ON
} led_state_t;

typedef struct leds_cont
{
    led_color_t led_color;
    led_state_t led_state;
}leds_cont_t;

#define WR_VALUE _IOW('a', 'b', leds_cont_t *)

#endif /*IOCTL_COMMON_H */
