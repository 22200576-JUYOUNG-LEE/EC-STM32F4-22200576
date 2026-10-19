// #include "ecSysTick2.h"
#include "ecSysTick2.h"


volatile uint32_t msTicks;

void SysTick_init(void){
    // 1. Disable
    SysTick->CTRL = 0;
    // 2. CLK satting
    SysTick->CTRL |= (1UL << 2UL);
    // 3. Reload Value
    SysTick->LOAD = (uint32_t)(1e-3)*(EC_SYSCLK) - 1;
    // 4. Clear Val
    SysTick->VAL = 0;
    // 5. interrup
    SysTick->CTRL |= (1UL << 1UL);
    // 6. Sytick EN
    SysTick->CTRL |= 1UL;
    // 7. NVIC setting 
    NVIC_SetPriority(SysTick_IRQn, 15);
    NVIC_EnableIRQ(SysTick_IRQn); 
}


// SysTick_Handler() is called every 1 ms by the SysTick IRQ
void SysTick_Handler(void){
	SysTick_counter();	
}

// Increment msTicks every 1 ms
void SysTick_counter(void){
	msTicks++;
}	

// Blocking delay in milliseconds
void delay_ms (uint32_t msec){
  	uint32_t curTicks;
  	curTicks = msTicks;
	while ((msTicks - curTicks) < msec){;}		// unsigned math: safe across wrap-around
}


// Reset the SysTick counter to 0
void SysTick_reset(void)
{
	SysTick->VAL = 0;
}

// Return the current value of the SysTick counter
uint32_t SysTick_val(void) {
	return SysTick->VAL;
}



