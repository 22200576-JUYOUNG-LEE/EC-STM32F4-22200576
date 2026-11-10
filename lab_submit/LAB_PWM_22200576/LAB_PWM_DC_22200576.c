/*----------------------------------------------------------------
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Juyoung Lee
Created          : 10-13-2026
Modified         : 11-02-2026
Language/ver     : C in VS Code (PlatformIO)

Description      : LAB - PWM DC Motor
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"

// ---------------- Pin & Parameter ----------------
#define PWM_MOTOR       PA_0      // TIM2_CH1 (Configuration table: PA_1 = TIM2_CH2)
#define MOTOR_DIR_PIN   PC_2
#define PHOTO_PIN		PB_4
#define STEP_TIME		1			// [ms]  1000Hz

#define PWM_PERIOD      1000        // [us]  1000Hz

// ---- State ----
#define S0 		0	// OFF
#define S1 		1	// MID
#define S2 		2 	// HIGH

// ---- LED output codes ----
#define L_OFF   0
#define L_ON    1
#define L_BLINK 2

typedef struct {
    uint32_t next[4];   // next state for input index 0~3
    uint32_t vel[4];    // duty [%]
    uint32_t led[4];    // LED mode
} State_t;

// input index = X*2 + Y  →  (0,F)=0, (0,T)=1, (1,F)=2, (1,T)=3
State_t FSM[3] = {
    //   next              vel             led
    { {S0,S0,S1,S0}, {  0,0, 50,0}, {LOW,L_BLINK,L_ON, L_BLINK} },  // S0
    { {S1,S1,S2,S1}, { 50,0,100,0}, {L_ON, L_BLINK,L_ON, L_BLINK} },  // S1
    { {S2,S2,S0,S2}, {100,0,  0,0}, {L_ON, L_BLINK,L_OFF,L_BLINK} },  // S2
};

volatile uint32_t state    = S0;
volatile uint32_t btn_flag = 0;     // X (set in EXTI)
volatile uint32_t obj_near = 0;     // Y
volatile uint32_t led_mode = L_OFF;
volatile float    duty     = 0.0f;
volatile uint32_t print_flag = 0;

// ---- FSM core ----
void nextState(void) {
    uint32_t X = btn_flag;  btn_flag = 0;     // consume button event once
    uint32_t Y = obj_near;
    uint32_t in = X * 2 + Y;

    duty     = FSM[state].vel[in] / 100.0f;   // output
    led_mode = FSM[state].led[in];
    state    = FSM[state].next[in];           // transition
}

void stateOutput(void) {
    GPIO_write(MOTOR_DIR_PIN, LOW);           // DIR LOW → duty = speed (no inversion)
    PWM_duty(PWM_MOTOR, duty);

    if (led_mode == L_OFF) GPIO_write(LED_PIN, LOW);
    if (led_mode == L_ON)  GPIO_write(LED_PIN, HIGH);
    // L_BLINK is handled in TIM3 ISR
}

void setup(){

    MCU_init();

	// 1. PWM 
	PWM_init_us(PWM_MOTOR, PWM_PERIOD);

    // 2. MOTOR DIR 
    GPIO_init(MOTOR_DIR_PIN, OUTPUT);
    GPIO_pupd(MOTOR_DIR_PIN, PULL_DOWN);
    GPIO_otype(MOTOR_DIR_PIN, PUSH_PULL);
    GPIO_ospeed(MOTOR_DIR_PIN, SPEED_LOW);

	// 3. photo sensor
    // initialize the pushbutton pin as an input:
	GPIO_init(PHOTO_PIN, INPUT);  
	GPIO_pupd(PHOTO_PIN, PULL_UP);
    
	// 4. Angle update : TIM3 update interrupt every 500 ms
	TIM_UI_init(TIM3, STEP_TIME);
	NVIC_SetPriority(TIM3_IRQn, 2);                // lower than button
}

int main(void) {
	setup();
	printf("EC_LAB_PWM_DC_22200576 Initialized!\r\n");

	while (1) {
        obj_near = GPIO_read(PHOTO_PIN);  // Y
        nextState();
        stateOutput();

        if (print_flag) {                     // print outside ISR
            printf("MODE=S%ld  duty=%.2f  %s\r\n",
                   state, duty, obj_near ? "PAUSE" : "RUN");
            print_flag = 0;
        }
        delay_ms(50);
    }
}

// ---------------- EXTI : button -> reset to 0 deg ----------------
void EXTI15_10_IRQHandler(void) {                  // PC13 -> EXTI line 13
	if (is_pending_EXTI(BUTTON_PIN)) {

        btn_flag = 1;

		clear_pending_EXTI(BUTTON_PIN);
	}
}

// ---- 1 ms timer: blink + 2 s print ----
void TIM3_IRQHandler(void) {
    static uint32_t blink_cnt = 0, print_cnt = 0;
    if (is_UIF(TIM3)) {
        if (led_mode == L_BLINK && ++blink_cnt >= 1000) {
            GPIO_write(LED_PIN, !GPIO_read(LED_PIN));
            blink_cnt = 0;
        }
        if (++print_cnt >= 2000) { print_flag = 1; print_cnt = 0; }
        clear_UIF(TIM3);
    }
}