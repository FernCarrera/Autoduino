#pragma once

class Autoduino {

	/*Roll,pitch,heading,height*/
	int* state;
	int* command_state;
	bool init_command;
	//char err[20]
	int* throttle;
	bool* vehicle_armed;

public:

	/*Constructor, takes in the state and command array*/
	Autoduino(int* state_in, int* command_state_in, int* throttle_ino, bool* status_in ) ;
	
	/*Destructor*/
	~Autoduino();

	/*Checks if command can be performed*/
	bool posible();

	/*Changes reference values*/
	//void updateCommand();

	/*Returns a pointer to state array*/
	int* getCurrentState();

	/*Sets reference values to zero and stops when vehicle is stable*/
	void stabilize();

	/*sets altitude in command state*/
	void alt(int altitude);

	/*Moves vehicle forward for X seconds*/
	void forward(int sec);

	/*Moves vehicle backward for X seconds*/
	void backward(int sec);

	/*translates vehicle left for X seconds*/
	void left(int sec);

	/*translates vehicle right for X seconds*/
	void right(int sec);

	/*Equally raises speed of motors*/
	void throttle_up(int sec);

	/*Lands the vehicle if its stable*/
	void land();

	/*Compares state vector to command vector. true if both are the same*/
	bool completedPreviousCommand(); 

	/*Resets command in case of multiple takeoffs and landings*/
	void resetCommand();
	 




};