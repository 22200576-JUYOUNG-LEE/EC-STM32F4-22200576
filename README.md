# EC API Documentation

### Embedded Controller - STM32F411 Driver Library

Written by: Juyoung Lee

Program: C/C++

IDE/Compiler: Keil uVision 5

OS: WIn10/11

MCU: STM32F411RE, Nucleo-64

**Table of Contents**

[TOC]



***

## 1 Pin Names & Bitwise Macros (`ecPinNames.h` / `ecPinNames.c`)

### Header File (`ecPinNames.h`)

```c
#include "stm32f411xe.h"

// Bitwise Macro Definition
#define BIT_SET(REG, BIT)          ((REG) |= 1 << (BIT))
#define BIT_CLEAR(REG, BIT)        ((REG) &= ~(1 << (BIT)))
#define BIT_READ(REG, BIT)         (((REG) >> (BIT)) & 1)
#define BITS_SET(REG, BIT, NUM)    ((REG) |= (NUM) << (BIT))
#define BITS_CLEAR(REG, BIT, NUM)  ((REG) &= ~((NUM) << (BIT)))

// Pinname Config
typedef enum {
    PortA = 0, PortB = 1, PortC = 2, PortD = 3,
    PortE = 4, PortF = 5, PortG = 6, PortH = 7,
    PortI = 8, PortJ = 9, PortK = 10
} PortName_t;

typedef enum {
    PA_0  = 0x00, PA_1  = 0x01, PA_2  = 0x02, PA_3  = 0x03,
    PA_4  = 0x04, PA_5  = 0x05, PA_6  = 0x06, PA_7  = 0x07,
    PA_8  = 0x08, PA_9  = 0x09, PA_10 = 0x0A, PA_11 = 0x0B,
    PA_12 = 0x0C, PA_13 = 0x0D, PA_14 = 0x0E, PA_15 = 0x0F,
    
    PB_0  = 0x10, PB_1  = 0x11, PB_2  = 0x12, PB_3  = 0x13,
    PB_4  = 0x14, PB_5  = 0x15, PB_6  = 0x16, PB_7  = 0x17,
    PB_8  = 0x18, PB_9  = 0x19, PB_10 = 0x1A, PB_12 = 0x1C,
    PB_13 = 0x1D, PB_14 = 0x1E, PB_15 = 0x1F,

    PC_0  = 0x20, PC_1  = 0x21, PC_2  = 0x22, PC_3  = 0x23,
    PC_4  = 0x24, PC_5  = 0x25, PC_6  = 0x26, PC_7  = 0x27,
    PC_8  = 0x28, PC_9  = 0x29, PC_10 = 0x2A, PC_11 = 0x2B,
    PC_12 = 0x2C, PC_13 = 0x2D, PC_14 = 0x2E, PC_15 = 0x2F,

    PD_2  = 0x32, PH_0  = 0x70, PH_1  = 0x71,

    // Arduino connector namings
    A0 = PA_0, A1 = PA_1, A2 = PA_4, A3 = PB_0, A4 = PC_1, A5 = PC_0,
    D0 = PA_3, D1 = PA_2, D2 = PA_10, D3 = PB_3, D4 = PB_5, D5 = PB_4,
    D6 = PB_10, D7 = PA_8, D8 = PA_9, D9 = PC_7, D10 = PB_6, D11 = PA_7,
    D12 = PA_6, D13 = PA_5, D14 = PB_9, D15 = PB_8,

    NC = (int)0xFFFFFFFF
} PinName_t;

void ecPinmap(PinName_t pinName, GPIO_TypeDef **GPIOx, unsigned int *pin);

```

### Functions & Bitwise Macros

#### Bitwise Helper Macros

- `BIT_SET(REG, BIT)`: Sets the bit at position `BIT` in register `REG`.   
- `BIT_CLEAR(REG, BIT)`: Clears the bit at position `BIT` in register `REG`.   
- `BIT_READ(REG, BIT)`: Reads and returns the binary value (`0` or `1`) at position `BIT` in register `REG`.   
- `BITS_SET(REG, BIT, NUM)`: Writes a multi-bit mask `NUM` starting at position `BIT`.   
- `BITS_CLEAR(REG, BIT, NUM)`: Clears a multi-bit mask `NUM` starting at position `BIT`.   

#### `ecPinmap()`   

Extracts the GPIO register peripheral pointer (`GPIOx`) and bit position (`pin`) corresponding to a given `PinName_t` alias.   

```c
void ecPinmap(PinName_t pinName, GPIO_TypeDef **GPIOx, unsigned int *pin);
```

- **Parameters:**   

  - `pinName`: Pin identifier of type `PinName_t` (e.g., `PA_5`, `D13`)   
  - `GPIOx`: Pointer to `GPIO_TypeDef*` where the GPIO port address (`GPIOA`, `GPIOB`, etc.) will be stored   
  - `pin`: Pointer to an `unsigned int` where the bit index (`0` ~ `15`) will be stored   

- **Example Code:**   

  ```c
  GPIO_TypeDef *port;
  unsigned int pinNo;
  ecPinmap(PA_5, &port, &pinNo); // Sets port = GPIOA, pinNo = 5
  ```



## 2 GPIO Digital InOut

#### Header File

`#include "ecGPIO.h"`

```c++
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


// GPIO
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

```

### 2.1 GPIO

#### GPIO\_init()

Initializes GPIO pins with default setting and Enables GPIO Clock. Mode: In/Out/AF/Analog

```
void GPIO_init(PinName_t pinName, uint32_t mode);  
```

**Parameters**

* **pinName:** (port name)_(pin number) (PinName_t) PA_0
* **mode**: INPUT(0), OUTPUT(1), AF(02), ANALOG (03)

**Example code**

```
GPIO_init(PA_0, INPUT);
```

#### GPIO\_mode()

Configures GPIO pin modes: In/Out/AF/Analog

```
void GPIO_mode(PinName_t pinName, uint32_t mode);
```

**Parameters**

* **pinName:** (port name)_(pin number) (PinName_t) PA_0
* **mode**: INPUT (0), OUTPUT (1), AF(02), ANALOG (03)

**Example code**

```
GPIO_mode(PA_0, INPUT); 
```

#### GPIO\_write()

Write the data to GPIO pin: High, Low

```
void GPIO_write(PinName_t pinName, uint32_t Output);
```

**Parameters**

* **pinName:** (port name)_(pin number) (PinName_t) PB_12
* **output**: LOW(0), HIGH(1)

**Example code**

```
GPIO_write(PB_12, HIGH);
```

#### GPIO\_read()

Read the data from GPIO pin

```
uint32_t  GPIO_read(PinName_t pinName);
```

**Parameters**

* **pinName:** (port name)_(pin number) (PinName_t) PA_0

**Example code**

```
GPIO_read(PA_0);
```

#### GPIO\_ospeed()

Configures output speed of GPIO pin : Low, Mid, Fast, High

```
void GPIO_ospeed(PinName_t pinName, uint32_t speed);
```

**Parameters**

* **pinName:** (port name)_(pin number) (PinName_t) PB_12
* **speed**: SPEED_LOW (0), SPEED_MEDIUM (1), SPEED_FAST (2) , SPEED_HIGH (3)

**Example code**

```c
GPIO_ospeed(PB_12, SPEED_MEDIUM);
```

#### GPIO\_otype()

Configures output type of GPIO pin: Push-Pull / Open-Drain

```
void GPIO_otype(PinName_t pinName, uint32_t type);
```

**Parameters**

* **pinName:** (port name)_(pin number) (PinName_t) PB_12
* **type**: PUSH\_PULL(0), OPEN\_DRAIN(1)

**Example code**

```
GPIO_otype(PB_12, PUSH_PULL);
```

#### GPIO\_pupd()

Configures Pull-up/Pull-down mode of GPIO pin: No Pull-up, Pull-down/ Pull-up/ Pull-down/ Reserved

```
void GPIO_pupd(PinName_t pinName, uint32_t pupd);
```

**Parameters**

* **pinName:** (port name)_(pin number) (PinName_t) PB_12
* **pupd**: NO\_PUPD(0), PULL\_UP(1), PULL\_DOWN(2), RESERVED(3)

**Example code**

```
GPIO_pupd(PB_12, PULL_UP);
```

#### Example Code

```
#include "ecRCC2.h"
#include "ecGPIO2.h"

#define LED_PIN PB_12
#define BUTTON_PIN PA_0

// Initialiization 
void setup(void) {
	RCC_HSI_init();
	
	// initialize the pushbutton pin as an input:
	GPIO_init(BUTTON_PIN, INPUT);  
	GPIO_pupd(BUTTON_PIN, PULL_UP);

	// initialize the LED pin as an output:
	GPIO_init(LED_PIN, OUTPUT);   
	GPIO_pupd(LED_PIN, PULL_UP);
	GPIO_otype(LED_PIN, PUSH_PULL);
	GPIO_ospeed(LED_PIN, SPEED_MEDIUM);

}
	
int main(void) { 
 	setup();
	int buttonState=0;
	
	while(1){
		// check if the pushbutton is pressed. Turn LED on/off accordingly:
		buttonState = GPIO_read(BUTTON_PIN);
		if(buttonState)	GPIO_write(LED_PIN, LOW);
		else GPIO_write(LED_PIN, HIGH);
		delay_ms_HSI(100);             					// delay 100 ms
	}
}
```



### 2.2 Arduino Syntax

#### pinMode()

Configures GPIO pin modes: In/Out/AF/Analog

```
void pinMode(PinName_t pin, uint32_t mode);
```

**Parameters**

* **pinName:** (port name)_(pin number) (PinName_t) PA_0
* **mode**: INPUT (0), OUTPUT (1), AF(02), ANALOG (03)

**Example code**

```
pinMode(PB_12, OUTPUT)
```

#### digitalWrite()

Write the data to GPIO pin: High, Low

```
void digitalWrite(PinName_t pin, uint32_t value);
```

**Parameters**

* **pinName:** (port name)_(pin number) (PinName_t) PB_12
* **output**: LOW(0), HIGH(1)

**Example code**

```
digitalWrite(PB_12, HIGH);
```

#### digitalRead()

Read the data from GPIO pin

```
uint32_t digitalRead(PinName_t pin);
```

**Parameters**

* **pinName:** (port name)_(pin number) (PinName_t) PB_12

**Example code**

```
digitalRead(PA_0);
```

#### Example Code

```c
#include "ecRCC2.h"
#include "ecGPIO2.h"

#define LED_PIN    PB_12
#define BUTTON_PIN PA_0

void setup(void) {
    RCC_HSI_init();
    
    // Configure pins using Arduino Syntax
    pinMode(BUTTON_PIN, INPUT);
    GPIO_pupd(BUTTON_PIN, PULL_UP); // Pull-up configuration for active-low button
    
    pinMode(LED_PIN, OUTPUT);
    GPIO_pupd(LED_PIN, PULL_UP);
    GPIO_otype(LED_PIN, PUSH_PULL);
    GPIO_ospeed(LED_PIN, SPEED_MEDIUM);
}

int main(void) {
    setup();
    
    while(1) {
        // Read input pin and update LED state using Arduino API
        uint32_t buttonState = digitalRead(BUTTON_PIN);
        
        if (buttonState == LOW) { // Button pressed
            digitalWrite(LED_PIN, HIGH);
        } else {                  // Button released
            digitalWrite(LED_PIN, LOW);
        }
        
        delay_ms_HSI(50); // Debounce delay
    }
}
```



### 2.3 FND for JKIT board 

#### FND_display_init()

Initializes and configures the 8 GPIO segment pins (a, b, c, d, e, f, g, dp) as output pins.

```
void FND_display_init(PinName_t *pinFND);
```

- **Parameters:**

  - `pinFND`: Pointer to an array of 8 `PinName_t` values specifying the segment pins `[a, b, c, d, e, f, g, dp]`.

- **Example Code:**

  ```
  PinName_t fndPins[8] = {PA_5, PA_6, PA_7, PB_6, PC_7, PA_9, PA_8, PB_10};
  FND_display_init(fndPins);
  ```

#### FND_select_init()

Initializes and configures the digit selection GPIO pins as output pins for multi-digit display control.

```
void FND_select_init(PinName_t *selectFND);
```

- **Parameters:**

  - `selectFND`: Pointer to an array of `PinName_t` values specifying the digit select pins (e.g., Digit 1 to Digit 4).

- **Example Code:**

  ```
  PinName_t selectPins[4] = {PC_6, PC_8, PC_9, PB_8};
  FND_select_init(selectPins);
  ```

#### FND_display()

Outputs the specified numeric digit pattern onto the currently selected segment display.

```
void FND_display(uint8_t num);
```

- **Parameters:**

  - `num`: Integer value to display (`0` ~ `11`). `0`–`9` render numbers 0 to 9, while `10`–`11` correspond to special characters or symbols defined in `numberFND`.

- **Example Code:**

  ```
  FND_display(7); // Displays number 7 on active digit
  ```

#### FND_select()

Activates a specific digit line for multiplexed multi-digit FND driving.

```
void FND_select(uint8_t digit);
```

- **Parameters:**

  - `digit`: Digit index to activate (e.g., `0` for 1st digit, `1` for 2nd digit, `2` for 3rd digit, `3` for 4th digit).

- **Example Code:**

  ```
  FND_select(0); // Activates the 1st digit position
  ```

#### Example Code

```c++
#include "ecRCC2.h"
#include "ecGPIO2.h"

// Define FND segment pins [a, b, c, d, e, f, g, dp]
PinName_t fndPins[8] = {PA_5, PA_6, PA_7, PB_6, PC_7, PA_9, PA_8, PB_10};

// Define FND digit select pins [DIG0, DIG1, DIG2, DIG3]
PinName_t selectPins[4] = {PC_6, PC_8, PC_9, PB_8};

void setup(void) {
    RCC_HSI_init();
    
    // Initialize FND segment and select pins
    FND_display_init(fndPins);
    FND_select_init(selectPins);
}

int main(void) {
    setup();
    
    while(1) {
        // Display '1' on Digit 0
        FND_select(0);
        FND_display(1);
        delay_ms_HSI(5);
        
        // Display '2' on Digit 1
        FND_select(1);
        FND_display(2);
        delay_ms_HSI(5);
    }
}
```

***

## 3 EXTI

### Header File 

`#include ecEXTI2.h`

```c
// Trigger Type Definitions
#define FALL 0
#define RISE 1
#define BOTH 2

void EXTI_init(PinName_t pinName, uint32_t trig_type, uint32_t priority);
void EXTI_enable(PinName_t pinName);
void EXTI_disable(PinName_t pinName);
uint32_t is_pending_EXTI(PinName_t pinName);
void clear_pending_EXTI(PinName_t pinName);
```

#### `EXTI_init()`

Initializes and configures external interrupt for the specified GPIO pin line, including trigger edge selection and NVIC priority setting.

```c
void EXTI_init(PinName_t pinName, uint32_t trig_type, uint32_t priority);
```

- **Parameters:**

  - `pinName`: `(port name)_(pin number)` of type `PinName_t` (e.g., `PA_0`, `PC_13`)
  - `trig_type`: Trigger condition — `FALL` (`0`), `RISE` (`1`), `BOTH` (`2`)
  - `priority`: Interrupt priority level in NVIC (e.g., `0` ~ `15`)

- **Example Code:**

  ```c
  EXTI_init(PA_0, FALL, 0); // Active-low button interrupt with highest priority
  ```

#### `EXTI_enable()`

Enables external interrupt mask (IMR) for the specified pin line.

```
void EXTI_enable(PinName_t pinName);
```

- **Parameters:**

  - `pinName`: `PinName_t` (e.g., `PA_0`)

- **Example Code:**

  ```
  EXTI_enable(PA_0);
  ```

#### `EXTI_disable()`

Disables external interrupt mask (IMR) for the specified pin line.

```
void EXTI_disable(PinName_t pinName);
```

- **Parameters:**

  - `pinName`: `PinName_t` (e.g., `PA_0`)

- **Example Code:**

  ```
  EXTI_disable(PA_0);
  ```

#### `is_pending_EXTI()`

Checks if the EXTI pending bit for the specified pin line is set.

```
uint32_t is_pending_EXTI(PinName_t pinName);
```

- **Parameters:**

  - `pinName`: `PinName_t` (e.g., `PA_0`)

- **Return Value:** `1` if interrupt request is pending, `0` otherwise.

- **Example Code:**

  ```
  if (is_pending_EXTI(PA_0)) {
      // Handle interrupt event
  }
  ```

#### `clear_pending_EXTI()`   

Clears the pending bit in the EXTI pending register (PR) for the specified pin line.   

```
void clear_pending_EXTI(PinName_t pinName);
```

- **Parameters:**

  - `pinName`: `PinName_t` (e.g., `PA_0`)

- **Example Code:**

  ```
  clear_pending_EXTI(PA_0);
  ```

#### **Example code**

This example uses two switch inputs via EXTI to control a 2-digit 7-segment counter:

```c
#include "ecSTM32F4v2.h"

extern volatile uint32_t msTicks;

uint8_t numDisplay1 = 0;
uint8_t numDisplay2 = 0;
uint8_t selectFND1  = 0;
uint8_t selectFND2  = 1;

uint8_t state = 0;
uint8_t pause = 0;

volatile uint32_t curTicks = 0;

// Initialization
void setup(void) {
    MCU_JK_init();  
    curTicks = msTicks;
}

// MAIN ----------------------------------------
int main(void) {
    setup();

    while (1) {
        // 1) Write - Decompose state into digits and multiplex display
        numDisplay1 = state % 10;
        numDisplay2 = state / 10;

        FND_select(selectFND1);
        FND_display(numDisplay1);
        delay_ms(5);

        FND_select(selectFND2);
        FND_display(numDisplay2);
        delay_ms(5);

        // 2) Update counter periodically when not paused
        if (((msTicks - curTicks) > EXTI_TIME_PERIOD) && (pause == 0)) {
            state = (state + 1) % EXTI_NUM_STATE;
            curTicks = msTicks;
        }
    }
}

// EXTI Handler for SW2: Toggle Pause State
void EXTI4_IRQHandler(void) {
    if (is_pending_EXTI(SW2_PIN_JK)) {   
        pause ^= 1;          // Toggle pause flag
        curTicks = msTicks;  // Reset time reference

        clear_pending_EXTI(SW2_PIN_JK);
    }
}

// EXTI Handler for SW1: Reset State to #0
void EXTI2_IRQHandler(void) {
    if (is_pending_EXTI(SW1_PIN_JK)) {   
        state = 0;           // Reset count value
        curTicks = msTicks;  // Reset time reference
        
        clear_pending_EXTI(SW1_PIN_JK);
    }
}
```

## 4 Stepper Motor

Header File (`ecStepper2.h`)

```c
// Stepper Mode
#define HALF    0
#define FULL    1   
#define DIR_CW  0
#define DIR_CCW 1   

// Stepper Motor Spec (28BYJ-48 : 64 steps/rev x 1/32 gear = 2048 steps/rev)
#define STEP_PER_REV  (64*32)   

// State number 
typedef enum {
    S0, S1, S2, S3, S4, S5, S6, S7
} StateNum;

// FULL / HALF stepping sequence - FSM
typedef struct {  
    uint32_t next[2];
    uint8_t out[4];
} StateStepper_t;

// Function Declarations
void stepper_init(uint32_t mode, PinName_t *pinStepper);
void stepper_speed(uint32_t speedRPM);
void stepper_step(uint32_t steps, uint32_t direction);
void stepper_out(uint32_t state);
void stepper_stop(void);
```



### Stepper Motor Functions

#### `stepper_init()`

Initializes the 4 control GPIO pins (A, B, A', B') as digital outputs and sets the operating FSM mode (`FULL` or `HALF` stepping).

```c
void stepper_init(uint32_t mode, PinName_t *pinStepper);
```

- **Parameters:**

  - `mode`: Stepping mode — `FULL` (`1`), `HALF` (`0`)
  - `pinStepper`: Pointer to an array of 4 `PinName_t` pins `[A, B, A', B']`

- **Example Code:**

  ```c
  PinName_t stepperPins[4] = {PB_10, PB_4, PB_5, PB_3};
  stepper_init(FULL, stepperPins);
  ```

#### `stepper_speed()`

Calculates step delay in milliseconds based on the desired rotational speed (RPM) and stepping mode.

```c
void stepper_speed(uint32_t speedRPM);
```

- **Parameters:**

  - `speedRPM`: Target rotational speed in RPM (Revolutions Per Minute)

- **Example Code:**

  ```
  stepper_speed(14); // Set motor speed to 14 RPM
  ```

#### `stepper_step()`

Rotates the stepper motor for a specified number of steps in the given direction using FSM transitions.

```c
void stepper_step(uint32_t steps, uint32_t direction);
```

- **Parameters:**

  - `steps`: Number of steps to move (e.g., `2048` steps for 1 full revolution in `FULL` mode)
  - `direction`: Direction of rotation — `DIR_CW` (`0`), `DIR_CCW` (`1`)

- **Example Code:**

  ```c
  stepper_step(STEP_PER_REV, DIR_CW); // Rotate 1 full revolution clockwise
  ```

#### `stepper_out()`

Outputs digital logic values (`0` or `1`) to the 4 motor drive pins corresponding to the specified FSM state.

```c
void stepper_out(uint32_t state);
```

- **Parameters:**

  - `state`: Current state index (`S0` ~ `S7`) in the active FSM array

- **Example Code:**

  ```c
  stepper_out(S0);
  ```

#### `stepper_stop()`

Stops motor rotation and sets all 4 driver output pins to low logic (`0`) to prevent motor overheating and power consumption.

```c
void stepper_stop(void);
```

- **Example Code:**

  ```c
  stepper_stop();
  ```

#### Complete Example Code (Stepper Motor Control)

This example rotates a 28BYJ-48 stepper motor 1 revolution Clockwise (`DIR_CW`), pauses for 1 second, rotates 1 revolution Counter-Clockwise (`DIR_CCW`), and then stops the motor.

```c
#include "ecSTM32F4v2.h"
#include "ecStepper2.h"

// Define Stepper Motor Pins [A, B, A', B']
PinName_t stepperPins[4] = {PB_10, PB_4, PB_5, PB_3};

void setup(void) {
    RCC_HSI_init();
    
    // Initialize Stepper Motor in Full Step mode
    stepper_init(FULL, stepperPins);
    
    // Set motor speed to 12 RPM
    stepper_speed(12);
}

int main(void) {
    setup();
    
    while(1) {
        // 1. Rotate 1 full turn Clockwise
        stepper_step(STEP_PER_REV, DIR_CW);
        delay_ms(1000);
        
        // 2. Rotate 1 full turn Counter-Clockwise
        stepper_step(STEP_PER_REV, DIR_CCW);
        delay_ms(1000);
        
        // 3. Stop motor drivers to prevent heating
        stepper_stop();
        
        while(1); // Hold execution
    }
}
```

## 5 System Clock (`ecRCC2.h` / `ecRCC2.c`)

### Header File (`ecRCC2.h`)   

```c
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "ecPinNames.h"

extern volatile int EC_SYSCLK; // Current system clock frequency [Hz]

void RCC_HSI_init(void);
void RCC_PLL_init(void);
void delay_ms_HSI(uint32_t ms); 

void RCC_GPIOA_enable(void);
void RCC_GPIOB_enable(void);
void RCC_GPIOC_enable(void);
void RCC_GPIOD_enable(void);
void RCC_GPIOE_enable(void);
void RCC_GPIOH_enable(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __EC_RCC2_H
```

### RCC Functions

#### `RCC_HSI_init()`

Initializes and enables the internal High-Speed Oscillator (HSI, 16 MHz) as the system clock source. Sets `EC_SYSCLK = 16000000`.   

```c
void RCC_HSI_init(void);
```

- **Example Code:**

  ```c
  RCC_HSI_init(); // Configure system clock to 16 MHz
  ```

#### `RCC_PLL_init()`

Configures Flash latency and PLL multipliers to set the system clock (`SYSCLK`) to 84 MHz using the 16 MHz HSI oscillator. Sets `EC_SYSCLK = 84000000`.   

```c
void RCC_PLL_init(void);
```

- **Example Code:**

  ```c
  RCC_PLL_init(); // Set clock to 84 MHz
  ```

#### `delay_ms_HSI()`

Executes a busy-wait loop delay in milliseconds based on the 16 MHz HSI clock.   

```c
void delay_ms_HSI(uint32_t ms);
```

- **Parameters:**

  - `ms`: Delay duration in milliseconds   

- **Example Code:**

  ```c
  delay_ms_HSI(100); // Delay 100 ms
  ```

#### `RCC_GPIOx_enable()`

Enables peripheral clock gate for the specified GPIO port in `RCC->AHB1ENR`.   

```c
void RCC_GPIOA_enable(void);
void RCC_GPIOB_enable(void);
void RCC_GPIOC_enable(void);
void RCC_GPIOD_enable(void);
void RCC_GPIOE_enable(void);
void RCC_GPIOH_enable(void);
```

- **Example Code:**

  ```c
  RCC_GPIOA_enable(); // Enable GPIOA clock
  ```



## 6 SysTick Timer (`ecSysTick2.h` / `ecSysTick2.c`)

### Header File (`ecSysTick2.h`)

```c
#ifndef __EC_SYSTICK2_H
#define __EC_SYSTICK2_H

#include "stm32f4xx.h"
#include "ecRCC2.h"
#include <stdint.1h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern volatile uint32_t msTicks; 

void SysTick_init(void);
void SysTick_Handler(void);
void SysTick_counter(void);
void delay_ms(uint32_t msec);
void SysTick_reset(void);
uint32_t SysTick_val(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __EC_SYSTICK2_H
```

### SysTick Functions

#### `SysTick_init()`

Configures the ARM Cortex-M SysTick core hardware timer to trigger an interrupt every 1 ms using the active `EC_SYSCLK` frequency.   

```c
void SysTick_init(void);
```

- **Note:** Must be called **after** calling `RCC_HSI_init()` or `RCC_PLL_init()`.   

- **Example Code:**

  ```c
  RCC_PLL_init();
  SysTick_init(); // Initialize 1 ms timer ticks
  ```

#### `delay_ms()`

Blocking millisecond delay using the global `msTicks` variable. Uses unsigned arithmetic subtraction to safely handle tick counter overflow.   

```c
void delay_ms(uint32_t msec);
```

- **Parameters:**

  - `msec`: Delay time in milliseconds   

- **Example Code:**

  ```c
  delay_ms(500); // Pause execution for 500 ms
  ```

#### `SysTick_reset()`

Resets the current SysTick counter value register (`VAL`) back to 0.   

```c
void SysTick_reset(void);
```

#### `SysTick_val()`

Reads and returns the current raw value of the SysTick down-counter register (`VAL`).   

```c
uint32_t SysTick_val(void);
```

- **Return Value:** Raw counter value   

## 7 STM32 Board Initialization Helper (`ecSTM32F4v2.h`)

### Header File (`ecSTM32F4v2.h`)

```c
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "math.h"

#include "ecPinNames.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecEXTI2.h"
#include "ecSysTick2.h"
#include "ecStepper2.h"

#define SW1_PIN          PC_13
#define LED_PIN          PA_5

#define SW2_PIN_JK       PA_4
#define SW1_PIN_JK       PD_2

#define EXTI_NUM_STATE   10
#define EXTI_TIME_PREIOD 1000 // [ms]

static PinName_t pinFND[8]    = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
static PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3};

static inline void MCU_JK_init(void) {
    RCC_PLL_init();
    SysTick_init();  
    
    GPIO_init(SW1_PIN_JK, INPUT);
    GPIO_pupd(SW1_PIN_JK, PULL_UP);    

    GPIO_init(SW2_PIN_JK, INPUT);
    GPIO_pupd(SW2_PIN_JK, PULL_UP);

    FND_display_init(pinFND);
    FND_select_init(selectFND);

    EXTI_init(SW2_PIN_JK, FALL, 10);
    EXTI_init(SW1_PIN_JK, FALL, 10);
}

static inline void MCU_init(void) {
    RCC_PLL_init();
    SysTick_init();    
    
    GPIO_init(PC_13, INPUT);
    GPIO_pupd(PC_13, PULL_DOWN);    
    
    GPIO_init(PA_5, OUTPUT);    
}

#endif // __EC_STM_H
```

### Initialization Functions

#### `MCU_init()`

Performs default peripheral initializations for standard STM32 Nucleo-64 board setups:

1. Configures PLL system clock to 84 MHz.   
2. Configures SysTick timer for 1 ms tick timing.   
3. Configures onboard User Button (`PC_13`) with Pull-Down resistor.   
4. Configures onboard Green LED (`PA_5`) as Digital Output.   

```
static inline void MCU_init(void);
```

#### `MCU_JK_init()`

Performs default peripheral initializations for the JKIT evaluation board:

1. Configures PLL system clock to 84 MHz and SysTick for 1 ms ticks.   
2. Initializes push buttons `SW1_PIN_JK` (`PD_2`) and `SW2_PIN_JK` (`PA_4`) as inputs with pull-up resistors.   
3. Initializes 7-segment display segment pins (`pinFND`) and digit select pins (`selectFND`).   
4. Configures EXTI interrupt lines on `SW1_PIN_JK` and `SW2_PIN_JK` on falling edge triggers with priority level 10.   

```
static inline void MCU_JK_init(void);
```


## 8 USART (`ecUART2.h` / `ecUART2.c`)

### Header File (`ecUART2.h`)

```c
#include <stdio.h>
#include <math.h>
#include "stm32f411xe.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"

#define POL 0
#define INT 1

// Baud rate
#define BAUD_9600     9600
#define BAUD_19200    19200
#define BAUD_38400    38400
#define BAUD_57600    57600
#define BAUD_115200   115200
#define BAUD_921600   921600

// General configuration
void USART_init(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX, uint32_t baud);
void USART_baud(USART_TypeDef* USARTx, uint32_t baud);
void USART_pins(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX);
void USART1_baud(uint32_t baud);
void USART2_baud(uint32_t baud);

// Default configuration for USART1, USART2 (9600 baud)
void USART1_init(void);
void USART2_init(void);

// USART write & read
void     USART1_write(uint8_t* buffer, uint32_t nBytes);
void     USART2_write(uint8_t* buffer, uint32_t nBytes);
uint8_t  USART1_read(void);
uint8_t  USART2_read(void);

// RX flag
uint32_t is_USART1_RXNE(void);
uint32_t is_USART2_RXNE(void);

// Low-level
void     USART_clock_enable(USART_TypeDef* USARTx);
void     USART_write(USART_TypeDef* USARTx, uint8_t* buffer, uint32_t nBytes);
uint8_t  USART_read(USART_TypeDef* USARTx);
uint32_t is_USART_RXNE(USART_TypeDef* USARTx);
void     USART_delay(uint32_t us);
```

### USART Pin Map

| USART | TX (default / option) | RX (default / option) | AF | Bus (PLL 84 MHz) | IRQn |
|---|---|---|---|---|---|
| USART1 | `PA_9` / `PB_6` | `PA_10` / `PB_3` | AF7 | APB2 (84 MHz) | `USART1_IRQn` (37) |
| USART2 | `PA_2` | `PA_3` | AF7 | APB1 (42 MHz) | `USART2_IRQn` (38) |
| USART6 | `PA_11` / `PC_6` | `PA_12` / `PC_7` | AF8 | APB2 (84 MHz) | `USART6_IRQn` (71) |

- USART2 (`PA_2`/`PA_3`) is connected to the ST-Link virtual COM port. `printf()` and `scanf()` are retargeted to USART2.
- Frame format: **8 data bits, no parity, 1 stop bit (8N1)**, oversampling ×16.



### 8.1 General Configuration

#### `USART_init()`

Initializes a USART with the given TX/RX pins and baud rate. It configures the GPIO pins (AF), enables the peripheral clock, sets the frame format to 8N1, sets the baud rate, enables TX/RX, and enables the RXNE interrupt in the NVIC (priority 1).

```c
void USART_init(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX, uint32_t baud);
```

- **Parameters:**

  - `USARTx`: USART peripheral — `USART1`, `USART2`, `USART6`
  - `pin_GPIO_TX`: TX pin of type `PinName_t` (e.g., `PA_9`)
  - `pin_GPIO_RX`: RX pin of type `PinName_t` (e.g., `PA_10`)
  - `baud`: Baud rate (e.g., `BAUD_9600`, `BAUD_115200`)

- **Note:** The RXNE interrupt is always enabled. The matching `USARTx_IRQHandler()` must be defined, or the MCU will hang in `Default_Handler` when data is received.

- **Example Code:**

  ```c
  USART_init(USART2, PA_2, PA_3, BAUD_115200);   // PC (USB)
  USART_init(USART1, PA_9, PA_10, BAUD_9600);    // Bluetooth module
  ```

#### `USART_baud()`

Calculates `USARTDIV` from the bus clock and writes the mantissa and fraction to `BRR`. USART2 uses APB1 (`EC_SYSCLK / 2`), and USART1/USART6 use APB2 (`EC_SYSCLK`).

```c
void USART_baud(USART_TypeDef* USARTx, uint32_t baud);
```

- **Formula:**

  - `USARTDIV = f_CK / (16 × baud)`
  - `BRR[15:4]` = Mantissa (integer part), `BRR[3:0]` = Fraction (× 16, rounded)

- **Parameters:**

  - `USARTx`: `USART1`, `USART2`, `USART6`
  - `baud`: Baud rate

- **Note:** This function disables the USART (`UE = 0`) and does **not** re-enable it. `USART_init()` enables it afterwards. When called alone, set `USARTx->CR1 |= USART_CR1_UE;` after it.

- **Example Code:**

  ```c
  USART_baud(USART1, BAUD_38400);
  USART1->CR1 |= USART_CR1_UE;   // re-enable USART1
  ```

#### `USART_pins()`

Configures the TX/RX pins as Alternate Function, Push-Pull, No Pull-up/Pull-down, High speed, and sets the AF number in `AFR` (AF7 for USART1/2, AF8 for USART6).

```c
void USART_pins(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX);
```

- **Parameters:**

  - `USARTx`: `USART1`, `USART2`, `USART6`
  - `pin_GPIO_TX`, `pin_GPIO_RX`: TX/RX pins of type `PinName_t`

- **Example Code:**

  ```c
  USART_pins(USART1, PA_9, PA_10);
  ```

#### `USART1_baud()` / `USART2_baud()`

Wrapper functions of `USART_baud()` for USART1 and USART2.

```c
void USART1_baud(uint32_t baud);
void USART2_baud(uint32_t baud);
```

- **Parameters:**

  - `baud`: Baud rate

- **Example Code:**

  ```c
  USART2_baud(BAUD_115200);
  ```



### 8.2 Default Configuration for Each USART

#### `USART1_init()`

Initializes USART1 with default settings: `PA_9` (TX), `PA_10` (RX), 9600 baud, 8N1, RXNE interrupt enabled.

```c
void USART1_init(void);
```

- **Example Code:**

  ```c
  USART1_init();   // Bluetooth module at 9600 baud
  ```

#### `USART2_init()`

Initializes USART2 with default settings: `PA_2` (TX), `PA_3` (RX), 9600 baud, 8N1, RXNE interrupt enabled. USART2 is connected to the PC through the ST-Link USB.

```c
void USART2_init(void);
```

- **Example Code:**

  ```c
  USART2_init();
  printf("Hello\r\n");   // printed on PC terminal (9600 baud)
  ```



### 8.3 Write & Read

#### `USART1_write()` / `USART2_write()`

Transmits `nBytes` from the buffer through USART1 or USART2 (blocking).

```c
void USART1_write(uint8_t* buffer, uint32_t nBytes);
void USART2_write(uint8_t* buffer, uint32_t nBytes);
```

- **Parameters:**

  - `buffer`: Pointer to the data to send
  - `nBytes`: Number of bytes to send

- **Example Code:**

  ```c
  uint8_t msg[] = "OK\r\n";
  USART1_write(msg, 4);
  
  uint8_t c = 'A';
  USART2_write(&c, 1);   // pass the address, not the value
  ```

#### `USART1_read()` / `USART2_read()`

Waits until one byte is received (RXNE = 1) and returns it (blocking). Reading `DR` clears RXNE.

```c
uint8_t USART1_read(void);
uint8_t USART2_read(void);
```

- **Return Value:** Received byte

- **Example Code:**

  ```c
  uint8_t data = USART1_read();
  ```

#### `is_USART1_RXNE()` / `is_USART2_RXNE()`

Checks if received data is ready in `DR` (non-blocking).

```c
uint32_t is_USART1_RXNE(void);
uint32_t is_USART2_RXNE(void);
```

- **Return Value:** `1` if data is received, `0` otherwise

- **Example Code:**

  ```c
  if (is_USART1_RXNE()) {
      uint8_t data = USART1_read();
  }
  ```



### 8.4 Low-Level Functions

#### `USART_clock_enable()`

Enables the peripheral clock in RCC: USART1/USART6 in `APB2ENR`, USART2 in `APB1ENR`.

```c
void USART_clock_enable(USART_TypeDef* USARTx);
```

#### `USART_write()`

For each byte, waits until `TXE = 1` and writes it to `DR`. After the last byte, waits until `TC = 1` and clears TC.

```c
void USART_write(USART_TypeDef* USARTx, uint8_t* buffer, uint32_t nBytes);
```

#### `USART_read()`

Waits until `RXNE = 1` and returns the lower 8 bits of `DR`.

```c
uint8_t USART_read(USART_TypeDef* USARTx);
```

#### `is_USART_RXNE()`

Returns the RXNE flag of `SR` (`1` = data received).

```c
uint32_t is_USART_RXNE(USART_TypeDef* USARTx);
```

#### `USART_delay()`

Busy-wait delay in microseconds (approximate). Used between bytes in `USART_write()`.

```c
void USART_delay(uint32_t us);
```



### 8.5 printf / scanf Retarget

`printf()` and `scanf()` are redirected to **USART2** (PC terminal).

- Keil (ARMCC): `fputc()` / `fgetc()`
- GCC: `_write()` / `_read()`

```c
USART2_init();                  // or USART_init(USART2, PA_2, PA_3, BAUD_9600);
printf("MCU Initialized\r\n");
```

- **Note:** Do not use `scanf()` / `USART2_read()` in `main` while `USART2_IRQHandler()` also reads `DR`. The ISR takes the byte first, and `main` waits forever.



#### Complete Example Code (Bluetooth → Stepper Motor, PC Monitor)

This example receives commands from a Bluetooth module on USART1 and controls the stepper motor direction. Received characters are printed on the PC terminal through USART2.

```c
#include "ecSTM32F4v2.h"

PinName_t pinStepper[4] = {PB_10, PB_4, PB_5, PB_3};

volatile uint8_t BT_data = 0;
volatile uint8_t BT_flag = 0;

void setup(void) {
    RCC_PLL_init();                      // 84 MHz
    SysTick_init();

    stepper_init(STEPPER_MODE_FULL, pinStepper);
    stepper_speed(5);

    USART_init(USART2, PA_2, PA_3, BAUD_9600);    // PC
    USART_init(USART1, PA_9, PA_10, BAUD_9600);   // Bluetooth
}

int main(void) {
    setup();
    uint32_t steps = 2 * STEP_PER_REV;
    printf("MCU Initialized\r\n");

    while (1) {
        if (BT_flag) {                   // print in main, not in ISR
            BT_flag = 0;
            printf("RX: %c\r\n", BT_data);
        }
        stepper_step(steps, STEPPER_DIR_CCW);
        delay_ms(2000);
    }
}

// Bluetooth command: 'J' = CCW, 'K' = CW, 'P' = stop
void USART1_IRQHandler(void) {
    if (is_USART1_RXNE()) {
        BT_data = USART1_read();         // reading DR clears RXNE
        BT_flag = 1;

        if      (BT_data == 'J') stepper_dir(STEPPER_DIR_CCW);
        else if (BT_data == 'K') stepper_dir(STEPPER_DIR_CW);
        else if (BT_data == 'P') stepper_stop();
    }
}

// PC echo
void USART2_IRQHandler(void) {
    if (is_USART2_RXNE()) {
        uint8_t c = USART2_read();
        USART2_write(&c, 1);
    }
}
```



## 9 Timer & Timer Interrupt (`ecTIM2.h` / `ecTIM2.c`)

### Header File (`ecTIM2.h`)

```c
#include "stm32f411xe.h"
#include "ecPinNames.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "math.h"

// Timer Update Interrupt Initialization
void TIM_UI_init(TIM_TypeDef* TIMx, uint32_t msec);
void TIM_UI_init_us(TIM_TypeDef* TIMx, uint32_t usec);

// Timer Update Interrupt On/Off
void TIM_UI_enable(TIM_TypeDef* TIMx);
void TIM_UI_disable(TIM_TypeDef* TIMx);

// Timer Update Interrupt Flag
uint32_t is_UIF(TIM_TypeDef *TIMx);
void clear_UIF(TIM_TypeDef *TIMx);

// Timer Counter Initialization
void TIM_init(TIM_TypeDef *TIMx, uint32_t msec);
void TIM_init_us(TIM_TypeDef *TIMx, uint32_t usec);

// Timer Counter Period Config
void TIM_period(TIM_TypeDef* TIMx, uint32_t msec);
void TIM_period_us(TIM_TypeDef* TIMx, uint32_t usec);

// Timer Counter Auxiliary Functions
uint32_t get_TIM_IRQn(TIM_TypeDef* TIMx);
void TIM_clock_enable(TIM_TypeDef* TIMx);
```

### Timer Summary

| Timer | Counter | Bus (PLL 84 MHz) | IRQn |
|---|---|---|---|
| TIM1 | 16-bit (Advanced) | APB2 | `TIM1_UP_TIM10_IRQn` |
| TIM2 | 32-bit | APB1 | `TIM2_IRQn` |
| TIM3 | 16-bit | APB1 | `TIM3_IRQn` |
| TIM4 | 16-bit | APB1 | `TIM4_IRQn` |
| TIM5 | 32-bit | APB1 | `TIM5_IRQn` |
| TIM9 | 16-bit | APB2 | `TIM1_BRK_TIM9_IRQn` |
| TIM10 | 16-bit | APB2 | `TIM1_UP_TIM10_IRQn` |
| TIM11 | 16-bit | APB2 | `TIM1_TRG_COM_TIM11_IRQn` |

- Counter clock: `f_cnt = EC_SYSCLK / (PSC + 1)`
- Update event period: `T = (ARR + 1) / f_cnt`
- The library always uses **up-counting** mode.



### 9.1 Timer Counter Initialization

#### `TIM_init()`

Enables the timer clock, sets the period in **msec** (`f_cnt = 10 kHz`), selects up-counter mode, and starts the counter (`CEN = 1`).

```c
void TIM_init(TIM_TypeDef *TIMx, uint32_t msec);
```

- **Parameters:**

  - `TIMx`: Timer peripheral — `TIM1` ~ `TIM5`, `TIM9` ~ `TIM11`
  - `msec`: Period in milliseconds (16-bit timer: `1` ~ `6,553` msec)

- **Example Code:**

  ```c
  TIM_init(TIM3, 100);   // TIM3 update event every 100 ms
  ```

#### `TIM_init_us()`

Same as `TIM_init()`, but the period is in **usec** (`f_cnt = 1 MHz`).

```c
void TIM_init_us(TIM_TypeDef *TIMx, uint32_t usec);
```

- **Parameters:**

  - `TIMx`: Timer peripheral
  - `usec`: Period in microseconds (16-bit timer: `1` ~ `65,536` usec)

- **Example Code:**

  ```c
  TIM_init_us(TIM4, 500);   // TIM4 update event every 500 us
  ```

#### `TIM_period()`

Sets `PSC` and `ARR` for a period in msec with `f_cnt = 10 kHz`.

```c
void TIM_period(TIM_TypeDef* TIMx, uint32_t msec);
```

- **Formula:**

  - `PSC = EC_SYSCLK / 10,000 - 1` (84 MHz → `PSC = 8399`)
  - `ARR = 10 × msec - 1`

- **Limit:** 16-bit ARR (`ARR ≤ 0xFFFF`) → `msec ≤ 6,553`

- **Example Code:**

  ```c
  TIM_period(TIM3, 500);   // 500 ms : PSC = 8399, ARR = 4999
  ```

#### `TIM_period_us()`

Sets `PSC` and `ARR` for a period in usec with `f_cnt = 1 MHz`.

```c
void TIM_period_us(TIM_TypeDef* TIMx, uint32_t usec);
```

- **Formula:**

  - `PSC = EC_SYSCLK / 1,000,000 - 1` (84 MHz → `PSC = 83`)
  - `ARR = usec - 1`

- **Limit:** 16-bit ARR → `usec ≤ 65,536`

- **Example Code:**

  ```c
  TIM_period_us(TIM2, 20000);   // 20 ms : PSC = 83, ARR = 19999
  ```

#### `TIM_clock_enable()`

Enables the timer peripheral clock in RCC: TIM1/TIM9/TIM10/TIM11 in `APB2ENR`, TIM2 ~ TIM5 in `APB1ENR`.

```c
void TIM_clock_enable(TIM_TypeDef* TIMx);
```

#### `get_TIM_IRQn()`

Returns the NVIC IRQ number of the given timer (see the Timer Summary table).

```c
uint32_t get_TIM_IRQn(TIM_TypeDef* TIMx);
```

- **Return Value:** `IRQn_Type` value (e.g., `TIM3_IRQn`)



### 9.2 Timer Update Interrupt

#### `TIM_UI_init()`

Initializes the timer with a period in msec (`TIM_init()`), enables the update interrupt (`UIE`), and enables the IRQ in the NVIC with priority **2**.

```c
void TIM_UI_init(TIM_TypeDef* TIMx, uint32_t msec);
```

- **Parameters:**

  - `TIMx`: Timer peripheral
  - `msec`: Interrupt period in milliseconds (16-bit timer: `1` ~ `6,553`)

- **Note:** The matching `TIMx_IRQHandler()` must be defined, and `clear_UIF()` must be called inside it. Otherwise the interrupt is triggered again immediately.

- **Example Code:**

  ```c
  TIM_UI_init(TIM3, 1);   // TIM3 interrupt every 1 ms
  ```

#### `TIM_UI_init_us()`

Same as `TIM_UI_init()`, but the period is in usec (`TIM_init_us()`).

```c
void TIM_UI_init_us(TIM_TypeDef* TIMx, uint32_t usec);
```

- **Parameters:**

  - `TIMx`: Timer peripheral
  - `usec`: Interrupt period in microseconds (16-bit timer: `1` ~ `65,536`)

- **Example Code:**

  ```c
  TIM_UI_init_us(TIM4, 100);   // TIM4 interrupt every 100 us
  ```

#### `TIM_UI_enable()` / `TIM_UI_disable()`

Enables or disables the update interrupt (`DIER.UIE`). `TIM_UI_enable()` clears `UIF` first, so an old flag does not cause an immediate interrupt.

```c
void TIM_UI_enable(TIM_TypeDef* TIMx);
void TIM_UI_disable(TIM_TypeDef* TIMx);
```

- **Example Code:**

  ```c
  TIM_UI_disable(TIM3);   // pause periodic interrupt
  TIM_UI_enable(TIM3);    // resume
  ```

#### `is_UIF()`

Checks the update interrupt flag (`SR.UIF`).

```c
uint32_t is_UIF(TIM_TypeDef *TIMx);
```

- **Return Value:** non-zero if an update event occurred, `0` otherwise

#### `clear_UIF()`

Clears the update interrupt flag (`SR.UIF`).

```c
void clear_UIF(TIM_TypeDef *TIMx);
```

- **Example Code:**

  ```c
  void TIM3_IRQHandler(void) {
      if (is_UIF(TIM3)) {
          // periodic task
          clear_UIF(TIM3);
      }
  }
  ```

#### Complete Example Code (Timer Interrupt LED Blink)

This example toggles the onboard LED (`PA_5`) every 1 second using a 1 ms TIM3 update interrupt.

```c
#include "ecSTM32F4v2.h"
#include "ecTIM2.h"

volatile uint32_t count = 0;

void setup(void) {
    RCC_PLL_init();              // 84 MHz
    GPIO_init(LED_PIN, OUTPUT);  // PA_5
    TIM_UI_init(TIM3, 1);        // 1 ms update interrupt
}

int main(void) {
    setup();
    while (1) {}
}

void TIM3_IRQHandler(void) {
    if (is_UIF(TIM3)) {
        count++;
        if (count >= 1000) {     // 1000 x 1 ms = 1 sec
            GPIO_write(LED_PIN, !GPIO_read(LED_PIN));
            count = 0;
        }
        clear_UIF(TIM3);         // must clear the flag
    }
}
```



## 10 PWM (`ecPWM2.h` / `ecPWM2.c`)

### Header File (`ecPWM2.h`)

```c
#include "math.h"
#include "stm32f411xe.h"
#include "ecGPIO2.h"
#include "ecTIM2.h"

// PWM Output Mode : OCxM field in CCMRx
#define EC_PWM_MODE1    0   // output active while CNT <  CCR  (default)
#define EC_PWM_MODE2    1   // output active while CNT >= CCR  (inverted)

// PWM Initialization
void PWM_init(PinName_t pinName, uint32_t msec);
void PWM_init_us(PinName_t pinName, uint32_t usec);
void PWM_init_config(PinName_t pinName, uint32_t usec);

// Duty ratio / Pulse width
void PWM_duty(PinName_t pinName, float duty);
void PWM_pulsewidth(PinName_t pinName, uint32_t pulse_width_ms);
void PWM_pulsewidth_us(PinName_t pinName, uint32_t pulse_width_us);

// Internal Helper Functions
void PWM_mode_config(TIM_TypeDef *TIMx, int chN, uint32_t mode);
void PWM_pinmap(PinName_t pinName, TIM_TypeDef **TIMx, int *chN);
```

### PWM Pin Map

| Pin | Timer / Channel | AF |
|---|---|---|
| `PA_0` | TIM2 CH1 | AF1 |
| `PA_1` | TIM2 CH2 | AF1 |
| `PA_5` | TIM2 CH1 | AF1 |
| `PA_6` | TIM3 CH1 | AF2 |
| `PA_8` | TIM1 CH1 | AF1 |
| `PA_9` | TIM1 CH2 | AF1 |
| `PA_10` | TIM1 CH3 | AF1 |
| `PA_15` | TIM2 CH1 | AF1 |
| `PB_3` | TIM2 CH2 | AF1 |
| `PB_4` | TIM3 CH1 | AF2 |
| `PB_5` | TIM3 CH2 | AF2 |
| `PB_6` | TIM4 CH1 | AF2 |
| `PB_7` | TIM4 CH2 | AF2 |
| `PB_8` | TIM4 CH3 | AF2 |
| `PB_9` | TIM4 CH4 | AF2 |
| `PB_10` | TIM2 CH3 | AF1 |
| `PC_6` | TIM3 CH1 | AF2 |
| `PC_7` | TIM3 CH2 | AF2 |
| `PC_8` | TIM3 CH3 | AF2 |
| `PC_9` | TIM3 CH4 | AF2 |

- Pins on the **same timer** share one period (`PSC`, `ARR`). Only the duty ratio (`CCRx`) can be different per channel.
- `PA_0`, `PA_5` and `PA_15` all use **TIM2 CH1**, so they cannot output different PWM signals at the same time.
- TIM5 and TIM9 ~ TIM11 are not mapped in `PWM_pinmap()` yet.

### PWM Mode

| Mode | `OCxM` | Output (up-counting) |
|---|---|---|
| `EC_PWM_MODE1` | `110` | HIGH while `CNT < CCR`, LOW otherwise |
| `EC_PWM_MODE2` | `111` | LOW while `CNT < CCR`, HIGH otherwise |

- Duty ratio (Mode 1): `duty = CCR / (ARR + 1)`



### 10.1 PWM Initialization

#### `PWM_init()`

Initializes a PWM output pin with a period in **msec**. Default: PWM Mode 1, active high, 50% duty.

```c
void PWM_init(PinName_t pinName, uint32_t msec);
```

- **Parameters:**

  - `pinName`: PWM pin of type `PinName_t` (see PWM Pin Map)
  - `msec`: PWM period in milliseconds (`0` is changed to `1`)

- **Range:**

  - TIM2/TIM5 (32-bit): `1` ~ `4,294,967` msec
  - TIM1/TIM3/TIM4 (16-bit): `1` ~ `6,553` msec

- **Example Code:**

  ```c
  PWM_init(PA_1, 20);   // 20 ms period (50 Hz) for RC servo motor
  ```

#### `PWM_init_us()`

Initializes a PWM output pin with a period in **usec**. Use it for high frequency PWM (1 kHz ~ 100 kHz). Default: PWM Mode 1, active high, 50% duty.

```c
void PWM_init_us(PinName_t pinName, uint32_t usec);
```

- **Parameters:**

  - `pinName`: PWM pin of type `PinName_t`
  - `usec`: PWM period in microseconds (minimum `10` usec = 100 kHz)

- **Example Code:**

  ```c
  PWM_init_us(PA_5, 1000);   // 1 kHz PWM on onboard LED
  ```

#### `PWM_init_config()`

Internal function called by `PWM_init()` and `PWM_init_us()`. Steps:

1. Finds the timer and channel from the pin (`PWM_pinmap()`).
2. Sets the GPIO pin as Alternate Function and writes the AF number to `AFR[0]` (pin 0~7) or `AFR[1]` (pin 8~15).
   - AF1: TIM1, TIM2 / AF2: TIM3, TIM4 / AF3: TIM9 ~ TIM11
3. Initializes the timer:
   - TIM2/TIM5, or `usec ≤ 65,536` → `TIM_init_us()` (`f_cnt = 1 MHz`)
   - 16-bit timer with `usec > 65,536` → `TIM_init()` (`f_cnt = 10 kHz`, max 6,553 msec)
4. Sets PWM Mode 1, preload enable, 50% duty, active high, and enables the channel output (`PWM_mode_config()`).
5. For TIM1 only, sets the main output enable (`BDTR.MOE`). Then generates an update event (`EGR.UG`), clears `UIF`, and starts the counter.

```c
void PWM_init_config(PinName_t pinName, uint32_t usec);
```

- **Parameters:**

  - `pinName`: PWM pin of type `PinName_t`
  - `usec`: PWM period in microseconds

- **Note:** TIM1 (advanced timer) does not output any signal without `MOE = 1`.



### 10.2 Duty Ratio Setup

#### `PWM_duty()`

Sets the duty ratio of the PWM pin. Values outside `0.0` ~ `1.0` are clipped.

```c
void PWM_duty(PinName_t pinName, float duty);
```

- **Formula:** `CCRx = (ARR + 1) × duty`

- **Parameters:**

  - `pinName`: PWM pin of type `PinName_t`
  - `duty`: Duty ratio `0.0` (always LOW) ~ `1.0` (always HIGH)

- **Example Code:**

  ```c
  PWM_duty(PA_5, 0.25f);   // 25% duty
  ```

#### `PWM_pulsewidth()`

Sets the HIGH time of the PWM pulse in **msec**. It is converted to a duty ratio and applied with `PWM_duty()`.

```c
void PWM_pulsewidth(PinName_t pinName, uint32_t pulse_width_ms);
```

- **Formula:**

  - `f_cnt = EC_SYSCLK / (PSC + 1)`
  - `counts = f_cnt × pulse_width_ms / 1000`
  - `duty = counts / (ARR + 1)`

- **Parameters:**

  - `pinName`: PWM pin of type `PinName_t`
  - `pulse_width_ms`: Pulse width in milliseconds (must be smaller than the period)

- **Example Code:**

  ```c
  PWM_init(PB_6, 100);
  PWM_pulsewidth(PB_6, 30);   // 30 ms HIGH in 100 ms period (30%)
  ```

#### `PWM_pulsewidth_us()`

Sets the HIGH time of the PWM pulse in **usec**.

```c
void PWM_pulsewidth_us(PinName_t pinName, uint32_t pulse_width_us);
```

- **Formula:** `counts = f_cnt × pulse_width_us / 1,000,000`, `duty = counts / (ARR + 1)`

- **Parameters:**

  - `pinName`: PWM pin of type `PinName_t`
  - `pulse_width_us`: Pulse width in microseconds

- **Example Code:**

  ```c
  PWM_init(PA_1, 20);               // 20 ms period
  PWM_pulsewidth_us(PA_1, 1500);    // 1.5 ms pulse → servo 90°
  ```



### 10.3 Internal Helper Functions

#### `PWM_mode_config()`

Configures the output compare channel of the timer for PWM output.

- `OCxM` = `110` (Mode 1) or `111` (Mode 2) in `CCMR1` (CH1, CH2) / `CCMR2` (CH3, CH4)
- `OCxPE = 1`: CCR preload enable (new CCR value is applied at the next update event)
- `CCRx = ARR / 2`: 50% duty by default
- `CCxP = 0`: active high, `CCxE = 1`: channel output enable

```c
void PWM_mode_config(TIM_TypeDef *TIMx, int chN, uint32_t mode);
```

- **Parameters:**

  - `TIMx`: Timer peripheral
  - `chN`: Channel number `1` ~ `4`
  - `mode`: `EC_PWM_MODE1` or `EC_PWM_MODE2`

#### `PWM_pinmap()`

Returns the timer (`TIMx`) and channel number (`chN`) connected to the given pin (see PWM Pin Map).

```c
void PWM_pinmap(PinName_t pinName, TIM_TypeDef **TIMx, int *chN);
```

- **Parameters:**

  - `pinName`: PWM pin of type `PinName_t`
  - `TIMx`: Pointer where the timer address will be stored
  - `chN`: Pointer where the channel number will be stored

- **Example Code:**

  ```c
  TIM_TypeDef *TIMx;
  int chN;
  PWM_pinmap(PA_1, &TIMx, &chN);   // TIMx = TIM2, chN = 2
  ```



#### Complete Example Code (RC Servo Motor Sweep)

This example drives an RC servo motor on `PA_1` (TIM2 CH2) with a 20 ms PWM period. Every 500 ms (TIM3 interrupt), the pulse width changes from 0.5 ms (0°) to 2.5 ms (180°) in 10 steps. The button (`PC_13`, EXTI) resets the angle to 0°.

```c
#include "ecSTM32F4v2.h"
#include "ecTIM2.h"
#include "ecPWM2.h"

#define PWM_PIN    PA_1

volatile uint32_t angleStep = 0;    // 0 ~ 10 → 0° ~ 180°
volatile uint32_t dir = 0;          // 0 = increase, 1 = decrease

void setup(void) {
    RCC_PLL_init();                  // 84 MHz
    SysTick_init();

    // Button (PC_13) EXTI
    GPIO_init(SW1_PIN, INPUT);
    GPIO_pupd(SW1_PIN, PULL_DOWN);
    EXTI_init(SW1_PIN, FALL, 0);

    // PWM : 20 ms period, 0.5 ms pulse (0°)
    PWM_init(PWM_PIN, 20);
    PWM_pulsewidth_us(PWM_PIN, 500);

    // TIM3 : 500 ms update interrupt
    TIM_UI_init(TIM3, 500);
}

int main(void) {
    setup();
    while (1) {}
}

// Change angle every 500 ms
void TIM3_IRQHandler(void) {
    if (is_UIF(TIM3)) {
        if (dir == 0) { angleStep++; if (angleStep >= 10) dir = 1; }
        else          { angleStep--; if (angleStep == 0)  dir = 0; }

        // 0.5 ms + 0.2 ms per step → 0.5 ms ~ 2.5 ms
        PWM_pulsewidth_us(PWM_PIN, 500 + 200 * angleStep);

        clear_UIF(TIM3);
    }
}

// Button: reset angle to 0°
void EXTI15_10_IRQHandler(void) {
    if (is_pending_EXTI(SW1_PIN)) {
        angleStep = 0;
        dir = 0;
        PWM_pulsewidth_us(PWM_PIN, 500);
        clear_pending_EXTI(SW1_PIN);
    }
}
```



## 11 Timer Input Capture (`ecICAP2.h` / `ecICAP2.c`)

### Header File (`ecICAP2.h`)

```c
#include "stm32f411xe.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecTIM2.h"
#include <math.h>

// Edge Type
#define IC_RISE 0
#define IC_FALL 1
#define IC_BOTH 2

// Input Capture Number
#define IC_1 1
#define IC_2 2
#define IC_3 3
#define IC_4 4

// Input Capture Filter
#define IC_FILTER_0     0   // no filter, transition sampled at fDTS
#define IC_FILTER_1     1   // N = 2 samples at fCK_INT
#define IC_FILTER_2     2   // N = 4 samples at fCK_INT
#define IC_FILTER_3     3   // N = 8 samples at fCK_INT

// Input Capture Initialization
void ICAP_init(PinName_t pinName, uint32_t edge_type);
void ICAP_ICn_init(PinName_t pinName, uint32_t edge_type, uint32_t ICn);
void ICAP_tick(PinName_t pinName, uint32_t usec);
uint32_t ICAP_read(PinName_t pinName, uint32_t ICn);

// Capture/Compare Interrupt Flag
uint32_t is_CCIF(TIM_TypeDef *TIMx, uint32_t ICn);
void clear_CCIF(TIM_TypeDef *TIMx, uint32_t ICn);

// Internal Functions
void ICAP_pinmap(PinName_t pinName, TIM_TypeDef **TIMx, int *chN);
void ICAP_filter(TIM_TypeDef *TIMx, uint32_t ICn, uint16_t filterOpt);
void ICAP_ICn_select(TIM_TypeDef *TIMx, uint32_t TIn, uint32_t ICn);
```

### Input Capture Overview

- When the selected edge appears on the pin, the current counter value (`CNT`) is copied to `CCRn`, and the `CCnIF` flag is set.
- **TIn** is the timer input pin channel (e.g., `PB_6` = TIM4 **TI1**). **ICn** is the capture channel that stores the value in `CCRn`.
- One pin (TI1) can be connected to two capture channels: **IC1** (direct, `CCxS = 01`) and **IC2** (indirect, `CCxS = 10`). Using IC1 = rising edge and IC2 = falling edge, the pulse width of one signal can be measured.

| TIn (pin) | Direct ICn (`CCxS = 01`) | Indirect ICn (`CCxS = 10`) |
|---|---|---|
| TI1 | IC1 | IC2 |
| TI2 | IC2 | IC1 |
| TI3 | IC3 | IC4 |
| TI4 | IC4 | IC3 |

- **Time between two captures:**

  `Δt = [ (CCR_b − CCR_a) + ovf_cnt × (ARR + 1) ] × tick`

  `ovf_cnt` is the number of counter overflows (Update events) between the two captures.

### Input Capture Pin Map

The pin map is the same as the PWM pin map (section 10).

| Pin | Timer / TIn | AF |
|---|---|---|
| `PA_0`, `PA_5`, `PA_15` | TIM2 TI1 | AF1 |
| `PA_1` | TIM2 TI2 | AF1 |
| `PA_6` | TIM3 TI1 | AF2 |
| `PA_8` / `PA_9` / `PA_10` | TIM1 TI1 / TI2 / TI3 | AF1 |
| `PB_3` | TIM2 TI2 | AF1 |
| `PB_4` / `PB_5` | TIM3 TI1 / TI2 | AF2 |
| `PB_6` / `PB_7` / `PB_8` / `PB_9` | TIM4 TI1 / TI2 / TI3 / TI4 | AF2 |
| `PB_10` | TIM2 TI3 | AF1 |
| `PC_6` / `PC_7` / `PC_8` / `PC_9` | TIM3 TI1 / TI2 / TI3 / TI4 | AF2 |

- `ICAP_ICn_init()` also sets AF2 for TIM5 and AF3 for TIM9 ~ TIM11, but these timers are not mapped in `ICAP_pinmap()` yet.



### 11.1 Input Capture Initialization

#### `ICAP_init()`

Initializes Input Capture on the pin with the **default capture channel (ICn = TIn)** and a **1 usec tick**.

```c
void ICAP_init(PinName_t pinName, uint32_t edge_type);
```

- **Parameters:**

  - `pinName`: Input Capture pin of type `PinName_t` (see Pin Map)
  - `edge_type`: `IC_RISE` (`0`), `IC_FALL` (`1`), `IC_BOTH` (`2`)

- **Example Code:**

  ```c
  ICAP_init(PB_6, IC_RISE);   // TIM4 TI1 → IC1, rising edge
  ```

#### `ICAP_ICn_init()`

Initializes Input Capture on the pin with a **user-selected capture channel (ICn)**. Steps:

1. **GPIO**: AF mode, no pull-up/pull-down, high speed, and the AF number in `AFR` (AF1: TIM1/TIM2, AF2: TIM3 ~ TIM5, AF3: TIM9 ~ TIM11).
2. **Timer**: Enables the clock, the Update Interrupt and the NVIC (priority 2) using `TIM_UI_init_us()`. Then sets tick = 1 usec and ARR = maximum (`ICAP_tick()`). The counter is stopped during configuration.
3. **Input Capture**:
   - Connects TIn to ICn (`ICAP_ICn_select()`, `CCxS` in `CCMRx`)
   - Input filter: `IC_FILTER_3` (N = 8 samples), for this channel only
   - Capture prescaler: default (capture every edge)
   - Edge (`CCnP` / `CCnNP` in `CCER`): rising `00`, falling `01`, both `11`
   - Enables capture (`CCnE`)
4. **Interrupt**: Enables the capture interrupt (`CCnIE`) and the update interrupt (`UIE`) in `DIER`, then starts the counter.

```c
void ICAP_ICn_init(PinName_t pinName, uint32_t edge_type, uint32_t ICn);
```

- **Parameters:**

  - `pinName`: Input Capture pin of type `PinName_t`
  - `edge_type`: `IC_RISE`, `IC_FALL`, `IC_BOTH`
  - `ICn`: Capture channel `IC_1` ~ `IC_4` (must be a direct or indirect channel of TIn, see the table above)

- **Note:**

  - Both `CCnIE` and `UIE` are enabled, so the matching `TIMx_IRQHandler()` must be defined, and it must clear **both** `CCnIF` and `UIF`.
  - Every call resets the tick to 1 usec. Call `ICAP_tick()` **after** all `ICAP_init()` / `ICAP_ICn_init()` calls.

- **Example Code:**

  ```c
  ICAP_init(PB_6, IC_RISE);              // TI1 → IC1 : rising edge
  ICAP_ICn_init(PB_6, IC_FALL, IC_2);    // TI1 → IC2 : falling edge
  ICAP_tick(PB_6, 10);                   // tick = 10 usec  <- last
  ```

#### `ICAP_tick()`

Sets the counter tick (time of one count) in **usec** by changing `PSC`. `ARR` is set to maximum (`0xFFFF`, or `0xFFFFFFFF` for TIM2/TIM5). Then generates an update event (`UG`) and clears `UIF`.

```c
void ICAP_tick(PinName_t pinName, uint32_t usec);
```

- **Formula:**

  - `PSC = usec × EC_SYSCLK / 1,000,000 − 1`
  - `f_cnt = EC_SYSCLK / (PSC + 1) = 1 / tick`

- **Parameters:**

  - `pinName`: Input Capture pin of type `PinName_t`
  - `usec`: Tick in microseconds (`0` is changed to `1`)

- **Range:** PSC is 16-bit → `1` ~ `780` usec at 84 MHz (larger values are clamped to `PSC = 65535`).

- **Note:** `TIM_init_us()` / `TIM_period_us()` cannot be used for Input Capture, because they fix `PSC` for a 1 MHz counter and set `ARR` by the period.

- **Example Code:**

  ```c
  ICAP_tick(PB_6, 10);   // 84 MHz: PSC = 839, ARR = 0xFFFF
                         // overflow period = 65536 × 10 us ≈ 655 ms
  ```

#### `ICAP_read()`

Returns the captured counter value `CCRn` of the given capture channel.

```c
uint32_t ICAP_read(PinName_t pinName, uint32_t ICn);
```

- **Parameters:**

  - `pinName`: Input Capture pin of type `PinName_t`
  - `ICn`: Capture channel `IC_1` ~ `IC_4`

- **Return Value:** Captured counter value (in ticks)

- **Note:** Reading `CCRn` also clears `CCnIF` in hardware.

- **Example Code:**

  ```c
  uint32_t t1 = ICAP_read(PB_6, IC_1);
  ```



### 11.2 Capture/Compare Interrupt Flag

#### `is_CCIF()`

Checks the capture/compare interrupt flag `CCnIF` (`SR` bit n).

```c
uint32_t is_CCIF(TIM_TypeDef *TIMx, uint32_t ICn);
```

- **Parameters:**

  - `TIMx`: Timer peripheral
  - `ICn`: Capture channel `1` ~ `4`

- **Return Value:** `1` if a capture occurred, `0` otherwise

#### `clear_CCIF()`

Clears the capture/compare interrupt flag `CCnIF`. Only that bit is written as `0`, so the other flags are not changed (`SR` bits are cleared by writing `0`).

```c
void clear_CCIF(TIM_TypeDef *TIMx, uint32_t ICn);
```

- **Example Code:**

  ```c
  if (is_CCIF(TIM4, IC_1)) {
      time1 = ICAP_read(PB_6, IC_1);
      clear_CCIF(TIM4, IC_1);
  }
  ```



### 11.3 Internal Functions

#### `ICAP_pinmap()`

Returns the timer (`TIMx`) and the timer input channel (`TIn`) connected to the pin (see Pin Map).

```c
void ICAP_pinmap(PinName_t pinName, TIM_TypeDef **TIMx, int *chN);
```

#### `ICAP_filter()`

Sets the input filter `ICnF` (4 bits) of one capture channel. `ICnF` is at bit 4 (IC1, IC3) or bit 12 (IC2, IC4) of `CCMR1` (IC1, IC2) / `CCMR2` (IC3, IC4). An edge is accepted only after N stable samples, so short noise pulses are removed.

```c
void ICAP_filter(TIM_TypeDef *TIMx, uint32_t ICn, uint16_t filterOpt);
```

- **Parameters:**

  - `TIMx`: Timer peripheral
  - `ICn`: Capture channel `1` ~ `4`
  - `filterOpt`: `IC_FILTER_0` ~ `IC_FILTER_3`

- **Note:** Only the selected channel is written. In `CCMRx`, the `ICxF` bits are at the same position as the `OCxM` bits, so writing every channel would change a PWM output channel on the same timer.

#### `ICAP_ICn_select()`

Connects the timer input `TIn` to the capture channel `ICn` by setting `CCnS` in `CCMRx`.

- `ICn == TIn` → `CCnS = 01` (direct)
- `ICn != TIn` → `CCnS = 10` (indirect: TI1 ↔ IC2, TI3 ↔ IC4)

```c
void ICAP_ICn_select(TIM_TypeDef *TIMx, uint32_t TIn, uint32_t ICn);
```

- **Parameters:**

  - `TIMx`: Timer peripheral
  - `TIn`: Timer input channel of the pin `1` ~ `4`
  - `ICn`: Capture channel `1` ~ `4`



#### Complete Example Code (Ultrasonic Distance Sensor HC-SR04)

The HC-SR04 sends a HIGH pulse on ECHO whose width is the round-trip time of the ultrasonic wave.

- TRIG (`PA_6`, TIM3 CH1): PWM, 50 ms period, 10 us pulse
- ECHO (`PB_6`, TIM4 TI1): IC1 = rising edge, IC2 = falling edge, tick = 10 us
- Distance [cm] = pulse width [us] / 58

```c
#include "ecSTM32F4v2.h"
#include "ecTIM2.h"
#include "ecPWM2.h"
#include "ecICAP2.h"
#include "ecUART2.h"

#define TRIG    PA_6
#define ECHO    PB_6
#define TICK_US 10

volatile uint32_t ovf_cnt = 0;
volatile uint32_t time1 = 0, time2 = 0;
volatile float    distance = 0.0f;

void setup(void) {
    RCC_PLL_init();                        // 84 MHz
    SysTick_init();
    USART2_init();                         // printf → PC

    // TRIG : PWM 50 ms period, 10 us pulse
    PWM_init(TRIG, 50);
    PWM_pulsewidth_us(TRIG, 10);

    // ECHO : Input Capture
    ICAP_init(ECHO, IC_RISE);              // TI1 → IC1 : rising edge
    ICAP_ICn_init(ECHO, IC_FALL, IC_2);    // TI1 → IC2 : falling edge
    ICAP_tick(ECHO, TICK_US);              // tick = 10 us  <- last
}

int main(void) {
    setup();
    while (1) {
        printf("Distance: %.1f cm\r\n", distance);
        delay_ms(500);
    }
}

void TIM4_IRQHandler(void) {
    // Counter overflow
    if (is_UIF(TIM4)) {
        ovf_cnt++;
        clear_UIF(TIM4);
    }
    // Rising edge : pulse start
    if (is_CCIF(TIM4, IC_1)) {
        time1 = ICAP_read(ECHO, IC_1);
        ovf_cnt = 0;
        clear_CCIF(TIM4, IC_1);
    }
    // Falling edge : pulse end
    if (is_CCIF(TIM4, IC_2)) {
        time2 = ICAP_read(ECHO, IC_2);
        uint32_t ticks = (time2 - time1) + ovf_cnt * (TIM4->ARR + 1);
        distance = (float)(ticks * TICK_US) / 58.0f;   // [cm]
        ovf_cnt = 0;
        clear_CCIF(TIM4, IC_2);
    }
}
```
