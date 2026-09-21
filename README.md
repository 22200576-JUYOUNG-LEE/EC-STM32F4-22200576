# Tutorial: Documentation

## Tutorial: Documentation

## EC API Documentation

[See Class Github](https://github.com/ykkimhgu/EC-student/blob/main/docs/EC_HAL_Documentation.md) for the example documentation

[See Tutorial: Documentation with Markdown](https://ykkim.gitbook.io/ec/numerical-programming/ta-tutorial/tutorial-documentation-with-markdown#preparation)

You must submit your API documentation before the last day of class

### Embedded Controller - STM32F411 Driver Library

Written by: Your Name

Program: C/C++

IDE/Compiler: Keil uVision 5

OS: WIn10/11

MCU: STM32F411RE, Nucleo-64

**Table of Contents**

* [GPIO Digital In/Out](/ec/ec-course/tutorial.md#gpio-digital-inout)
  * [Header File](/ec/ec-course/tutorial.md#header-file)
  * [GPIO\_init()](/ec/ec-course/tutorial.md#gpio-init----)
  * [GPIO\_mode()](/ec/ec-course/tutorial.md#gpio-mode----)
  * [GPIO\_write()](/ec/ec-course/tutorial.md#gpio-write----)
  * [GPIO\_read()](/ec/ec-course/tutorial.md#gpio-read----)
  * [GPIO\_ospeed()](/ec/ec-course/tutorial.md#gpio-ospeed----)
  * [GPIO\_otype()](/ec/ec-course/tutorial.md#gpio-otype----)
  * [GPIO\_pupdr()](/ec/ec-course/tutorial.md#gpio-pupdr----)
  * [Example Code](/ec/ec-course/tutorial.md)
* [EXTI](/ec/ec-course/tutorial.md#gpio-digital-inout)
  * [Header File](/ec/ec-course/tutorial.md#header-file)

***

### GPIO Digital InOut

#### Header File

`#include "ecGPIO.h"`

```c++
// MODER
#define INPUT  		0x00
#define OUTPUT 		0x01
#define AF     		0x02
#define ANALOG 		0x03

// IDR & ODR
#define HIGH 		1
#define LOW  		0

// OSPEED
#define LOW_SPEED		0x00
#define MID_SPEED		0x01
#define FAST_SPEED		0x02
#define HIGH_SPEED		0x03

// OTYPER
#define PUSH_PULL 		0	// Push-pull
#define OPEN_DRAIN 		1 	// Open-Drain

// PUDR
#define NO_PUPD			0x00 	// No pull-up, pull-down
#define PULL_UP			0x01 	// Pull-up
#define PULL_DOWN 		0x02 	// Pull-down	
#define RESERVED 		0x03 	// Reserved

// PIN
#define LED_PIN 		5
#define BUTTON_PIN 		13

void GPIO_init(GPIO_TypeDef *Port, int pin, int mode);
void GPIO_mode(GPIO_TypeDef* Port, int pin, int mode);
void GPIO_write(GPIO_TypeDef *Port, int pin, int output);
int  GPIO_read(GPIO_TypeDef *Port, int pin);
void GPIO_ospeed(GPIO_TypeDef* Port, int pin, int speed);
void GPIO_otype(GPIO_TypeDef* Port, int pin, int type);
void GPIO_pupdr(GPIO_TypeDef* Port, int pin, int pupd);

```

#### GPIO\_init()

Initializes GPIO pins with default setting and Enables GPIO Clock. Mode: In/Out/AF/Analog

```
void GPIO_init(GPIO_TypeDef *Port, int pin, int mode);
```

**Parameters**

* **Port:** Port Number, GPIOA\~GPIOH
* **pin**: pin number (int) 0\~15
* **mode**: INPUT(0), OUTPUT(1), AF(02), ANALOG (03)

**Example code**

```
GPIO_init(GPIOA, 5, OUTPUT);
GPIO_init(GPIOC, 13, INPUT); //GPIO_init(GPIOC, 13, 0);
```

#### GPIO\_mode()

Configures GPIO pin modes: In/Out/AF/Analog

```
void GPIO_init(GPIO_TypeDef *Port, int pin, int mode);
```

**Parameters**

* **Port:** Port Number, GPIOA\~GPIOH
* **pin**: pin number (int) 0\~15
* **mode**: INPUT (0), OUTPUT (1), AF(02), ANALOG (03)

**Example code**

```
GPIO_mode(GPIOA, 5, OUTPUT);
```

#### GPIO\_write()

Write the data to GPIO pin: High, Low

```
write(GPIO_TypeDef *Port, int pin, int output);
```

**Parameters**

* **Port:** Port Number, GPIOA\~GPIOH
* **pin**: pin number (int) 0\~15
* **output**: LOW(0), HIGH(1)

**Example code**

```
GPIO_write(GPIOA, 5, 1);  // 1: High
```

#### GPIO\_read()

Read the data from GPIO pin

```
int  GPIO_read(GPIO_TypeDef *Port, int pin);
```

**Parameters**

* **Port:** Port Number, GPIOA\~GPIOH
* **pin**: pin number (int) 0\~15

**Example code**

```
GPIO_read(GPIOC, 13);
```

#### GPIO\_ospeed()

Configures output speed of GPIO pin : Low, Mid, Fast, High

```
void GPIO_ospeed(GPIO_TypeDef* Port, int pin, int speed);
```

**Parameters**

* **Port:** Port Number, GPIOA\~GPIOH
* **pin**: pin number (int) 0\~15
* **speed**: LOW\_SPEED(0), MID\_SPEED(1), FAST\_SPEED(2) , HIGH\_SPEED(3)

**Example code**

```
GPIO_ospeed(GPIOA, 5, 2);  // 2: FAST_SPEED
```

#### GPIO\_otype()

Configures output type of GPIO pin: Push-Pull / Open-Drain

```
void GPIO_otype(GPIO_TypeDef* Port, int pin, int type);
```

**Parameters**

* **Port:** Port Number, GPIOA\~GPIOH
* **pin**: pin number (int) 0\~15
* **type**: PUSH\_PULL(0), OPEN\_DRAIN(1)

**Example code**

```
GPIO_otype(GPIOA, 5, 0);  // 0: Push-Pull
```

#### GPIO\_pupdr()

Configures Pull-up/Pull-down mode of GPIO pin: No Pull-up, Pull-down/ Pull-up/ Pull-down/ Reserved

```
void GPIO_pupdr(GPIO_TypeDef* Port, int pin, int pupd);
```

**Parameters**

* **Port:** Port Number, GPIOA\~GPIOH
* **pin**: pin number (int) 0\~15
* **pupd**: NO\_PUPD(0), PULL\_UP(1), PULL\_DOWN(2), RESERVED(3)

**Example code**

```
GPIO_pupdr(GPIOA, 5, 0);  // 0: No Pull-up, Pull-down
```

#### Example Code

```c++
#include "ecSTM32F411.h"

#define LED_PIN 5
#define BUTTON_PIN 13

// Initialiization 
void setup(void) {
	RCC_PLL_init();
	SysTick_init();
	GPIO_init(GPIOA, LED_PIN, OUTPUT);    
}
	
int main(void) { 
	setup();
	
	while(1){
		delay_ms(500);  
		GPIO_write(GPIOA, LED_PIN, LOW);
		delay_ms(500);  
		GPIO_write(GPIOA, LED_PIN, HIGH);
	}
}
```

***

### EXTI

#### Function Name

```
```

**Parameters**

* p1
* p2

**Example code**

```
```
