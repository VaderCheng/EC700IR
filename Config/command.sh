#!/bin/sh
#�������ű�����Ҫ������һϵ�еĺ�����ͨ���ű����þ���ĺ��������÷�������
#command.sh ������ ����
#�ɹ�����0��ʧ�ܷ��ض�Ӧ�Ĵ������

#����:���Ӳ�̵Ĵ�С
#����ֵ:Ӳ�̴�С
disk_size()
{
	if [ `disk_is_exist $1` -eq 0 ];then
		echo "0"
	fi
	if [ -z "echo $1 | grep mmcblk" ]; then
	  fdisk -l $1 | grep "Disk /dev/sd[a-z]"|awk '{print $3}'|awk -F . '{print $1}'
	else
	  fdisk -l $1 | grep "Disk /dev/mmcblk[0-9]"|awk '{print $3}'|awk -F . '{print $1}'
	fi
	
}

#����: Ӳ���Ƿ����
#����ֵ��Ӳ�̸���
disk_is_exist()
{
  fdisk -l $1 |wc -l 
}

#����:Ӳ�̷��������Ӳ�̻�û�з����������������ֱ�Ӹ�ʽ��
#���������$1:�̷�;$2:������ 
fdisk_disk()
{
	if [ `disk_is_exist $1` -eq 0 ];then
		return 1
	fi

	HDD=$1	
	num=$2
	
	if [ $num -gt 8 ]
	then
		return 1
	fi
	
	if [ $num -le 0 ]
	then
		if [ `disk_size $1` -ge 1000 ]
		then
			num=6
		else
			num=4
		fi
	fi
	
	
	if [ -z "echo ${HDD} | grep mmcblk" ]; then
	  hasDiskNum=`fdisk -l|egrep "${HDD}[1-9]"|grep -v grep|wc -l`
	else
	  hasDiskNum=`fdisk -l ${HDD}|egrep "${HDD}p[1-9]"|grep -v grep|wc -l`
	fi
	echo HDD = $HDD
	while [ `df|grep "/dev/sd[a-z][1-9]"|wc -l` -ne 0 ]
	do
		echo "umount all exists disk..."
		df|grep "/dev/sd[a-z][1-9]"|while read DISK MOUNT
		do
			umount $DISK
		done
	done
	echo "start fdisk,hasDiskNum=$hasDiskNum..."
	#��ɾ�����еķ���
	while [ $hasDiskNum -gt 0 ]
	do
	  if [ -z "echo ${HDD} | grep mmcblk" ]; then
  	  hasDiskNum=`fdisk -l|egrep "${HDD}[1-9]"|grep -v grep|wc -l`
  	else
  	  
  	  hasDiskNum=`fdisk -l ${HDD}|egrep "${HDD}p[1-9]"|grep -v grep|wc -l`
  	fi
		echo "=========remove disk partition = $hasDiskNum============"
		if [ $hasDiskNum -gt 1 ]
		then
			fdisk ${HDD} <<-!
				d
				$hasDiskNum
				w
			!
			if [ $? -ne 0 ]
			then
				echo "remove partition $HDD fail.."
			fi
		else
			fdisk ${HDD} <<-!
				d
				w
			!
			if [ $? -ne 0 ]
			then
				echo "remove partition $HDD fail.."
			fi
		fi		
		hasDiskNum=`expr $hasDiskNum - 1`
	done
	#Ȼ�󴴽�����
	if [ `fdisk -l $HDD | grep MB|wc -l` -eq 1 ]
	then
		size=`echo $num \`disk_size $1\`|awk '{print "+"$2/$1"M"}' | awk -F "." '{print $1"M"}'`
	else
	  size=`echo $num \`disk_size $1\`|awk '{print "+"$2/$1*1000"M"}' | awk -F "." '{print $1"M"}'`
	fi

  echo num = $num
  echo size = $size
  
	if [ $num -gt 4 ]
	then
		num=`expr $num + 1`
	fi

	i=1
	while [ $i -le $num ]
	do
		echo "===========create disk partition $i,size =$size============="
		#���һ������
		if [ $i -eq $num ]
		then
			if [ $i -le 4 ]
			then
				fdisk ${HDD} <<-!
					n
					p
					$i
					
					
					w
				!
				if [ $? -ne 0 ];then
					echo "create partition $i failed"
					return $?
				fi
			else
				fdisk ${HDD} <<-!
					n
			    	
					
					w
				!
				if [ $? -ne 0 ];then
					echo "create partition $i failed"
					return $?
				fi
			fi
		#С��4�������ķ�����ʽ
		elif [ $i -lt 4 ]
		then
			fdisk ${HDD} <<-!
				n
				p
				$i
				
				$size
				w
			!
			if [ $? -ne 0 ];then
				echo "create partition $i failed"
				return $?
			fi
		#��4�������ķ�����ʽ
		elif [ $i -eq 4 ]
		then
			fdisk ${HDD} <<-!
				n
				e
				
				
				w
			!
			if [ $? -ne 0 ];then
				echo "create partition $i failed"
				return $?
			fi
		#����4�������ķ�����ʽ
		else
			fdisk ${HDD} <<-!
				n
				
				$size
				w
			!
			if [ $? -ne 0 ];then
				echo "create partition $i failed"
				return $?
			fi
		fi
		i=`expr $i + 1`
	done
	partprobe	
	return 0	
}

#����    : ��ʽ������Ӳ��
#�����������
format_disk()
{
	if [ `disk_is_exist $1` -eq 0 ];then
		return 1
	fi
  
	fdisk -l $1| grep Linux | awk '{print $1 " " $4}'|while read DISK INODE
	do
		INODE=`echo $INODE|awk -F '+' '{print $1}'`
		INODE=`expr $INODE / 256 + 100000`
		echo disk = $DISK, inode=$INODE

		ls /dev/sd[a-z]
		if [ $? -eq 0 ]; then
		    echo "==================mkfs.ext3 -N $INODE -T largefile $DISK================="
		    mkfs.ext3 -N $INODE -T largefile $DISK 
        else
		    echo "==========mkfs.ext4 -O ^huge_file -N $INODE -T largefile $DISK==========="
		    mkfs.ext4 -O ^huge_file -N $INODE -T largefile $DISK 
        fi
		
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
	if [ `disk_is_exist $1` -eq 0 ];then
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
	if [ $# -eq 1 ]
	then
		umount -f $1
		echo $?
	else
		df|grep "/dev/sd[a-z]"|while read DISK
		do
			umount $DISK
		done
	fi
}

#����:��Ⲣ�޸�Ӳ��
#�������:0�����򵥵ļ�⣬1�����������
check_disk()
{
	if [ `disk_is_exist $1` -eq 0 ];then
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
			fdisk -l|grep Linux|awk '{print $1}'|while read DISK
			do
				badblocks -s -v /dev/${DISK}
			done
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

#Ӳ�̹�����
#$1 ����id
#$2 �������
disk_cmd()
{
	CMD=$1
	PARAM=$2
	echo "disk comamnd:$CMD, parameter:$PARAM"
	case $CMD in
		0)
			if [ ! -f /tmp/.disk_cmd_${CMD}_running ]
			then
				touch /tmp/.disk_cmd_${CMD}_running
				rm  -f /tmp/disk_cmd_${CMD}.txt			
				echo "1" > /tmp/disk_cmd_${CMD}.txt
				umount_disk
				
				ls /dev/sd[a-z]
				if [ $? -eq 0 ]; then
				  DEVPATH=/dev/sd[a-z]
				else
				  DEVPATH=/dev/mmcblk[0-9]
				fi
				
				ls $DEVPATH | while read DISK
				do
					fdisk_disk $DISK $PARAM
				done
				if [ $? -eq 0 ]
				then
				  ls $DEVPATH | while read DISK
				  do
					  format_disk $DISK 
					  if [ $? -ne 0 ]
					  then
					  	echo "4" > /tmp/disk_cmd_${CMD}.txt
					  fi
				  done
				else
					echo "3" > /tmp/disk_cmd_${CMD}.txt
				fi			
				if [ `cat /tmp/disk_cmd_${CMD}.txt` -eq 1 ]
				then
					echo "2" > /tmp/disk_cmd_${CMD}.txt
				fi
				rm -f /tmp/.disk_cmd_${CMD}_running
			else
				echo "fdisk command is running."
			fi
		;;
	esac
}

#ntpʱ��ͬ��
#$1 ͬ��ʱ��������λ����
#$2 ntp������ 
ntp_sync()
{
	PARAM=$@
	if [ ! -f /tmp/.running_ntp ]
	then
		touch /tmp/.running_ntp
		while [ -f /tmp/.running_ntp ]
		do
			if [ -f /tmp/ntp_param ]
			then
				PARAM=`cat /tmp/ntp_param`
			fi
			echo "$PARAM"|while read TIME IP
			do
				echo "ntpdate -u -t 0.6 $IP"
				s=`date +"%s"`
				ntpdate -u -t 0.6 $IP
				e=`date +"%s"`
				if [ $? -eq 0 ]
				then
					if [ `expr $e - $s` -gt 3 ] || [ `expr $s - $e` -gt 3 ]
					then
						echo "hwclock -w --utc"
						hwclock -w --utc
					fi
					touch /tmp/ntp_ok
				fi
				sleep $TIME
			done	
		done
		rm /tmp/.running_ntp
	else
		echo $PARAM > /tmp/ntp_param
	fi
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
  "fdisk_disk")  
  	(fdisk_disk $param) &
  	return 0
	;;
	
	"format_disk") 
		(format_disk $param) &
		return 0
	;;
	
	"check_disk")  return `check_disk $param`
	;;
	
	"mount_disk")  return `mount_disk $param`
	;;
	
	"umount_disk") return `umount_disk $param`
	;;
	
	"init_LprApp") return `init_LprApp`
	;;
	
	"disk_cmd")
		(disk_cmd $param) &
		return 0
	;;
	
	"ntp_sync") 
		(ntp_sync $param) &
		return 0
	;;
	
	
	"clear_h264fifo") 
		cat /tmp/test.264 > /dev/null &
		return 0
	;;
	
	"restore_h264fifo") 
		killall "cat"
		return 0
	;;

	"clear_gb28181fifo") 
		cat /tmp/gbtest.264 > /dev/null &
		return 0
	;;
	
	"restore_gb28181fifo") 
		killall "cat"
		return 0
	;;
	*)
	(exec_cmd "$command $param"; return $?)
	;;
esac
