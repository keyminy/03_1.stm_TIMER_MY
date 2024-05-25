#include "led.h"
#include "button.h"

void led_all_on(void);
void led_all_off(void);
void led2_toggle(void);
void led_on_up();
void led_on_down();
void ledbar0_toggle(void);
void ledbar1_toggle(void);
void ledbar2_toggle(void);
void led_main(void);

extern char button0_count;
extern int TIM3_10ms_counter;

// one button처리 BUTTON0
// 1: led_all_on
// 2: led_all_off
// 3: led_on_up
// 4: led_on_down

// BUTTON1
// led_keepon_up  <--> led_keepon_down

// BUTTON2
// led_flower_on  <--> led_flower_off


extern led_state;

void led_main() {
	switch (led_state) {
	//함수포인터로 만들면 좋다고함
	case 0:
		led_all_off(); // default
		break;
	case 1:
		led_all_on();
		break;
	case 2:
		led_all_off();
		break;
	case 3:
		led_on_up();
		break;
	case 4:
		led_on_down();
		break;
	case 5:
		led_keppon_up();
		break;
	case 6:
		led_keepon_down();
		break;
	case 7:
		flower_on();
		break;
	case 8:
		flower_off();
		break;
	}
}

void led_all_on(void)
{
	HAL_GPIO_WritePin(GPIOB, 0xff, 1);
	// HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|
	//		                 GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 1);
}

void led_all_off(void)
{
	HAL_GPIO_WritePin(GPIOB, 0xff, 0);
	// HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|
	//		                 GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0);
}

void ledbar0_toggle(void)
{
	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
}

void ledbar1_toggle(void)
{
	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
}

void ledbar2_toggle(void)
{
	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
}

void led2_toggle(void)
{
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}
// 0 -> 1..... ->7
void led_on_up(){
	static uint8_t i = 0;
	if(TIM3_10ms_counter>=20){
		TIM3_10ms_counter = 0;
		if(i==0){
			HAL_GPIO_WritePin(GPIOB, 0xff, 0);
		}
		led_all_off();
		HAL_GPIO_WritePin(GPIOB, 0x01 << i++, 1);
		i%=8;
	}

//	for(int i=0; i<8;i++){
//		led_all_off();
//		HAL_GPIO_WritePin(GPIOB, 0x01 << i, 1);
//		my_delay_ms(200);
//	}
}

// 7->6->5...->0
void led_on_down(){
	static uint8_t i = 0;
	if(TIM3_10ms_counter>=20){
		TIM3_10ms_counter = 0;
		if(i==0){
			HAL_GPIO_WritePin(GPIOB, 0xff, 0);
		}
		led_all_off();
		HAL_GPIO_WritePin(GPIOB, 0x80 >> i++, 1);
		i %= 8;
	}
//	for(int i=0; i<8;i++){
//		led_all_off();
//		HAL_GPIO_WritePin(GPIOB, 0x80 >> i, 1);
//		my_delay_ms(200);
//	}
}

// 0->1->....->7 기존 on된 것 끄지말고 유지하기
void led_keppon_up(){
   uint16_t pattern = 0x01;
   static uint8_t i=0;

   if(TIM3_10ms_counter >= 20){
      TIM3_10ms_counter = 0;
      if(i == 0){
         led_all_off();
      }
      while(TIM3_10ms_counter<=10); // 100ms delay
      HAL_GPIO_WritePin(GPIOB, pattern<<i++, 1);
      i%=8;
   }
}

// 7 -> 6 -> 5 -> .... -> 0 keep other LEDs on
void led_keepon_down() {
    static uint16_t pattern = 0x80; // Initial LED pattern
    static uint8_t i = 0; // Index for current LED

    if (TIM3_10ms_counter >= 20) { // 200ms delay
        TIM3_10ms_counter = 0;
        if(i==0){
            led_all_on();
        }
        while(TIM3_10ms_counter<=10); // 100ms delay
        HAL_GPIO_WritePin(GPIOB, pattern>>i++, GPIO_PIN_RESET);
        if (i >= 8) {
            i = 0;
        }
    }
}

void flower_on() {
    static int i = 0;
    if (TIM3_10ms_counter >= 20) { // 200ms delay
        TIM3_10ms_counter = 0;
    	if(i==0){
			 led_all_off();
    	}
    	while(TIM3_10ms_counter<=10); // 100ms delay
        HAL_GPIO_WritePin(GPIOB, (0b00001000 >> i) | (0b00010000 << i), GPIO_PIN_SET);
        i++;
       if(i==4){
    	   while(TIM3_10ms_counter<=30); // 300ms delay
    	   i=0;
       }
    }
}

void flower_off() {
    static int i = 0;
    if (TIM3_10ms_counter >= 20) { // 200ms delay
        TIM3_10ms_counter = 0;
    	if(i==0){
    		led_all_on();
    	}
    	while(TIM3_10ms_counter<=10); // 100ms delay
    	 HAL_GPIO_WritePin(GPIOB, (0b10000000 >> i) | (0b00000001 << i), GPIO_PIN_RESET);
        i++;
        if(i==4){
     	   while(TIM3_10ms_counter<=30); // 300ms delay
     	   i=0;
        }
    }
}
