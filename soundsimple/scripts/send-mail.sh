#!/bin/bash 
set -x

function wait_for_file() { 
    file=$1 

    while [ ! -f $file ] ; do
        sleep 2
    done
}

function send() { 
    mess=$1 
    rec=$2 
    echo "sending $mess"
    cd $HOME/PyMailSender
    ./sendmail.py "ALARM"  "$mess" $rec
}


function sign() { 
    file=$1 
    mess=$2 
    echo $mess
    raw=$(echo $file  |  awk -F\/ '{print $2}') 
    echo $raw 
#    read _ 
    pack=$(echo $raw  |  awk -F\. '{print $1}')

    cd $HOME/chainsign
    ./chainsign_cmd SIGN-NEXTKEY
    ./chainsign_cmd $HOME/motion-alert/soundsimple/src/$file

    wait_for_file $HOME/chainsign/$pack.tar.gz
    send "\"$mess\"" $HOME/chainsign/$pack.tar.gz
}

