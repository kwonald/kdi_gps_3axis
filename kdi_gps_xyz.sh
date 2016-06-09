#!/bin/bash
#My first script 
#
DATE1="$(date +'%m-%d-%Y')"
echo "Started on: " $DATE1
echo "Start script has been started"
#	sudo sleep 30                        #Should be changed to look up if gpsd has been properly recognized/ connected. The next
										# 3 lines aren't guaranteed to run automatically at boot up, thus the delay
	sudo killall gpsd
	sudo gpsd /dev/ttyAMA0 -F /var/run/gpsd.sock
	sudo gcc -o gps_kdi /home/pi/KDI_GPS_AXIS/position_logger.c -lgps -lm -lwiringPi -lconfig
	
	sudo ./gps_kdi& 
#Program ID 
	PID=$!
#Set up FTP 
HOST=192.168.1.20
USER=ivy
PASSWD=n3wm@y2016

	sleep 10

while :
do
	if ping -q -c 1 -W 1 google.com >/dev/null; then
		echo "The network is up"
		DATE2="$(date +'%m-%d-%Y')"
		sudo kill $PID

		cat axisgps_kdi.txt >> ${DATE1}.txt
		ftp -inv $HOST <<-EOF
		user $USER $PASSWD
		cd alex/CANBUS
		put ${DATE1}.txt
		bye
		EOF
		
		if [ "$DATE1" != "$DATE2" ]; then 
			$DATE1="$DATE2"
			yes| rm ${DATE1}.txt
			echo "=======OLD BACK UP FILE WAS REMOVED FROM MEMORY=========="
		fi
		echo "gps_data.txt===================> sent to server"
		
		yes| rm axisgps_kdi.txt
		echo "gps_data.txt ==================> Removed"
		
		
		sudo ./gps_kdi&
		echo "PositionLog.c ==================> Restarted"
		
		sleep 1m
		# This delay is to account for IF a boat is sitting at the dock (connected to internet) for a prolonged 
		# period of time. We don't want multiple files to server with useless data (change from 1m to whatever chosen, for final product)
#		sleep 1m
	else
		echo "The network is down. Do nothing."

	fi

done
