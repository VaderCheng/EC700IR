#!/bin/bash

# �ļ����ƣ�mkrootfs_200w.sh
# ���ܣ�200wһ������ļ�ϵͳ���������ű�
# ʹ�ã�
# ��rootȨ�����У�ubuntu����sudo ./mkrootfs_200w.sh

# Ĭ��������ʽ�汾�����ļ�
# ���������԰汾(����������)��ʹ�����
# ./mkrootfs_200w.sh debug

# ��ʱ����Ŀ¼
TMP=tmp

# �ļ�ϵͳָ��Ŀ¼
OUR_DIR=usr/local/signalway
DRIVER_DIR=lib/modules
BOOT_DIR=etc/rc.d/rc3.d

# ����Ӧ�ó����ļ�
APP_MASTER="App/CamApp App/CamDsp.out App/fpga.xsvf"
APP_SLAVE="App/LprApp App/HvDsp.out"
LOADER="App/loader"

# �����ļ�
SLW_DEV_FILE="Script/slw-dev_200w.ko"
DRIVER_FILES="Script/cmemk.ko Script/DSPLink.ko  \
              Script/slw-pci.slave.ko Script/slw-pci.master.ko"
# �ű�
ADD_FILES="Script/run.sh Script/dm.sh Script/reloadko.sh"
ADD_KO_MASTER="Script/loadko_pci_master.sh"
ADD_KO_SLAVE="Script/loadko_pci_slave_200w.sh"
BOOT_FILE="Script/S99hdc"

# ������ʱ����WEB
DEL_FILES_M="usr/sbin/goaheadbin/"

# �Ӷ������磬ɾ����ص��ļ�
DEL_FILES="usr/sbin/goaheadbin/ usr/sbin/ifplugd"

# ����DSP������������ڽű��н��о���(������������汾)
HvCoffReduce="Script/HvCoffReduce"
COFF_DSP1="App/CamDsp.out"
COFF_TMP1="App/CamDsp_tmp.out"
COFF_DSP2="App/HvDsp.out"
COFF_TMP2="App/HvDsp_tmp.out"

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
if [ ! -f $LOADER ]; then
    echo "Error!! Not found" `basename $LOADER`
    exit
fi
if [ $# -eq 1 ] && [ $1 = "debug" ]; then
    for i in $APP_MASTER $APP_SLAVE; do
        if [ ! -f $i ]; then
            echo "Error!! Not found" `basename $i`
            exit
        fi
    done
fi
cd ../

#################################################����

# (��ѹ�ļ�ϵͳ���񣬵õ�initrd.img)
cp -f initrd.signalway.img.gz initrd.img.gz
gunzip -f initrd.img.gz

# (���ص���ʱĿ¼���粻���ڣ��򴴽�)
if [ ! -d $TMP ]; then mkdir -p $TMP; fi
sudo mount -o loop initrd.img $TMP

cd patch

# (��patchĿ¼��Ӧ�ļ����Ƶ��ļ�ϵͳ��)
echo "copying master files ..."
chmod +x * -R
for i in $LOADER $ADD_FILES $ADD_KO_MASTER; do
    sudo cp -fp $i ../$TMP/$OUR_DIR/`basename $i`
done

# ���������ļ�
for i in $DRIVER_FILES; do
    sudo cp -fp $i ../$TMP/$DRIVER_DIR/`basename $i`
done

cp -fp $SLW_DEV_FILE ../$TMP/$DRIVER_DIR/slw-dev.ko

cp -fp $BOOT_FILE ../$TMP/$BOOT_DIR/`basename $BOOT_FILE`

if [ $# -eq 1 ] && [ $1 = "debug" ]; then

    ## ����DSP����
    $HvCoffReduce $COFF_DSP1 $COFF_TMP1
    sudo mv $COFF_TMP1 $COFF_DSP1

    for i in $APP_MASTER; do
        sudo cp -fp $i ../$TMP/$OUR_DIR/`basename $i`
    done
    echo "master: debug version"
fi

cd ../

####

mv $TMP/$OUR_DIR/`basename $ADD_KO_MASTER` \
   $TMP/$OUR_DIR/loadko.sh

# (ɾ��)
for i in $DEL_FILES_M; do
    sudo rm -rf $TMP/$i
done

# (��ֹж��ʱ�����豸æ����)
sleep 1

# (ж����ʱĿ¼)
sudo umount $TMP

# (ѹ��������initrd.img.gz)
gzip -fv9 initrd.img
mv initrd.img.gz initrd.img.gz.master

#################################################�Ӷ�

# (��ѹ�ļ�ϵͳ���񣬵õ�initrd.img)
cp -f initrd.signalway.img.gz initrd.img.gz
gunzip -f initrd.img.gz

# (���ص���ʱĿ¼���粻���ڣ��򴴽�)
if [ ! -d $TMP ]; then mkdir -p $TMP; fi
sudo mount -o loop initrd.img $TMP

cd patch

# (��patchĿ¼��Ӧ�ļ����Ƶ��ļ�ϵͳ��)
echo "copying slave files ..."
chmod +x * -R
for i in $LOADER $ADD_FILES $ADD_KO_SLAVE; do
    sudo cp -fp $i ../$TMP/$OUR_DIR/`basename $i`
done

# ���������ļ�
for i in $DRIVER_FILES; do
    sudo cp -fp $i ../$TMP/$DRIVER_DIR/`basename $i`
done

cp -fp $SLW_DEV_FILE ../$TMP/$DRIVER_DIR/slw-dev.ko

if [ $# -eq 0 ]; then
    cp -fp $BOOT_FILE ../$TMP/$BOOT_DIR/`basename $BOOT_FILE`
fi

if [ $# -eq 1 ] && [ $1 = "debug" ]; then

    ## ����DSP����
    $HvCoffReduce $COFF_DSP2 $COFF_TMP2
    sudo mv $COFF_TMP2 $COFF_DSP2

    for i in $APP_SLAVE; do
        sudo cp -fp $i ../$TMP/$OUR_DIR/`basename $i`
    done
    rm -rf ../$TMP/$OUR_DIR/`basename $LOADER`
    echo "slave: debug version"
fi

cd ../

mv $TMP/$OUR_DIR/`basename $ADD_KO_SLAVE` \
   $TMP/$OUR_DIR/loadko.sh

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
mv initrd.img.gz initrd.img.gz.slave

# (ɾ����ʱĿ¼)
rm -rf $TMP

echo "Job done!"
