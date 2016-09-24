#include <asf.h>
#include <stdio.h>

void setupLED(ioport_pin_t pin);
void lightLED(char ledToToggle);
void setupButton(ioport_pin_t pin);
void buttonHandler_SW0(void);
void buttonHandler_EXT9(void);
void buttonHandler_EXT3(void);
void buttonHandler_EXT4(void);

void setupLED(ioport_pin_t pin) {
	ioport_set_pin_dir(pin, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(pin, IOPORT_PIN_LEVEL_HIGH);
}

void setupButton(ioport_pin_t pin) {
	// Enable the button as input.
	ioport_set_pin_dir(pin, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(pin, IOPORT_MODE_PULLUP);
	// Enable the interrupt as a falling edge.
	ioport_set_pin_sense_mode(pin, IOPORT_SENSE_FALLING);
	
	// Get the corresponding LED and callback for the interrupt.
	ioport_pin_t LED_PIN;
	gpio_pin_callback_t callbackFunction;
	switch(pin) {
	case PIN_PC24:
		LED_PIN = LED0;
		callbackFunction = buttonHandler_SW0;
		// Set the glitch filter.
		GPIO->GPIO_PORT[IOPORT_GPIOC].GPIO_GFERS = (1u << 24);
		break;
	case EXT1_PIN_9:
		LED_PIN = EXT1_PIN_7;
		callbackFunction = buttonHandler_EXT9;
		// Set the glitch filter.
		GPIO->GPIO_PORT[IOPORT_GPIOC].GPIO_GFERS = (1u << 25);
		break;
	case EXT1_PIN_3:
		LED_PIN = EXT1_PIN_8;
		callbackFunction = buttonHandler_EXT3;
		// Set the glitch filter.
		GPIO->GPIO_PORT[IOPORT_GPIOA].GPIO_GFERS = (1u << 4);
		break;
	case EXT1_PIN_4:
		LED_PIN = EXT1_PIN_6;
		callbackFunction = buttonHandler_EXT4;
		// Set the glitch filter.
		GPIO->GPIO_PORT[IOPORT_GPIOA].GPIO_GFERS = (1u << 5);
		break;	
	}
	
	// Setup the button interrupts.
	// Tell the controller to call the button handler function with priority 1.
	gpio_set_pin_callback(pin, callbackFunction, 1);
	
	// Clear current interrupts before enabling.
	gpio_clear_pin_interrupt_flag(pin);
	gpio_enable_pin_interrupt(pin);
}

void buttonHandler_SW0(void) {
	printf("Button 0 \r\n");
}

void buttonHandler_EXT9(void) {
	printf("Button 1 \r\n");
}

void buttonHandler_EXT3(void) {
	printf("Button 2 \r\n");
}

void buttonHandler_EXT4(void) {
	printf("Button 3 \r\n");
}

void lightLED(char ledToToggle) {
	
	//printf(" %c \r\n", ledToToggle);
	
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


int main (void)
{
	sysclk_init();
	board_init();
	
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
	
	// LED 0
	setupLED(LED0);
	// LED 1
	setupLED(EXT1_PIN_7);
	// LED 2
	setupLED(EXT1_PIN_8);
	// LED 3
	setupLED(EXT1_PIN_6);
	// Button 0 (SW0)
	setupButton(PIN_PC24);
	// Button 1
	setupButton(EXT1_PIN_9);
	// Button 2
	setupButton(EXT1_PIN_3);
	// Button 3
	setupButton(EXT1_PIN_4);
	
	printf("Which LED should I light up - Press 0, 1, 2, or 3? \r\n");
	printf("Or PUSH a button to get a response! \r\n");
	while (1) {
		// SET RX pin to interrupt?
		char ledToToggle;
		usart_serial_getchar(USART1, &ledToToggle);
		lightLED(ledToToggle);
	}
}
