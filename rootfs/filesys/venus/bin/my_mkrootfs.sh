#!/bin/bash

# ���ܣ����ļ�ϵͳĿ¼���ɾ����ļ���ѹ����
# ��ǰĿ¼�����ROOTFSĿ¼�������Ǹ��ļ�ϵͳ����
# ע���������޸�ROOTFS��ROOTIMG��SIZE�Ķ��塣
#
# ʹ�ã���rootȨ�����У�����sudo ./mkrootfs.sh [�ļ�ϵͳĿ¼] [ѹ�������ļ�] [�����С]
# ����ubuntu�²���ͨ��

ROOTFS=filesys_org
TMP=tmp1
ROOTIMG=rootfs.dm8127.img
# 16: 16384
# 32: 32768
# 48: 49152
# 64: 65536
SIZE=49152

if [ $# -ne 3 ]; then
    echo "usage:" $0 "[rootfs dir] [rootfs img file] [size]"
    echo "eg:" $0 "rootfs_org rootfs.img(!!!not .img.gz!!!) 49152"
    exit
fi

ROOTFS=$1
ROOTIMG=$2
SIZE=$3

# (������Ӧ��С��ӳ���ļ�)
dd if=/dev/zero of=$ROOTIMG bs=1k count=$SIZE
# (��ʽ��)
mke2fs -F -v -m0 $ROOTIMG
# (���ص�ͬһĿ¼��tmp)
if [ ! -d $TMP ]; then mkdir -p $TMP; fi
sudo mount -o loop $ROOTIMG $TMP
# (��rootfsĿ¼�������ļ����Ƶ�tmp��)
echo "copying..."
sudo cp -a $ROOTFS/* $TMP
# (ж��tmp)
sudo umount $TMP
# (ѹ��֮)
gzip -fv9 $ROOTIMG

#sudo umount $ROOTFS
#rm -rf $ROOTFS

# (ɾ����Ŀ¼)
rm -rf $TMP
