#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/barrel_os.kernel isodir/boot/barrel_os.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "Barrel OS" {
	multiboot /boot/barrel_os.kernel
}
EOF
grub-mkrescue -o barrel_os.iso isodir
