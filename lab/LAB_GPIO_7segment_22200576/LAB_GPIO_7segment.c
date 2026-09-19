/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ Juyoung Lee ]
Created          : 09-11-2026
Modified         : 09-12-2026
Language/ver     : C in VS Code

Description      : Control 7-segments display 
/----------------------------------------------------------------*/

#include "stm32f4xx.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"

#define BUTTON_PIN      PA_4
#define BUTTON_DLAY     30

#define SEVEN_SEG_DLAY  2

#define NUM_STATE       20

PinName_t pinFND[8]     = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
PinName_t selectFND[4]  = {PA_10, PA_11, PC_4, PC_3};

uint8_t numDisplay1     = 0;
uint8_t numDisplay2     = 0;
uint8_t selectFND1      = 0;
uint8_t selectFND2      = 1;

void setup(void){
    // Intialize System Clock
    RCC_HSI_init();    

    // Intialize FND pins and Button!!
    FND_display_init(pinFND);
    FND_select_init(selectFND);   

    GPIO_init(BUTTON_PIN, INPUT);  
    GPIO_pupd(BUTTON_PIN, PULL_UP);

    // set state 0
    FND_select(selectFND1);
    FND_display(numDisplay1);
    FND_select(selectFND2);
    FND_display(numDisplay2);

};

int main(void) {

    uint8_t state = 0;
    uint32_t currButtonState = HIGH;
    uint32_t prevButtonState = HIGH;

    setup();

    while(1) {

        // 1) Read data
        currButtonState = GPIO_read(BUTTON_PIN);

        // 2) Process data
        if(currButtonState == LOW && prevButtonState == HIGH){

            // 2-1) Set state and display digits 
            state       = (++state) % NUM_STATE;
            numDisplay1 = state % 10;
            numDisplay2 = state / 10;

            // 2-2) Protect from jittering
            delay_ms_HSI(BUTTON_DLAY);

        }

        // 3) Write data
        FND_select(selectFND1);
        FND_display(numDisplay1);
        delay_ms_HSI(SEVEN_SEG_DLAY);  

        FND_select(selectFND2);
        FND_display(numDisplay2);
        delay_ms_HSI(SEVEN_SEG_DLAY);

        // 2-3) Update button state
        prevButtonState = currButtonState;
    }
}