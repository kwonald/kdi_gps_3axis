#include <stdio.h>
#include <stdlib.h>
#include <wiringPiI2C.h>
#include <errno.h>
#include <math.h>
#include "gps.h"
#include <libconfig.h>
#define pi 3.14159265358979323846

#define DevAddr 0x53 //device address

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Function prototypes                                           :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double deg2rad(double);
double rad2deg(double);
double timestamp_since(double, double);
double distance_since(double, double, double, double, char);
double convert_To_Gs(int);

struct acc_dat{
int x;
int y;
int z;
};
void adxl345_init(int fd)
{
	wiringPiI2CWriteReg8(fd, 0x31, 0x0b);
	wiringPiI2CWriteReg8(fd, 0x2d, 0x08);
	// wiringPiI2CWriteReg8(fd, 0x2e, 0x00);
	wiringPiI2CWriteReg8(fd, 0x1e, 0x00);
	wiringPiI2CWriteReg8(fd, 0x1f, 0x00);
	wiringPiI2CWriteReg8(fd, 0x20, 0x00);

	wiringPiI2CWriteReg8(fd, 0x21, 0x00);
	wiringPiI2CWriteReg8(fd, 0x22, 0x00);
	wiringPiI2CWriteReg8(fd, 0x23, 0x00);
	wiringPiI2CWriteReg8(fd, 0x24, 0x01);
	wiringPiI2CWriteReg8(fd, 0x25, 0x0f);
	wiringPiI2CWriteReg8(fd, 0x26, 0x2b);
	wiringPiI2CWriteReg8(fd, 0x27, 0x00);

	wiringPiI2CWriteReg8(fd, 0x28, 0x09);
	wiringPiI2CWriteReg8(fd, 0x29, 0xff);
	wiringPiI2CWriteReg8(fd, 0x2a, 0x80);
	wiringPiI2CWriteReg8(fd, 0x2c, 0x0a);
	wiringPiI2CWriteReg8(fd, 0x2f, 0x00);
	wiringPiI2CWriteReg8(fd, 0x38, 0x9f);
}

struct acc_dat adxl345_read_xyz(int fd)
{
	char x0, y0, z0, x1, y1, z1;
	struct acc_dat acc_xyz;
	x0 = 0xff - wiringPiI2CReadReg8(fd, 0x32);
	x1 = 0xff - wiringPiI2CReadReg8(fd, 0x33);
	y0 = 0xff - wiringPiI2CReadReg8(fd, 0x34);
	y1 = 0xff - wiringPiI2CReadReg8(fd, 0x35);
	z0 = 0xff - wiringPiI2CReadReg8(fd, 0x36);
	z1 = 0xff - wiringPiI2CReadReg8(fd, 0x37);
	acc_xyz.x = (int)(x1 << 8) + (int)x0;
	acc_xyz.y = (int)(y1 << 8) + (int)y0;
	acc_xyz.z = (int)(z1 << 8) + (int)z0;
	return acc_xyz;
}


int main(void) {
    // Open
	printf("KDI 3AXIS & GPS PROGRAM INITIATED\n");
	
	// Set up log file 
	 

	// ================Set up Config file=====================
	  config_t cfg;               /*Returns all parameters in this structure */
    config_setting_t *setting;
    const char *str1, *str2;
    double distanceThreshold;
    double timestampThreshold;
    double posGzThreshold;
    double negGzThreshold;
    double posGxThreshold;
    double negGxThreshold;
    double posGyThreshold;
    double negGyThreshold;
 
    char *config_file_name = "threshold.cfg";
 
    /*Initialization */
    config_init(&cfg);
 
    /* Read the file. If there is an error, report it and exit. */
    if (!config_read_file(&cfg, config_file_name))
    {
        printf("\n%s:%d - %s \n", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        return -1;
    }
 
    /*Read the parameter group*/
    setting = config_lookup(&cfg, "threshold");
    if (setting != NULL)
    {

        /*Read the integer*/
        config_setting_lookup_float(setting, "Distance", &distanceThreshold);
        config_setting_lookup_float(setting, "Timestamp", &timestampThreshold);
        config_setting_lookup_float(setting, "PosGz", &posGzThreshold);
        config_setting_lookup_float(setting, "NegGz", &negGzThreshold);
        config_setting_lookup_float(setting, "PosGx", &posGxThreshold);
        config_setting_lookup_float(setting, "NegGx", &negGxThreshold);
        config_setting_lookup_float(setting, "PosGy", &posGyThreshold);
        config_setting_lookup_float(setting, "NegGy", &negGyThreshold);
        printf("\ndistance set at: %lf", distanceThreshold);
        printf("\ntimestamp set at: %lf", timestampThreshold);
        printf("\nGz+ set at: %lf", posGzThreshold);
        printf("\nGz- set at: %lf", negGzThreshold);
        printf("\nGx+ set at: %lf", posGxThreshold);
        printf("\nGx- set at: %lf", negGxThreshold);
        printf("\nGy+ set at: %lf", posGyThreshold);
        printf("\nGy- set at: %lf", negGyThreshold);
        printf("\n\n>>Configuration complete<< \n\n");
    }

    // ==== END OF CONFIG FILE SET UP ====

	// Set up gps 
    gps_init();
    loc_t data;
   	loc_t prev_data;
	printf("-----> GPS INITIALIZED\n");
	
    // Set up Accelerometer
    int fd;
	struct acc_dat acc_xyz;
	fd = wiringPiI2CSetup(DevAddr);

	if(-1 == fd){
		perror("I2C device setup error"); 
	}
	adxl345_init(fd);
	printf("-----> ACCELEROMETER INITIALIZED\n");
    
// TODO:
	// In This script: 
	// 		set up fprintf to run only when threshold reached.   (20 minutes)
	//
	// BASH SCRIPT WORK:
	//		Look up of internet in the bash script file must change to once a minute (10 minutes)
	// 		upload file in bash script has to change 	(10 minutes)
	//
	// GENERAL:  (Rest of the day)
	//		what does the accelerometer readings mean (why 65000)
	//		how to distinguish +/- axes 

double acc_x;
double acc_y;
double acc_z;

	gps_location(&prev_data);
    while (1) { 
      FILE *fp;
      fp = fopen("/home/pi/KDI_GPS_AXIS/axisgps_kdi.txt", "a");
      gps_location(&data);
      acc_xyz = adxl345_read_xyz(fd);
 	  
      acc_x = convert_To_Gs(acc_xyz.x);
      acc_y = convert_To_Gs(acc_xyz.y);
      acc_z = convert_To_Gs(acc_xyz.z);
        

      // printf("\n\n===========================\n\n");
      // printf("TEST VALUES\n");
      // printf("\ndistance set at: %lf", distanceThreshold);
      // printf("\ntimestamp set at: %lf", timestampThreshold);
      // printf("\nGz+ set at: %lf", posGzThreshold);
      // printf("\nGz- set at: %lf", negGzThreshold);
      // printf("\nGx+ set at: %lf", posGxThreshold);
      // printf("\nGx- set at: %lf", negGxThreshold);
      // printf("\nGy+ set at: %lf", posGyThreshold);
      // printf("\nGy- set at: %lf", negGyThreshold);
      // printf("\n");
      // printf("current data\n");
      printf("TimeStamp: %lf lat: %lf long: %lf alt: %lf speed: %lf x: %lf y: %lf z: %lf\n", data.timestamp, data.latitude, 
          data.longitude, data.altitude, data.speed, acc_x, acc_y, acc_z);
      // printf("previous data\n");
      // printf("TimeStamp: %lf lat: %lf long: %lf alt: %lf speed: %lf x: %lf y: %lf z: %lf\n", prev_data.timestamp, prev_data.latitude, 
      //     prev_data.longitude, prev_data.altitude, prev_data.speed, acc_x, acc_y, acc_z);
      // printf("\n");
      // printf("DISTANCE DIFFERENCE: %lf", distance_since(data.latitude, data.longitude, prev_data.latitude, prev_data.longitude, 'K'));
          


      // printf("\n\n===========================\n\n");


     if(timestampThreshold <= timestamp_since(data.timestamp, prev_data.timestamp) || distanceThreshold <= distance_since(data.latitude, data.longitude, prev_data.latitude, prev_data.longitude, 'K') || posGzThreshold <= acc_z || 
     	negGzThreshold <= acc_z || posGxThreshold <= acc_x || negGxThreshold <= acc_x || 
     	posGyThreshold <= acc_y || negGyThreshold <= acc_y)
     {
        printf("\n\n**********DATA ENTRY***************\n\n");
        fprintf(fp, "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf\n", data.timestamp, data.latitude, data.longitude, data.altitude, 
        	data.speed, acc_x, acc_y, acc_z);       
	
 			  printf("TimeStamp: %lf lat: %lf long: %lf alt: %lf speed: %lf x: %lf y: %lf z: %lf\n", data.timestamp, data.latitude, 
   				data.longitude, data.altitude, data.speed, acc_x, acc_y, acc_z);
   			
        prev_data = data; 
      }
	   fclose(fp);
   }

    
    config_destroy(&cfg);
    return EXIT_SUCCESS;
}


double timestamp_since(double curr_timestamp, double old_timestamp){
  double diff; 
  diff = curr_timestamp - old_timestamp;

  return diff; 
}
double distance_since(double lat1, double lon1, double lat2, double lon2, char unit) {
  double theta, dist;
  theta = lon1 - lon2;
  dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
  dist = acos(dist);
  dist = rad2deg(dist);
  dist = dist * 60 * 1.1515;
  switch(unit) {
    case 'M':
      break;
    case 'K':
      dist = dist * 1.609344;
      break;
    case 'N':
      dist = dist * 0.8684;
      break;
  }
  return (dist);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts decimal degrees to radians             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double deg2rad(double deg) {
  return (deg * pi / 180);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts radians to decimal degrees             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double rad2deg(double rad) {
  return (rad * 180 / pi);
}

double convert_To_Gs(int acc_data){
	double convertedData;
  double scale = 255;   // convert the LSB to G force by dividing LSB by 1111 1111  	
  double data = (double)acc_data;

  // now in this if statement, find where sign extension ends  because we know x1 is the MSB
  if(data >= 61440){          //61440 = 1111 0000 0000 0000
    if(data >= 63488){        //63488 = 1111 1000 0000 0000
      if(data >= 64512){      //64512 = 1111 1100 0000 0000
        if(data >= 65024){    //65024 = 1111 1110 0000 0000
          if(data >= 65280){  //65280 = 1111 1111 0000 0000  
            convertedData = data - 65280;
            return convertedData / scale; 
          }
          convertedData = data - 65024;
          return convertedData / scale;
        }
        convertedData = data - 64512;
        return convertedData / scale;
      }
      convertedData = data - 63488;
      return convertedData / scale;
    } 
    convertedData = data - 61440;
    return convertedData / scale;  
  }

  else{
    if(data == 0){return data;}
    convertedData = -1*data/ scale;   // convert the LSB to negative 
    return convertedData;
  }

}
