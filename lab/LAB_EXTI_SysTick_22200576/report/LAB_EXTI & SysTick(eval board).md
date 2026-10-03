# LAB: EXTI & SysTick(eval board)

##

**Date:** 2026-09-02

**Author/Partner:** Juyoung Lee

**Demo Video:** Youtube link

### Introduction

In this lab, you are required to create two simple programs using interrupt:

(1) Display the number counting from 0 to 19 with Button Press

(2) Counting at a rate of 1 second

You must submit

* LAB Report (\*.md & \*.pdf)
* Zip source files(main\*.c, ecRCC2.h, ecGPIO2.h, ecSysTick2.c etc...).
  * Only the source files. Do not submit project files

#### Requirement

**Hardware**

* MCU
  * NUCLEO-F411RE
* Eval Board (JKIT)
* Actuator/Sensor/Others:

**Software**

* PlatformIO, CMSIS, EC\_HAL library

***

## Tutorial: Programming Tips

#### 1.Tutorial: Managing library header files

Read how to manage library header files for MCU register configurations. Apply it in your LAB.

{% embed url="<https://ykkim.gitbook.io/ec/ec-course/tutorial/tutorial-library-header-files#ec-2024>" %}

#### 2.Tutorial: Custom Initialization

Instead of writing initial setting functions for each registers, you can call a user defined function e.g. `MCU_init()` for the commonly used default initialization. Follow the tutorial and apply it in your LAB.

{% embed url="<https://ykkim.gitbook.io/ec/ec-course/tutorial/tutorial-custom-initialization>" %}

***

## Problem 0: STM-Arduino

{% embed url="<https://ykkim.gitbook.io/ec/ec-course/tutorial/tutorial-arduino-stm32/tutorial-arduino-stm32-part1#external-interrupt>" %}

We are going to create a simple program that turns LED(LD2) on triggered by **External Interrupt** of user button(BT1) (on NUCLEO-64, not on JKIT)

[**attachInterrupt()**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

```c
attachInterrupt(digitalPinToInterrupt(pin), ISR, mode)
```

* `digitalPinToInterrupt(pin)`: translate the digital pin to the specific interrupt number.
* `ISR`: a function called whenever the interrupt occurs.
* `mode`: defines when the interrupt should be triggered. (LOW, CHANGE, RISING, FALLING)

#### Procedure

1. Create a new project under the directory `\repos\EC\lab\`
2. Open *Arduino IDE* and Create a new program named as ‘**TU\_arduino\_EXTI.ino**’.
3. Write the following code.
   * Compare this code with the polling method in [Lab: GPIO-STM-Arduino](https://ykkim.gitbook.io/ec/ec-course/lab/lab-gpio-digital-inout-eval#example-code)

```c
const int btnPin = 3;
const int ledPin =  13; 

int btnState = HIGH;

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(btnPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(btnPin), blink, CHANGE);
}

void loop() {
    // blank
}


void blink(){
    btnState = digitalRead(btnPin);

    if (btnState == HIGH)
        digitalWrite(ledPin, LOW);
    else 
        digitalWrite(ledPin, HIGH);
}
```

> The user button pin is `PC13`, but this pin cannot be used in arduino. So, you should connect `PC13` to `pinName` `D3` by using a wire.

<figure><img src="https://ykkim.gitbook.io/~gitbook/image?url=https%3A%2F%2Fuser-images.githubusercontent.com%2F91526930%2F186584565-3dc47e19-e5c5-43a2-b4c4-d4de8916a2c8.png&#x26;width=300&#x26;dpr=4&#x26;quality=100&#x26;sign=ff32bff&#x26;sv=1" alt="" width="375"><figcaption></figcaption></figure>

4. Click on **upload** button.
5. Whenever the user button(BT1) is pressed (at fall), LED should be ON. When the button is released, the LED should be OFF.

***

## Prelab: EXTI Register Setting

Do the PreLAB: External Interrupt

* It is about creating `ecEXTI.*` library
* Fill-in the blanks in the given header files
* Submit the Prelab report

```c
void EXTI_init(PinName_t pinName, uint32_t trig_type, uint32_t priority);
void EXTI_enable(uint32_t pin);  // mask in IMR
void EXTI_disable(uint32_t pin);  // unmask in IMR
uint32_t  is_pending_EXTI(uint32_t pin);
void clear_pending_EXTI(uint32_t pin);
```

## Problem 1: Counter on 7-Segment display using EXTI

#### Problem

Create a code to display the counter-up from 0 to 9 and repeating.

* Count up at the rate of 1 sec.
  * Use `delay_ms()` in the main loop
* Pause the counting when SW2 is pressed
* Resets to zero when the push button is pressed: SW1
  * Must use External Interrupts for SW1, SW2

**\* Challenge : Extend the count-up number : 0 to 59**

#### Procedure

1. Connect the evaluation board [(JKIT-NUCLEO) ](https://ykkim.gitbook.io/ec/ec-course/hardware/stm32f-evaluation-board#reference-manual)to the MCU.
2. Make sure that your library are in `EC\include\`.
   * **ecRCC2.h, ecRCC2.c**
   * **ecGPIO2.h, ecGPIO2.c**
   * **ecSysTick2.h, ecSysTick2.c**
   * **ecEXTI2.h, ecEXTI2.c**
   * **ecSTM32F4v2.h**
3. Create a new project under the directory `EC\lab\`

* Environment: “**LAB\_EXTI”.**
* Source File: “**LAB\_EXTI.c”**

4. You must modify the `platformio.ini` **,** to add new environment.

#### Configuration

Configure the MCU GPIO

| Function                           | Port - Pin                                                                                        | Configuration                                 |
| ---------------------------------- | ------------------------------------------------------------------------------------------------- | --------------------------------------------- |
| **Button (SW1) on JKIT**           | PD\_2                                                                                             | DIN, Pull-Up                                  |
| **Button (SW2) on JKIT**           | PA\_4                                                                                             | DIN, Pull-Up                                  |
| **7-Segment DOUT**                 | <p>PB\_0, PB\_1, PB\_2, PB\_3, PB\_4, PB\_5, PB\_6, PB\_7</p><p>('a'\~'g''dot', respectively)</p> | Push-Pull, No Pull-up-Pull-down, Medium Speed |
| **Selection of 7-Segment Display** | PA\_10 (FND\_0)                                                                                   | DOUT, Push-Pull,                              |

#### Circuit Diagram

> You need to include the circuit diagram (if necessary)

![image](https://user-images.githubusercontent.com/38373000/192134563-72f68b29-4127-42ac-b064-2eda95a9a52a.png)

#### Code

Your code goes here.

Explain your source code with the necessary comments.

```c
// YOUR MAIN CODE ONLY
// YOUR CODE
```

**Sample Code**

{% code expandable="true" %}

```c
/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ YOUR NAME GOES HERE !!!!!]
Created          : 05-03-2021
Modified         : 09-09-2026
Language/ver     : C++ in VS Code

Description      : Tutorial - [Your Description GOES HERE !!] 
/----------------------------------------------------------------*/


#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecEXTI2.h"

#define LED0_PIN   PB_12 		//EVAL board JKIT LED0
#define SW2_PIN    PA_4			//EVAL board JKIT SW2


void LED_toggle(PinName_t pinName);

// Initialiization 
void setup(void)
{
    RCC_PLL_init();                 // System Clock = 84MHz
    SysTick_init();                 // SysTick Timer Initialization
    // Initialize GPIOB_12 for Output
    GPIO_init(LED0_PIN, OUTPUT);    // LED0 for EVAL board	

    // Initialize GPIOA_4 for Input Button
    GPIO_init(SW2_PIN, INPUT);  // INPUT for EVAL board
    GPIO_pupd(SW2_PIN, EC_PU);  // PULL-UP for EVAL board

    // Initialize EXTI PA_4
    EXTI_init(SW2_PIN, FALL, 10);		
}


// MAIN  ----------------------------------------
int main(void) {
	setup();
	while (1){
        delay_ms(5000);
    }
}

void EXTI4_IRQHandler(void) {
	if (is_pending_EXTI(SW2_PIN)) {   
			LED_toggle(LED0_PIN);
			clear_pending_EXTI(SW2_PIN);
	}
}

void LED_toggle(PinName_t pinName){
	GPIO_TypeDef *Port;    
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);    
	Port->ODR ^= 1<<pin;
}
```

{% endcode %}

#### Results

Experiment images and results go here

> Show experiment images /results

Add [demo video link](https://github.com/ykkimhgu/course-doc/blob/master/ec-course/lab/link/README.md)

#### Discussion

1. Analyze the result and explain any other necessary discussion.
2. We can use two different methods to detect an external signal: polling and interrupt. What are the advantages and disadvantages of each approach?

> Answer discussion questions

3. What would happen if the EXTI interrupt handler does not clear the interrupt pending flag? Check with your code

> Answer discussion questions

### Reference

Complete list of all references used (github, blog, paper, etc)

```
```

### Troubleshooting

(Option) You can write a Troubleshooting section
