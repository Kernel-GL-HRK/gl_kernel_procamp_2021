### Steps for the successfull buildroot launch

* download and install buildroot
* for out-of-tree build create the separate directory. Add it to the envvars of cthe console session:

`export QEMU_BUILDROOT_IMAGE_BUILD=${PWD}`

* In the buildroot directory execute the following:

`make qemu_x86_64_defconfig O=${QEMU_BUILDROOT_IMAGE_BUILD}`

The build artifacts will appear in the specified directory.

For enabling SSH support it's necessary to specify the corresponding package, such as dropbear.
Navigate to the build directory with:

`cd {QEMU_BUILDROOT_IMAGE_BUILD}`
and launch the configuration of the image:

`make menuconfig`

Check the option: `Target Packages-> Networking tools->dropbear`

* Start build with:
    `make -j4`

Where `4` is the number of a host CPU cores, so it can be changed on demand.

After the build completion the system image will appear in the specifie directory. Also, the script for launching QEMU will be generated. Open it with a text editor and modify it for enable forwaring TCP connections.
```sh
#!/bin/sh
(
BINARIES_DIR="/home/valenti/Development/Embedded/exercise8/buildroot_qemu_x64_image/images"
cd ${BINARIES_DIR}

if [ "${1}" = "serial-only" ]; then
    EXTRA_ARGS='-nographic'
else
    EXTRA_ARGS='-serial stdio'
fi

export PATH="/home/valenti/Development/Embedded/exercise8/buildroot_qemu_x64_image/host/bin:${PATH}"
exec qemu-system-x86_64 -M pc -kernel bzImage -drive file=rootfs.ext2,if=virtio,format=raw -append "rootwait root=/dev/vda console=tty1 console=ttyS0"  -net nic,model=virtio -net user,hostfwd=tcp::8022-:22  ${EXTRA_ARGS}
)
```
The modified line is:
```sh
net user,hostfwd=tcp::8022-:22
```

Launch QEMU by the script execution and wait a little.
In the launched image console it's necessary to add a user, because by default the ssh connections are forbidden for `root`.

So, login to the emulator as a root and execute the following for adding the user:
```sh
mkdir -p /home/user
adduser user
passwd user *********
chown user:user /home/user
```
After this, you are able to connect via SSH to the running image by the following:

```sh
ssh -p 8022 user@localhost
```
A file can be copied to the running VM by the following script:
```sh
scp -P 8022 sum_module.ko user@localhost:~/task03_module
```

Note, that ssh uses __-p__ for port specifying and scp uses __-P__ for this.


