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

# ��rs485�豸����ΪttyS1���Լ���ˮ�Ǵ���
ln -s /dev/ttyO1 /dev/ttyS1

# �Ѳɼ�ͼ��������ӵ�homeĿ¼������������в�����
ln -sf /opt/ipnc/ipc_arch  /home/root/ipc_arch

#����FPGA
cd /opt/ipnc
#./load_fpga fpga.xsvf
if [ -e /mnt/nand/realimagefpga185 ] ; then
	echo "185 realimagefpga mode!"
	./load_fpga fpga_185.xsvf
elif [ -e /mnt/nand/realimagefpga178 ] ; then
	echo "178 realimagefpga mode!"
	./load_fpga fpga_178.xsvf
elif [ -e /mnt/nand/realimagefpga249 ] ; then
	echo "249 realimagefpga mode!"
	./load_fpga fpga_249.xsvf	
else
	echo "fakeiamgefpga mode!"
	./load_fpga fpga.xsvf
fi

#����ͼ��ɼ����Գ��򣬺�̨����
#cd /home/root/ipc_arch/bin
#./ipc_client.out &


# �����ػ�����
cd /opt/ipnc
./deamon &
