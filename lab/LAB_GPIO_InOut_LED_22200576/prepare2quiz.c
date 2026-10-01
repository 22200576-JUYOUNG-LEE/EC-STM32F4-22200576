#include "ecRCC2.h"
#include "ecGPIO2.h"

#define LED_PIN PB_12
#define BUTTON_PIN PA_0


void setup(void){
    RCC_HSI_init();

    GPIO_init(BUTTON_PIN,INPUT);
    GPIO_pupd(BUTTON_PIN, PULL_UP);
    
    GPIO_init(LED_PIN, OUTPUT);
    GPIO_pupd(LED_PIN, PULL_UP);
    GPIO_otype(LED_PIN, PUSH_PULL);
    GPIO_ospeed(LED_PIN, SPEED_FAST);

}

void main(void){
    setup();

    int buttonState = 0;


    while(1){
        buttonState = GPIO_read(BUTTON_PIN);

        if (buttonState == 0){
            GPIO_write(LED_PIN, HIGH);
        }else {
            GPIO_write(LED_PIN, LOW);
        }

        delay_ms_HSI(100);
    }
}

