/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Juyoung Lee
Created          : 09-11-2026
Modified         : 09-12-2026
Language/ver     : C in VS Code

Description      : Control PB_12, PB_13, PB_14, PB_15 LEDs sequentially using PA_4 button input
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"

#define NUM_LED         (unsigned int)(    4) 
#define BUTTON_PIN      (PinName_t)(    PA_4)

const PinName_t ledPins[NUM_LED] = {PB_12, PB_13, PB_14, PB_15};

void State(int numState);

void setup(void) {
    RCC_HSI_init();
    
    // 1. Initialize SW2 Button (PA_4) as Input with Pull-Up
    GPIO_init           (BUTTON_PIN, INPUT);  
    GPIO_pupd           (BUTTON_PIN, PULL_UP);

    // 2. Initialize LEDs (PB_12 ~ PB_15) as Output (Open-Drain, Pull-Up, Medium Speed)
    for(int i = 0; i < NUM_LED; i++) {
        GPIO_init       (ledPins[i], OUTPUT);
        GPIO_pupd       (ledPins[i], PULL_UP);          // Pull-Up
        GPIO_otype      (ledPins[i], PUSH_PULL);       // Open-Drain
        GPIO_ospeed     (ledPins[i], SPEED_MEDIUM);           // Medium Speed
    }

    State(0);
}

int main(void) { 
    setup();
    
    int currentButtonState = HIGH;
    int prevButtonState    = HIGH;
    int numState           = 0;

    while(1) {
        currentButtonState = GPIO_read(BUTTON_PIN);

        // Detect Falling Edge (Button Pressed: HIGH -> LOW)
        if (prevButtonState == HIGH && currentButtonState == LOW) {
            delay_ms_HSI(30); // Software Debouncing delay

            if (GPIO_read(BUTTON_PIN) == LOW) {
                // Increment LED index sequentially (0 -> 1 -> 2 -> 3 -> 0)
                numState = (numState + 1) % NUM_LED;
                State(numState);
            }
        }
        prevButtonState = currentButtonState;
    }
}

// Function to control LED states (Turn ON numState LED, Turn OFF all other LEDs)
void State(int numState) {
    for (int i = 0; i < NUM_LED; i++) {
        if (i == numState)     GPIO_write(ledPins[i], HIGH); 
        else                    GPIO_write(ledPins[i], LOW);
        
    }
}