#include "gpio_lib.h"
#include "main.h"
#define SERVO_PIN PIN_PG0
#define PIN_OUTPUT 1

#define NANO_SECOND_MULTIPLIER  1000000  // 1 millisecond = 1,000,000 Nanoseconds
//#const long INTERVAL_MS = 500 * NANO_SECOND_MULTIPLIER;
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <sys/time.h>

void main (int argc, char **argv)
{
	int pulseDelay=1000;
	if (argc >= 2) {
		pulseDelay = atoi(argv[1]);
	}
	printf("%d", pulseDelay);
	fflush(stdout);
	//return;
	
	// We'll operate on the currently running thread.
    pthread_t this_thread = pthread_self();
	// struct sched_param is used to store the scheduling priority
	struct sched_param params;
	// We'll set the priority to the maximum.
	params.sched_priority = sched_get_priority_max(SCHED_FIFO);
	// Attempt to set thread real-time priority to the SCHED_FIFO policy
	int ret = pthread_setschedparam(this_thread, SCHED_FIFO, &params);
	if (ret != 0) {
		// Print the error
		//std::cout << "Unsuccessful in setting thread realtime prio" << std::endl;
		return;
	}
	
	
	
	sunxi_gpio_init();
	sunxi_gpio_set_cfgpin(SERVO_PIN, PIN_OUTPUT);
	
	
	int callpointer = sunxi_gpio_getCallPointer(SERVO_PIN);
	int pindef = sunxi_gpio_getPindef(SERVO_PIN);
	
	
	
	const long frameLenght = 1000 * NANO_SECOND_MULTIPLIER;
	
	struct timespec onInterval = {0};
	struct timespec offInterval = {0};
	
	onInterval.tv_nsec = pulseDelay * 1000;
	
	
	//sunxi_gpio_output(SERVO_PIN, 1);
	
	
	/*
	while(1){
	*/
	int i;
	struct timeval  tStart, tStop, tLen;
	//for(i = 0;i<50;i++){
	while(1){
		gettimeofday (&tStart, NULL) ;
		sunxi_gpio_setFastOn(callpointer,pindef);
		// Berechnen wie lang man den Pulse einschalten sollte:
		gettimeofday (&tStop, NULL);
		timersub(&tStop,&tStart,&tLen);
		onInterval.tv_nsec = pulseDelay * 1000;;
		onInterval.tv_nsec -= tLen.tv_usec * 1000 ;
		
		nanosleep(&onInterval, NULL);
		
		
		sunxi_gpio_setFastOff(callpointer,pindef);
		//sunxi_gpio_output(SERVO_PIN, 0);
		// Berechnen wie lang noch schlafen:
		gettimeofday (&tStop, NULL);
		// TimesStop - TimeStart   = TLen
		timersub(&tStop,&tStart,&tLen);
		offInterval.tv_nsec = frameLenght;
		offInterval.tv_nsec -= tLen.tv_usec * 1000 ;
		
		
		
		nanosleep(&offInterval, NULL);
		//return;
		//
		//nanosleep(&offInterval, NULL);	
		
	}
  
  
}