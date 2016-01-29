#!/bin/bash

# ���ܣ�ѹ�����񲢹��ص�ָ��Ŀ¼��
# ע�����ű����߼��÷�������������
#
# ʹ�ã���rootȨ�����У�����sudo ./ungunzip.sh [ѹ�������ļ�] [����Ŀ¼]
# ����ubuntu�²���ͨ��

TMP1=tmp

RFS_TMP=rootfs_tmp.img.gz
RFS_1=rootfs_tmp.img

if [ $# -ne 2 ]; then
    echo "usage:" $0 "[rootfs img file] [rootfs dir]"
    exit
fi

cp -f $1 $RFS_TMP
gunzip -f $RFS_TMP

TMP=$2

# (���ص���ʱĿ¼���粻���ڣ��򴴽�)
if [ ! -d $TMP1 ]; then mkdir -p $TMP1; fi
sudo mount -o loop $RFS_1 $TMP1

sudo cp -a $TMP1 $TMP

sudo umount $TMP1

sudo rm -rf $TMP1
sudo rm -rf $RFS_1

sudo rm -rf $TMP/lost+found

# ע����Ҫ����ж��
# ж��
# umount $TMP