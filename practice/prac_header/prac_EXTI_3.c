// practice EXTI_3

#include "prac_EXTI.h"

// 1. EXTI_init
void EXTI_init_1(PinName_t pinName, uint32_t trig_type, uint32_t priority){
    GPIO_TypeDef * Port;
    unsigned int pin;
    ecPinmap(pinName, &Port, &pin);

    // 1. RCC 
    RCC->APB2ENR = RCC_APB2ENR_SYSCFGEN;

    // 2. connecting port and pin
    uint32_t EXTICR_port = 0;
    if      (Port == GPIOA) EXTICR_port = 0;
    else if (Port == GPIOB) EXTICR_port = 1;
    else if (Port == GPIOC) EXTICR_port = 2;
    else if (Port == GPIOD) EXTICR_port = 3;
    else                    EXTICR_port = 4;

    SYSCFG->EXTICR[pin/4] &= ~(0xFUL << (pin % 4) * 4);
    SYSCFG->EXTICR[pin/4] |= (EXTICR_port << (pin % 4) * 4);

    // 3. trig_type 
    if(trig_type == FALL)       EXTI->FTSR |= (1UL << pin);
    else if(trig_type == RISE)  EXTI->RTSR |= (1UL << pin);
    else{
        EXTI->FTSR |= (1UL << pin);
        EXTI->RTSR |= (1UL << pin);
    }    

    // 4. EXTI enable
    EXTI->IMR |= (1UL << pin);

    // 5. NVIC (set periority, enable )
    uint32_t IRQn = 0;
    if      (pin<5)         IRQn = EXTI0_IRQn + pin;
    else if (pin<10)        IRQn = EXTI9_5_IRQn;
    else                    IRQn = EXTI15_10_IRQn;
    
    NVIC_SetPriority(IRQn, priority);
    NVIC_EnableIRQ(IRQn);

}
// 2. exti enable
void EXTI_enable_1(PinName_t pinName){
    GPIO_TypeDef *Port;
    unsigned int pin;
    ecPinmap(pinName, &Port, &pin);

    EXTI->IMR |= (1UL << pin);
}

// 3. exti disable
void EXTI_disable_1(PinName_t pinName){
    GPIO_TypeDef *Port;
    unsigned int pin;
    ecPinmap(pinName, &Port, &pin);

    EXTI->IMR &= ~(1UL << pin);
}

// 4. is pending 
uint32_t is_pending_EXTI_1(PinName_t pinName){
    GPIO_TypeDef *Port;
    unsigned int pin;
    ecPinmap(pinName, &Port, &pin);

    return (EXTI->PR >> pin) & 1UL;
}

// 5. clear pending 
void clear_pending_EXTI_1(PinName_t pinName){
    GPIO_TypeDef *Port;
    unsigned int pin;
    ecPinmap(pinName, &Port, &pin);

    EXTI->PR = (1UL << pin);
}

