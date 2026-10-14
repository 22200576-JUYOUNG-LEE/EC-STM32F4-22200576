/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ Juyoung Lee ]
Created          : 05-03-2021
Modified         : 09-11-2026
Language/ver     : C++ in VS Code

Description      : GPIO Library. Distributed to Students for LAB_GPIO
/----------------------------------------------------------------*/

#ifndef __ECGPIO2_H
#define __ECGPIO2_H

#include "stm32f411xe.h"
#include "ecRCC2.h"
#include "ecPinNames.h"

// MODER
#define INPUT       0x00
#define OUTPUT      0x01
#define AF          0x02
#define ANALOG      0x03

// IDR & ODR
#define HIGH        0x01
#define LOW         0x00

// OSPEED
typedef enum {
    SPEED_LOW         = 0,
    SPEED_MEDIUM      = 1,
    SPEED_FAST        = 2,
    SPEED_HIGH        = 3,
}OSPEED;

// OTYPE
typedef enum {
    PUSH_PULL         = 0,
    OPEN_DRAIN        = 1,
} OTYPE;

// PUPD
typedef enum {
    NO_PUPD           = 0,
    PULL_UP           = 1,
    PULL_DOWN         = 2,
    RESERVED          = 3,
}PUPD;


#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
	 
void GPIO_init      (PinName_t pinName, uint32_t mode);     
void GPIO_mode      (PinName_t pinName, uint32_t mode);
void GPIO_ospeed    (PinName_t pinName, uint32_t speed);
void GPIO_otype     (PinName_t pinName, uint32_t type);
void GPIO_pupd      (PinName_t pinName, uint32_t pupd);
void GPIO_write     (PinName_t pinName, uint32_t Output);
uint32_t  GPIO_read (PinName_t pinName);

// ARDUINO Syntax
void        pinMode             (PinName_t pin, uint32_t mode);
void        digitalWrite        (PinName_t pin, uint32_t value);
uint32_t    digitalRead         (PinName_t pin);

// FND for JKIT board 
extern int numberFND[12][8];
void FND_display_init       (PinName_t *pinFND);
void FND_select_init        (PinName_t *selectFND);
void FND_display            (uint8_t num);
void FND_select             (uint8_t digit);
 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __ECGPIO2_H
