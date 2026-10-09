# LAB: EXTI & SysTick(eval board)

**Date:** 2026-10-07

**Author/Partner:** Juyoung Lee

**Demo Video:** [Youtube link](https://youtube.com/shorts/34tNeJ3PVM4)

### <font color="#205867">Introduction</font>

In this lab, you are required to create two simple programs using interrupt:


***

## <font color="#205867">Tutorial: ecSTM32F4v2.h</font>

- **Header Protection & Library Includes**
    
    - **`#ifndef __EC_STM_H` ~ `#endif`**: Include guard to prevent multiple inclusion errors during compilation.
        
    - **STM32 Built-In Libraries**:
        
        - `stm32f4xx.h`, `stm32f411xe.h`: Register definitions and CMSIS headers for the STM32F411 MCU.
            
        - `math.h`: Standard C math library for mathematical operations.
            
    - **EC Course Libraries**:
        
        - `ecPinNames.h`: Friendly pin name aliases for hardware mapping.
            
        - `ecRCC2.h`: Reset and Clock Control (RCC) system clock configuration.
            
        - `ecGPIO2.h`: GPIO pin configuration utilities.
            
        - `ecEXTI2.h`: External Interrupt (EXTI) configuration utilities.
            
        - `ecSysTick2.h`: SysTick timer library for precise delays and time tracking.
            
- **Macro Definitions (`#define`)**
    
    - **`SW2_PIN_JK (PA_4)`**: Pin assignment for Switch 2 (`PA_4`).
        
    - **`SW1_PIN_JK (PD_2)`**: Pin assignment for Switch 1 (`PD_2`).
        
    - **`NUM_STATE_EXTI_1 (10)`**: Total number of display states (0 through 9).
        
- **Global Pin Mapping Arrays**
    
    - **`pinFND[8]`**: Pin array mapping to individual 7-segment LED segments (a–g, dp: `PB_0` through `PB_7`).
        
    - **`selectFND[4]`**: Pin array mapping to 4-digit display selector control lines (`PA_10`, `PA_11`, `PC_4`, `PC_3`).
        
- **`MCU_JK_init(void)` (Custom System Initialization)** Initializes all hardware peripherals required for the custom lab setup (7-Segment display + 2 Switches):
    
    - **`RCC_PLL_init()`**: Configures the main system clock to **84 MHz** using PLL.
        
    - **`SysTick_init()`**: Initializes the SysTick timer for 1 ms periodic tick interrupts.
        
    - **GPIO Switch Config**: Sets `SW1_PIN_JK` (`PD_2`) and `SW2_PIN_JK` (`PA_4`) as **INPUT** with internal **PULL_UP** resistors (active-low).
        
    - **FND Display Config**: Initializes segment data pins (`pinFND`) and digit selector pins (`selectFND`) as outputs (`FND_display_init`, `FND_select_init`).
        
    - **EXTI Interrupt Config**: Configures external interrupts for both switches to trigger on the **Falling Edge** (button press) with priority level 10.
        
- **`MCU_init(void)` (Standard Board Initialization)** Initializes default NUCLEO board peripherals:
    
    - **`RCC_PLL_init()` / `SysTick_init()`**: Sets up system clock (84 MHz) and SysTick (1 ms).
        
    - **`PC_13` (User Button)**: Configures the blue onboard user button as INPUT with PULL_DOWN.
        
    - **`PA_5` (User LED)**: Configures the green onboard user LED (LD2) as OUTPUT.


```c
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
//#include "ecTIM2.h"
//#include "ecPWM2.h"
//#include "ecStepper2.h"
//#include "ecADC2.h"
//#include "ecUART2.h"

#define SW2_PIN_JK      PA_4
#define SW1_PIN_JK      PD_2

#define NUM_STATE_EXTI_1  10

PinName_t pinFND[8]     = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
PinName_t selectFND[4]  = {PA_10, PA_11, PC_4, PC_3};


void MCU_JK_init(void){
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

void MCU_init(void){
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



 
```
***

## <font color="#205867">Problem 0: STM-Arduino</font>

We are going to create a simple program that turns LED(LD2) on triggered by **External Interrupt** of user button(BT1) (on NUCLEO-64, not on JKIT)

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
#### <font color="#205867">Results</font>

![[Pasted image 20261007200417.png]]

**Demo Video:** [Youtube link](https://youtube.com/shorts/34tNeJ3PVM4)

***

## <font color="#205867">Prelab: EXTI Register Setting</font>

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

Create a code to display the counter-up from 0 to 59 and repeating. 

* Count up at the rate of 1 sec.
  * Use `delay_ms()` in the main loop
* Pause the counting when SW2 is pressed
* Resets to zero when the push button is pressed: SW1
  * Must use External Interrupts for SW1, SW2

**\* Challenge : Extend the count-up number : 0 to 59**
#### Configuration

Configure the MCU GPIO

| Function                           | Port - Pin                                                                                        | Configuration                                 |
| ---------------------------------- | ------------------------------------------------------------------------------------------------- | --------------------------------------------- |
| **Button (SW1) on JKIT**           | PD\_2                                                                                             | DIN, Pull-Up                                  |
| **Button (SW2) on JKIT**           | PA\_4                                                                                             | DIN, Pull-Up                                  |
| **7-Segment DOUT**                 | <p>PB\_0, PB\_1, PB\_2, PB\_3, PB\_4, PB\_5, PB\_6, PB\_7</p><p>('a'\~'g''dot', respectively)</p> | Push-Pull, No Pull-up-Pull-down, Medium Speed |
| **Selection of 7-Segment Display** | PA\_10 (FND\_0)                                                                                   | DOUT, Push-Pull,                              |

#### Circuit Diagram

![[Pasted image 20261007140513.png]]


#### Code
- **define**
    
    - **NUM_STATE_EXTI_1:** Total number of display states (0 to 9)
        
- **Global Variables**
    
    - **numDisplay1, numDisplay2:** Numerical values to display on digit 1 (units) and digit 2 (tens)
        
    - **selectFND1, selectFND2:** Digit selector indices corresponding to display 1 and display 2
        
    - **state:** Current counter value/state (0 to 9)
        
    - **pause:** Toggle flag for counter control (0: counting, 1: paused)
        
    - **curTicks:** Saved tick timestamp used for non-blocking 1-second timing
        
- **setup()**
    
    - **Initializes (`MCU_JK_init()`):**
        
        - Initializes the PLL system clock (84 MHz) and SysTick timer (1 ms)
            
        - Initializes segment pins (`pinFND`) and digit selector pins (`selectFND`)
            
        - Initializes switch input pins (`SW1_PIN_JK`, `SW2_PIN_JK`) with pull-up configuration
            
        - Initializes external interrupts (EXTI) for SW1 (`PD_2`) and SW2 (`PA_4`)
            
    - Saves current tick count (`curTicks = msTicks`) to synchronize timing
        
- **main()**
    
    - Calls `setup()` and enters the infinite loop (`while (1)`):
        
    - **1) Write (7-Segment Display Multiplexing):**
        
        - Calculates unit digit (`state % 10`) and tens digit (`state / 10`).
            
        - Multiplexes and displays `numDisplay1` on FND 1 for 5 ms, then `numDisplay2` on FND 2 for 5 ms (`delay_ms(5)`).
            
    - **2) Update (Counter Logic):**
        
        - Measures elapsed time without blocking CPU execution using `(msTicks - curTicks)`.
            
        - If 1000 ms (1 sec) has elapsed and the system is not paused (`pause == 0`), increments `state` modulo `NUM_STATE_EXTI_1` and resets `curTicks` to `msTicks`.
            
- **EXTI Interrupt Handlers**
    
    - **`EXTI4_IRQHandler()` (SW2 - Pause/Resume):**
        
        - Triggers when SW2 (`PA_4`) button is pressed (falling edge).
            
        - Toggles the `pause` state variable (`pause ^= 1`).
            
        - Updates `curTicks = msTicks` so that timing restarts cleanly upon resume without an immediate count jump.
            
        - Clears the pending interrupt flag via `clear_pending_EXTI(SW2_PIN_JK)`.
            
    - **`EXTI2_IRQHandler()` (SW1 - Reset):**
        
        - Triggers when SW1 (`PD_2`) button is pressed (falling edge).
            
        - Resets the counter state back to zero (`state = 0`).
            
        - Updates `curTicks = msTicks` to align the 1-second timer from the reset moment.
            
        - Clears the pending interrupt flag via `clear_pending_EXTI(SW1_PIN_JK)`.


```c
/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Juyoung Lee
Created          : 05-03-2021
Modified         : 10-07-2026
Language/ver     : C++ in VS Code

Description      : Counter on 7-Segment display using EXTI
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"

extern volatile uint32_t msTicks;

uint8_t numDisplay1     = 0;
uint8_t numDisplay2     = 0;
uint8_t selectFND1      = 0;
uint8_t selectFND2      = 1;

uint8_t state           = 0;
uint8_t pause           = 0;

volatile uint32_t curTicks = 0;

// Initialiization 
void setup(void)
{
    MCU_JK_init();	

    curTicks = msTicks;
    
}


// MAIN  ----------------------------------------
int main(void) {
	setup();

	while (1){

        // 1) Write
        numDisplay1 = state % 10;
        numDisplay2 = state / 10;

        FND_select(selectFND1);
        FND_display(numDisplay1);
        delay_ms(5);

        FND_select(selectFND2);
        FND_display(numDisplay2);
        delay_ms(5);
        
        // 2) Update
        if(((msTicks - curTicks) > 1000) && (pause == 0)){

            state = (++state) % NUM_STATE_EXTI_1;

            curTicks = msTicks;
        }
    }
}

void EXTI4_IRQHandler(void) {

	if (is_pending_EXTI(SW2_PIN_JK)) {   

        pause ^= 1;
        curTicks = msTicks;

        clear_pending_EXTI(SW2_PIN_JK);
	}
}

void EXTI2_IRQHandler(void) {

    if (is_pending_EXTI(SW1_PIN_JK)) {   

        state = 0;    
        curTicks = msTicks;
        
		clear_pending_EXTI(SW1_PIN_JK);
	}

}

```

#### Results

count up 0 to 9
![[0 3.png]]![[1 3.png]]![[2 3.png]]![[3 2.png]]![[4 2.png]]![[5 2.png]]![[6 1.png]]![[7 1.png]]![[8 1.png]]![[9 1.png]]

**Demo Video:** [Youtube link](https://youtube.com/shorts/34tNeJ3PVM4)
#### Discussion

1. Analyze the result and explain any other necessary discussion.

> - **Timer & Display**: Non-blocking timing with `msTicks` ensures accurate 1-second counting, while 5 ms multiplexing provides a flicker-free display via Persistence of Vision (POV).
    
- **Interrupt Control**: EXTI provides instant responses for SW2 (Pause/Resume) and SW1 (Reset). Re-synchronizing `curTicks` inside the ISRs prevents immediate time jumps upon resuming.
    
- **Button Bouncing**: Lack of hardware/software debouncing may cause occasional double-triggering due to mechanical switch chatter.

2. We can use two different methods to detect an external signal: polling and interrupt. What are the advantages and disadvantages of each approach?

> - **Polling**: Simple and easy to implement, but inefficient (100% CPU usage) and risks missing fast inputs during delays.
    
- **Interrupt**: Highly CPU-efficient with real-time response to asynchronous events, but increases logic complexity and requires `volatile` for shared variables.

3. What would happen if the EXTI interrupt handler does not clear the interrupt pending flag? Check with your code

>- Without `clear_pending_EXTI()`, the pending bit in `EXTI->PR` remains set (`1`).
    
- The NVIC continuously re-triggers the interrupt, trapping the MCU in an **infinite ISR loop**.
    
- Program execution never returns to `main()`, completely freezing both the 7-segment display and counter logic.

modified code for checking uncleared pending flag
```c 

[others are same]

void EXTI2_IRQHandler(void) {

    if (is_pending_EXTI(SW1_PIN_JK)) {   

        state = 0;    
        curTicks = msTicks;

	}

}
```

**Demo Video:** https://youtu.be/eaapB1Z1Wz8

### Reference

Complete list of all references used (github, blog, paper, etc)

STMicroelectronics, _STM32F411xC/E Reference Manual (RM0383)_, Rev 4, 2025. [Online]. Available: [STMicroelectronics RM0383](https://www.st.com/resource/en/reference_manual/rm0383-stm32f411xce-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)

