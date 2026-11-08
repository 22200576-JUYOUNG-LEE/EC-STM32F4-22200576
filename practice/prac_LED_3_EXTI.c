#include "ecSTM32F4v2.h"

uint32_t state = 0;

void setup(){
    MCU_init();
    printf("parc_LED_3_EXTI!\r\n");
}

int main(void){
    setup();

    while(1){
        delay_ms(2000);
    }

}

// EXTI PC_13 button
void EXTI15_10_IRQHandler(void){
    if(is_pending_EXTI(BUTTON_PIN)){

        state ^= 1UL;
        GPIO_write(LED_PIN, state);

        clear_pending_EXTI(BUTTON_PIN);
    }
}

