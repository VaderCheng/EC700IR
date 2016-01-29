#!/bin/bash

#
# ����: ��������ר��APP���������ļ���
# 
# ע�⣺mkfs.ubifs�����-c������Ҫ�ֶ����㡣�ɲο� http://processors.wiki.ti.com/index.php/UBIFS_Support
# ��ʹ��rootȨ��ִ�б��ű���

OUTPUT_IMAGE=ubifs_app_back.img
FACTORY_IMAGE=ubifs_app_bak.factoryuse.img
TMPFILE=ubifs.img
CFG_FILE=ubinize_app.cfg

TMP_DIR=app_back_tmp

#Ӧ�ó���Ŀ¼
APP_DIR=$TMP_DIR

#ͼ��ɼ�����Ŀ¼
VIDEO_APP_DIR=$TMP_DIR/ipc_arch

TMP=backup_sys_app

#��Ҫ���ļ�
APP_FILES="backup_sys_app/deamon backup_sys_app/log backup_sys_app/command backup_sys_app/UpgradeApp backup_sys_app/command.sh backup_sys_app/fpga.xsvf backup_sys_app/fpga_178.xsvf backup_sys_app/fpga_185.xsvf backup_sys_app/fpga_249.xsvf backup_sys_app/load_fpga backup_sys_app/switchsys"
XML_FILES="backup_sys_app/deamon.xml"
INIT_FILE="backup_sys_app/my_run.sh"

APP_FILES1="../backup_sys_app/deamon ../backup_sys_app/log ../backup_sys_app/command ../backup_sys_app/UpgradeApp ../backup_sys_app/command.sh ../backup_sys_app/fpga.xsvf ../backup_sys_app/fpga_178.xsvf ../backup_sys_app/fpga_185.xsvf ../backup_sys_app/fpga_249.xsvf ../backup_sys_app/load_fpga ../backup_sys_app/switchsys"
XML_FILES1="../backup_sys_app/deamon.xml"
INIT_FILE1="../backup_sys_app/my_run.sh"

#�жϱ�Ҫ���ļ��Ƿ���ڡ���������������ʧ�ܡ�
for i in $APP_FILES $XML_FILES $INIT_FILE; do
    if [ ! -f $i ]; 
	then
        echo -e "Error!! file not found:" $i
        exit
    fi
done

chmod +x bin/* -R

chmod +x $TMP/* -R

cd bin

mkdir -p $TMP_DIR

# �����ļ�����ӿ�ִ������
chmod +x ../backup_sys_app/* -R

if [ ! -d "$APP_DIR" ]; then
mkdir -p $APP_DIR
fi
if [ ! -d "$VIDEO_APP_DIR" ]; then
mkdir -p $VIDEO_APP_DIR
fi


# ���Ʋɼ�ͼ�����
cp -a ../backup_sys_app/video_test/* $VIDEO_APP_DIR

# ���������ű�
cp -a $INIT_FILE1 $APP_DIR

# ɾ��.svnĿ¼
#find $APP_DIR -type d -name ".svn"|xargs rm -rf
#find $VIDEO_APP_DIR -type d -name ".svn"|xargs rm -rf

# todo
#cp -a ../backup_sys_app/load_fpga $VIDEO_APP_DIR
#cp -a ../backup_sys_app/fpga.xsvf $VIDEO_APP_DIR

# ��Ҫ��app����
for i in $APP_FILES1 $XML_FILES1; do
    sudo cp -fp $i $APP_DIR/`basename $i`;
done

#cp -a ../$TMP/* $TMP_DIR

# ɾ��.svnĿ¼
find $TMP_DIR -type d -name ".svn"|xargs rm -rf

#./mk_ubifs.sh $TMP ../ubi_rootfs_back.img 40 rootfs

#step 1:���������ļ� 
echo "1. generating cfg file"

echo "[ubifs]" > $CFG_FILE
echo "mode=ubi" >> $CFG_FILE
echo "image="$TMPFILE >> $CFG_FILE
echo "vol_id=0" >> $CFG_FILE
echo "vol_size=50MiB" >> $CFG_FILE
# dynamic static
echo "vol_type=static" >> $CFG_FILE
echo "vol_name=app_back" >> $CFG_FILE
echo "vol_flags=autoresize" >> $CFG_FILE

#step 2:����ubi����
chmod +x mkfs.ubifs
chmod +x ubinize

#-c 471 was the result of calculation
echo "2. generating ubi file"

./mkfs.ubifs -r $TMP_DIR -m 2048 -e 126976 -c 416 -o $TMPFILE
./ubinize -o ../$OUTPUT_IMAGE -m 2048 -p 128KiB -s 2048 -O 2048 $CFG_FILE

# ������д(uboot��д)ר��
cp -f $TMPFILE ../$FACTORY_IMAGE

#step 3:ɾ����ʱ�ļ�
echo "3. removing tmp files..."
sudo rm -rf $TMPFILE
sudo rm -rf $CFG_FILE
sudo rm -rf $TMP_DIR

echo ""
echo "!!!! generate img file: " $OUTPUT_IMAGE "!!!!"
echo ""



cd ../
