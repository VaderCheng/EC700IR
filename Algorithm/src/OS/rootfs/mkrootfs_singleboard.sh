#!/bin/bash

# �ļ����ƣ�mkrootfs_singleboard.sh
# ���ܣ�������ļ�ϵͳ���������ű�
# ʹ�ã�
# ��rootȨ�����У�ubuntu����sudo ./mkrootfs_singleboard.sh

# Ĭ��������ʽ�汾�����ļ�
# ���������԰汾(����������)��ʹ�����
# ./mkrootfs_singleboard.sh debug

# ��ʱ����Ŀ¼
TMP=tmp

# �ļ�ϵͳָ��Ŀ¼
OUR_DIR=usr/local/signalway
DRIVER_DIR=lib/modules
RC_LOCAL_DIR=etc/rc.d
BOOT_DIR=etc/rc.d/rc3.d

# Ӧ�ó����ļ�
APP_FILES="App/LprApp App/HvDsp.out"
LOADER="App/loader"

# �����ļ�
SLW_DEV_FILE="Script/slw-dev_single.ko"
DRIVER_FILES="Script/cmemk.ko Script/DSPLink.ko"

# �ű�
ADD_FILES="Script/run.sh Script/dm.sh Script/reloadko.sh"
# �����ڴ��ļ���������
RC_LOCAL="Script/rc.local"
BOOT_FILE="Script/S99hdc"

# ��ɾ�����ļ�
DEL_FILES="usr/sbin/goaheadbin/ usr/sbin/ifplugd"

###############################################################

#### ʹ�ð���
if [ $# -eq 1 ] && [ $1 != "debug" ]; then
    echo "usage: "
    echo "release version:" $0
    echo "debug version:" $0 "debug"
    exit
fi

#### ����Ҫ���ļ��Ƿ񶼴���
cd patch
if [ $# -eq 0 ] && [ ! -f $LOADER ]; then
    echo "Error!! Not found" `basename $LOADER`
    exit
fi
if [ $# -eq 1 ] && [ $1 = "debug" ]; then
    for i in $APP_FILES; do
        if [ ! -f $i ]; then
            echo "Error!! Not found" `basename $i`
            exit
        fi
    done
fi
cd ../

###############################################################

# (��ѹ�ļ�ϵͳ���񣬵õ�initrd.img)
cp -f initrd.signalway.img.gz initrd.img.gz
gunzip -f initrd.img.gz

# (���ص���ʱĿ¼���粻���ڣ��򴴽�)
if [ ! -d $TMP ]; then mkdir -p $TMP; fi
sudo mount -o loop initrd.img $TMP

cd patch

# (��patchĿ¼��Ӧ�ļ����Ƶ��ļ�ϵͳ��)
echo "copying files ..."
# (��ֹ��Ϊ���ƶ���ʧ��ִ������)
chmod +x * -R
for i in $ADD_FILES; do
    sudo cp -fp $i ../$TMP/$OUR_DIR/`basename $i`
done

if [ $# -eq 0 ]; then
    sudo cp -fp $LOADER ../$TMP/$OUR_DIR/`basename $LOADER`
fi
# ���������ļ�
for i in $DRIVER_FILES; do
    sudo cp -fp $i ../$TMP/$DRIVER_DIR/`basename $i`
done

cp -fp $SLW_DEV_FILE ../$TMP/$DRIVER_DIR/slw-dev.ko

cp -fp $RC_LOCAL ../$TMP/$RC_LOCAL_DIR/rc.local

cp -fp $BOOT_FILE ../$TMP/$BOOT_DIR/`basename $BOOT_FILE`

if [ $# -eq 1 ] && [ $1 = "debug" ]; then
    for i in $APP_FILES; do
        sudo cp -fp $i ../$TMP/$OUR_DIR/`basename $i`
    done
echo "debug version"
fi

cd ../

# (ɾ��)
for i in $DEL_FILES; do
    sudo rm -rf $TMP/$i
done

# (��ֹж��ʱ�����豸æ����)
sleep 1

# (ж����ʱĿ¼)
sudo umount $TMP

# (ѹ��������initrd.img.gz)
gzip -fv9 initrd.img
mv initrd.img.gz initrd.img.gz.6467s

# (ɾ����ʱĿ¼)
rm -rf $TMP

echo "Job done!"
