#!/bin/bash 

cd $HOME/motion-alert/soundsimple/src/
mess="Confirmation: 6, reason:  high confirmation level; "

cmd1="../scripts/send.sh xmpp \"$mess \" " 
echo $cmd1 
read _
eval $cmd1

