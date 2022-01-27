all: led_ioctl

led_ioctl: led_ioctl.c
	g++ -o $@ $< -L.-std=c++11

clean:
	@rm -vf led_ioctl

clear: clean
	@rm -vf led_ioctl
