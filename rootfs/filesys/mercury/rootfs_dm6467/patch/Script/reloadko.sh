#!/bin/sh

## file: /etc/reloadko.sh
## auth: panxw
## disc:
## ���¼�������
## ����������ƽ̨������һ���200W��500W������CPU�͵���
## 
## 2012-02-15 lijj��
## ���ڽ�loadko.sh�ŵ�/usr/local/signalwayĿ¼�����ļ�����Ӧ�޸�
##
## 2012-02-23
## Ϊ����loader����/etc/reloadko.sh���ӵ�
## /usr/local/signalway/reloadko.sh

# �����Դ���
max_test_count=30

# ж������
# ��������������
rmmod_device() {
	device_name=$1
	
	if [ -z $device_name ]; then
		# ����Ϊ�շ���ʧ��
		return 1;
	fi
	
	i=$max_test_count
	while [ $i -ne 0 ]; do
		tmp=`lsmod | awk "\\$1==\"$device_name\" {print \\$3}"`
		if [ -z "$tmp" ]; then
			# ������
			return 0
		else
			## ������������Դ��ռ��
			if [ $tmp -eq 0 ]; then
				rmmod ${device_name}
				return 0
			fi
		fi
		
		i=`expr $i - 1`
	done
	
	return 1;
}

# ��������
# ����1�������ļ���
# ����2����������
insmod_device() {
	if [ $# -lt 2 ] || [ -z $1 ] || [ -z $2 ]; then
		# ���ֺ��ļ�Ϊ�շ���ʧ��
		return 1;
	fi
	
	device_file=$1
	device_name=$2
	
	insmod $device_file
	
	# ����Ƿ���سɹ�
	i=$max_test_count
	while [ $i -ne 0 ]; do
		tmp=`lsmod | grep $device_name`
		if [ -n "$tmp" ]; then
			## ��������
			return 0;
		fi
		i=`expr $i - 1`
	done
	
	return 1;
}

## ж������
unload_devices() {
	rmmod_device "slw_pci"
	if [ $? -ne 0 ]; then
		echo "rmmod device 'slw_pci' error!"
		return 1
	fi
	
	rmmod_device "DSPLink"
	if [ $? -ne 0 ]; then
		echo "rmmod device 'DSPLink' error!"
		return 1
	fi
	
	rmmod_device "cmemk"
	if [ $? -ne 0 ]; then
		echo "rmmod device 'cmemk' error!"
		return 1
	fi
	
	if [ -c /dev/swpci ]; then
		rm -f /dev/swpci
	fi
	rm -f /dev/DSPLink
	
	return 0
}

## ��������
load_devices() {
	insmod_device "/lib/modules/DSPLink.ko" "DSPLink"
	if [ $? -ne 0 ]; then
		echo "insmod device 'DSPLink' error!"
		return 1
	fi
	
	mknod /dev/DSPLink c `awk "\\$2==\"DSPLink\" {print \\$1}" /proc/devices` 0
	
	if [ -f /usr/local/signalway/loadko.sh ]; then
		/bin/sh /usr/local/signalway/loadko.sh
	fi
	
	return 0
}

# ͳһ��core�ļ��ŵ�/corefileĿ¼��
echo "/log/core-%e-%p-%t" > /proc/sys/kernel/core_pattern

unload_devices
if [ $? -ne 0 ]; then
	echo "unload devices error!"
	exit 1
fi

load_devices
if [ $? -ne 0 ]; then
	echo "load devices error!"
	exit 1
fi
