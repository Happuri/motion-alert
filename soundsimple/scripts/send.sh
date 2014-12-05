#!/bin/bash 

set -x
via=$1
mess=$2 
file=$3

echo $# $@
echo -e "$via \n  "$mess" \n  $file"

path=$HOME/motion-alert/soundsimple/scripts
source $path/send-mail.sh

echo $mess

if [[ $via == "xmpp"  ]]; then 
    eval  $path/send-xmpp.sh   "\"$mess\""
elif [[ $via == "mail" ]]; then
    sign recordings/$file " \" $mess \""
fi
