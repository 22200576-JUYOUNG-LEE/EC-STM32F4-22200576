#include "ecSTM32F4v2.h"


PinName_t pinStepper[4] = {PB_10,PB_4,PB_5,PB_3};

uint8_t PC_data = 0;
uint8_t BT_data = 0;

// setup
void setup(void){
    // 1.CLK, (RCC, Sysclock)
    // RCC_HSI_init();
    RCC_PLL_init();
    SysTick_init();

    // 2. steppermotor init
    stepper_init(STEPPER_MODE_FULL, pinStepper);
    stepper_speed(10);

    // 3. USART_1, 2, init
    USART1_init();
    USART2_init();


}


// main
int main(void){
    setup();

    uint8_t rev = 2;
    uint8_t steps = rev * STEP_PER_REV;

    printf("MCU Initialized\r\n");

    // loop
    while(1){

        stepper_step(steps, STEPPER_DIR_CCW);
        delay_ms(5000);

    }

}

// interrupt

void USART1_IRQHandler(){
    if(is_USART1_RXNE()){
        BT_data = USART1_read();
        USART1_write(&BT_data,1);
        printf("RX: %c \r\n",BT_data); // write on PC by USART2
        
        if(BT_data == 'J') stepper_dir(STEPPER_DIR_CCW);
        else if(BT_data == 'K') stepper_dir(STEPPER_DIR_CW);
        else if(BT_data == 'P') stepper_stop();

    }

}

// void USART2_IRQHandler(){
//     if(is_USART2_RXNE){
//         PC_data = USART2_read();
//         USART2_write(PC_data,1);

//     }
// }