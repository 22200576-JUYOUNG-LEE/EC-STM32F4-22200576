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
#include <string.h>

// EC course Library
#include "ecPinNames.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecEXTI2.h"
#include "ecSysTick2.h"
#include "ecStepper2.h"
#include "ecUART2.h"
//#include "ecTIM2.h"
//#include "ecPWM2.h"
//#include "ecADC2.h"

#define SW1_PIN                 PC_13
#define LED_PIN                 PA_5

#define SW2_PIN_JK              PA_4
#define SW1_PIN_JK              PD_2

#define LED_PIN_JK              PB_12

#define PHOTO_PIN_JK            PA_0


#define EXTI_NUM_STATE          10
#define EXTI_TIME_PREIOD        1000        // [ms]

#define STEPPER_SW1             P




static inline void MCU_JK_init(void){
    PinName_t pinFND[8]          = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
    PinName_t selectFND[4]       = {PA_10, PA_11, PC_4, PC_3};
    
    // CLOCK PLL 84MHz
    RCC_PLL_init();

    // SysTick 1msec for delay 
    SysTick_init();  

    // UART
    USART1_init();
    USART2_init();
    
    // Switch1 = PD_2, Switch2 = PA_4
    GPIO_init(SW1_PIN_JK, INPUT);
    GPIO_pupd(SW1_PIN_JK, PULL_UP);    

    GPIO_init(SW2_PIN_JK, INPUT);
    GPIO_pupd(SW2_PIN_JK, PULL_UP);

    // LED PB_12
    GPIO_init   (LED_PIN_JK, OUTPUT);   
    GPIO_pupd   (LED_PIN_JK, PULL_DOWN);
    GPIO_otype  (LED_PIN_JK, PUSH_PULL);
    GPIO_ospeed (LED_PIN_JK, SPEED_MEDIUM); 

    // photo sensor
    // initialize the pushbutton pin as an input:
	GPIO_init(PHOTO_PIN_JK, INPUT);  
	GPIO_pupd(PHOTO_PIN_JK, PULL_UP);

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
    
    // Serial communication 
    USART1_init();
    USART2_init();
    
    // Button PC13
    GPIO_init(SW1_PIN, INPUT);
    GPIO_pupd(SW1_PIN, PULL_DOWN);  
      
    EXTI_init(SW1_PIN, FALL, 10);
    
    // LED PA5
    GPIO_init   (LED_PIN, OUTPUT);   
    GPIO_pupd   (LED_PIN, PULL_DOWN);
    GPIO_otype  (LED_PIN, PUSH_PULL);
    GPIO_ospeed (LED_PIN, SPEED_MEDIUM); 

    

    // TIMx Default Initialization
    // ...
    // PWM Default Initialization
    // ...        
    // USART Default Initialization
    // ...
    // Others
}

#endif



 