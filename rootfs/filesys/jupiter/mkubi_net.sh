#!/bin/bash

#
# ����: ����NET���������ļ���
# ��Ҫ�Ӳ���
# 
# ע�⣺mkfs.ubifs�����-c������Ҫ�ֶ����㡣�ɲο� http://processors.wiki.ti.com/index.php/UBIFS_Support
# ��ʹ��rootȨ��ִ�б��ű���

# �����ļ������ƽ̨���ã��ô˺궨��
INPUT_DIR=../venus
OUTPUT_DIR=$PWD

TMPFILE=ubifs.img
CFG_FILE=ubinize_net.cfg

# .net��������Ŀ¼
NET_DIR=$OUTPUT_DIR/$INPUT_DIR/net_patch

TMP_DIR=net_tmp

chmod +x $INPUT_DIR/bin/* -R

#IMAGE_FILE should be ��ubifs_net.img��
IMG_FILE=$OUTPUT_DIR/ubifs_net.img

cd $INPUT_DIR/bin

#./mk_ubifs.sh $TMP ../ubi_rootfs_back.img 40 rootfs
mkdir -p $TMP_DIR

cp -a $NET_DIR/* $TMP_DIR

#step 1:���������ļ�
echo "1. generating cfg file"

echo "[ubifs]" > $CFG_FILE
echo "mode=ubi" >> $CFG_FILE
echo "image="$TMPFILE >> $CFG_FILE
echo "vol_id=0" >> $CFG_FILE
echo "vol_size=81MiB" >> $CFG_FILE
# dynamic
echo "vol_type=static" >> $CFG_FILE
echo "vol_name=net" >> $CFG_FILE
echo "vol_flags=autoresize" >> $CFG_FILE

#step 2:����ubi����
chmod +x mkfs.ubifs
chmod +x ubinize

echo "2. generating ubi file"
./mkfs.ubifs -r $TMP_DIR -m 2048 -e 126976 -c 668 -o $TMPFILE
./ubinize -o $IMG_FILE -m 2048 -p 128KiB -s 2048 -O 2048 $CFG_FILE

#step 3:ɾ����ʱ�ļ�
echo "3.removing tmp files..."
sudo rm -rf $TMPFILE
sudo rm -rf $CFG_FILE
sudo rm -rf $TMP_DIR

echo ""
echo "!!!! generate img file: " $IMG_FILE "!!!!"
echo ""

cd ../
