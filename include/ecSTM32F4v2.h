/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Juyoung Lee
Created          : 05-03-2021
Modified         : 10-01-2026 
Language/ver     : C++ in VS Code

Description      : STM32 F411 Library
/----------------------------------------------------------------*/


#ifndef __EC_STM_H
#define __EC_STM_H

// STM built-In Library
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "math.h"

// EC course Library
#include "ecPinNames.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecEXTI2.h"
#include "ecSysTick2.h"
#include "ecStepper2.h"
//#include "ecTIM2.h"
//#include "ecPWM2.h"
//#include "ecADC2.h"
//#include "ecUART2.h"

#define SW1_PIN                 PC_13
#define LED_PIN                 PA_5

#define SW2_PIN_JK              PA_4
#define SW1_PIN_JK              PD_2


#define EXTI_NUM_STATE          10
#define EXTI_TIME_PREIOD        1000        // [ms]

#define STEPPER_SW1             P

static PinName_t pinFND[8]     = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
static PinName_t selectFND[4]  = {PA_10, PA_11, PC_4, PC_3};


static inline void MCU_JK_init(void){
    // CLOCK PLL 84MHz
    RCC_PLL_init();

    // SysTick 1msec for delay 
    SysTick_init();  
    
    // Switch1 = PD_2, Switch2 = PA_4
    GPIO_init(SW1_PIN_JK, INPUT);
    GPIO_pupd(SW1_PIN_JK, PULL_UP);    

    GPIO_init(SW2_PIN_JK, INPUT);
    GPIO_pupd(SW2_PIN_JK, PULL_UP);

    // Segment init
    FND_display_init(pinFND);
    FND_select_init(selectFND);

    // Initialize EXTI SW
    EXTI_init(SW2_PIN_JK, FALL, 10);
    EXTI_init(SW1_PIN_JK, FALL, 10);

    
}

static inline void MCU_init(void){
    // CLOCK PLL 84MHz
    RCC_PLL_init();
    
    // SysTick 1msec for delay 
    SysTick_init();    
    
    // Button PC13
    GPIO_init(PC_13, INPUT);
    GPIO_pupd(PC_13, PULL_DOWN);    
    
    // LED PA5
    GPIO_init(PA_5, OUTPUT);    

    // TIMx Default Initialization
    // ...
    // PWM Default Initialization
    // ...        
    // USART Default Initialization
    // ...
    // Others
}

#endif



 