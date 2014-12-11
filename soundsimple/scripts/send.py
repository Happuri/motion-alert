#!/usr/bin/python
#-*- coding: utf-8 -*-

import sys
import os 
import time
import logging
#  ./send.py mail  "  Confirmation: 6, reason:  high confirmation level  SIMULATION "  /home/abby/Alarm_data/2014-12-08/12-40-26/2014-12-08_12-40-26.wav
# xmpp " 2014-12-08.12:40:24  ALARM DETECTED: 6 high confirmation level;  <!SIMULATION!> " 

logging.basicConfig(filename='sending.log', level=logging.DEBUG, format='%(asctime)s %(levelname)s %(message)s', datefmt='%d-%m-%Y %R')


def sign(filename) :
	command1 = "echo \"SIGN-NEXTKEY\" >> $HOME/chainsign/fifo"
	print command1
	logging.info(command1)
	os.system(command1) 
	command2 = "echo  \"" + filename + "\" >> $HOME/chainsign/fifo"
	print command2
	logging.info(command2)
	os.system(command2) 

def getPackedFilename(filename) : 
	raw = filename.split(".")
	packed = raw[0]
	packed += ".tar.gz"
	return packed


def waitForFile(packedFile, filename) :
	#exist = True
	exist = False
	print "waiting for " + packedFile
	logging.info("waiting for " + packedFile)

	i = 0
	while exist == False : 
		exist = os.path.isfile(packedFile)
		#print "sleep"
		time.sleep(2) 
		i = i + 1 
		if i > 10 : 
			logging.info("Aborting")
			return filename
			break
	logging.info("found " + packedFile) 
	return packadFile

def sendMail(message, filename) :
	command = "$HOME/PyMailSender/sendmail.py ALARM " + " \"" + message + "\" " + filename 
	print command 
	sftp_cmd = "cp " + filename + " $HOME/sftp " 
	print sftp_cmd 
	logging.info(command) 
	logging.info(sftp_cmd) 
	os.system(sftp_cmd)
	os.system(command)

def sendXMPP(message) : 
	command = "$HOME/motion-alert/soundsimple/scripts/send-xmpp.sh " + " \"" + message + "\" " 
	print command
	os.system(command)
	logging.info(command)

#for a in sys.argv : 
#	print a
message = sys.argv[2] 
logging.info(message + " " + sys.argv[1]) 
if sys.argv[1] == "mail" :
	filename = sys.argv[3]
	packedFile = getPackedFilename(filename)
	logging.info(" file:" + filename + " packed:" + packedFile)
	sign(filename) 
	
	filetosend = waitForFile(packedFile, filename) 
	sendMail(message, filetosend)
	#sendMail(message, filename)
	#getPackedFilename(sys.argv[3]) 
elif sys.argv[1] == "xmpp" : 
	sendXMPP(message) 
else :
	print "error"
