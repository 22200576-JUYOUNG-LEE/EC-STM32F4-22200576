#include "ecSTM32F4v2.h"

#define MAX_BUF 10
#define END_CHAR 13

static volatile uint8_t buffer[MAX_BUF]={0, };
static volatile uint8_t PC_string[MAX_BUF]={0, };
static volatile uint8_t PC_data = 0;

static volatile int idx = 0;
static volatile int bReceive =0;

void setup(){
    MCU_init();
    printf("prac_Stepper_3_UART1 Initialized\r\n");


}

int main(){
    setup();

    while(1){

        // Once enter 
        if(bReceive == 1){
            // same string -> 0
            if(strcmp(PC_string, "HIGH") == 0){
                printf("LED on!\r\n");
                GPIO_write(LED_PIN,HIGH);

            }else if(strcmp(PC_string, "LOW") == 0){
                printf("LED off!\r\n");
                GPIO_write(LED_PIN,LOW);
            }

            bReceive = 0;
        }
    }

}

// Added: receive a string from the other MCU through PA9/PA10.
void USART1_IRQHandler(){
    if(is_USART1_RXNE()){
        PC_data = USART1_read();
        USART2_write(&PC_data,1); // Echo received character to local PC

        if(PC_data != END_CHAR && (idx < MAX_BUF)){
            buffer[idx] = PC_data;
            idx++;
        }
        else if (PC_data == END_CHAR){
            bReceive = 1;
            memset(PC_string, 0, sizeof(char) * MAX_BUF);
            memcpy(PC_string, buffer, sizeof(char) * idx);
            memset(buffer, 0, sizeof(char) * MAX_BUF);
            idx = 0;
            printf("USART1 INPUT data: %s\r\n",PC_string);
        }
        else{
            idx = 0;
            memset(PC_string, 0, sizeof(char) * MAX_BUF);
            memset(buffer, 0, sizeof(char) * MAX_BUF);
            printf("ERROR : Too long string\r\n");
        }
    }
}