// Move a Dynamixel motor to a given position or back & forth between 2 given positions, through the USB2Dynamixel device. by Shervin Emami 17/12/08.
// You can also move the motor manually using UP & DOWN keys, or hit ESC or Spacebar to disable torque.

#include "dynamixel.h"			// Official USB2Dynamixel SDK file.
#include "../DynamixelUtils.h"	// Additional functions created by Shervin 8thFeb2009.


int main(int argc, char **argv)
{
	int id = -1;
	int baudnum = -1;

	int GoalPos[2] = {-1, -1};
	int index = 0;
	int Moving, presentPos;

	printf("usage: Move_Motor <id> <baudnum> [<posA> [<posB>]]\n");
	if (argc > 1)
		id = atoi(argv[1]);
	if (argc > 2)
		baudnum = atoi(argv[2]);
	// If a position wasn't supplied, find out the current position from the motor.
	if (argc > 3)
		GoalPos[0] = atoi(argv[3]);
	// Make the 2nd pos to be same as the 1st if not supplied
	if (argc > 4)
		GoalPos[1] = atoi(argv[4]);
	else
		GoalPos[1] = GoalPos[0];
	printf("called: Move_Motor <id=%d> <baudnum=%d> <posA=%d> <posB=%d>\n", id, baudnum, GoalPos[0], GoalPos[1]);

	if (id < 0 || id > 255 || baudnum < 0 || baudnum > 255 || GoalPos[0] < -1 || GoalPos[0] > 1023 || GoalPos[1] < -1 || GoalPos[1] > 1023) {
		printf("bad args!\n");
		return 1;
	}

	///////// Open USB2Dynamixel ////////////
	if( dxl_initialize() == 0 )
	{
		printf( "Failed to open USB2Dynamixel!\n" );
		goto END_MAIN;
	}
	printf( "Succeed to open USB2Dynamixel!\n" );

	// Create a monitor thread to make sure the driver hasn't frozen.
	_beginthread( MonitorThread, 0, NULL );

	dxl_set_baud( baudnum );

	// Set a slowish speed, for safety
	dxl_write_word( id, P_GOAL_SPEED_L, 40);

	// Set the initial Goal position to the current position.
	if (GoalPos[0] == -1) {
		GoalPos[0] = GetMotorPosition(id);
		if (GoalPos[0] < 0 || GoalPos[0] > 1023) {
			printf("ERROR: Cant find out the initial position of the motor. You must supply it.\n");
			int p = GetMotorPosition(id);
			ShowMotorValue(p);
			printf("\n");
			goto END_MAIN;
		}

		// Make the 2nd pos to be same as the 1st if not supplied
		if (argc > 4)
			GoalPos[1] = atoi(argv[4]);
		else
			GoalPos[1] = GoalPos[0];
	}

	printf( "Press ESC for emergency stop, or Spacebar to toggle movement. A & Z also move motor manually\n\n" );
	int askedPos = -1;
	bool enableMovement = true;
	while(1)
	{
		Moving = dxl_read_byte( id, P_MOVING );	// Check if the motor is still moving.
		if( enableMovement && (dxl_get_result() == COMM_RXSUCCESS) )
		{
			if( Moving == 0 )
			{
				if( index == 0 )
					index = 1;
				else
					index = 0;

				// Move the motor
				dxl_write_word( id, P_GOAL_POSITION_L, GoalPos[index]);		// Move smoothly to the desired position

/*				// Display the current pos for 2 seconds
				for(int i=0; i<20; i++) {
					int pos = GetMotorPosition(id);
					ShowMotorValue(pos);
					Sleep(100);
				}
				printf("\n");
*/
			}
		}

		// Show the current position
		printf( "desired pos: %d,  currPos: ",GoalPos[index]);
		presentPos = GetMotorPosition(id);
		ShowMotorValue(presentPos);
		// Show the current Torque value
		printf(", currTorque: ");
		int presentTorque = ReadMotorWord(id, P_PRESENT_TORQUE_L);
		ShowMotorValue(presentTorque);
		// Show the current Temperature
		printf(", currTemp: ");
		int presentTemp = ReadMotorWord(id, P_PRESENT_TEMP);
		ShowMotorValue(presentTemp);

		Sleep(10);

		printf("\n");

		// key check
		if( _kbhit() > 0 )
		{
			int ch = getch();	// get keypress
			if(ch == ' ' )	// toggle movement
			{
				enableMovement = !enableMovement;
				printf("Toggling movement\n");
			}
			else if( ch == 0x1b )
			{
				dxl_write_byte(id, P_TORQUE_ENABLE, 0);	// Disable torque.
				printf( "Emergency stopped motor!\n" );
				enableMovement = false;
				//goto END_MAIN;
			}
			else if (ch == 0x48) {	// Up key
				GoalPos[0] += 10;
				GoalPos[1] += 10;
			}
			else if (ch == 0x50) {	// Down key
				GoalPos[0] -= 10;
				GoalPos[1] -= 10;
			}
			else {
				if (ch != 0xE0) {	// (Make sure its not an extended character)
					printf("Quitting due to unknown key press. Remebr, motor torque may still be enabled!\n");
					goto END_MAIN;
				}
			}
		}
	}

END_MAIN:
	dxl_terminate();
	printf( "Press any key to terminate...\n" );
	getch();

	return 0;
}
