#!/bin/sh
# ������PCI����(������200��500��)

insmod /lib/modules/slw-pci.master.ko
rm -f /dev/swpci
mknod /dev/swpci c `awk "\\$2==\"swpci\" {print \\$1}" /proc/devices` 0
