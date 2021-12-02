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