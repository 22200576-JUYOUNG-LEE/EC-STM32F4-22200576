#include "ecSTM32F4v2.h"

#define time_delay 1000

uint32_t state = HIGH;

void setup(void){
    MCU_init();
    printf("prac_LED_4_Systick\r\n");

}

int main(void){
    setup();
    uint32_t curr_Tick = msTicks;

    // write data
    GPIO_write(LED_PIN, state);     

    while(1){


        // process data
        if(msTicks - curr_Tick > time_delay){

            // process data
            state ^= 1UL;

            if (state == HIGH) printf("LED_HIGH!\r\n");
            else               printf("LED_LOW!\r\n");

            // write 
            GPIO_write(LED_PIN, state);   

            // update
            curr_Tick = msTicks;
        }

        
    }

}