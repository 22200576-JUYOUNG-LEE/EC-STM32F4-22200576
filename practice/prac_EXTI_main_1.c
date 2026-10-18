#include "ecGPIO2.h"
#include "ecRCC2.h"
#include "ecSysTick2.h"
#include "prac_EXTI.h"


extern volatile uint32_t msTicks;

#define SW2_PIN_JK              PA_4
#define SW1_PIN_JK              PD_2

static PinName_t pinFND[8]     = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
static PinName_t selectFND[4]  = {PA_10, PA_11, PC_4, PC_3};

uint32_t state = 0;
uint32_t pause = 0;
uint32_t curTick = 0;


// 1. setup
void setup (void){
    // 1. clock + systick
    RCC_PLL_init();
    SysTick_init();
    curTick = msTicks;

    // 2. GPIO init two buttons
    GPIO_init_1(SW1_PIN_JK, INPUT);
    GPIO_pupd_1(SW1_PIN_JK, PULL_UP);
    GPIO_init_1(SW2_PIN_JK, INPUT);
    GPIO_pupd_1(SW2_PIN_JK, PULL_UP);

    // 3. FND init + FND select init
    FND_display_init_1(pinFND);
    FND_select_init_1(selectFND);
    
    // 4. EXTI init
    EXTI_init_1(SW1_PIN_JK, FALL, 10);
    EXTI_init_1(SW2_PIN_JK, FALL, 10);


}

int main(void){
    // 1. setup
    setup();

    // 2. declare state, curTick
    uint32_t numDisplay1 = 0;
    uint32_t numDisplay2 = 0;
    uint32_t selectFND1 = 0;
    uint32_t selectFND2 = 1;
    
    while(1){

        // 3.1 write FND
        // 3.2.2 Display number update
        numDisplay1 = state % 10;
        numDisplay2 = state / 10;

        FND_select_1(selectFND1);
        FND_display_1(numDisplay1);
        delay_ms(5);
        FND_select_1(selectFND2);
        FND_display_1(numDisplay2);
        delay_ms(5);

        // 3.2 update 
        if((msTicks - curTick > 500) && (pause == 0)){

            // 3.2.1 state update
            state = (state + 1) % 20;

            // 3.2.3 Tick reset 
            curTick = msTicks;

        }
    }
}

void EXTI4_IRQHandler(void){

    if(is_pending_EXTI_1(SW2_PIN_JK)){

        pause ^= 1; 
        curTick = msTicks;

        clear_pending_EXTI_1(SW2_PIN_JK);

    }
}

void EXTI2_IRQHandler(void){

    if(is_pending_EXTI_1(SW1_PIN_JK)){

        state = 0;
        curTick = msTicks; 

        clear_pending_EXTI_1(SW1_PIN_JK);

    }
}