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
    printf("prac_UART2_string Initialized\r\n");
}


int main (){
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

void USART2_IRQHandler(){       // USART2 RX Interrupt
    if(is_USART2_RXNE()){
        PC_data = USART2_read();     // RX from UART2 (PC)
        USART2_write(&PC_data,1);    // TX to USART2 (PC) Echo of keyboard typing
        USART1_write(&PC_data,1);    // TX to USART2 (PC) Echo of keyboard typing
        

        // Creates a String from received serial character
        if(PC_data != END_CHAR && (idx < MAX_BUF)){
            buffer[idx] = PC_data;
            idx++;
        }
        else if (PC_data== END_CHAR) {
            bReceive = 1;
            // reset PC_string
            memset(PC_string, 0, sizeof(char) * MAX_BUF);
            // copy to PC_string
            memcpy(PC_string, buffer, sizeof(char) * idx);
            // reset buffer
            memset(buffer, 0, sizeof(char) * MAX_BUF);
            idx = 0;

            printf("INPUT string: %s\r\n",PC_string);
        }
        else{       // if(idx >= MAX_BUF)
            idx = 0;
            // reset PC_string
            memset(PC_string, 0, sizeof(char) * MAX_BUF);
            // reset buffer
            memset(buffer, 0, sizeof(char) * MAX_BUF);
            printf("ERROR : Too long string\r\n");
        }
    }
}