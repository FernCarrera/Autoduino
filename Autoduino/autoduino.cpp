#include "autoduino.h"
#include <Arduino.h>

#define MIN_ALT 2 //2cm min Ultrasonic sensor read

/*Constructor*/
Autoduino::Autoduino(int* state_in, int* command_state_in, int* throttle_ino, bool* status_in) {

	state = state_in;
	command_state = command_state_in;
	throttle = throttle_ino;
	vehicle_armed = status_in;
	init_command = false;

}
/*Destructor*/
Autoduino::~Autoduino(){}

/*Checks if roll & pitch are +-3 degrees from zero*/
bool Autoduino::stable() {
	
	if ((*state && *(state + 1)) <= 3 && (*state && *(state + 1)) >= -3 ) { return true; }
	else {return false;}



}

/*Sets roll,pitch & heading ref values to zero*/
void Autoduino::stabilize() {

	*command_state = 0;
	*(command_state+1) = 0;
	//*(command_state + 2) = 0;

}

/*Returns pointer to first index of state vector*/
int* Autoduino::getCurrentState() {
	return state;
}

/*Changes command state to pitch down slightly and throttle*/
void Autoduino::forward(int sec) {

	if (stable() && completedPreviousCommand()) {
		*(command_state + 1) = -15; //pitch down 15 degrees
		//delay(sec)
		stabilize();
	}
	



}

/*Moves vehicle backward for X seconds*/
void Autoduino::backward(int sec) {

	if (stable() && completedPreviousCommand()) {
		*(command_state + 1) = 15; //pitch up 15 degrees
		//delay(sec)
		stabilize();
	}


}

/*translates vehicle left for X seconds*/
void Autoduino::left(int sec) {

	if (stable() && completedPreviousCommand()) {
		*(command_state) = 15; //roll 15 degrees
		delay(sec*1000);
		stabilize();
	}
	else { left(sec); }


}

/*translates vehicle right for X seconds*/
void Autoduino::right(int sec) {

	if (stable() && completedPreviousCommand()) {
		*(command_state) = -15; //roll 15 degrees
		delay(sec*1000);
		stabilize();
	}
	else { right(sec); }

	

}


/*Changes altitude in command state. Checks if quad has had an initial takeoff*/
void Autoduino::alt(int altitude) {
	// maybe add extra initialization of command state? Set all states to  zero?
	if (init_command == false) { 
		
		resetCommand();
		*(command_state + 3) = altitude;	//Convert to cm
		init_command = true;

	} // launch condition
	
	else if (stable() && completedPreviousCommand()) {
		*(command_state+3) = altitude;
	}

}

void Autoduino::land() {
	if (stable() && completedPreviousCommand()) {
		*(command_state + 3) = MIN_ALT;
	}
	if (stable() && completedPreviousCommand()) {
		vehicle_armed = false;	// turns off motors
		init_command = false;
		return;
	}
	//else { land(); }	//recursive call a good idea?
	

}


/*Throttles up for a specified amoount of time*/
void Autoduino::throttle_up(int sec) {
	*throttle = 100;
	delay(sec * 1000);
	*throttle = 0;

}

/*Compares states to check if previous command has been completed*/
bool Autoduino::completedPreviousCommand() {
	
	if (stateInRange()) { return true; }	
	else {return false;}

}

/*resets command vector to zero*/
void Autoduino::resetCommand() {

	for (int i = 0; i < 4; i++) {
		*(command_state + i) = 0;
	}

}

/* Used in ALt_hold, checks if current altitude is within acceptable error
*	returns true if altitude is within range
*/
bool Autoduino::altInRange() {
	if (*(state + 3) <= 1.1(*(command_state + 3)) && *(state + 3) >= 0.9 * (*(command_state + 3))) {
		return true;
	}
	else { return false; }


}

/*Checks if states are within specific error to verify that a maneuver has been completed
* +-10% error
*/
bool Autoduino::stateInRange() {
	int count = 0;
	for (int i = 0; i < 4; i++) {
		if (*(state + i) <= 1.1(*(command_state + i)) && *(state + i) >= 0.9 * (*(command_state + i))) {
			count++;
		}
	if (count == 4) { return true }
	else { return false; }
	}


}


/* Checks to see if vehicle is in specified range then makes small throttle corrections 
 * to keep vehicle altitude closer to 
* Future version could use a rate of change computation to control climb speed
* this function might need to operate independently outside of mission_command functions
* ie: call this at begining of program letting the system know that you want this feature
*/
void Autoduino::altHold(bool active) {
	if (active) {
		if (altInRange() && (*(state + 3) < *(command_state + 3))) { *throttle += 25; }
		else if (altInRange() && (*(state + 3) > * (command_state + 3))) { *throttle += -25; }
	}
	else { return; }

}
