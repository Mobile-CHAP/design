//##########################################
//##           R O B O T I S              ##
//## CM-700 (Atmega2561) Example code for ##
//##       Serial COmm.                   ##
//##                          2009.10.29  ##
//##########################################


#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRC  = 0x7F;
	PORTC = 0x7E;

	while (1)
	{
		int i;
		for(i = 0; i <= 6; i++)	// Print Value on USART
		{
			
			PORTC = ~(1<<i);
			_delay_ms(250);
		}
	}
	return 1;
}
