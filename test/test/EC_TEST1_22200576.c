/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Juyoung Lee
Student ID       : 22200576
Created          : 10-22-2026
Language/ver     : C in VS Code

Description      : EC Test 1 (2026) Programming Test
                   
                   Rename this file to  EC_Test1_yourID.c
/----------------------------------------------------------------*/


#include "stm32f411xe.h"

//==================================================================
//  EC Library Include
//==================================================================
#include "ecSTM32F4v2.h"


//==================================================================
// User GPIO Pin definitions
//==================================================================
// YOUR CODE GOES HERE
// YOUR CODE GOES HERE

// Stepper ( A, B, AN, BN )
PinName_t   pinStepper[4]         = {PC_5, PC_6, PC_7, PC_8};

// FND segments
PinName_t pinFND[8]          = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};

// FND digit select : FND_0 ~ FND_3
PinName_t selectFND[4]       = {PA_10, PA_11, PC_4, PC_3};

// Global Variables
// use   volatile uint32_t   instead of uint32_t if used in Interrupt ISR
volatile uint32_t motor_mode   = 0;	
volatile uint32_t motor_dir    = STEPPER_DIR_CW;	
volatile uint32_t motor_speed  = 0;	


volatile uint32_t preserve_motor_mode   = 0;	
volatile uint32_t preserve_motor_dir    = STEPPER_DIR_CW;	
volatile uint32_t preserve_motor_speed  = 0;	
	

uint8_t numDisplay1     = 0;
uint8_t selectFND1      = 0;

uint8_t     PC_data 	= 0;


//State number 
typedef enum {
	T_S0, T_S1, T_S2, T_S3
} TEST_StateNum;

// FULL stepping sequence  - FSM
typedef struct {	
  uint32_t next[2];
  uint8_t out[2];
} TEST_StateStepper_t;

// Output: A B A' B'
static const TEST_StateStepper_t TEST_fsm[4] = {	
	{{T_S1,T_S0},{0,0}},		
	{{T_S2,T_S0},{2,1}},		
	{{T_S3,T_S1},{6,2}},		
	{{T_S3,T_S2},{10,3}},		
		
};




//==================================================================
//  Setup 
//==================================================================
void setup(void){
	// RCC_PLL_init();					// System Clock = 84MHz
	// SysTick_init();					// SysTick, 1 msec
	

	MCU_JK_init();

	stepper_init(STEPPER_MODE_FULL, pinStepper);
    stepper_speed(motor_speed);

	// set state 0
    FND_select(selectFND1);
    FND_display(numDisplay1);

	GPIO_write(LED_PIN_JK, HIGH);

	motor_mode = T_S0;

	// USART2, 9600 baud
	// YOUR CODE GOES HERE			
	// YOUR CODE GOES HERE			

	// GPIO Inputs
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE

	// EXTI 
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE
	
	// GPIO Outputs	
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE

	// Other Setup
	// YOUR CODE GOES HERE
}


//==================================================================
//  Main
//==================================================================
int main(void){
	setup();
	printf("EC_TEST1_22200576 Initialized!\r\n");

	uint32_t currTick = msTicks;
	uint32_t ledTick = msTicks;


	uint32_t currButtonState = HIGH;
    uint32_t prevButtonState = HIGH;
	

	while(1){

		// Check PAUSE/RESUME 
		currButtonState = GPIO_read(PHOTO_PIN_JK);


		// // 2. process data
        // if((currButtonState == HIGH) && (prevButtonState ==LOW)){
        //     printf("button pressed!\r\n");
        //     ledTick = msTicks;
		// 	GPIO_write(LED_PIN_JK, LOW);

		// 	preserve_motor_mode   = motor_mode;
		// 	preserve_motor_dir    = motor_dir;
		// 	preserve_motor_speed  = motor_speed;

		// 	motor_mode = 0;
		// 	motor_dir = 0;
		// 	motor_speed =0;

        // }
		if ((currButtonState == LOW) && (prevButtonState ==HIGH)){
			printf("button pressed!\r\n");
            ledTick = msTicks;
			GPIO_write(LED_PIN_JK, LOW);

			preserve_motor_mode   = motor_mode;
			preserve_motor_dir    = motor_dir;
			preserve_motor_speed  = motor_speed;

			motor_mode = 0;
			motor_dir = 0;
			motor_speed =0;
			stepper_speed(0);
		}else{
			printf("button unpressed!\r\n");
			GPIO_write(LED_PIN_JK, HIGH);

			motor_mode = preserve_motor_mode;
			motor_dir = preserve_motor_dir;
			motor_speed =preserve_motor_speed;
		}




		if ((msTicks- ledTick)> 2000){
			
			GPIO_write(LED_PIN_JK, HIGH);


		}
		// Related to controlling motor speed/dir etc
		// HINT:  use  stepper_speed(vel);	only when vel>0
		stepper_step(1, motor_dir);	 // Keep step=1 or a low value to stop blocking other code
		// YOUR CODE GOES HERE
		// YOUR CODE GOES HERE
		// YOUR CODE GOES HERE


		// Print to the serial monitor every 2 sec
		// PAUSE, blink at 2 sec
		//     e.g.  printf("Mode=%lu Speed=%lu DIR=%lu\r\n", ... );
		// HINT:  msTicks in ecSysTick2.h  is increasing at a 1 msec rate accurately.
		if(msTicks - currTick > 2000){
			printf("MODE=%lu SPEED=%lu DIR=%lu\r\n", motor_mode, motor_speed, motor_dir);

			//update
			currTick = msTicks;
		}

		// 3) Write data
        FND_select(selectFND1);
        FND_display(numDisplay1);





		// // check if the pushbutton is pressed. Turn LED on/off accordingly:
		// photo_state = GPIO_read(PHOTO_PIN_JK);
		// if(photo_state)	{
		// 	printf("photosensor pressed \r\n");
		// 	GPIO_write(LED_PIN_JK, LOW);
		// }
		// else GPIO_write(LED_PIN, HIGH);


		
			
		delay_ms(1);  // use this delay
	}
}


//==================================================================
//  EXTI Interrupt Service Routines
//==================================================================

// sw2
void EXTI4_IRQHandler(void) {

	if (is_pending_EXTI(SW2_PIN_JK)) {   
		printf("sw2 pressed \r\n");

        motor_mode = TEST_fsm[motor_mode].next[1];
		numDisplay1 = TEST_fsm[motor_mode].out[1];
		motor_speed = TEST_fsm[motor_mode].out[0];

		stepper_speed(motor_speed);

        clear_pending_EXTI(SW2_PIN_JK);
	}
}

//sw1
void EXTI2_IRQHandler(void) {

    if (is_pending_EXTI(SW1_PIN_JK)) {   
		printf("sw1 pressed \r\n");

        motor_mode = TEST_fsm[motor_mode].next[0];
		numDisplay1 = TEST_fsm[motor_mode].out[1];
		motor_speed = TEST_fsm[motor_mode].out[0];

		stepper_speed(motor_speed);
        
		clear_pending_EXTI(SW1_PIN_JK);
	}

}



//==================================================================
//  UART2 Interrupt Service Routines
//==================================================================
void USART2_IRQHandler(void){
	if(is_USART2_RXNE()){
	
		PC_data = USART2_read();
		USART2_write(&PC_data,1);

		// HINT: 	comparing a character:  
		//  		uint8_t ch = USART2_read();		
		//			if(ch == 'C' || ch == 'c')	 etc..

		if (PC_data == 'C'){
            motor_dir = STEPPER_DIR_CW;

			selectFND1 = 0;

            printf("Motor clock wise\r\n");
        }else if (PC_data == 'B'){
            motor_dir = STEPPER_DIR_CCW;

			selectFND1 = 1;

            printf("Motor Counter clock\r\n");
		}

	}
}
