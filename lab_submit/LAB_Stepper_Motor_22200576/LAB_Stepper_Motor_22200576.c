/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Juyoung Lee
Created          : 05-03-2021
Modified         : 10-01-2026 
Language/ver     : C++ in VS Code

Description      : Stepper motor P1.Rotate the stepper motor 5 revolutions with 1 rpm.
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"

// ( A, B,  AN,  BN)
PinName_t pinStepper[4]    = {PB_10,PB_4,PB_5,PB_3};
uint32_t motor_speed = 14;

// Initialiization 
void setup(void){	
	RCC_PLL_init();                                 // System Clock = 84MHz
	SysTick_init();                                 // Systick init

	EXTI_init(SW1_PIN, FALL, 10);

	stepper_init(HALF, pinStepper); 				// Stepper GPIO pin initialization
	stepper_speed(motor_speed);                          		//  set stepper motor speed
}


int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	uint32_t rev=1;
	uint32_t steps=STEP_PER_REV*rev;

	// Inifinite Loop ----------------------------------------------------------
	while(1){

		stepper_step(steps, STEPPER_DIR_CCW);   // (Step : 2048, Direction : 0 or 1, Mode : FULL or HALF)
		delay_ms(2000);
		stepper_step(steps, STEPPER_DIR_CW);  // (Step : 2048, Direction : 0 or 1, Mode : FULL or HALF)
		delay_ms(2000);
		
	}

}



void EXTI15_10_IRQHandler(void) {  
	if (is_pending_EXTI(SW1_PIN)) {
		stepper_stop();

        clear_pending_EXTI(SW1_PIN); // cleared by writing '1'
	}
}