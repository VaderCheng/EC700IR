#!/bin/bash

#
# ����: �������ļ�ϵͳ����
# 
# ��ʹ��rootȨ��ִ�б��ű���

# �����ļ������ƽ̨���ã��ô˺궨��
INPUT_DIR=../venus
OUTPUT_DIR=$PWD
#��������
OUTPUT_IMAGE=jupiter_rootfs.img
OUTPUT_IMGE_GZ=$OUTPUT_IMAGE.gz
ORG_FS_DIR=rootfs_org

# ԭʼ���ļ�ϵͳ����
ROOTFS_IMG=$INPUT_DIR/rootfs.dm8127.img.gz

#��ʱר��
RFS_TMP=rootfs_tmp.img

chmod +x $INPUT_DIR/bin/* -R
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

#cp -a ../normal_sys_app/* $NORMAL_SYS_APP_DIR

# ������������23MB - 23552
echo "3. gziping img file..."
./my_mkrootfs.sh $ORG_FS_DIR $OUTPUT_IMAGE 23552

mv $OUTPUT_IMGE_GZ $OUTPUT_IMAGE

./mkimage -A arm -O linux -T ramdisk -C none -a 0x0 -e 0x0 -n 'rootfs' -d $OUTPUT_IMAGE $RFS_TMP

mv $RFS_TMP $OUTPUT_DIR/$OUTPUT_IMAGE

#step 4:ɾ����ʱ�ļ�
echo "4. removing tmp files..."
#umount $ORG_FS_DIR
sudo rm -rf $ORG_FS_DIR
sudo rm -rf $OUTPUT_IMAGE

echo ""
echo "!!!! generate img file: " $OUTPUT_IMAGE "!!!!"
echo ""


cd ../
