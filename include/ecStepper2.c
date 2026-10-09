#include "ecStepper2.h"

//State number 
#define S0 0
#define S1 1
#define S2 2
#define S3 3
#define S4 4
#define S5 5
#define S6 6
#define S7 7

#define REV_PER_STEP 64.0

#define MS_PER_MIN 60000.0

// SysTick variable
extern volatile uint32_t msTicks;
volatile uint32_t curTicks = 0;

// Stepper Motor function
uint32_t direction = 1; 
uint32_t step_period = 100; 
uint32_t step_per_rev = 64*32;
	 

// Stepper Motor variable
volatile Stepper_t myStepper; 


//FULL stepping sequence  - FSM
typedef struct {
  	uint32_t next[2];
	uint8_t out[4];

} State_full_t;

State_full_t FSM_full[4] = {  	// 1010 , 0110 , 0101 , 1001
 	{{S3,S1},{1,1,0,0}},		// A B A'B'
 	{{S0,S2},{0,1,1,0}},		// A B A'B'
 	{{S1,S3},{0,0,1,1}},		// A B A'B'
 	{{S2,S0},{1,0,0,1}},		// A B A'B'
 	
};

//HALF stepping sequence
typedef struct {
	uint32_t next[2];
	uint8_t out[4];
} State_half_t;

State_half_t FSM_half[8] = {	// 1000 , 1010 , 0010 , 0110 , 0100 , 0101, 0001, 1001
 	{{S7,S1},{1,0,0,0}},	
 	{{S0,S2},{1,0,1,0}},	
 	{{S1,S3},{0,0,1,0}},	
 	{{S2,S4},{0,1,1,0}},	
 	{{S3,S5},{0,1,0,0}},	
 	{{S4,S6},{0,1,0,1}},	
 	{{S5,S7},{0,0,0,1}},	
 	{{S6,S0},{1,0,0,1}},	

};



void Stepper_init(PinName_t pinName1, PinName_t pinName2, PinName_t pinName3, PinName_t pinName4){
	 
	//  GPIO Digital Out Initiation
	myStepper.pin1 = pinName1;
	myStepper.pin2 = pinName2;
	myStepper.pin3 = pinName3;
	myStepper.pin4 = pinName4;
	
	//  GPIO Digital Out Initiation
	// No pull-up Pull-down , Push-Pull, Fast	

	// pin1
	GPIO_init(myStepper.pin1,OUTPUT);
	GPIO_pupd(myStepper.pin1,NO_PUPD);
	GPIO_otype(myStepper.pin1,PUSH_PULL);
	GPIO_ospeed(myStepper.pin1,SPEED_FAST);
	
	// pin2
	GPIO_init(myStepper.pin2,OUTPUT);
	GPIO_pupd(myStepper.pin2,NO_PUPD);
	GPIO_otype(myStepper.pin2,PUSH_PULL);
	GPIO_ospeed(myStepper.pin2,SPEED_FAST);
	
	// pin3
	GPIO_init(myStepper.pin3,OUTPUT);
	GPIO_pupd(myStepper.pin3,NO_PUPD);
	GPIO_otype(myStepper.pin3,PUSH_PULL);
	GPIO_ospeed(myStepper.pin3,SPEED_FAST);
	
	// pin4
	GPIO_init(myStepper.pin4,OUTPUT);
	GPIO_pupd(myStepper.pin4,NO_PUPD);
	GPIO_otype(myStepper.pin4,PUSH_PULL);
	GPIO_ospeed(myStepper.pin4,SPEED_FAST);
	
}


void Stepper_pinOut (uint32_t state, uint32_t mode){	
   	if (mode == FULL){         // FULL mode
		GPIO_write(myStepper.pin1, FSM_full[state].out[0]);
		GPIO_write(myStepper.pin2, FSM_full[state].out[1]);
		GPIO_write(myStepper.pin3, FSM_full[state].out[2]);
		GPIO_write(myStepper.pin4, FSM_full[state].out[3]);

	}	 
 	else if (mode == HALF){    // HALF mode
		GPIO_write(myStepper.pin1, FSM_half[state].out[0]);
		GPIO_write(myStepper.pin2, FSM_half[state].out[1]);
		GPIO_write(myStepper.pin3, FSM_half[state].out[2]);
		GPIO_write(myStepper.pin4, FSM_half[state].out[3]);
	}
}


void Stepper_setSpeed (long speed_rpm){      // rpm [rev/min]
	step_period = (MS_PER_MIN)/(speed_rpm * REV_PER_STEP)	;// Convert rpm to  [msec/step] delay
}


void Stepper_step(uint32_t steps, uint32_t direction, uint32_t mode){

	// Set initial time
	curTicks = msTicks;

	// Set initial state
	 uint32_t state = 0;
	 myStepper._step_num = steps;

	 for(; myStepper._step_num > 0; myStepper._step_num--){ // run for step size
		if (mode == FULL) 		 												
			state += direction;					// state = next state


		// else if (mode == HALF) 
		// 	state = direction// YOUR CODE       // state = next state		
		Stepper_pinOut(state, mode);
   	}
}
	

void Stepper_stop (void){ 
	myStepper._step_num = 0;    

	// All pins(A,AN,B,BN) set as DigitalOut '0'
	GPIO_write(myStepper.pin1, 0);
	GPIO_write(myStepper.pin2, 0);
	GPIO_write(myStepper.pin3, 0);
	GPIO_write(myStepper.pin4, 0);
	
	


}

