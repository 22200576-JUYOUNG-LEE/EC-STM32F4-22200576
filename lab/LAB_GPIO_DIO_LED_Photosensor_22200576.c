/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ Juyoung Lee ]
Created          : 09-11-2026
Modified         : 09-13-2026
Language/ver     : C in VS Code

Description      : Control PB_12 LED state using PA_0 Photodetector digital input
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"

#define LED_PIN PB_12
#define BUTTON_PIN PA_0

// Initialiization 
void setup(void) {
	RCC_HSI_init();
	
	// initialize the pushbutton pin as an input:
	GPIO_init(BUTTON_PIN, INPUT);  
	GPIO_pupd(BUTTON_PIN, PULL_UP);

	// initialize the LED pin as an output:
	GPIO_init(LED_PIN, OUTPUT);   
	GPIO_pupd(LED_PIN, PULL_UP);
	GPIO_otype(LED_PIN, PUSH_PULL);
	GPIO_ospeed(LED_PIN, SPEED_MEDIUM);

}
	
int main(void) { 
 	setup();
	int buttonState=0;
	
	while(1){
		// check if the pushbutton is pressed. Turn LED on/off accordingly:
		buttonState = GPIO_read(BUTTON_PIN);
		if(buttonState)	GPIO_write(LED_PIN, LOW);
		else 		GPIO_write(LED_PIN, HIGH);
		delay_ms_HSI(100);             					// delay 100 ms
	}
}
