#include "ecSTM32F4v2.h"

uint8_t PC_data     = 0;
volatile uint32_t time_delay = 1000;

void setup(){
    MCU_init();
    printf("prac_LED_5_UART2 Initialized!\r\n");
}

int main(){
    setup();

    uint32_t curr_tick  = msTicks;
    uint32_t state      = 0;

    while(1){
        if((msTicks - curr_tick) >time_delay){
            state ^= 1UL;
            printf("LED_toggle!\r\n");
            GPIO_write(LED_PIN, state);

            // update !!!!!!
            curr_tick = msTicks;
        }
    }

}

void USART2_IRQHandler(void){
    if(is_USART2_RXNE()){

        PC_data = USART2_read();

        // time mapping
        time_delay = (PC_data - '0') * 1000UL;
        printf("Time_delay is : %lu\r\n",time_delay);

    }

}