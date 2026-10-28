#include "ecGPIO2.h"

void GPIO_init_1(PinName_t pinName, uint32_t mode){
    GPIO_TypeDef *Port;
    unsigned int pin;
    ecPinmap(pinName, &Port, &pin);

    uint32_t GPIO_port = 0;
    if      (Port == GPIOA)     GPIO_port = 0;
    else if (Port == GPIOB)     GPIO_port = 1;
    else if (Port == GPIOC)     GPIO_port = 2;
    else if (Port == GPIOD)     GPIO_port = 3;
    else if (Port == GPIOE)     GPIO_port = 4;
    else if (Port == GPIOH)     GPIO_port = 7;

    RCC->AHB1ENR |= (1UL << GPIO_port);

    GPIO_mode_1(pinName, mode);
}

void GPIO_mode_1(PinName_t pinName, uint32_t mode){
    GPIO_TypeDef *Port;
    unsigned int pin;
    ecPinmap(pinName, &Port, &pin);

    Port->MODER &=  ~(3UL <<(2*pin));     
    Port->MODER |=   mode <<(2*pin);    
}

void GPIO_ospeed_1(PinName_t pinName, uint32_t speed){
    GPIO_TypeDef *Port;
    unsigned int pin;
    ecPinmap(pinName, &Port, &pin);

    Port->OSPEEDR &=  ~(3UL <<(2*pin));
    Port->OSPEEDR |=   speed <<(2*pin); 
}

void GPIO_otype_1(PinName_t pinName, uint32_t type){
    GPIO_TypeDef *Port;
    unsigned int pin;
    ecPinmap(pinName, &Port, &pin);

    Port->OTYPER &=  ~(1UL << pin);
    Port->OTYPER |=  type << pin;
}

void GPIO_pupd_1(PinName_t pinName, uint32_t pupd){
    GPIO_TypeDef *Port;
    unsigned int pin;
    ecPinmap(pinName, &Port, &pin);

    Port->PUPDR &= ~(3UL << (pin * 2));
    Port->PUPDR |=  (pupd <<(pin * 2));
}

void GPIO_write_1(PinName_t pinName, uint32_t Output){
    GPIO_TypeDef *Port;
    unsigned int pin;
    ecPinmap(pinName, &Port, &pin);

    Port->ODR &= ~(1UL << pin);
    Port->ODR |= (Output << pin);
}

uint32_t GPIO_read_1(PinName_t pinName){
    GPIO_TypeDef *Port;
    unsigned int pin;
    ecPinmap(pinName, &Port, &pin);

    return (Port->IDR >> pin) & 1UL;
}

// FND - JKIT board
static PinName_t _fndPins_1[8];
static PinName_t _fndSel_1[4];

//Each led that has to light up gets a 1, every other led gets a 0
//its in order  { (a,b,c,d,e,f,g,dp) }
int numberFND_1[12][8]={
                    {1,1,1,1,1,1,0,0},          //zero
                    {0,1,1,0,0,0,0,0},          //one
                    {1,1,0,1,1,0,1,0},          //two
					{1,1,1,1,0,0,1,0},			//three
					{0,1,1,0,0,1,1,0},			//four
					{1,0,1,1,0,1,1,0},			//five
					{1,0,1,1,1,1,1,0},			//six
					{1,1,1,0,0,1,0,0},			//seven
					{1,1,1,1,1,1,1,0},			//eight
					{1,1,1,1,0,1,1,0},			//nine
                    {0,0,0,0,0,0,0,1},          //dot
                   	{0,0,0,0,0,0,0,0}		    //blank
                  };
                  

// Initialize DOUT pins for 7 segment leds
void FND_display_init_1(PinName_t *pinFND){
    for (int i=0; i<8; i++){
        _fndPins_1[i] = pinFND[i];
        GPIO_init       (_fndPins_1[i], OUTPUT);
        GPIO_pupd       (_fndPins_1[i], NO_PUPD);      
        GPIO_otype      (_fndPins_1[i], PUSH_PULL);      
        GPIO_ospeed     (_fndPins_1[i], SPEED_MEDIUM);     
    }
}

// Initialize select pins (FND0~FND3) as DOUT
void FND_select_init_1(PinName_t *selectFND){
    for (int i=0; i<4; i++){
        _fndSel_1[i] = selectFND[i];
        GPIO_init       (_fndSel_1[i], OUTPUT);
        GPIO_pupd       (_fndSel_1[i], NO_PUPD);      
        GPIO_otype      (_fndSel_1[i], PUSH_PULL);      
        GPIO_ospeed     (_fndSel_1[i], SPEED_MEDIUM);     
    }
}

// Display a number 0 - 9 only
void FND_display_1(uint8_t num){
    for (int i=0; i<8; i++){
        GPIO_write(_fndPins_1[i], numberFND[num][i]); 
    }       
}

// Select display: FND0 to FND3
void FND_select_1(uint8_t digit){
    for(int i = 0; i < 4; i++){
        if(i == digit) GPIO_write(_fndSel_1[i], HIGH);
        else GPIO_write(_fndSel_1[i], LOW);
    }   
}