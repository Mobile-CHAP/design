#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <dynamixel.h>
#include <serial.h>

int main(void)
{


	serial_initialize(57600);
	dxl_initialize( 0, 1 ); // Not using device index
    //sei(); // Interrupt Enable

	int CommStatus;
	CommStatus = dxl_get_result();
	


	if( CommStatus == COMM_RXSUCCESS )
	{


		dxl_write_word( 3, 30,4095 );


	}

	else
	Printf(" %d \n",1);
}
