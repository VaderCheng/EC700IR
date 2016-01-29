#!/bin/sh
#
#���ܣ�
#	Ӳ�̹���ű�
#	��Ҫ����Ӳ�̵ķ����ͼ��
#
#���з�ʽ: 
#	dm.sh type number dir
#
#˵��:
#	type˵�����еķ�ʽ
#	1--�����Ȱ���number����Ӳ�̣�Ȼ���ڽ�Ӳ�̹��ص�dirĿ¼�µ�0,1,2...��Ŀ¼
#	2--�������Ӳ�̣�������ж��Ӳ�̣����Ӳ�̣��ҽ�Ӳ�̵�dirĿ¼�µ�0,1,2...��Ŀ¼
#   3--������ʽ��Ӳ��,Ȼ���ڹ���Ӳ��
#	number
#	type=1��˵,����Ҫ�ֶ��ٸ���,-1�����Զ����䣬500G��4������1T��6����
#	type=2��˵��������
#	type=3��˵������Ҫ��ʽ���Ǹ�����
#	dir Ŀ¼
#	���гɹ�����0�����򷵻�1����ϸ����־��Ϣ���������̨������ͨ���ض�����־������ļ�
#
#����:�ƹ��� ��д��2011-09-25
#
#	2011-10-25 �޸ĸ�ʽ��Ӳ������ڹ���Ӳ��ʱ�����ַ������ڵ���3������Ҫ��2�������1	
#   2011-11-02 ������־�����ĩβΪ[Finish],�Ը�ʾ�ű����гɹ��������ϲ���õ��ж�
#	2011-11-09 �޸ĸýű�Ϊ�ػ����̣�ͨ��cmd.txtִ������,�����ڲ�������ⲿ����

#����:���Ӳ�̵Ĵ�С
#����ֵ:Ӳ�̴�С
disk_size()
{
	if [ `disk_is_exist` -eq 0 ];then
		return $1
	fi
	fdisk -l| grep "Disk /dev/sda"|awk '{print $3}'|awk -F . '{print $1}'
	return 0
}

#����: Ӳ���Ƿ����
#����ֵ��Ӳ�̸���
disk_is_exist()
{
	fdisk -l|wc -l
}

#����:Ӳ�̷��������Ӳ�̻�û�з����������������ֱ�Ӹ�ʽ��
#��������������� 
fdisk_disk()
{
	if [ `disk_is_exist` -eq 0 ];then
		return 1
	fi

	num=$1
	if [ $num -gt 8 ]
	then
		return 1
	fi
	
	if [ $num -le 0 ]
	then
		if [ `disk_size` -ge 1000 ]
		then
			num=6
		else
			num=4
		fi
	fi
	
	hasDiskNum=`fdisk -l|egrep "/dev/sda[1-9]"|grep -v grep|wc -l`
	echo "start fdisk,hasDiskNum=$hasDiskNum..."
	#��ɾ�����еķ���
	while [ $hasDiskNum -gt 0 ]
	do
	  hasDiskNum=`fdisk -l|egrep "/dev/sda[1-9]"|grep -v grep|wc -l`
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
		hasDiskNum=`expr $hasDiskNum - 1`
	done
	#Ȼ�󴴽�����
	if [ `fdisk -l|grep MB|wc -l` -eq 1 ]
	then
		size=`echo $num \`disk_size\`|awk '{print "+"$2/$1-100"M"}'`
	else
	  size=`echo $num \`disk_size\`|awk '{print "+"$2/$1*1000-100"M"}'`
	fi
	#���ʣ��Ŀռ����־
	num=`expr $num + 1`
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
				fdisk /dev/sda <<-!
					n
					p
					$i
					
					
					w
				!
				if [ $? -ne 0 ];then
					return $?
				fi
			else
				fdisk /dev/sda <<-!
					n
			    	
					
					w
				!
				if [ $? -ne 0 ];then
					return $?
				fi
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
			if [ $? -ne 0 ];then
				return $?
			fi
		#��4�������ķ�����ʽ
		elif [ $i -eq 4 ]
		then
			fdisk /dev/sda <<-!
				n
				e
				
				
				w
			!
			if [ $? -ne 0 ];then
				return $?
			fi
		#����4�������ķ�����ʽ
		else
			fdisk /dev/sda <<-!
				n
				
				$size
				w
			!
			if [ $? -ne 0 ];then
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
#�������:�ҽӵ���Ŀ¼
mount_disk()
{
	if [ `disk_is_exist` -eq 0 ];then
		return 1
	fi

	if [ ! -d $1 ]
	then
		mkdir $1
	fi
	dir=0
	num=`fdisk -l|grep Linux|wc -l|awk '{print $1-1}'`
	fdisk -l|grep Linux|awk '{print $1}'|while read DISK
	do
		if [ $dir -lt $num ]
		then
			if [ ! -d $1/$dir ]
			then
				mkdir $1/$dir
			fi
			echo "============mount $DISK $1/$dir================"
			if [ `df|grep $DISK|wc -l` -eq 0 ]
			then
				mount $DISK $1/$dir
				if [ $? -ne 0 ];then
					return $?
				fi
			fi
		else
			echo "=============mount $DISK /log================="
			if [ ! -d /log ]
			then
				mkdir /log
			fi
			if [ `df|grep $DISK|wc -l` -eq 0 ]
			then
				mount $DISK /log
				if [ $? -ne 0 ];then
					return $?
				fi
			fi
		fi
		echo "=============tune2fs -i 0 -c0 $DISK ==============="
		tune2fs -i 0 -c0 $DISK 
		dir=`expr $dir + 1`
	done
	return 0
}
#����    :ж��Ӳ��
#�������:��
umount_disk()
{
	num=0
	while [ `df|egrep "Result_disk|log"|wc -l` -ne 0 ] && [ $num -lt 3 ]
	do
			echo "retry umount disk $num times."
	    ps -ef|grep find|grep -v grep|awk '{print $1}'|xargs kill
	    df|egrep "Result_disk|log"|awk '{print $NF}'|while read LINE
	    do               
	    	echo "umount -f $LINE"
	    	umount -f $LINE
	    done
	    num=`expr $num + 1`
	done
	return 0
}

#����:��Ⲣ�޸�Ӳ��
#�������:��
check_disk()
{
	if [ `disk_is_exist` -eq 0 ];then
		return 1
	fi
	
	logFile=$1
	#�ȴ���log����
	umount /dev/sda3
	e2fsck -p /dev/sda3
	e2fsck -y /dev/sda3
	mkdir /log
	mount /dev/sda3 /log
	#���ȼ�⵱ǰ���õķ���
	resultDisk="NONE"
	if [ -f /log/Result.dat ]
	then
		if [ `cat /log/Result.dat|wc -l` -eq 0 ]
		then
			index=0
		else
			index=`cat /log/Result.dat`
		fi
		resultDisk=`echo $index|awk '{ if($1 < 4) print "/dev/sda"$1+1; else print "/dev/sda"$1+2;}'`
		echo "==========check $resultDisk first.====================="
		umount $resultDisk
		e2fsck -p $resultDisk
		e2fsck -y $resultDisk
		mkdir /Result_disk
		mkdir /Result_disk/$index
		mount $resultDisk /Result_disk/$index
	fi
	videoDisk="NONE"
	if [ -f /log/Video.dat ]
	then
		if [ `cat /log/Video.dat|wc -l` -eq 0 ]
		then
			index=4
		else
			index=`cat /log/Video.dat`
		fi
		videoDisk=`echo $index|awk '{ if($1 < 4) print "/dev/sda"$1+1; else print "/dev/sda"$1+2;}'`
		echo "=========check $videoDisk first.========================"
		umount $videoDisk
		e2fsck -p $videoDisk
		e2fsck -y $videoDisk
		mkdir /Result_disk
		mkdir /Result_disk/$index
		mount $videoDisk /Result_disk/$index
	fi
	if [ $resultDisk != "-1" ] || [  $videoDisk != "NONE" ]
	then
		return 0
	fi
	#�ڼ����������
	fdisk -l|grep Linux|awk '{print $1}'|while read DISK
	do
		if [ $DISK != $resultDisk ] && [ $DISK != $videoDisk ]
		then
			echo "===================e2fsck -p $DISK ======================="
			e2fsck -p $DISK 
			if [ $? -ne 0 ];then
				return $?
			fi
			echo "===================e2fsck -y $DISK ======================="
			e2fsck -y $DISK 
			if [ $? -ne 0 ];then
				return $?
			fi
		fi
	done
	return 0		
}

#����:��Ⲣ�޸�Ӳ�� (����ʹ��)
#�������:��
check_disk_2()
{
    echo "=================== check_disk_2 ======================="
	if [ `disk_is_exist` -eq 0 ];then
		return 1
	fi
	
	fdisk -l|grep Linux|awk '{print $1}'|while read DISK
	do
		echo "===================e2fsck -p $DISK ======================="
		e2fsck -p $DISK 
		if [ $? -ne 0 ];then
			return $?
		fi
		echo "===================e2fsck -y $DISK ======================="
		e2fsck -y $DISK 
		if [ $? -ne 0 ];then
			return $?
		fi
	done
	return 0		
}

detect_disk()
{
	echo "===================check $1==========================="
	df|grep $1|awk '{printf("%s %s\n", $1, $NF)}'|while read DISK DIR
	do
		touch $DIR/test.txt
		if [ $? -ne 0 ]
		then
			umount $DISK
			e2fsck -p $DISK
			e2fsck -y $DISK
			umount $DISK
			mount $DISK $DIR
		fi
		rm $DIR/test	
	done
}

#Ӳ�̹���ű�
#type   Ӳ�̹������������
#number ����������Ŀǰ�Զ����䣬��0
#dir    Ŀ¼
dm()
{
	type=$1
	number=$2
	dir=$3
	logFile=$4
	
	if [ $number -eq 0 ]
	then
		number=`fdisk -l|grep Linux|wc -l|awk '{print $1-1}'`
	fi
	
	if [ ! -z $logFile ] && [ -r $logFile ]
	then
		rm $logFile
	fi
	if [ `disk_is_exist` -eq 0 ];then
		echo "not found disk."
		echo 1 > $logFile
	return
	fi
	case $type in
		1)
			while [ `ps -ef|egrep "fdisk|mkfs.ext3|e2fsck"|grep -v grep|wc -l` -ne 0 ]
			do
				ps -ef|egrep "fdisk|mkfs.ext3|e2fsck"|grep -v grep|xargs kill
				sleep 1
			done
			umount_disk
 			fdisk_disk $number
			if [ $? -ne 0 ];then
				echo 1 > $logFile
				return
			fi
			format_disk
			if [ $? -ne 0 ];then
				echo 1 > $logFile
				return
			fi
			mount_disk $dir
			if [ $? -ne 0 ];then
				echo 1 > $logFile
				return
			fi
			ret=`df|grep Result_disk|grep -v log|grep -v grep|wc -l`
			echo "df|grep Result_disk|grep -v log|grep -v grep|wc -l=$ret, number=$number"
			if [ $ret -eq $number ] && [ $ret -ne 0 ]
			then
				echo 0 > $logFile
			else
				echo 1 > $logFile
			fi
		;;
		2)
			while [ `ps -ef|egrep "fdisk|mkfs.ext3|e2fsck"|grep -v grep|wc -l` -ne 0 ]
			do
				ps -ef|egrep "fdisk|mkfs.ext3|e2fsck"|grep -v grep|xargs kill
				sleep 1
			done
			umount_disk
			check_disk $logFile
			if [ $? -ne 0 ];then
				echo 1 > $logFile
				return
			fi
			mount_disk $dir
			if [ $? -ne 0 ];then
				echo 1 > $logFile
				return
			fi
			ret=`df|grep Result_disk|grep -v log|grep -v grep|wc -l`
			echo "df|grep Result_disk|grep -v log|grep -v grep|wc -l=$ret, number=$number"
			if [ $ret -eq $number ] && [ $ret -ne 0 ]
			then
				echo 0 > $logFile
			else
				echo 1 > $logFile
			fi
		;;
		3)
			if [ $number -ge 3 ]
			then
				DISK=/dev/sda`expr $number + 2`
			else
				DISK=/dev/sda`expr $number + 1`
			fi
			umount $DISK
			mkfs.ext3 -N 1000000 -T largefile $DISK <<-!
			
			!
			if [ $? -ne 0 ];then
				echo $? > $logFile
				return
			fi
			mount $DISK $dir/$number 
			if [ $? -ne 0 ];then
				echo $? > $logFile
				return
			fi
			ret=`df|grep Result_disk|grep -v log|grep -v grep|wc -l`
			echo "df|grep Result_disk|grep -v log|grep -v grep|wc -l=$ret, number=$number"
			if [ $ret -eq $number ] && [ $ret -ne 0 ]
			then
				echo 0 > $logFile
			else
				echo 1 > $logFile
			fi
		;;
		4)
			detect_disk $dir
			echo 0 > $logFile
		;;
		5)
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
			ret=`df|grep Result_disk|grep -v log|grep -v grep|wc -l`
			echo "df|grep Result_disk|grep -v log|grep -v grep|wc -l=$ret, number=$number"
			if [ $ret -eq $number ] && [ $ret -ne 0 ]
			then
				echo 0 > $logFile
			else
				echo 1 > $logFile
			fi
		;;
		6)
			umount_disk
			echo 0 > $logFile
		;;
		7)
			umount_disk
			check_disk_2 $logFile
			if [ $? -ne 0 ];then
				echo $? > $logFile
				return
			fi
			mount_disk $dir
			if [ $? -ne 0 ];then
				echo $? > $logFile
				return
			fi
			ret=`df|grep Result_disk|grep -v log|grep -v grep|wc -l`
			echo "df|grep Result_disk|grep -v log|grep -v grep|wc -l=$ret, number=$number"
			if [ $ret -eq $number ] && [ $ret -ne 0 ]
			then
				echo 0 > $logFile
			else
				echo 1 > $logFile
			fi
		;;
		*)
			echo "Usage:$0 type number dir"
			echo "Exam: $0 1 4 Result_Disk"
			echo 1 > $logFile
		;;
	esac
}

#����ping ����
ping_gateway()
{
	while [ true ]
	do
		route|grep default|awk '{print "ping -c 1 "$2}'|sh
		sleep 5
	done
}

#ɾ��Ŀ¼
delete_directory()
{
	#Ҫɾ����Ŀ¼
	dir=$1
	me=$2
	if [ ! -d $dir ]
	then
		return
	fi
	num=`basename $dir`
	if [ -z $num ] || [ $num = "file" ] || [ $num = "data" ]
	then
		echo "$dir is the top directory,can not delete."
		return
	fi
	#ɾ���Լ���Ŀ¼
	if [ $me -eq 1 ]
	then
		dir=`dirname $dir`
		#ɾ��С�ڵ��ڵ�ǰ������Ŀ¼
		while [ $num -ge 0 ]
		do
			if [ -d $dir/$num ]
			then
				echo "delete overwrite directory:rm -Rf $dir/$num"
				rm -Rf $dir/$num
			elif [ -d $dir/0$num ]
			then
				echo "delete overwrite directory:rm -Rf $dir/0$num"
				rm -Rf $dir/0$num
			fi
			num=`expr $num - 1`
		done
		#��Ŀ¼Ϊ�գ���ɾ��
		if [ `(rmdir -p $dir 2>/dev/null;echo $?)` -eq 0 ]
		then
			echo "delete empty directory:rmdir -p $dir"
		fi
	else		
		#ɾ�����Լ�С��Ŀ¼
		while [ ! -z $num ] && [ $num  != "file" ] && [ $num  != "data" ]
		do
			dir=`dirname $dir`
			ls $dir|while read DIR
			do
				if [ $DIR -lt $num ]
				then
					while [ `ps -ef|grep "rm -Rf $dir/$DIR"|grep -v grep|wc -l` -ne 0 ]
					do
						sleep 1
					done
					echo "delete invalid directory:rm -Rf $dir/$DIR"
					rm -Rf $dir/$DIR
				fi	
			done
			num=`basename $dir`
		done
	fi	
}

#����:ִ��һ���ⲿ����
#����: ���� ���� ����ֵ�ļ�
exec_cmd()
{
	if [ $# -eq 3 ]
	then
		rm -f $3
		echo "$1 $2 > $3"
		($1 $2;echo $? > $3)
	else
		echo "$1 $2"
		($1 $2)
	fi
}

#�ػ����̽ű�
deamon()
{
	while [ true ]
	do
		if [ -f loader ]
		then
			echo "Start Run loader...."
			loader /dev/swdev
		elif [ -f LprApp ]
		then
			echo "Start Run LprApp...."
			./LprApp
		elif [  -f CamApp ]
		then
			echo "Start Run CamApp...."
			./CamApp
		fi
		if [ -f core ]
		then
			mv core /log/core.`date +"%Y-%m-%d %H:%M:%S"`
			break
		fi
	done
}

#�����̿�ʼ,ͨ����ȡ�ļ�cmd.txt��������
#�����ʽ������ ����|���淵��ֵ���ļ�
main()
{
	times=0
	while [ true ]
	do
		#���1��֮ǰ��txt�ļ�
		if [ $times -ge 86400 ]
		then			
			find /usr/local/signalway -name '*.txt' -mtime +1|xargs rm -f
			times=0
		fi
		
		if [ ! -r cmd.txt ] || [ ! -r exec.txt ]
		then
			sleep 1
			times=`expr $times + 1`
			continue
		fi

		#�����ļ�
		cat cmd.txt|while read CMD LINE
		do
			#��������
			cmdParam=`echo $LINE|awk -F '|' '{print $1}'`
			logFile=`echo $LINE|awk -F '|' '{print $2}'`
			#��ʾ׼��ִ�е�����
			if [ -z $logFile ]
			then
				logFile=`echo $CMD|awk -F '|' '{print $2}'`
				CMD=`echo $CMD|awk -F '|' '{print $1}'`
				if [ -z $logFile ]
				then
					echo "$CMD"
				else
					echo "$CMD,return $logFile"
				fi
			else
				echo "$CMD $cmdParam,return $logFile"
			fi 
			#ִ������
			if [ $CMD = "ping_gateway" ]
			then
				ping_gateway &
			elif [ $CMD = "dm" ] || [ $CMD = "dm.sh" ]
			then
				(dm $cmdParam $logFile) &
			elif [ $CMD = "RM" ] || [ $CMD = "rm" ]
			then
				(delete_directory $cmdParam;echo "0" > "$logFile") &
			else
				(exec_cmd "$CMD" "$cmdParam" "$logFile") &
			fi
		done
		rm -f cmd.txt exec.txt
		sleep 1
		times=`expr $times + 1`
	done	
}

#���±��ű���niceֵ
renice 5 $$
#�������
if [ ! -f .run_dm ]
then
	touch .run_dm
	echo "start running dm.sh..."
	main
	echo "stop running dm.sh..."
	rm .run_dm
else
	echo "dm.sh is running, can not run it again..."
fi
