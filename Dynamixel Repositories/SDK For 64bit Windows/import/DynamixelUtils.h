// Handy utilities for controlling Dynamixel motors through USB. by Shervin Emami 3/2/09.

#ifndef DYNAMIXEL_UTILS_H
#define DYNAMIXEL_UTILS_H

//// Force a static link to the USB2Dynamixel library
//#pragma comment(lib, "dynamixel.lib")


#ifndef NUM_RETRIES_FOR_READING_DYNAMIXEL
#define NUM_RETRIES_FOR_READING_DYNAMIXEL	10	// Check each port 10 times before saying that its unreadable.
#endif


// Windows version
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <process.h>    /* Needed for timeout thread functions: _beginthread() & _endthread() */


//------ Control table address for USB2Dynamixel ------
// Settings of Motor (stored in Motor's ROM)
#define P_MODEL_NUMBER_L		0	// Lowest byte of Model Number (Read, should be 28 or 64 for RX-28 or RX-64 motors)
#define P_MODEL_NUMBER_H		1	// Highest byte of Model Number (Read, should be 0 for most motors)
#define P_FIRMWARE_VERSION		2	// (Read)
#define P_MOTOR_ID				3	// ID of this motor (Read or Write, 0 - 252)
#define P_BAUDRATE				4	// Baudrate number, typically 1 or 34 (Read or Write, 1 - 255)
#define P_RETURN_DELAY_TIME		5	// Typically 0 or 250 (Read or Write, 0 - 255)
#define P_CW_ANGLE_LIMIT_L		6	// Lowest byte of Clockwise Angle Limit (Read or Write, 0 - 1023)
#define P_CW_ANGLE_LIMIT_H		7	// Highest byte of Clockwise Angle Limit (Read or Write, 0 - 1023)
#define P_CCW_ANGLE_LIMIT_L		8	// Lowest byte of CounterClockwise Angle Limit (Read or Write, 0 - 1023)
#define P_CCW_ANGLE_LIMIT_H		9	// Highest byte of CounterClockwise Angle Limit (Read or Write, 0 - 1023)
#define P_MAX_TEMP				11	// in degrees Celcius (Read or Write, 0 - 255)
#define P_MIN_VOLTAGE			12	// 10 x Minimum voltage to motor before it sets of Alarm. (Read or Write, 0 - 255)
#define P_MAX_VOLTAGE			13	// 10 x Maximum voltage to motor before it sets of Alarm. (Read or Write, 0 - 255)
#define P_MAX_TORQUE_L			14	// Initial Max Torque (Read or Write, 0 - 1023)
#define P_MAX_TORQUE_H			15	// Initial Max Torque (Read or Write, 0 - 1023)
#define P_STATUS_RETURN_LEVEL	16	// When should data be sent back from the motors (Read or Write, 0 - 2)
#define P_ALARM_LED				17	// Flags for enabling the LED on errors (Read or Write, 0 - 255)
#define P_ALARM_SHUTDOWN		18	// Flags for disabling the motor on errors (Read or Write, 0 - 255)
// Runtime values of Motor (stored in Motor's RAM)
#define P_TORQUE_ENABLE			24	// Enable or Disable Motor Torque (Read or Write, 0 or 1)
#define P_LED_STATUS			25	// (Read or Write, 0 or 1)
#define P_GOAL_POSITION_L		30	// Lowest byte of desired servo Goal Position (Read or Write, 0 - 1023)
#define P_GOAL_POSITION_H		31	// Highest byte of desired servo Goal Position (Read or Write, 0 - 1023)
#define P_GOAL_SPEED_L			32	// Lowest byte of desired servo Goal Speed (Read or Write, 0 - 1023)
#define P_GOAL_SPEED_H			33	// Highest byte of desired servo Goal Speed (Read or Write, 0 - 1023)
#define P_TORQUE_LIMIT_L		34	// Current Max Torque (Read or Write, 0 - 1023)
#define P_TORQUE_LIMIT_H		35	// Current Max Torque (Read or Write, 0 - 1023)
#define P_PRESENT_POSITION_L	36	// Lowest byte of actual servo Position (Read, 0 - 1023)
#define P_PRESENT_POSITION_H	37	// Highest byte of actual servo Position (Read, 0 - 1023)
#define P_PRESENT_SPEED_L		38	// Lowest byte of actual servo Speed (Read, 0 - 1023)
#define P_PRESENT_SPEED_H		39	// Highest byte of actual servo Speed (Read, 0 - 1023)
#define P_PRESENT_TORQUE_L		40	// Current load on motor (Read, 0 - 1023)
#define P_PRESENT_TORQUE_H		41	// Current load on motor (Read, 0 - 1023)
#define P_PRESENT_VOLTAGE		42	// 10 x Current motor voltage (Read, 0 - 255)
#define P_PRESENT_TEMP			43	// Current motor temperature in deg Celcius (Read, 0 - 255)
#define P_REGD_INSTRUCTION		44	// Check if instruction is registered (Read or Write, 0 or 1)
#define P_MOVING				46	// Shows if the is any movement (Read, true or false)
#define P_LOCK_EEPROM			47	// Whether the Motor settings are locked from editing (Read or Write, 0 or 1)
#define P_PUNCH_L				48	// Motor Punch value (or minimum torque), for reducing motor shaking (Read or Write, 0 - 1023)
#define P_PUNCH_H				49	// Motor Punch value (or minimum torque), for reducing motor shaking (Read or Write, 0 - 1023)


// Try reading the 1-byte value several times, to make sure it worked.
int ReadMotorByte (int motorID, int ofset);

// Try reading the 2-byte value several times, to make sure it worked.
int ReadMotorWord (int motorID, int ofset);

// Try reading the value several times, to make sure it worked.
int GetMotorPosition(int id);

// Convert the unsigned 12bit value (0 to 1023) to a signed 12bit value (-512 to +511). Useful for velocities, etc.
int GetSignedMotorValue(int val);

// Separate thread that runs in the background to make sure the USB2Dynamixel driver hasn't frozen.
// Because sometimes it will keep trying to read a value forever.
void MonitorThread( void *dummy );

// Show the value from the Dynamixel motor, either as an N-digit number or a 3-digit error message.
// MUST be called just after ReadByteFromMotor() if you want a valid error message.
char* ShowMotorValue(int value, int N=3);



// Background Monitor thread will check this. Hence volatile (shouldn't be cached or optimised).
extern volatile bool g_monitorFlag;
extern volatile int g_monitorCounter;

//// Global variable for baudnum, so its easy to always print it out.
//extern int baudnum;

extern int g_numReadTries;	// For debugging the motors, shows how many times ReadMotorWord() tried to read the same value.


#endif