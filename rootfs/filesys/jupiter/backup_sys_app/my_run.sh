#!/bin/sh
echo "in run.sh"

# ����(��S99finish.sh�й��أ��˴����ٹ���)
#echo "mounting our ubifs...."
#sh /etc/rcS.d/mount_ubifs.sh

sync
sleep 1

# ����IP
setip_fromeeprom

# ���(�ں˽�����led���������˴������ɫled)
echo 0 > /sys/class/leds/led_red/brightness

# ��omapϵ�е�tty���ӳɴ�ͳ��tty�豸����
ln -s /dev/ttyO1 /dev/ttyS1
ln -s /dev/ttyO3 /dev/ttyS3
ln -s /dev/ttyO4 /dev/ttyS4

# �Ѳɼ�ͼ��������ӵ�homeĿ¼������������в�����
ln -sf /opt/ipnc/ipc_arch  /home/root/ipc_arch

#����FPGA
# ľ��û��FPGA��
#cd /opt/ipnc
#./load_fpga fpga.xsvf

#����ͼ��ɼ����Գ��򣬺�̨����
#cd /home/root/ipc_arch/bin
#./ipc_client.out &


# �����ػ�����
cd /opt/ipnc
./deamon &
