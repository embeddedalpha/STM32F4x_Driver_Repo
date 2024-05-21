/*
 * DMA.c
 *
 *  Created on: Nov 16, 2023
 *      Author: kunal
 */


#include "DMA.h"


void DMA_Clock_Enable(DMA_Config *config)
{
	if(config->controller == DMA1) RCC -> AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	if(config->controller == DMA2) RCC -> AHB1ENR |= RCC_AHB1ENR_DMA2EN;
}
//

void DMA_Clock_Disable(DMA_Config *config)
{

	if(config->controller == DMA1) RCC -> AHB1ENR &= ~RCC_AHB1ENR_DMA1EN;
	if(config->controller == DMA2) RCC -> AHB1ENR &= ~RCC_AHB1ENR_DMA2EN;
}
//

void DMA_Reset(DMA_Config *config)
{
	if(config->controller == DMA1) RCC -> AHB1RSTR |= RCC_AHB1RSTR_DMA1RST;
	if(config->controller == DMA2) RCC -> AHB1RSTR |= RCC_AHB1RSTR_DMA2RST;
}
//

void DMA_Init(DMA_Config *config)
{
	DMA_Clock_Enable(config);
	config -> stream -> CR |= config -> channel << DMA_SxCR_CHSEL_Pos;
	config -> stream -> CR |= config -> circular_mode;
	config -> stream -> CR |= config -> flow_control;
	config -> stream -> CR |= config -> priority_level;
	config -> stream -> CR |= config -> memory_data_size;
	config -> stream -> CR |= config -> peripheral_data_size;
	config -> stream -> CR |= config -> transfer_direction;
	config -> stream -> CR |= config -> interrupts;
	config -> stream -> CR |= config -> memory_pointer_increment;
	config -> stream -> CR |= config -> peripheral_pointer_increment;
}

void DMA_Set_Target(DMA_Config *config)
{
	config -> stream -> NDTR = config -> buffer_length;
	config -> stream -> M0AR = (uint32_t)config->memory_address;
	config -> stream -> PAR = (uint32_t)config->peripheral_address;

}


void DMA_Set_Trigger(DMA_Config *config)
{
	int retvalue = 0;

	config->controller->LIFCR = 0xF7D0F7D;
	config->controller->HIFCR = 0xF7D0F7D;
	config -> stream -> CR |= DMA_SxCR_EN;




	if((config->stream == DMA1_Stream0) || (config->stream == DMA2_Stream0))
	{
		while(!(config->controller->LISR & DMA_LISR_TCIF0));
		config->controller->LIFCR |= 0x3D;
	}
	else if((config->stream == DMA1_Stream1) || (config->stream == DMA2_Stream1))
	{
		while(!(config->controller->LISR & DMA_LISR_TCIF1));
		config->controller->LIFCR |= 0x3D<<6;
	}
	else if((config->stream == DMA1_Stream2) || (config->stream == DMA2_Stream2))
	{
		while(!(config->controller->LISR & DMA_LISR_TCIF2));
		config->controller->LIFCR |= 0x3D<<16;
	}
	else if((config->stream == DMA1_Stream3) || (config->stream == DMA2_Stream3))
	{
		while(!(config->controller->LISR & DMA_LISR_TCIF3));
		config->controller->LIFCR |= 0x3D<<22;
	}
	if((config->stream == DMA1_Stream4) || (config->stream == DMA2_Stream4))
	{
		while(!(config->controller->HISR & DMA_HISR_TCIF4));
		config->controller->HIFCR |= (1 << 0);
	}
	else if((config->stream == DMA1_Stream5) || (config->stream == DMA2_Stream5))
	{
		while(!(config->controller->HISR & DMA_HISR_TCIF5));
		config->controller->LIFCR |= 0x3D<<6;
	}
	else if((config->stream == DMA1_Stream6) || (config->stream == DMA2_Stream6))
	{
		while(!(config->controller->HISR & DMA_HISR_TCIF6));
		config->controller->HIFCR |= 0x3D<<16;
	}
	else if((config->stream == DMA1_Stream7) || (config->stream == DMA2_Stream7))
	{
		while(!(config->controller->HISR & DMA_HISR_TCIF7));
		config->controller->HIFCR |= 0x3D<<22;
	}

	retvalue = 1;



	return retvalue;



}



