#!/bin/sh
echo "in run.sh"

# 1. ����
#echo "mounting our ubifs...."
#sh /etc/rcS.d/mount_ubifs.sh

sync
sleep 1

# 2. ����IP
setip_fromeeprom

# 3. ���(�ں˽�����led���������˴������ɫled)
echo 0 > /sys/class/leds/led_red/brightness

# 4. ��rs485�豸����ΪttyS1���Լ���ˮ�Ǵ���
ln -s /dev/ttyO1 /dev/ttyS1

# 5. web�������
ln -sf /opt/ipnc/web_publish/* /var/www/

#��6. ���ܵ�tmpĿ¼
./DecryptFile.sh

chmod +x /tmp/*

# 7. �����еĳ�����ӿ�ִ������
#chmod +x * -R

# 8. ����web������
./boa &


# 9. ������������
# ������������������������
# ��Ӧ�ÿ���������Ա��������ӣ�лл������
# ������������������������

ulimit -c unlimited
echo "/home/root/core" > /proc/sys/kernel/core_pattern

# ����fpga
./load_fpga fpga.xsvf

# load cmemk
insmod /lib/modules/cmemk.ko phys_start=0x88000000 phys_end=0x8e000000, pools=1x100663296

# �ػ�����
./deamon
