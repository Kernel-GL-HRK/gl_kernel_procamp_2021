#ifndef DEV_H_
#define DEV_H_

enum e_led_state
{
	e_led_state_off = 0,
	e_led_state_on,
};

enum e_led_type
{
	e_led_type_green = 0,
	e_led_type_red,
};

struct led_control
{
	enum e_led_type e_type;
	enum e_led_state e_state;
};

#define CHRDEV_IOCTRL_MAGIC 's'
/** Set LED state */
#define CHRDEV_IOCTRL_SET_VAL _IOW(CHRDEV_IOCTRL_MAGIC, 1, const struct led_control *)
/** Get LED state */
#define CHRDEV_IOCTRL_GET_VAL _IOR(CHRDEV_IOCTRL_MAGIC, 2, struct led_control *)
#define CHRDEV_PATH "/dev/chrdev0"

static inline int chrdev_led_cmd_is_ok(const struct led_control *pled_cmd)
{
	return (pled_cmd->e_type == e_led_type_green || pled_cmd->e_type == e_led_type_red)
		&& (pled_cmd->e_state == e_led_state_off || pled_cmd->e_state == e_led_state_on);
}

#endif // DEV_H_
