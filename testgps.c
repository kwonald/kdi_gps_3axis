#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "gps.h"


int main (void){
	gps_init();
	loc_t data;

	while(1){
		gps_location(&data);
		printf("TimeStamp: %lf lat: %lf long: %lf alt: %lf speed: %lf \n", data.timestamp, data.latitude, 
 				data.longitude, data.altitude, data.speed);
	}

}