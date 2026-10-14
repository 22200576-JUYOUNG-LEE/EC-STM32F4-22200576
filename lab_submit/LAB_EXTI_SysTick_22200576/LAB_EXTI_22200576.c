/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Juyoung Lee
Created          : 05-03-2021
Modified         : 10-07-2026
Language/ver     : C++ in VS Code

Description      : Counter on 7-Segment display using EXTI
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"

extern volatile uint32_t msTicks;

uint8_t numDisplay1     = 0;
uint8_t numDisplay2     = 0;
uint8_t selectFND1      = 0;
uint8_t selectFND2      = 1;

uint8_t state           = 0;
uint8_t pause           = 0;

volatile uint32_t curTicks = 0;

// Initialiization 
void setup(void)
{
    MCU_JK_init();	

    curTicks = msTicks;
    
}


// MAIN  ----------------------------------------
int main(void) {
	setup();

	while (1){

        // 1) Write
        numDisplay1 = state % 10;
        numDisplay2 = state / 10;

        FND_select(selectFND1);
        FND_display(numDisplay1);
        delay_ms(5);

        FND_select(selectFND2);
        FND_display(numDisplay2);
        delay_ms(5);
        
        // 2) Update
        if(((msTicks - curTicks) > 1000) && (pause == 0)){

            state = (++state) % NUM_STATE_EXTI_1;

            curTicks = msTicks;
        }
    }
}

void EXTI4_IRQHandler(void) {

	if (is_pending_EXTI(SW2_PIN_JK)) {   

        pause ^= 1;
        curTicks = msTicks;

        clear_pending_EXTI(SW2_PIN_JK);
	}
}

void EXTI2_IRQHandler(void) {

    if (is_pending_EXTI(SW1_PIN_JK)) {   

        state = 0;    
        curTicks = msTicks;
        
		clear_pending_EXTI(SW1_PIN_JK);
	}

}
