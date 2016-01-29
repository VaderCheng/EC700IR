#!/bin/sh

USERPROG=loader
USERARGS=/dev/swdev

cd /usr/local/signalway

# ˮ��ƽ̨
if [ -x deamon ] && [ -x LprApp ]; then
    ## web server
    if [ -d /usr/sbin/goaheadbin/bin/ ]; then
        /usr/sbin/goaheadbin/bin/webs &
    fi

    ## arp ping
    if [ -x /usr/sbin/ifplugd ]; then
        /usr/sbin/ifplugd -a -d 0 -r /etc/ifplugd.action
    fi
    ./loader
    ret=$?
    if [ $ret -eq "0" ]; then
        ./deamon
    else
        echo "Run loader failed! Will not run deamon!!!"
        echo "Run loader failed! Will not run deamon!!!" >> /var/log/messages
    fi

# ��ʽ��
elif [ ! -x HvDsp.out ] && [ ! -x LprApp ]; then
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
    ret=$?
    if [ $ret -eq "0" ]; then
        ./deamon
    else
        echo "Run loader failed! Will not run deamon!!!"
        echo "Run loader failed! Will not run deamon!!!" >> /var/log/messages
    fi

else
    echo "no program found."
fi
