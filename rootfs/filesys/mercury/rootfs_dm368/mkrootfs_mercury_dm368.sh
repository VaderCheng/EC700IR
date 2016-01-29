#!/bin/bash

# �ļ����ƣ�mkrootfs_mercury_dm368.sh
# ���ܣ�ˮ��ƽ̨�ļ�ϵͳ���������ű�
# ���ߣ�lijj 2013-04

# �޸���֪���ο��ű��ķ���������ӡ�ɾ���ļ�

# ʹ�ã�
# ����������ʹ��rootȨ�����У�����
# ��ʽ�汾�����ļ���
# ./mkrootfs_mercury_dm368.sh
# ���԰汾(����������)
# ./mkrootfs_mercury_dm368.sh debug

##########################################################

######################## ���޸��� ��ʼ

# ԭ���ļ�ϵͳ����
ORG_ROOTFS="rootfs_dm368.17MB.img.gz"
# �¸��ļ�ϵͳ����
NEW_ROOTFS="rootfs_dm368.img.gz.mercury"

# �ļ�ϵͳָ��Ŀ¼
OUR_DIR=home/root
DRIVER_DIR=lib/modules
BOOT_DIR=etc/rc5.d

# todo��ȷ�����ļ����ƣ��޸�
# !!!!!ע�⣡�������ʵ������޸����¸����������������
# Ӧ�ó����ļ�
APP_FILES="App/DM368CodecApp App/mFont.ttf Script/run.sh"

# �����ļ�
SLW_DEV_FILE="Script/mp_dev.ko"
#DRIVER_FILES="Script/cmemk.ko Script/DSPLink.ko Script/musb_hdrc.ko Script/usb-storage.ko"

# �ű�
BOOT_FILE="Script/S99hdc"

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

#### ����Ҫ���ļ��Ƿ񶼴���
cd patch
    for i in $APP_FILES; do
        if [ ! -f $i ]; then
            echo -e "Error!! Not found" `basename $i`
            exit
        fi
    done
	for i in $BOOT_FILE; do
        if [ ! -f $i ]; then
            echo -e "Error!! Not found" `basename $i`
            exit
        fi
    done
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

for i in $BOOT_FILE; do
    sudo cp -fp $i $TMP/$BOOT_DIR/`basename $i`;
    ret=$?
    myexit $ret 1
done

for i in $APP_FILES; do
    sudo cp -fp $i $TMP/$OUR_DIR/`basename $i`
    ret=$?
    myexit $ret 1
done

cp -fp $SLW_DEV_FILE $TMP/$DRIVER_DIR
ret=$?
myexit $ret 1

cd ../

# (ɾ��)
for i in $DEL_FILES; do
    sudo rm -rf $TMP/$i
done

# (��ֹж��ʱ�����豸æ����)
sleep 1

# (ж����ʱĿ¼)
sudo umount $TMP
ret=$?
myexit $ret 1

# (ѹ��������initrd.img.gz)
gzip -fv9 initrd.img
mv initrd.img.gz $NEW_ROOTFS

# (ɾ����Ŀ¼)
rm -rf $TMP
ret=$?
myexit $ret

echo -e $MSG_OK
echo -e $MSG_FILE
