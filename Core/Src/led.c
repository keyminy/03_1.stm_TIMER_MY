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
	for(int i=0; i<8;i++){
		led_all_off();
		HAL_GPIO_WritePin(GPIOB, 0x01 << i, 1);
		my_delay_ms(200);
	}
}

// 7->6->5...->0
void led_on_down(){
	for(int i=0; i<8;i++){
		led_all_off();
		HAL_GPIO_WritePin(GPIOB, 0x80 >> i, 1);
		my_delay_ms(200);
	}
}

// 0->1->....->7 기존 on된 것 끄지말고 유지하기
void led_keppon_up(){
	uint16_t pattern = 0x01;
	led_all_off();
	my_delay_ms(100);
	for(int i=0; i<8;i++){
		HAL_GPIO_WritePin(GPIOB, pattern, 1);
		my_delay_ms(200);
		pattern = pattern << 1 | 0x01;
	}
}
// 7->6->5->....->0 기존 off된 것 끄지말고 유지하기
void led_keepon_down(){
	uint16_t pattern = 0x80;
	led_all_on();
	my_delay_ms(100);
	for(int i=0; i<8;i++){
		// GPIO_PIN_SET : if it's set to 0, the pin is not affected.
		// 	HAL_GPIO_WritePin(GPIOB, pattern, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, pattern>>i, GPIO_PIN_RESET);
		my_delay_ms(200);
	}
}

void flower_on(){
	led_all_off();
	my_delay_ms(100);
	for(int i=0;i<4;i++){
		HAL_GPIO_WritePin(GPIOB,(0b00001000 >> i) | (0b00010000 << i), GPIO_PIN_SET);
		my_delay_ms(200);
	}
}

void flower_off(){
	led_all_on();
	my_delay_ms(100);
	for(int i=0;i<4;i++){
		HAL_GPIO_WritePin(GPIOB,(0b10000000 >> i) | (0b00000001 << i), GPIO_PIN_RESET);
		my_delay_ms(200);
	}
}
