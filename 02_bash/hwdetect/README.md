# Script for monitoring hot plugged hardware

Currently, detects:
* USB to TTL convertors
* flash drives
* i2c devices

## Script is used external tools:
### lsblk

That tool is used for getting information about block devices that connected to the USB bus.
It is the part of `util-linux` package.

```bash
apt install util-linux
```

### i2cdetect

That tool is used for detecting I2C slave devices on the I2C bus, required `sudo` permission.
It is the part of `i2c-tools` package.

```bash
apt install i2c-tools
```
