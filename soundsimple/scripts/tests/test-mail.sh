#!/bin/bash 
mess="Confirmation: 6, reason:  high confirmation level; "

ps aux | grep chainsign
cd $HOME/motion-alert/soundsimple/src/
file recordings/2014-12-05_09-05-58.wav

cmd1="../scripts/send.sh mail \"$mess \"  recordings/2014-12-05_09-05-58.wav"
echo -e  $cmd1
read _
eval $cmd1 
# $HOME/PyMailSender/pymailsender.log &
