# PostLAB: TIM PWM

**Date:** 2026-11-05

**Author/Partner:** Soli deo Gloria

## Theme: GPIO output LED

### TU_TIM_Interrupt (`TU_TIM_Interrupt.c`)

```c
#define LD2_PIN	PA_5
#define CNT_TIME 2000
uint32_t _count = 0;

// TIM2 Update Interrupt Handler
void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){			// Check UIF(update interrupt flag)
        _count++;
		if (_count > CNT_TIME) {
		    led_toggle(LD2_PIN);	// Toggle every 1000 msec
			_count = 0;
		}
        clear_UIF(TIM2); 		// Clear TIM_UI flag
	}
}
```

#### Result

![image-20261105114356286](C:\Users\GILBERT\AppData\Roaming\Typora\typora-user-images\image-20261105114356286.png)

In AC coupling mode,  the scope puts a capacitor in series with the input. The capacitor blocks any steady (DC) voltage and only passes changes. Together with the scope's input resistance it forms a high-pass fileter 

![image-20261105120113834](C:\Users\GILBERT\AppData\Roaming\Typora\typora-user-images\image-20261105120113834.png)

