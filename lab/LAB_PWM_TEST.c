#include "ecSTM32F4v2.h"

#define PHOTO_PIN		PB_4


void setup(){

    MCU_init();

	// 3. photo sensor
    // initialize the pushbutton pin as an input:
	GPIO_init(PHOTO_PIN, INPUT);  
	GPIO_pupd(PHOTO_PIN, PULL_UP);
    
}

int main(void){
    uint32_t photo_pressed = 0;

	setup();
	printf("EC_TEST1_22200576 Initialized!\r\n");

    

    while(1){
        photo_pressed = GPIO_read(PHOTO_PIN);
        printf("photo_pressed = %lu\r\n", photo_pressed);

        delay_ms(1000);
    }


}
