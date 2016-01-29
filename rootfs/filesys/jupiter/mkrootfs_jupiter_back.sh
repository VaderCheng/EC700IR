#!/bin/bash

#
# ����: �������ݸ��ļ�ϵͳ����
# 
# ��ʹ��rootȨ��ִ�б��ű���

# �����ļ������ƽ̨���ã��ô˺궨��
INPUT_DIR=../venus
OUTPUT_DIR=$PWD

OUTPUT_IMAGE=jupiter_rootfs_back.img
OUTPUT_IMGE_GZ=$OUTPUT_IMAGE.gz
ORG_FS_DIR=rootfs_org

MOUNT_DIR=$ORG_FS_DIR/etc/rcS.d

# ԭʼ���ļ�ϵͳ����
ROOTFS_IMG=$INPUT_DIR/rootfs.dm8127.img.gz
#��ʱר��
RFS_TMP=rootfs_tmp.img.gz
RFS_1=rootfs_tmp.img

MOUNT_FILES="backup_sys_app/script/mount_ubifs.sh backup_sys_app/script/umount_ubifs.sh"

#�жϱ�Ҫ���ļ��Ƿ���ڡ���������������ʧ�ܡ�
for i in $MOUNT_FILES; do
    if [ ! -f $i ]; 
	then
        echo -e "Error!! file not found:" $i
        exit
    fi
done

# �����ļ�����ӿ�ִ������
chmod +x backup_sys_app/* -R

chmod +x $INPUT_DIR/bin/*.sh
cd $INPUT_DIR/bin

if [ -d "$ORG_FS_DIR" ]; then
sudo rm -rf $ORG_FS_DIR
fi

#step 1����ѹԭʼ�ļ�ϵͳ 

echo "1. ungunziping img file..."

./ungunzip.sh ../$ROOTFS_IMG $ORG_FS_DIR

#step 2: ����Ҫ���ļ��������ļ�ϵͳ��
echo "2. copying files..."

# ���ɹ̼��汾�ţ��ļ�����Ϊfirmware_ver
./local_ver_build.sh . firmware_ver

# �汾���ļ���������/��Ŀ¼��
mv firmware_ver $ORG_FS_DIR

# ���ؽű�(����ʽϵͳ��ͬ����Ҫ�޸�)
for i in $MOUNT_FILES; do
    sudo cp -fp $OUTPUT_DIR/$i $MOUNT_DIR/`basename $i`;
done

# ������������
echo "3. gziping img file..."
./my_mkrootfs.sh $ORG_FS_DIR $OUTPUT_IMAGE 23552

mv $OUTPUT_IMGE_GZ $OUTPUT_IMAGE

./mkimage -A arm -O linux -T ramdisk -C none -a 0x0 -e 0x0 -n 'rootfs_bak' -d $OUTPUT_IMAGE $RFS_TMP

mv $RFS_TMP $OUTPUT_DIR/$OUTPUT_IMAGE

#umount $ORG_FS_DIR
sudo rm -rf $ORG_FS_DIR
sudo rm -rf $RFS_1
sudo rm -rf $OUTPUT_IMAGE

echo ""
echo "!!!! generate img file: " $OUTPUT_IMAGE "!!!!"
echo ""

cd ../
