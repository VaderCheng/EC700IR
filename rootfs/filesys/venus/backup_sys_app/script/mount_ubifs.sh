#!/bin/sh
#
# ʹ�ã����ر���Ӧ�ò������DATA����
# ע���˴������ѹ滮�õ�flash����������

# backup app
ubiattach /dev/ubi_ctrl -m 5 -O 2048 -d 1
ret=$?
if [ $ret -eq 0 ]; then
    sync
    sleep 1
    mount -r -o chk_data_crc -t ubifs /dev/ubi1_0 /opt/ipnc
fi

sleep 1
# ����data����������ʹ�ó������
smart_mount /dev/mtd10 /mnt/nand 3 data 2048

sync
