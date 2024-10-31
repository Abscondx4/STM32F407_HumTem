#include "iwdg.h"

void Iwdg_Init(void)
{
	//
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	// 
	IWDG_SetPrescaler(IWDG_Prescaler_128);
	
	//
	IWDG_SetReload(500-1);
	
	//
	IWDG_Enable();
}
