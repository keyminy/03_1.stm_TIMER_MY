#include "timer.h"

extern TIM_HandleTypeDef htim11;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

extern volatile int TIM3_10ms_counter;

void delay_us(unsigned int us);
void my_delay_ms(unsigned int ms);

// count us timer plus
// 0.000001sec --> 1us : 1개의 pulse소요 시간
// 0.001sec : 1ms  <== 1000us
// 0.1sec : 100ms
//
void delay_us(unsigned int us)
{
	// TCNT=0;  AVR
	__HAL_TIM_SET_COUNTER(&htim11,0);   // timer 11번 counter를 clear한다.
	while (__HAL_TIM_GET_COUNTER(&htim11) < us)   // 지정한 pulse count를 만날때 까지 wait
		;        // NOP
}

// delay 1ms
void my_delay_ms(unsigned int ms){
	if(ms < 10){
		__HAL_TIM_SET_COUNTER(&htim3,0);
		while (__HAL_TIM_GET_COUNTER(&htim3) < ms)
			;        // NOP
	}else{
		unsigned int one_ms = ms/10;
		//__HAL_TIM_SET_COUNTER(&htim3,0);
		TIM3_10ms_counter = 0;
		while (TIM3_10ms_counter < (one_ms+ms%10))
			;        // NOP
	}
}
