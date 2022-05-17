#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include "clocks.h"
#include "gpio.h"
#include "systick.h"

typedef void(*pFunction)(void);

void JumpToApplication(void)
{
	GPIO_Reset();
	uint32_t AppResetHandlerAddress = *(volatile uint32_t*)(0x08000000 + 0x4000 + 4);
	pFunction AppResetHandler = (pFunction)AppResetHandlerAddress;
	AppResetHandler(); //call application's reset handler
}


int main(void)
{
	Clocks_Init();
	SysTick_Init();
	GPIO_Reset();
	GPIO_OutputInit(GPIOC,GPIO_PORT_REG_HIGH, GPIO_PIN13_OUTPUT_MODE_2MHZ, GPIO_GEN_PUR_OUTPUT_PUSH_PULL);
	
	while(1)
	{
		GPIO_OutputWrite(GPIOC, GPIO_PIN13, false);
		SysTick_DelayMs(2000);
		GPIO_OutputWrite(GPIOC, GPIO_PIN13, true);
		SysTick_DelayMs(2000);
		JumpToApplication();
	}
}
