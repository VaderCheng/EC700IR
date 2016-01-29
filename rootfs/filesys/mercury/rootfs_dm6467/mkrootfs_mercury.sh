#!/bin/bash

# �ļ����ƣ�mkrootfs_mercury.sh
# ���ܣ�ˮ��ƽ̨�ļ�ϵͳ���������ű�
# ���ߣ�lijj 2013-04

# �޸���֪���ο��ű��ķ���������ӡ�ɾ���ļ�

# ʹ�ã�
# ����������ʹ��rootȨ�����У�����
# ��ʽ�汾�����ļ���
# ./mkrootfs_mercury.sh
# ���԰汾(����������)
# ./mkrootfs_mercury.sh debug

##########################################################

######################## ���޸��� ��ʼ

# ԭ���ļ�ϵͳ����
ORG_ROOTFS="rootfs_dm6467.32MB.img.gz"
# �¸��ļ�ϵͳ����
NEW_ROOTFS="rootfs.img.gz.mercury"

# �ļ�ϵͳָ��Ŀ¼
OUR_DIR=usr/local/signalway
DRIVER_DIR=lib/modules
BOOT_DIR=etc/rc.d/rc3.d
BIN_DIR=usr/sbin
ETC_DIR=etc
LIB_DIR=usr/lib

# todo��ȷ�����ļ����ƣ��޸�
# !!!!!ע�⣡�������ʵ������޸����¸����������������

# ARM��DSP����FPGA�ļ�
ARM_DSP_FILES="App/LprApp App/MercuryDsp.out App/fpga.xsvf"
# Ӧ�ó����ļ�
APP_FILES=" App/deamon App/command App/log App/fpga_test App/UpgradeApp App/OnvifApp App/RtspApp"
XML_FILES="App/deamon.xml App/LprApp.xml App/command.sh App/fz_songti.ttf"
LOADER="App/loader"

# ����DSP������������ڽű��н��о���
HvCoffReduce="Script/HvCoffReduce"
COFF_DSP="App/MercuryDsp.out"
COFF_TMP="App/HvDsp_tmp.out"

# �����ļ�
SLW_DEV_FILE="Script/slw-dev_mercury.ko"
DRIVER_FILES="Script/cmemk.ko Script/DSPLink.ko Script/musb_hdrc.ko Script/usb-storage.ko"

# �ű�
ADD_FILES="Script/run.sh Script/dm.sh Script/reloadko.sh"
BOOT_FILE="Script/S99hdc"

BIN_FILES="Script/ntpdate"
ETC_FILES="Script/localtime"
LIB_FILES="Script/libcap.so.1 Script/libelf.so.1 Script/libmd5.so.0"

# ��ɾ�����ļ�
DEL_FILES=

######################## ���޸��� ����

##########################################
# ��ɫ����ʾ��Ϣ
COLOR_G="\x1b[0;32m" # green
COLOR_R="\x1b[1;31m" # red
COLOR_C="\x1b[1;35m"
RESET="\x1b[0m"

STR_ERR="[Oops!! Error occurred!! Please see the message upside!!]"
STR_OK="[Job done!]"
STR_FILE="New rootfs: "$NEW_ROOTFS

MSG_ERR=$COLOR_R$STR_ERR$RESET
MSG_OK=$COLOR_G$STR_OK$RESET
MSG_FILE=$COLOR_C$STR_FILE$RESET
###########################################

# ��ʱ����Ŀ¼
TMP=$PWD/tmp

###############################################################

myexit()
{
if [ $1 -eq "1" ]
then
    # ǰ���Ѿ����أ�����Ҫж��
    if [ $2 ]
    then
        #echo "umounting" $TMP "..."
        sleep 1
        sudo umount $TMP
        rm -rf $TMP
    fi
    echo -e $MSG_ERR
    exit;
fi
}

#### ʹ�ð���
if [ $# -eq 1 ] && [ $1 != "debug" ]; then
    echo "usage: "
    echo "release version:" $0
    echo "debug version:" $0 "debug"
    exit
fi

#### ����Ҫ���ļ��Ƿ񶼴���
cd patch

for i in $APP_FILES $XML_FILES $LOADER; do
    if [ ! -f $i ]; then
        echo -e "Error!! Not found" `basename $i`
        exit
    fi
done

if [ $# -eq 1 ] && [ $1 = "debug" ]; then
    for i in $ARM_DSP_FILES; do
        if [ ! -f $i ]; then
            echo -e "Error!! Not found" `basename $i`
            exit
        fi
    done
fi
cd ../

###############################################################

# (��ѹ�ļ�ϵͳ���񣬵õ�initrd.img)
cp -f  $ORG_ROOTFS initrd.img.gz
ret=$?
myexit $ret

gunzip -f initrd.img.gz

# (���ص���ʱĿ¼���粻���ڣ��򴴽�)
if [ ! -d $TMP ]; then mkdir -p $TMP; fi
sudo mount -o loop initrd.img $TMP

cd patch
ret=$?
myexit $ret 1

# (��patchĿ¼��Ӧ�ļ����Ƶ��ļ�ϵͳ��)
echo "copying files ..."
# (��ֹ��Ϊ���ƶ���ʧ��ִ������)
chmod +x * -R

ret=$?
myexit $ret 1

# ��Ҫ���ļ�
for i in $ADD_FILES $LOADER; do
    sudo cp -fp $i $TMP/$OUR_DIR/`basename $i`;
    ret=$?
    myexit $ret 1
done


# ���������ļ�
for i in $DRIVER_FILES; do
    sudo cp -fp $i $TMP/$DRIVER_DIR/`basename $i`
    ret=$?
    myexit $ret 1
done

cp -fp $SLW_DEV_FILE $TMP/$DRIVER_DIR/slw-dev.ko
ret=$?
myexit $ret 1

cp -fp $BOOT_FILE $TMP/$BOOT_DIR/`basename $BOOT_FILE`
ret=$?
myexit $ret 1

for i in $BIN_FILES; do
    sudo cp -fp $i $TMP/$BIN_DIR/`basename $i`
    ret=$?
    myexit $ret 1
done

for i in $ETC_FILES; do
    sudo cp -fp $i $TMP/$ETC_DIR/`basename $i`
    ret=$?
    myexit $ret 1
done

for i in $LIB_FILES; do
    sudo cp -fp $i $TMP/$LIB_DIR/`basename $i`
    ret=$?
    myexit $ret 1
done

for i in $APP_FILES $XML_FILES; do
    sudo cp -fp $i $TMP/$OUR_DIR/`basename $i`
    ret=$?
    myexit $ret 1
done

# ����Ӧ�ó���(�������������)
if [ $# -eq 1 ] && [ $1 = "debug" ]; then

    ## ����DSP����
    $HvCoffReduce $COFF_DSP $COFF_TMP
    sudo mv $COFF_TMP $COFF_DSP
    ret=$?
    myexit $ret 1

    for i in $ARM_DSP_FILES; do
        sudo cp -fp $i $TMP/$OUR_DIR/`basename $i`
        ret=$?
        myexit $ret 1
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
mv initrd.img.gz $NEW_ROOTFS

# (ɾ����Ŀ¼)
rm -rf $TMP

echo -e $MSG_OK
echo -e $MSG_FILE
