/*
 * fsm.c
 *
 *  Created on: Nov 15, 2023
 *      Author: Hoai Thanh
 */

#include "fsm.h"

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;

uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

uint8_t cmd_flag = 0, cmd_data[30];

int status = START;
char str[50];

void clear_buffer(){
	memset(buffer, 0, sizeof(buffer));
	index_buffer = 0;
}

void command_parser_fsm(){
	switch(status){
	case START:
		if( temp == '!'){
			clear_buffer();
			status = RECEIVING;
		}
		clear_buffer();
		break;
	case RECEIVING:
		if(index_buffer == 0){
			status = START;
		}
		else if(temp == '#'){
			memcpy(cmd_data, buffer, sizeof(cmd_data));
			cmd_flag = 1;
			status = START;
			HAL_UART_Transmit(&huart2, (void*)str, sprintf(str, "\r\n"), 50);
		}
		else if(temp == '!'){
			clear_buffer();
		}
		break;
	}
}

uint32_t ADC_Value = 0;
uint8_t uart_communication_state = START, cmd_ok[3] = {'O','K','#'}, cmd_rst[4] = {'R','S','T','#'}, compare[30];
char str[50];
void uart_communication_fsm(){
	switch(uart_communication_state){
		case START:
			if(cmd_flag == 1){
				cmd_flag = 0;

//				Compare !RST#
				memset(compare, 0, sizeof(compare));
				memcpy(compare, cmd_rst, sizeof(cmd_rst));

				if(memcmp(cmd_data, compare, sizeof(cmd_data)) == 0){
					setTimer2(11);
					HAL_ADC_Start(&hadc1);
					ADC_Value = HAL_ADC_GetValue(&hadc1);
					HAL_ADC_Stop(&hadc1);
					uart_communication_state = SENDING;
				}
				else{
					HAL_UART_Transmit(&huart2, (void*) str, sprintf(str, "\r\nIt's not !RST#\r\n"), 300);
				}

			}
			break;
		case SENDING:
			if(cmd_flag == 1){
				cmd_flag = 0;

				//Compare to !OK#
				memset(compare, 0, sizeof(compare));
				memcpy(compare, cmd_ok, sizeof(cmd_rst));

				if(memcmp(cmd_data, compare, sizeof(cmd_data)) == 0){
					uart_communication_state = START;
					break;
				}
				else{
					HAL_UART_Transmit(&huart2, (void*) str, sprintf(str, "It's not !OK#\r\n"), 300);
				}
			}

			if(timer2_flag == 1){
				HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
				setTimer2(300);
				HAL_UART_Transmit(&huart2, (void*) str, sprintf(str, "\r\n!ADC=%ld#\r\n", ADC_Value), 200);
			}
			break;

	}
}
