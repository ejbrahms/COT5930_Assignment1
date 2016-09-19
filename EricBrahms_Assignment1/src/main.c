#include <asf.h>
#include <stdio.h>

void setupLED(ioport_pin_t pin);

void setupLED(ioport_pin_t pin) {
	ioport_set_pin_dir(pin, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(pin, IOPORT_PIN_LEVEL_HIGH);
}

int main (void)
{
	sysclk_init();
	board_init();
	
	setupLED(EXT1_PIN_7);
	setupLED(EXT1_PIN_8);
	setupLED(EXT1_PIN_6);
	
	usart_serial_options_t serial_config = {
		.baudrate = 9600,
		.charlength = US_MR_CHRL_8_BIT,
		.paritytype = US_MR_PAR_NO,
		.stopbits = US_MR_NBSTOP_1
	};

	usart_serial_init(USART1, &serial_config);
	
	stdio_base = USART1;
	ptr_put = (int(*)(void volatile*, char)) usart_serial_putchar;
	//ptr_get = (int(*)(void volatile*, char)) usart_serial_getchar;
	setbuf(stdout, NULL);
	
	while (1) {

		char ledToToggle;
		printf("Which LED should I light up?");
		usart_serial_getchar(USART1, &ledToToggle);
		
		printf(" %c \r\n", ledToToggle);
		
		switch(ledToToggle) {
			case '0':
				ioport_toggle_pin_level(LED0);
				break;
			case '1':
				ioport_toggle_pin_level(EXT1_PIN_7);
				break;
			case '2':
				ioport_toggle_pin_level(EXT1_PIN_8);
				break;
			case '3':
				ioport_toggle_pin_level(EXT1_PIN_6);
				break;
			default:
				printf("Unknown number! \r\n");
				break;
			
		}
	}
}
