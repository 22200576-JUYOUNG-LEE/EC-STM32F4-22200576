/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Juyoung Lee
Created          : 05-03-2021
Modified         : 10-21-2026 
Language/ver     : C++ in VS Code

Description      : UART P1, Communicate MCU1-MCU2 using RS-232
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"
//#include "ecUART2.h" 

#define LED_PIN_2 PA_5

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

	// setting led
	GPIO_init	(LED_PIN_2, OUTPUT);   
	GPIO_pupd	(LED_PIN_2, PULL_UP);
	GPIO_otype	(LED_PIN_2, PUSH_PULL);
	GPIO_ospeed	(LED_PIN_2, SPEED_MEDIUM);


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

	if (BT_Data == 'L'){
		GPIO_write(LED_PIN_2, LOW);
	}else if (BT_Data == 'H'){
		GPIO_write(LED_PIN_2, HIGH);
	}
}