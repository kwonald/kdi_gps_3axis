# kdi_gps_3axis
KOBELT DEVELOPMENT INC.

Raspberry Pi GPS/Accelerometer/CANBUS Logging System

Installation Guide 

Written By
Alex Kwon

6/9/2016






ABSTRACT
How to set up Raspberry Pi 3 Model B, Adafruit ADXL345 Accelerometer, Ultimate GPS Breakout, the wiring of these microcontrollers and how to run the main software. 



Index

Hardware and Basic Information…………………………………………………………………………………………………………3-4
Procedure…………………………………………………………………………………………………………………………………………..5-14
Setting up the Raspberry Pi………………………………………………………………………………………………………..5
Configuring your Raspberry Pi……………………………………………………………………………………………………6
Set up Wifi on Raspberry PI ……………………………………………………………………………………………………….7
Wiring ADXL345 to RASBERRY PI……………………………………………………………………………………………….7
Preparing Raspberry Pi for I2C Communication…………………………………………………………………………7
Wiring Adafruit Ultimate GPS Breakout V3………………………………………………………………………….……9
Setting Up Raspberry Pi for HW UART…………………………………………………………………………………..…10
Getting the KDI Data Logging Software……………………………………………………………………………………11
Set Up FTP (File Transfer Protocol)…………………………………………………………………………………………..12 
Set up Crontab for automatic script start…………………………………………………………………………………13
Future Improvements…………………………………………………………………………………………………………………………..13
Acknowledgements ……………………………………………………………………………………………………………………………..13
Appendix ………………………………………………………………………………………………………………………………………...14-18
















What you will require:
Hardware:
•	Computer
•	Ethernet Cable
•	A spare monitor with HDMI or HDMI adaptor
•	A spare keyboard
•	A spare mouse
•	ADXL345 Accelerometer
•	Ultimate GPS Breakout V3
•	Raspberry PI 3 Model B
•	MicroSD adaptor
•	Micro SD with minimum 8GB
•	UART connectors
•	5V, 2.5A micro usb power supply
Software:
All software can be found :
	https://github.com/kwonald/kdi_gps3axis.git
Hardware 
•	Sparkfun 9 Degrees of Freedom Breakout MPU9150 
 The inertial measurement sensor used for this project is the Invensense MPU-9150 with breakout board designed by sparkfun. The MPU-9150 is a multi-chip module consisting of two dies integrated into a single package. One die is MPU-6050 with a 3-axis gyroscope and a 3-axis accelerometer. The other die houses the AK8975 3-axis digital compass/magnetometer from Asahi Kasei Microdevices Corporation. This chip was designed for the low power, low cost, and high performance and currently being using in many places including smartphones and tablets.


•	Adafruit Ultimate GPS Breakout 

 The breakout is built around the third generation MTK3339-based module which has external antenna support and Pulse-Per-Second output. It has an 10 Hz update rate and can track up to 22 satellites on 66 channels. This gps module is built for a embedded system that offers a low power input (3.3 - 5V) and ENABLE pin can be used to turn off the module using any microcontroller pin. There is also a tiny red LED which is capable of indicating the signal status. The LED blinks at 1 Hz while it is searching for satellites and blinks once every 15 seconds when a fix is found. This gps only works at an open field when there is no building around. Whenever it acquires a FIX signal, it will have the course, longitude, latitude and height data updated at user defined frequency.
Communication standards 
•	I2C communication 
I2C stands for Inter-Integrated Circuit Bus. I2C uses microcontroller as a master and connecting with multiple slaves with unique addresses via a single-ended computer bus. It uses only two bidirectional opendrain lines: Serial Data Line (SDA), and a Serial Clock (SCL). SDA sets the transferred bit while SCL is low and the data is received when SCL is high. 
•	UART communication 

UART stands for universal asynchronous receiver/transmitter. It transmits/receives data serially from a byte (5-8 bits) of data written/ stored to a register. All the data will read at certain baud rate predefined by the programer. The UART transmission require three signal types: a Transmit Data (TxD), a Receive Data (RxD), and Signal Ground (SG). Since this kind of transmission is not bidirectional, two separate receive and transmit lines are needed.






Assumptions
This document assumes the reader has some basic understanding of linux navigation, commands such as cd, ls, nano etc., and has an internet connection.
Procedure
Familiarize yourself with the Raspberry PI GPIO configuration for serial communication. The serial pins we will use in this project are the GPIO 2 & 3 for I2C and GPIO 14 & 15 for UART.
 

1.	Setting up the Raspberry PI 
Requires: Working computer, Raspberry Pi, MicroSD (8GB), MicroSD Adaptor
Objective: Format SD card for Raspbian Jessie (OS) for the Raspberry Pi to run on

Follow the instructions set out on the raspberry pi website to get your raspberry pi running
https://www.raspberrypi.org/help/noobs-setup/
By the end of it you should have:
a.	Formatted your SD card using SDFormatter (Overwrite Format option)
b.	Downloaded and moved the NOOB (OS files) onto the formatted SD card
c.	Insert SD card into Raspberry PI 
d.	Hooked Up monitors, keyboards, etc onto raspberry pi
e.	Boot up and installed Raspbian Jessie 

2.	Configuring your Raspberry Pi
Once you have your raspberry pi up and running we need to update and upgrade the system files:
a.	Find and Open Terminal 
b.	Update the system by entering the following command followed by the ENTER key
sudo apt-get update
c.	Once that has completed, upgrade the system by entering the following command (This may take a while)
sudo apt-get upgrade
(When prompted if you wish to continue, enter ‘yes’ or ‘y’ without the ‘’ )

d.	We now have to open up the serial port, as raspberry pi occupies the serial port access by default. Do this by typing the command:
sudo raspi-config
A configuration window will open up (You can change your password, reset your clock to your time zone etc).  Navigate using the arrow keys to Advanced Options (9) -> then to Serial (A7).  Press No to disable access. 
e.	We now have to make I2C connection possible. Go back to the config option:
sudo raspi-config
then navigate to Advanced Options (9) -> then to I2C (A6).  Select <Yes> to enable ARM I2C.
Once finished, press the ‘->’ (right key arrow) to select <Finish> 
f.	Reboot your raspberry pi to enable all your changes
sudo reboot
3.	Set up Wifi on Raspberry PI 3
Raspberry Pi 3 comes with built in wifi capabilities. On the top right, click on the icon with two icons and find your internet/wireless connection and provide it with appropriate password to connect to wifi (At which point, you may disconnect from the ethernet cable). Remember to test your connection by trying to access a website in the browser.
IF you have a Raspberry Pi 2, you need a wifi dongle and need to configure some /etc/ files. Lot of documentation online.
ADXL345
4.	Wiring ADXL345 to RASBERRY PI
Connect the GPIO pins as shown below:

	






RPI GPIOS		ADXL345
Pin 1  (3.3V) -----------------VCC
Pin 3 (SDA) ---------------SDA
Pin 5 (SCL) ----------------SCL
Pin 9 (Ground) ---------- GND
Pin 17 (3.3V) ---------------CS

5.	Preparing Raspberry Pi for I2C Communication
a.	Open terminal and enter the following commands:
sudo apt-get install i2c-tools libi2c-dev
git clone git://git.drogon.net/wiringPi
cd wiringPi
sudo nano /etc/modules
b.	Add the following lines to the bottom of the file (/etc/modules) if they are not already added
snd-bcm2835
i2c-bcm2708
i2c-dev
c.	To exit the text editor, press CTL+X -> then  Y -> then Enter

d.	Next modify the boot parameters to turn i2c on at start. Type the command:
sudo nano /etc/config.txt
e.	Add the following to the bottom of the file
dtparam=i2c1=on
dtparam=i2c_arm=on
f.	To exit the text editor, press CTL+X -> then  Y -> then Enter
g.	Now, reboot: 
sudo reboot
h.	Make a working directory by typing in the following command in the terminal:
mkdir kdi
cd kdi
i.	Download the test file with demos by typing 
git clone git://github.com/richardghirst/PiBits.git
cd PiBits/MPU6050-Pi-Demo
sudo apt-get install libgtkmm-3.0-dev
j.	Some of the source files have to be edited to work with the Pi. We need to modify both I2Cdev.cpp and setup-i2c.sh files. Use the bash commands “nano” to edit the file. 
Change all the references to “/dev/i2c-0” to read “/dev/i2c-1” in this file and save
k.	Enter the following bash commands to compile the source: 
make
./setup-i2c.sh
l.	Wait until the sources has finished compiling, then enter the following bash command: 
sudo i2cdetect -y 1
You should see the following output, showing where your device is connected:
 

m.	Now execute the example program “demo_raw” in this folder by typing the following bash command: 
./demo_raw
This demo will display raw gyro and accel values in terminal a/g: Ax Ay Az Gx Gy Gz
Adafruit Ultimate GPS Breakout V3
6.	 Wiring




RPI GPIOS		GPS
Pin 2  (5V) -----------------VIN
Pin 5 (GND) ---------------GND
Pin 7 (TX) ------------------RX
Pin 9 (RX) ----------------- TX
7.	Setting Up Raspberry Pi for HW UART (This is for RPI3, RPI 2 connections are slightly different)
a.	Need the gps client downloaded on raspberry pi. Type the following in the terminal:
sudo apt-get install gpsd gpsd-clients python-gps
b.	EDIT CMD LINE 
sudo nano /boot/cmdline.txt
And Change 
dwc_otg.lpm_enable=0 console=ttyAMA0,115200 kgdboc=ttyAMA0,115200 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait
to
dwc_otg.lpm_enable=0 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait
(eg. Remove. console=ttyAMA0,115200, and if there is, kgdboc=ttyAMA0,115200 remove that as well)
c.	Edit /boot/config.txt
Sudo nano /boot/config.txt
And Add to the end of the file
core_freq=250
enable_uart=1

d.	Disable tty service: 
sudo systemctl stop serial-getty@ttyAMA0.service
sudo systemctl disable serial-getty@ttyAMA0.service
e.	Raspbian Jessie Service fix- type into the terminal: 
sudo systemctl stop gpsd.socket
sudo systemctl disable gpsd.socket
f.	Reboot Raspberry Pi 
sudo reboot
g.	Restart GPSD with HW UART
sudo killall gpsd
sudo gpsd /dev/ttyAMA0 -F /var/run/gpsd.sock
(Note: in RPI2, it is sudo gpsd /dev/ttyAMA0 –F /var/run/gpsd.sock)
h.	Test the connection
cgps -s
8.	Getting the KDI Data Logging Software
a.	Get the KDI logging software	
git clone https://github.com/kwonald/kdi_gps_3axis.git
cd kdi_gps_3axis

b.	Download the configuration library 
sudo apt-get install libconfig-dev
c.	Enable ping: icmp  So that we can check for internet connection. 
sudo chmod u+s /bin/ping

In raspberry pi 2, we get serial communication through GPIO 14/15 which on Model B, B+ is mapped on UART0; however, on raspberry pi 3, these pins are mapped to UART 1 since UART0 is now used for its new Bluetooth module. UART and baud rate is dependent to clock speed which can alter CPU load, under voltage and temp- thus unstable/reliable. A hack for this is to lower the CPU by force. This is what was done by core_freq=250 in config.txt. We need one more hack to disable Bluetooth and map UART1 back to UART0 (ttyAMA0) so we can talk to emonPi as done in Raspi2. 
d.	
sudo nano /boot/config.txt
e.	Add to the end of the file
dtoverlay=pi3-disable-bt
save and exit file.
f.	We also need to stop BT modem trying to use UART
sudo systemctl disable hciuart
g.	We need to change the execution path of our program so our start up script knows exactly where the position_logger.c program is at. 
nano kdi_gps_xyz.sh
edit the line sudo gcc –o gps_kdi /home/pi/kdi_gps_3axis/position_logger.c … to wherever you decided to store the kdi_gps_3axis directory. You can check your current working directory in linux with the command pwd. So, if you saved the directory in a folder path such as /home/pi/workspace/workthings/kdi_gps_3axis, the .sh file line should read:
sudo gcc –o gps_kdi /home/pi/workspace/workthings/kdi_gps_3axis/position_logger.c …
***Also change the cd path at the top of the script with: workspace/workthings/kdi_gps_3axis/
h.	Need to create gps library
cd libgps
make
sudo make install

9.	Set Up FTP (File Transfer Protocol) for where you want to send your logging data
a.	Install ftp on raspberry pi:
sudo apt-get install ftp
sudo apt-get install ftpd
b.	In kdi_gps_xyz.sh , edit the fields HOST, USER, PASSWD to the remote server you wish to send all your gps data to. 
nano kdi_gps_xyz.sh
HOST=[your remote server address here]
USER=[USER ID for the remote server]
PASSWD=[Password for the remote server]
*Note: there should be no spaces between the ‘=’ and your host name, user, or password. 
Ie. HOST=hostname and NOT HOST=¬_hostname

c.	Choose where you want your file to be stored on your remote server in the line cd right after user $USER $PASSWD. So if my path was ~/alex/CANBUS, I’d put cd alex/CANBUS
d.	Ctrl X -> Y -> Enter to save and exit file. 

10.	Set up the bash script to be executable
chmod +x kdi_gps_xyz.sh
and to execute the file 
./kdi_gps_xyz.sh
11.	Set up Crontab to make the program run on boot up
crontab  -e
Add the following lines to the end of the file
SHELL=/bin/bash
@reboot /home/pi/kdi_gps_3axis/kdi_gps_xyz.sh
or if your file path is different:
SHELL=/bin/bash
@reboot /path/to/the/directory/kdi_gps_xyz.sh

12.	If you want to configure the threshold for when the program logs data, go to threshold.cfg and alter the numbers on the provided file. NOTE: Read the notes provided to understand G-Forces in the file of provided graph and table in Appendix A at the bottom of this document. Also, all threshold data must be of type double.
nano threshold.cfg
For Future Improvements
•	The bash script kdi_gps_xyz.sh needs to check for gps FIX, if it has been done correctly or not. At the moment it waits for 30s on boot up to allow for a GPS fix however a better way would be to have the script check if a fix has been found, if not, sudo killall gpsd  then sudo gpsd /dev/ttyAMA0 –F /var/run/gpsd.sock until FIX is found. 
•	Encrypt password in .sh file so it is not in plain text
•	In rpi3, we disable BT and map UART 1 back to UART 0 to use the ttyAMA0, but in rpi3, as longas we have the baudrate reduced and core frequency reduced, we can use ttyS0. Was unsure how to get the serial port to correctly be used at that point as most tutorials were designed for rpi2. 
•	Write a test script to ensure all necessary functionalities are working. 
•	Remove all old data files. Keep only the current dated file on raspberry pi to minimize memory use.
Acknowledgements
1. Kevin Townsend. Adafruit Ultimate GPS on the Raspberry Pi, 13 June 2016. Web. https://learn.adafruit.com/downloads/pdf/adafruit-ultimate-gps-on-theraspberry-pi.pdf 
2. Walter Dal Mut. UART NMEA GPS library for Raspberry Pi. 08 Sept. 2014. Web.
3. https://github.com/wdalmut/libgps 3. 
4.InvenSense. MPU-9150 Datasheet, 18 Sept. 2013. Web.http://www.invensense.com/mems/gyro/documents/PS-MPU-9150A-00v4_3.pdf
5.Kumar, K. Vasantha, and William T. Norfleet. "Issues on Human Acceleration Tolerance After Long Duration Space Flights." NASA Technical Memorandum 104753 (1992): n. pag. Web. 13 June 2016.
Appendix A
 

 
 
 
 
 
 
 
 
 



