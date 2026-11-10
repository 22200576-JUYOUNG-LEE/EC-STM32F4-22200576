/*----------------------------------------------------------------
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Juyoung Lee
Created          : 10-13-2026
Modified         : 11-02-2026
Language/ver     : C in VS Code (PlatformIO)

Description      : LAB - PWM RC Servo Motor (SG90)
                   - PWM   : PA_1 (TIM2_CH2), period 20 ms (50 Hz),
                             pulse 0.5 ~ 2.5 ms  (0 ~ 180 deg)
                   - TIM3  : Update interrupt every 500 ms
                             angle 0 -> 180 -> 0, step 10 deg
                   - EXTI  : User button (PC13) -> reset to 0 deg
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"

// ---------------- Pin & Parameter ----------------
#define PWM_MOTOR     PA_1       // TIM2_CH2 (Configuration table: PA_1 = TIM2_CH2)

#define PWM_PERIOD    20          // [ms]  50 Hz
#define PULSE_MIN_US  500         // [us]  0 deg
#define PULSE_MAX_US  2500        // [us]  180 deg
#define ANGLE_STEP    10          // [deg]
#define STEP_TIME     500         // [ms]  TIM3 update period

// ---------------- Global (shared with ISR) ----------------
volatile int angle = 0;           // current angle [deg]
volatile int dir   = 1;           // +1: increasing, -1: decreasing

// angle [deg] -> pulse width [us]   (integer math, no truncation issue)
uint32_t angle2pulse_us(int deg) {
	return PULSE_MIN_US + (uint32_t)((PULSE_MAX_US - PULSE_MIN_US) * deg / 180);
}

void setup(void) {
	MCU_init();                                    // PLL 84 MHz, SysTick

	// 1. Servo PWM : TIM2 only for PWM (do NOT use TIM2 for UI)
	PWM_init(PWM_MOTOR, PWM_PERIOD);
	PWM_pulsewidth_us(PWM_MOTOR, angle2pulse_us(angle));  // start at 0 deg

	// 2. Angle update : TIM3 update interrupt every 500 ms
	TIM_UI_init(TIM3, STEP_TIME);
	NVIC_SetPriority(TIM3_IRQn, 2);                // lower than button
}

int main(void) {
	setup();

	while (1) {
		// All work is done in interrupts (TIM3, EXTI)
	}
}

// ---------------- TIM3 : every 500 ms, step angle ----------------
void TIM3_IRQHandler(void) {
	if (is_UIF(TIM3)) {
		angle += dir * ANGLE_STEP;

		if (angle >= 180)     { angle = 180; dir = -1; }   // turn back at 180
		else if (angle <= 0)  { angle = 0;   dir = +1; }   // turn back at 0

		PWM_pulsewidth_us(PWM_MOTOR, angle2pulse_us(angle));

		clear_UIF(TIM3);
	}
}

// ---------------- EXTI : button -> reset to 0 deg ----------------
void EXTI15_10_IRQHandler(void) {                  // PC13 -> EXTI line 13
	if (is_pending_EXTI(BUTTON_PIN)) {
		angle = 0;
		dir   = +1;
		PWM_pulsewidth_us(PWM_MOTOR, angle2pulse_us(angle));

		TIM3->CNT = 0;                             // restart 500 ms count from now

		clear_pending_EXTI(BUTTON_PIN);
	}
}