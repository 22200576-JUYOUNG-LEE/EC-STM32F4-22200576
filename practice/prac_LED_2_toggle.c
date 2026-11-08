#include "ecSTM32F4v2.h"

#define NUM_STATE 2

void setup(void){

    MCU_init();
    printf("Prac_LED_2_toggle_edge filter!\r\n");

}


// Toggling by Edge filter
int main(void){
    setup();

    uint32_t state = 0;
    uint32_t currButtonState = HIGH;
    uint32_t prevButtonState = HIGH;

    while(1){

        // 1. read date 
        currButtonState = GPIO_read(BUTTON_PIN);


        // 2. process data
        if((currButtonState == LOW) && (prevButtonState ==HIGH)){
            printf("button pressed!\r\n");
            if(state == HIGH) printf("LED ON!\r\n");
            else              printf("LED OFF!\r\n");
               

             // 2-1) Set state and display digits 
            state ^= 1UL;

            // 2-2) Protect from jittering
            delay_ms(100);
        }

        // 3. write 
        GPIO_write(LED_PIN,state);


        // 4. update !!!!!!!!!!!!!!!!!!!!!
        prevButtonState = currButtonState;

    } 

}