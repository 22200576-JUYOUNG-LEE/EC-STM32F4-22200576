#include "ecSTM32F4v2.h"

// ( A, B,  AN,  BN)
PinName_t   pinStepper[4]         = {PB_10,PB_4,PB_5,PB_3};
uint32_t    motor_speed = 2;
uint8_t     PC_data = 0;
static volatile int bReceive =0;

void setup(){

    MCU_init();
    printf("parc_Stepper_2_speed Initialized!\r\n");

    stepper_init(STEPPER_MODE_HALF, pinStepper);
    stepper_speed(motor_speed);

}

void main(){
    setup();

    uint32_t rev = 5;
    uint32_t steps = STEP_PER_REV*rev;

    stepper_step_set(steps,STEPPER_DIR_CCW);

    while(1){

        stepper_run();

         // Once enter 
        if(bReceive == 1){
  
            stepper_speed(motor_speed);

            bReceive = 0;
        }
    }
}

void USART2_IRQHandler(void){
    if(is_USART2_RXNE()){
        
        bReceive = 1;

        PC_data = USART2_read();
        USART2_write(&PC_data,1);

        if(PC_data>='0' && PC_data <='9'){
            motor_speed = (PC_data - '0');
            printf("Motor speed is : %lu\r\n",motor_speed);
        }else if (PC_data == 'J'){
            stepper_dir(STEPPER_DIR_CCW);
            printf("Motor counter clock wise\r\n");
        }else if (PC_data == 'K'){
            stepper_dir(STEPPER_DIR_CW);
            printf("Motor clock wise\r\n");
        }else if (PC_data == 'P'){
            stepper_pause();
            printf("Motor stop\r\n");
        }else if (PC_data == 'R'){
            stepper_resume();
            printf("Motor restart\r\n");
        }


    }

}