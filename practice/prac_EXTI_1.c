// practice EXTI - 1

#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecEXTI2.h"

void EXTI_init(PinName_t pinName, uint32_t trig_type, uint32_t priority){

    GPIO_TypeDef *Port;
    unsigned int pin;
    ecPinmap(pinName, &Port, &pin);

    // 1. Enable EXTI Clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // 2. Connect EXTI to GPIO
    //  2.1 mapping EXTIR_port
    uint32_t EXTICR_port = 0;
    if      (Port == GPIOA) EXTICR_port = 0;
    else if (Port == GPIOB) EXTICR_port = 1;
    else if (Port == GPIOC) EXTICR_port = 2;
    else if (Port == GPIOD) EXTICR_port = 3;
    else                    EXTICR_port = 4;

    //  2.2 setting system configuration 
    SYSCFG->EXTICR[pin/4] &= ~(0xFUL << (pin % 4)*4);       // clear 4 bits
    SYSCFG->EXTICR[pin/4] |= (EXTICR_port << (pin % 4)*4);   // clear 4 bits
    

    // 3. Select Trigger edge(RISE, FALL, BOTH)
    if      (trig_type == RISE) EXTI->RTSR |= 1UL << pin;
    else if (trig_type == FALL) EXTI->FTSR |= 1UL << pin;
    else if (trig_type == BOTH) {
        EXTI->RTSR |= 1UL << pin;
        EXTI->FTSR |= 1UL << pin;
    }

    // 4. Enable EXTI
    EXTI->IMR |= 1UL << pin;

    // 5. NVIC(IRQ) Setting 
    uint32_t EXTI_IRQn = 0;
    if      (pin<5)  EXTI_IRQn = EXTI0_IRQn + pin;
    else if (pin<10) EXTI_IRQn = EXTI9_5_IRQn;
    else             EXTI_IRQn = EXTI15_10_IRQn;

    NVIC_SetPriority(EXTI_IRQn,priority);
    NVIC_EnableIRQ(EXTI_IRQn);

}

void EXTI_enable(PinName_t pinName){
    GPIO_TypeDef *Port;
    uint32_t pin;
    ecPinmap(pinName, &Port, &pin);

    EXTI->IMR |= 1UL << pin;
}

void EXTI_disable(PinName_t pinName){
    GPIO_TypeDef *Port;
    uint32_t pin;
    ecPinmap(pinName, &Port, &pin);

    EXTI->IMR &= ~(1UL << pin);
}

uint32_t is_pending_EXTI(PinName_t pinName){
    GPIO_TypeDef *Port;
    uint32_t pin;
    ecPinmap(pinName, &Port, &pin);

    return (EXTI->PR >> pin) & 1UL;
}

void clear_pending_EXTI(PinName_t pinName){
    GPIO_TypeDef *Port;
    uint32_t pin;
    ecPinmap(pinName, &Port, &pin);

    EXTI->PR = (1UL << pin);
}