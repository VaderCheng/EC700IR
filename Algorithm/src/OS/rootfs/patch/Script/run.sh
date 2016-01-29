#!/bin/sh
## �ļ����ƣ�run.sh
## ���ܣ�����Ӧ�ó��򣬱��ļ�����һ������Ӷˡ�����ĳ�������

## ���԰汾(��Ҫ��ARM��DSP�˳���ŵ�/usr/local/signalwayĿ¼)��
## ���ˣ�
## 1��loaderĬ�ϻ��Զ���ȡͬһĿ¼��fpga.xsvf�ļ�
## 2��CamAppĬ�ϻ��Զ���ȡͬһĿ¼��CamDsp.out�ļ�
## �Ӷ�(����)��
## LprAppĬ�ϻ��Զ���ȡͬһĿ¼��HvDsp.out
##

## ��ʽ�汾��
## ��loader��Flash��ȡ����

USERPROG=loader
USERARGS=/dev/swdev

cd /usr/local/signalway

# һ�����
if [ -x CamApp ]; then
    ## web server
    if [ -d /usr/sbin/goaheadbin/bin/ ]; then
        /usr/sbin/goaheadbin/bin/webs &
    fi

    ## arp ping
    if [ -x /usr/sbin/ifplugd ]; then
        /usr/sbin/ifplugd -a -d 0 -r /etc/ifplugd.action
    fi
    ./loader
    ./CamApp

# һ����ӣ�����
elif [ -x LprApp ]; then
    ./LprApp

# ��ʽ��
elif [ ! -x CamApp ] && [ ! -x LprApp ] && [ -x loader ]; then
    ## web server
    if [ -d /usr/sbin/goaheadbin/bin/ ]; then
        /usr/sbin/goaheadbin/bin/webs &
    fi

    ## arp ping
    if [ -x /usr/sbin/ifplugd ]; then
        /usr/sbin/ifplugd -a -d 0 -r /etc/ifplugd.action
    fi
    echo -n "Starting $USERPROG ... "
    ##Run user program
    ./$USERPROG $USERARGS
else
    echo "no program found."
fi
