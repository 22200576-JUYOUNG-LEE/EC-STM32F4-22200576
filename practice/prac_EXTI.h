// practice EXTI - 1

#ifndef __EC_EXTI2_H
#define __EC_EXTI2_H

#include "stm32f411xe.h"
#include "ecPinNames.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"

#define FALL 0
#define RISE 1
#define BOTH 2


#ifdef __cplusplus
extern "C"{
#endif

void EXTI_init_1(PinName_t pinName, uint32_t trig_type, uint32_t priority);
void EXTI_enable_1(PinName_t pinName_t);
void EXTI_disable_1(PinName_t pinName_t);
uint32_t is_pending_EXTI_1(PinName_t pinName_t);
void clear_pending_EXTI_1(PinName_t pinName_t);


#ifdef __cplusplus
}
#endif

#endif 