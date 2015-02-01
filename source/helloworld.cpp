#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <time.h> 
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

using namespace std;


long start;
long end;
bool firstTime = true;
bool sensing = true;
double centi = 0.0;
int a[] = {12,16,20,21};
int speed = 1;

void sensorCallBack() {
	
	if(firstTime) {
		
		if(digitalRead(27) == HIGH) {
			piLock(1);
			start = micros();
			firstTime = false;
		}
	} else {
		if(digitalRead(27) == LOW) {
			end = micros();
			centi = (end - start) * 0.017;
			firstTime = true;
			start = -1;
			end   = -1;
			piUnlock(1);
		}
	}
	
}

PI_THREAD (senseDistance) {
	while (sensing) {
		digitalWrite(17, LOW);
		digitalWrite(17, HIGH);
		delayMicroseconds(10);
		digitalWrite(17, LOW);
		delay(30);
	}
	return NULL;
}

PI_THREAD (controlMotor) {


	return NULL;
}


void setupMOtor()  {
	int a[] = {12,16,20,21};
	for (int i = 0; i < 4; i++) {
		pinMode(a[i], OUTPUT);
	}
}

void Step(int pos)  {
	if( pos == 0 || pos == 4) {
		digitalWrite (a[0], HIGH);
	} else {
		digitalWrite (a[0], LOW);
	}
	if( pos == 1 || pos == 5) {
		digitalWrite (a[1], HIGH);
	} else {
		digitalWrite (a[1], LOW);
	}
	if( pos == 2 || pos == 6) {
		digitalWrite (a[2], HIGH);
	} else {
		digitalWrite (a[2], LOW);
	}
	if( pos == 3 || pos == 7) {
		digitalWrite (a[3], HIGH);
	} else {
		digitalWrite (a[3], LOW);
	}
	
	delay(2 * speed);
}

void HalfStep(int pos) {
	if( pos == 0 || pos == 1 || pos == 7) {
		digitalWrite (a[0], HIGH);
	} else {
		digitalWrite (a[0], LOW);
	}
	if( pos == 1 || pos == 2 || pos == 3) {
		digitalWrite (a[1], HIGH);
	} else {
		digitalWrite (a[1], LOW);
	}
	if( pos == 3 || pos == 4 || pos == 5) {
		digitalWrite (a[2], HIGH);
	} else {
		digitalWrite (a[2], LOW);
	}
	if( pos == 5 || pos == 6 || pos == 7) {
		digitalWrite (a[3], HIGH);
	} else {
		digitalWrite (a[3], LOW);
	}
	
	delay(2 * speed);

}

int main() {

	wiringPiSetupGpio () ;
	pinMode (27, INPUT) ;
	pinMode (17, OUTPUT) ;
	pullUpDnControl (27, PUD_DOWN);
	if(wiringPiISR(27, INT_EDGE_BOTH, &sensorCallBack) < 0) {
        fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno)) ;
        return 1 ;
    }	
    piThreadCreate (senseDistance);
	for(int n = 0; n < 10; n++) {
		delay(10);
		double distance;
		piLock(1);
		distance = centi;
		piUnlock(1);
		cout << "distance = " <<  distance << endl;
		delay(500);
	}
	sensing = false;
	
	setupMOtor();
	for(int i = 0; i < 64*64; i++)  {
		HalfStep(i%8);
	}
	for(int i = 0; i < 64*64; i++)  {
		Step(i%8);
	}
	return 0;
}
