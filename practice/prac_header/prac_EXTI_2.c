// practice EXTI - 1

#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecEXTI2.h"

// 1. EXTI_init
void EXTI_inti(PinName_t pinName, uint32_t trig_type, uint32_t priority){
    GPIO_TypeDef *Port;
    uint32_t pin;
    ecPinmap(pinName, &Port, &pin);

    // 1.1 EXTI clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // 1.2 EXTI port setting 
    //  1.2.1 mapping port
    uint32_t EXTICR_port = 0;
    if          (Port == GPIOA) EXTICR_port = 0;
    else if     (Port == GPIOB) EXTICR_port = 1;
    else if     (Port == GPIOC) EXTICR_port = 2;
    else if     (Port == GPIOD) EXTICR_port = 3;
    else                        EXTICR_port = 4;
    //  1.2.2 connecting pin
    SYSCFG->EXTICR[pin/4] &= ~(0xFUL << (pin % 4) * 4);
    SYSCFG->EXTICR[pin/4] |= (EXTICR_port << (pin % 4) * 4);

    // 1.3 Select triger edge 
    if      (trig_type == FALL) EXTI->FTSR |= (1UL << (pin % 4) * 4);
    else if (trig_type == RISE) EXTI->RTSR |= (1UL << (pin % 4) * 4);
    else{
        EXTI->FTSR |= (1UL << (pin % 4) * 4);
        EXTI->RTSR |= (1UL << (pin % 4) * 4);
    }

    // 1.4 EXTI IMR setting 
    EXTI->IMR |= (1UL << (pin));

    // 1.5 NVIC setting 
    //  1.5.1 set IRQn
    uint32_t EXTI_IRQn = 0;
    if(pin < 5)         EXTI_IRQn = EXTI0_IRQn + pin;
    else if(pin < 10)   EXTI_IRQn = EXTI9_5_IRQn;
    else                EXTI15_10_IRQn;
    NVIC_SetPriority(EXTI_IRQn,priority);
    NVIC_EnableIRQq(EXTI_IRQn);


}

// 2. EXTI_enable
void EXTI_enable(PinName_t pinName){
    GPIO_TypeDef *Port;
    uint32_t pin;
    ecPinmap(pinName, &Port, &pin);

    EXTI->IMR |= (1UL <<(pin));
}

// 3. EXTI_disable
void EXTI_disable(PinName_t pinName){
    GPIO_TypeDef *Port;
    uint32_t pin;
    ecPinmap(pinName, &Port, &pin);

    EXTI->IMR &= (1UL << (pin));
}

// 4. is_pending_EXTI return uint32_t 
uint32_t is_Pending_EXTI(PinName_t pinName){
    GPIO_TypeDef *Port;
    uint32_t pin;
    ecPinmap(pinName, &Port, &pin);

    return (EXTI->PR >> pin) & 1UL;
}

// 5. clear_pending_EXTI
void clear_pending_EXTI(PinName_t pinName){
    GPIO_TypeDef *Port;
    uint32_t pin;
    ecPinmap(pinName, &Port, &pin);

    EXTI->PR = (1UL << pin);
}