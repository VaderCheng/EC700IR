#!/bin/sh
#�������ű�����Ҫ������һϵ�еĺ�����ͨ���ű����þ���ĺ��������÷�������
#command.sh ������ ����
#�ɹ�����0��ʧ�ܷ��ض�Ӧ�Ĵ������

#����: Ӳ���Ƿ����
#����ֵ��Ӳ�̸���
disk_is_exist()
{
	fdisk -l|wc -l
}

#����:Ӳ�̷���
#���������ÿ��������С
fdisk_disk()
{
	#�ж��Ƿ���Ӳ��
	if [ `disk_is_exist` -eq 0 ];then
		echo "1"
		return
	fi

	#��ɾ�����еķ���
	hasDiskNum=`fdisk -l|egrep "/dev/sda[1-9]"|grep -v grep|wc -l`
	while [ $hasDiskNum -gt 0 ]
	do
		echo "=========remove disk partition = $hasDiskNum============"
		if [ $hasDiskNum -gt 1 ]
		then
			fdisk /dev/sda <<-!
				d
				$hasDiskNum
				w
			!
		else
			fdisk /dev/sda <<-!
				d
				w
			!
		fi		
		hasDiskNum=`fdisk -l|egrep "/dev/sda[1-9]"|grep -v grep|wc -l`
	done
	
	#Ӳ�̷�������
	num=`expr $# - 1`
	if [ $num -gt 4 ]
	then
		num=`expr $num + 1`
	fi
	
	i=1
	while [ $i -le $num ]
	do
	  shift
		size=+$1"K"
		
		echo "===========create disk partition $i,size =$size============="
		#���һ������
		if [ $i -eq $num ]
		then
			if [ $i -le 4 ]
			then
				fdisk /dev/sda <<-!
					n
					p
					$i
					
					
					w
				!
			else
				fdisk /dev/sda <<-!
					n
			    	
					
					w
				!
			fi
		#С��4�������ķ�����ʽ
		elif [ $i -lt 4 ]
		then
			fdisk /dev/sda <<-!
				n
				p
				$i
				
				$size
				w
			!
		#��4�������ķ�����ʽ
		elif [ $i -eq 4 ]
		then
			fdisk /dev/sda <<-!
				n
				e
				
				
				w
			!
		#����4�������ķ�����ʽ
		else
			fdisk /dev/sda <<-!
				n
				
				$size
				w
			!
		fi
		i=`expr $i + 1`
	done
#	partprobe	
	echo "0"
}

#����    : ��ʽ������Ӳ��
#�����������
format_disk()
{
	if [ `disk_is_exist` -eq 0 ];then
		return 1
	fi

	fdisk -l|grep Linux|awk '{print $1 " " $4}'|while read DISK INODE
	do
		INODE=`echo $INODE|awk -F '+' '{print $1}'`
		INODE=`expr $INODE / 256 + 50000`
		echo "==================mkfs.ext3 -N $INODE -T largefile $DISK================="
		mkfs.ext3 -N $INODE -T largefile $DISK <<-!
		
		!
		if [ $? -ne 0 ];then
			return $?
		fi
		sleep 10
	done
	return 0
}

#����    :�ҽ�Ӳ��
#�������:���� Ŀ¼ ����
mount_disk()
{
	if [ `disk_is_exist` -eq 0 ];then
		return 1
	fi
	
	if [ ! -d $2 ]
	then
		mkdir -p $2
	fi
	mount $1 $2 $3
	echo $?
}
#����    :ж��Ӳ��
#�������:����
umount_disk()
{
	umount -f $1
	echo $?
}

#����:��Ⲣ�޸�Ӳ��
#�������:0�����򵥵ļ�⣬1�����������
check_disk()
{
	if [ `disk_is_exist` -eq 0 ];then
		return 1
	fi
	
	if [ $1 -eq 0 ]
	then
		fdisk -l|grep Linux|awk '{print $1}'|while read DISK
		do
			e2fsck -p $DISK
			e2fsck -y $DISK
		done		
	else
		echo "==========badblocks -s -v /dev/sda============="
		which badblocks
		if [ $? -eq 0 ];then
			badblocks -s -v /dev/sda
			if [ $? -ne 0 ];then
				echo $? > $logFile
			fi
		else
			echo "===============can not find badblocks==========="
		fi		
	fi
	echo $?
}

#LprApp���̳�ʼ���ű�
init_LprApp()
{
	#ж����������
	lsmod|grep -v Module|while read LINE
	do
		rmmod $LINE
	done
	
	#װ��DSPLink
	insmod /lib/modules/DSPLink.ko
	rm -f /dev/DSPLink
	mknod /dev/DSPLink c `cat /proc/devices|grep DSPLink|awk '{print $1}'` 0
	echo $?
}

#����:ִ��һ���ⲿ����
#����: ���� ���� ����ֵ�ļ�
exec_cmd()
{
	$1
}
     
#������֧  
command=$1
shift
param=$@
case $command in
  "fdisk_disk")  return `fdisk_disk $param`
	;;
	
	"format_disk") return `format_disk $param`
	;;
	
	"check_disk")  return `check_disk $param`
	;;
	
	"mount_disk")  return `mount_disk $param`
	;;
	
	"umount_disk") return `umount_disk $param`
	;;
	
	"init_LprApp") return `init_LprApp`
	;;
	
	*)
	(exec_cmd "$command $param"; return $?)
	;;
esac
