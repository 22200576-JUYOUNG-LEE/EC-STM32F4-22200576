/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ Juyoung Lee ]
Created          : 05-03-2021
Modified         : 09-11-2026
Language/ver     : C++ in VS Code

Description      : GPIO Library. Distributed to Students for LAB_GPIO
/----------------------------------------------------------------*/
#include "ecGPIO2.h"

void GPIO_init(PinName_t pinName, uint32_t mode){     
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);
	
	// mode  : Input(0), Output(1), AlterFunc(2), Analog(3)   
	if      (Port == GPIOA)		RCC_GPIOA_enable();
	else if (Port == GPIOB)     RCC_GPIOB_enable();
	else if (Port == GPIOC)     RCC_GPIOC_enable();
	else if (Port == GPIOD)     RCC_GPIOD_enable();
	else if (Port == GPIOE)     RCC_GPIOE_enable();
	else if (Port == GPIOH)     RCC_GPIOH_enable();

	GPIO_mode(pinName, mode);
}


// GPIO Mode : Input(00), Output(01), AlterFunc(10), Analog(11)
void GPIO_mode(PinName_t pinName, uint32_t mode){
   GPIO_TypeDef * Port;
   unsigned int pin;
   ecPinmap(pinName,&Port,&pin);

   Port->MODER &= 	~(3UL <<(2*pin));     
   Port->MODER |= 	 mode <<(2*pin);    
}


// GPIO Speed : Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
void GPIO_ospeed(PinName_t pinName, uint32_t speed){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

	Port->OSPEEDR &=  ~(3UL <<(2*pin));
	Port->OSPEEDR |=   speed <<(2*pin);
}

// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
void GPIO_otype(PinName_t pinName, uint32_t type){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

	Port->OTYPER &=  ~(1UL << pin);
	Port->OTYPER |=  type << pin;
}

// GPIO Push-Pull    : No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
void GPIO_pupd(PinName_t pinName, uint32_t pupd){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

	Port->PUPDR &= ~(3UL << (pin * 2));
	Port->PUPDR |= 	(pupd <<(pin * 2));
}

// Write digital output state
void GPIO_write(PinName_t pinName, uint32_t Output){
	GPIO_TypeDef *Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);

	if (Output == 0) Port->ODR &= ~(1UL << pin);
	else			 Port->ODR |=  (1UL << pin);
}


uint32_t GPIO_read(PinName_t pinName){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

	return (Port->IDR >> pin) & 1UL;
}

// making rapper using API
void pinMode(PinName_t pin, uint32_t mode){
	GPIO_mode(pin,  mode);
}

void digitalWrite(PinName_t pin, uint32_t value){
	GPIO_write(pin, value);
}

uint32_t  digitalRead(PinName_t pin){
	return GPIO_read(pin);
}

// FND - JKIT board
static PinName_t _fndPins[8];
static PinName_t _fndSel[4];

//Each led that has to light up gets a 1, every other led gets a 0
//its in order  { (a,b,c,d,e,f,g,dp) }
int numberFND[12][8]={
                    {1,1,1,1,1,1,0,0},          //zero
                    {0,1,1,0,0,0,0,0},          //one
                    {1,1,0,1,1,0,1,0},          //two
					{1,1,1,1,0,0,1,0},			//three
					{0,1,1,0,0,1,1,0},			//four
					{1,0,1,1,0,1,1,0},
					{1,0,1,1,1,1,1,0},
					{1,1,1,0,0,1,0,0},
					{1,1,1,1,1,1,1,0},
					{1,1,1,1,0,1,1,0},	
                    {0,0,0,0,0,0,0,1},          //dot
                   	{0,0,0,0,0,0,0,0}		      //blank
                  };
                  

// Initialize DOUT pins for 7 segment leds
void FND_init(PinName_t *pinFND){
	for (int i=0; i<8; i++){
		_fndPins[i] = pinFND[i];
		GPIO_init		(_fndPins[i], OUTPUT);
		GPIO_pupd       (_fndPins[i], 0);      
        GPIO_otype      (_fndPins[i], 0);      
        GPIO_ospeed     (_fndPins[i], 1);     
	}
}

// Initialize select pins (FND0~FND3) as DOUT
void FND_select_init(PinName_t *selectFND){	
    for (int i=0; i<8; i++){
		_fndSel[i] = selectFND[i];
		GPIO_init		(_fndSel[i], OUTPUT);
		GPIO_pupd       (_fndSel[i], 0);      
        GPIO_otype      (_fndSel[i], 0);      
        GPIO_ospeed     (_fndSel[i], 1);     
	}
}

// Display a number 0 - 9 only
void FND_display(uint8_t num){
    for (int i=0; i<8; i++){
		GPIO_write(_fndPins[i], ~(numberFND[num][i])); 
	}       
}

// Select display: FND0 to FND3
void FND_select(uint8_t digit){
	for(int i = 0; i < 4; i++){
		if(i == digit) GPIO_write(_fndSel[i], HIGH);
		else GPIO_write(_fndSel[i], LOW);
		
	}
	
}