#include "ecSTM32F4v2.h"

void setup(void){
    MCU_init();

    printf("MCU Initialized!\r\n");
    
}

int main(void){
    uint32_t button_state = 0;

    setup();

    while(1){

        button_state = GPIO_read(BUTTON_PIN);

        if(button_state == HIGH){
            GPIO_write(LED_PIN, LOW);   
            printf("LED off!\r\n");
        }    
        else{
            GPIO_write(LED_PIN, HIGH);  
            printf("LED on!\r\n");
        } 

    }
}