# LAB: USART - Bluetooth

**Date:** 2026-09-26

**Author/Partner:** Juyoung Lee

**Github:** [22200576-JUYOUNG-LEE/EC-STM32F4-22200576: Firmware for STM32F4, Embedded Controller, HGU](https://github.com/22200576-JUYOUNG-LEE/EC-STM32F4-22200576)

**Demo Video:** Youtube link

**PDF version:**

## Introduction

In this lab, we will learn how to configure and use ‘USART(Universal synchronous asynchronous receiver transmitter)’ of MCU. Then, we will learn how to communicate between your PC and MCU and MCU to another MCU with wired serial communication.

* **Mission 1**: Control LED(LD2) of other MCU using wired serial communication
* **Mission 2**: Control a stepper motor with wireless Bluetooth communication

You must submit

* LAB Report (\*.md & \*.pdf)
* Zip source files(main\*.c, ecRCC.h, ecGPIO.h, ecSysTick.c etc...).
  * Only the source files. Do not submit project files

### Requirement

#### Hardware

* MCU
  * NUCLEO-F411RE
* Actuator/Sensor/Others:
  * DC motor, DC motor driver(L9110s),
  * Bluetooth Module(HC-06),

#### Software

* Keil uVision, CMSIS, EC\_HAL library

## Tutorial: USART with TeraTerm

Learn how to use USART library and the serial monitor TeraTerm

{% content-ref url="/pages/gMZ3F9dPug4zL3Uy10CQ" %}
[Tutorial: USART with TeraTerm](/ec/ec-course/tutorial/tutorial-usart-with-teraterm.md)
{% endcontent-ref %}

## Problem 0: USART library

Download the following files:

* ecUART2\_student.c
* ecUART2\_student.h

Then, change their names as

* ecUART2.c
* ecUART2.h

> Header files must locate in  `EC\include\`. Fill-in the comment box.
>
> Update  ecSTM32F4v2.h&#x20;

### Exercise

1. Fill in the blanks in the library files.&#x20;
2. Then, run the test code in different baud rate to validate your code. &#x20;

   \* Tera-Term Terminal:   (1) Baud 9600,  (2) Baud 57600
3. You may do the exercise after you complete this Lab

**ecUSART2.h**

{% code expandable="true" %}

```cpp
#define BAUD_9600	9600
#define BAUD_38400  38400
#define BAUD_57600	57600
#define BAUD_115200 115200

/*================ EXERCISE: Fill In the Blanks ================*/
/*================ General Configuration ================*/
// General configuration
void USART_init(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX, uint32_t baud);	// copy of USART_setting
void USART_baud(USART_TypeDef* USARTx, uint32_t baud);
void USART_pins(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX);
void USART1_baud(uint32_t baud);
void USART2_baud(uint32_t baud);

// Example:
//  For BT serial : specific RX/TX pins 
//  USART_init(USART1, PA_9,PA_10, BAUD_9600); 	// PA9 - RXD , PA10 - TXD


/*================ PROVIDED  DON'T MODIFY HERE ================*/
/*================ Configuration for each USART ================*/
// Configuration UART 1, 2 using default settings
void USART1_init(void);
void USART2_init(void);

// USART write & read
void USART1_write(uint8_t* buffer, uint32_t nBytes);
void USART2_write(uint8_t* buffer, uint32_t nBytes);
uint8_t USART1_read(void);										
uint8_t USART2_read(void);	

// RX Inturrupt Flag USART1,2
uint32_t is_USART1_RXNE(void);
uint32_t is_USART2_RXNE(void);

/*================ General Configuration ================*/
void USART_clock_enable(USART_TypeDef* USARTx);
void USART_write(USART_TypeDef* USARTx, uint8_t* buffer, uint32_t nBytes);
uint8_t USART_read(USART_TypeDef * USARTx);										
uint32_t is_USART_RXNE(USART_TypeDef * USARTx);
void USART_delay(uint32_t us);  
```

{% endcode %}

**TU\_USART\_testrun.c**

```cpp
#include <stdio.h>
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecUART2.h"


uint8_t PC_string[]="MCU Initialized \r\n";

// Initialiization 
void setup(void)
{
	// MCU_init();
    RCC_PLL_init();
    SysTick_init();
    // USART2: PA_2=TX, PA_3=RX, 57600 baud
    USART_init(USART2, PA_2, PA_3, BAUD_57600);	
}


int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
  USART2_write(PC_string, sizeof(PC_string));
	
  uint32_t lineNumber = 0;
  // Inifinite Loop ----------------------------------------------------------
	while (1){
        printf("Line %lu >>\r\n", lineNumber);
        delay_ms(2000);
        lineNumber++;
    }
}

```

## Problem 1: Communicate MCU1-MCU2 using RS-232

<img src="https://user-images.githubusercontent.com/91526930/199908079-a7a06848-2246-43af-a973-03b50a77d8ea.png" alt="" width="375">

### Procedure

1\. Create a new project under the directory **`EC\LAB\`**

* The environment name is “**LAB\_USART\_LED”.**
* Create a new source files named as “**LAB\_USART\_LED.c”**

> You MUST write your name on the source file inside the comment section.

2\. Make sure you updated the library in `\EC\include\` &#x20;

* **ecUART2.h, ecUART2.c**
* Also, update **ecSTM32F4v2.h**

3\. Connect each MCUs to each PC with **USART 2** via USB cable (ST-Link)

* MCU1 to PC1
* MCU2 to PC2

4\. Connect MCU1 to MCU2 with **USART 1**

* Connect RX/TX pins externally with jumper wires as
  * MCU1\_TX to MCU2\_RXD
  * MCU1\_RX to MCU2\_TX

5. Connect **GND pins** between MCU1-MCU2

{% hint style="info" %}
MUST connect the same GND pin between MCU1 and MCU2
{% endhint %}

6. Send a message from PC\_1 by typing keys on Teraterm.&#x20;

> You have to press "Enter" to end the message.

7. The received message by MCU\_2 should be displayed on PC\_2.
8. Turn other MCU's LED(LD2) ON/OFF by :

* Press key "**L**" to Turn OFF LED
* Press key "**H**" to Turn ON LED

### Configuration

| Function                        | Port - Pin                   | Configuration                                                |
| ------------------------------- | ---------------------------- | ------------------------------------------------------------ |
| System Clock                    |                              | PLL 84MHz                                                    |
| USART2 : USB cable (ST-Link)    |                              | <p>No Parity, 8-bit Data,<br>1-bit Stop bit,<br>9600 baud-rate</p> |
| USART1 : MCU1,  MCU2            | <p>TXD: PA9<br>RXD: PA10</p> | <p>No Parity, 8-bit Data,<br>1-bit Stop bit,<br>9600 baud-rate</p> |
| <p>Digital Out: LD2<br><br></p> | <p>PA5<br><br></p>           |                                                              |

### Example Code

Example codes for communicating between two MCUs

**Example 1 :** RX/TX **one character** at a time

* Receive (Interrupt), Send (Polling)
* USART2:&#x20;
  * Sending a character from PC\_keyboard to MCU
  * Sending a character from MCU to PC serial monitor (**Echo keyboard type**)
* USART1:
  * Send and Receive a character from other MCU
* To Display the received message on PC monitor
  * MCU\_2:USART1 -> MCU\_1:USART1 -> MCU\_1:USART2-> Serial monitor\_1

**Example 2 :** **RX/TX** **a string**

{% tabs %}
{% tab title="Example 1 :  Character comm" %}
{% code expandable="true" %}

```c
#include "ecSTM32F4v2.h"// this should include #include "ecUART2.h" 
//#include "ecUART2.h" 

static volatile uint8_t PC_Data = 0;
static volatile uint8_t BT_Data = 0;
uint8_t PC_string[]="Loop:\r\n";

void setup(void){
	RCC_PLL_init();
	SysTick_init();
	
	// USART2: USB serial init
	USART2_init();  // default 9600
	//USART2_baud(BAUD_38400);
	
	// USART1: MCU2 serial init 
	USART1_init(); // default 9600
	//USART1_baud(BAUD_38400);
}

int main(void){	
	setup();
	printf("MCU Initialized\r\n");	
	while(1){
		// USART Receive: Use Interrupt only
		// USART Transmit: Polling
		//USART2_write(PC_string, 7);
		delay_ms(2000);        
	}
}

void USART2_IRQHandler(){       // USART2 RX Interrupt 
	if(is_USART2_RXNE()){
		PC_Data = USART2_read();		// RX from UART2 (PC)
		USART2_write(&PC_Data,1);		// TX to USART2	 (PC)	 Echo of keyboard typing
		USART1_write(&PC_Data,1);	  // TX to Other MCU
	}
}

void USART1_IRQHandler(){          	// USART2 RX Interrupt
	if(is_USART1_RXNE()){
		BT_Data = USART1_read();				// RX from other MCU UART1		
		printf("RX: %c \r\n",BT_Data); 	// TX to USART2(PC)
	}
}
```

{% endcode %}
{% endtab %}

{% tab title="Example 2: String comm" %}

```cpp
#include "ecSTM32F4v2.h"  // this should include #include "ecUART2.h"

#define MAX_BUF 10
#define END_CHAR 13

static volatile uint8_t buffer[MAX_BUF]={0, };
static volatile uint8_t PC_string[MAX_BUF]={0, };
static volatile uint8_t PC_data = 0;

static volatile int idx = 0;
static volatile int bReceive =0;

void setup(void){
    RCC_PLL_init();
    SysTick_init();

    // USART2: USB serial init
    USART2_init();
    //USART2_baud(BAUD_38400);
    // USART1: MCU2 serial init
    USART1_init();
    //USART1_baud(BAUD_38400);
}

int main(void){
    setup();
    printf("MCU Initialized\r\n");

    while(1){
        if (bReceive == 1){
            printf("PC_string: %s\r\n", PC_string);
            bReceive = 0;
        }
    }
}

void USART2_IRQHandler(){       // USART2 RX Interrupt
    if(is_USART2_RXNE()){
        PC_data = USART2_read();     // RX from UART2 (PC)
        USART2_write(&PC_data,1);    // TX to USART2 (PC) Echo of keyboard typing

        // Creates a String from received serial character
        if(PC_data != END_CHAR && (idx < MAX_BUF)){
            buffer[idx] = PC_data;
            idx++;
        }
        else if (PC_data== END_CHAR) {
            bReceive = 1;
            // reset PC_string
            memset(PC_string, 0, sizeof(char) * MAX_BUF);
            // copy to PC_string
            memcpy(PC_string, buffer, sizeof(char) * idx);
            // reset buffer
            memset(buffer, 0, sizeof(char) * MAX_BUF);
            idx = 0;
        }
        else{       // if(idx >= MAX_BUF)
            idx = 0;
            // reset PC_string
            memset(PC_string, 0, sizeof(char) * MAX_BUF);
            // reset buffer
            memset(buffer, 0, sizeof(char) * MAX_BUF);
            printf("ERROR : Too long string\r\n");
        }
    }
}

// Added: receive a string from the other MCU through PA9/PA10.
void USART1_IRQHandler(){
    if(is_USART1_RXNE()){
        PC_data = USART1_read();
        USART2_write(&PC_data,1); // Echo received character to local PC

        if(PC_data != END_CHAR && (idx < MAX_BUF)){
            buffer[idx] = PC_data;
            idx++;
        }
        else if (PC_data == END_CHAR){
            bReceive = 1;
            memset(PC_string, 0, sizeof(char) * MAX_BUF);
            memcpy(PC_string, buffer, sizeof(char) * idx);
            memset(buffer, 0, sizeof(char) * MAX_BUF);
            idx = 0;
        }
        else{
            idx = 0;
            memset(PC_string, 0, sizeof(char) * MAX_BUF);
            memset(buffer, 0, sizeof(char) * MAX_BUF);
            printf("ERROR : Too long string\r\n");
        }
    }
}

```

{% endtab %}
{% endtabs %}

#### Code

Your code goes here

Explain your source code with necessary comments.

```c++
// YOUR MAIN CODE ONLY
// YOUR CODE
```

#### Result

Experiment images and results

> Show experiment images /results

Add [demo video link](/ec/ec-course/lab/lab-usart-led-bluetooth.md)

#### **Discussion**

Analyze the result and explain any other necessary discussion.

***

## Problem 2: Control Stepper Motor via Bluetooth

![image](https://user-images.githubusercontent.com/38373000/200286160-bc9cd583-8f7e-4dc0-8acd-40135287287d.png)

### Tutorial: Bluetooth

Follow the tutorial for Bluetooth setting.

{% content-ref url="/pages/0hcwqpOQ2L0RzdlCgqES" %}
[Tutorial: Bluetooth](/ec/ec-course/tutorial/tutorial-bluetooth.md)
{% endcontent-ref %}

Search for the bluetooth module specification sheet (HC-06) and study the pin configurations.

* The default PIN number is 1234.

Example of connecting to USART1

![image](https://user-images.githubusercontent.com/38373000/200287085-970d9741-d80b-4af1-b3ac-e7b0b0b58913.png)

### Procedure

1\. Create a new project&#x20;

* The environment name is “**LAB\_USART\_Bluetooth”.**
* Create a new source files named as “**LAB\_USART\_Bluetooth.c”**

> You MUST write your name on the source file inside the comment section.

2\. Make sure your updated library in `include\` to your project.

* **ecUART2.h, ecUART2.c**
* Update **ecSTM32F4v2.h**

3\. Connect the MCU-BT module. Use USART 1

* Connect RX/TX pins as
  * **MCU\_TX to  BT\_RX**
  * **MCU\_ RX to BT\_TX**

4\.  Connect the BT module (HC-06) to the PC

5. Check the Bluetooth connection by turning MCU's LED(LD2) ON/OFF by sending text of "**L**" or "**H**" from the PC (serial terminal)
6. Connect a stepper motor to MCU  (**see Lab: Stepper**)&#x20;
7. Control Step motor by the Keyboard inputs

* Rotate the stepper motor 2 revolutions with an appropriate speed&#x20;
* Press 'J' : To run CW direction&#x20;
* Press 'K' : To run CCW direction
* Press 'P' : Emergency STOP

You may use other the key inputs as your preference.

### Configuration

| Function                                         | Port - Pin                   | Configuration                                                |
| ------------------------------------------------ | ---------------------------- | ------------------------------------------------------------ |
| **System Clock**                                 |                              | PLL 84MHz                                                    |
| **USART1 : MCU -Bluetooth**                      | <p>TXD: PA9<br>RXD: PA10</p> | <p>No Parity, 8-bit Data,<br>1-bit Stop bit,<br>9600 baud-rate</p> |
| <p><strong>Digital Out: LD2</strong><br><br></p> | <p>PA5<br><br></p>           |                                                              |
| **Motor Pin A, B, A', B'**                       | PB10, PB4, PB5, PB3          | DOUT                                                         |
|                                                  |                              |                                                              |

#### Code

Your code goes here

Explain your source code with necessary comments.

```c++
// YOUR MAIN CODE ONLY
// YOUR CODE
```

#### Result

Experiment images and results

> Show experiment images /results

Add [demo video link](/ec/ec-course/lab/lab-usart-led-bluetooth.md)

#### **Discussion**

Analyze the result and explain any other necessary discussion.

## Reference

Complete list of all references used (github, blog, paper, etc)

```
```

## Troubleshooting

####