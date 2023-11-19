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

uint8_t cmd_flag;

uint32_t ADC_Value = 0;

int stat = START;
int comm_stat = RECEIVE_CMD;

void command_parser_fsm(){
	switch(stat){
	case START:
		if( buffer[index_buffer - 1]== START_STRING) stat = FIRST_CMD;
		break;
	case FIRST_CMD:
		if(buffer[index_buffer - 1] == 'R'){
			stat = GOT_S;
		}
		else if(buffer[index_buffer - 1] == 'O'){
			stat = GOT_K;
		}
		else stat = START;
		break;
	case GOT_S:
		if(buffer[index_buffer - 1] == 'S'){
			stat = GOT_T;
		}
		else stat = START;
		break;
	case GOT_T:
		if(buffer[index_buffer - 1] == 'T'){
			stat = SEND;
		}
		else stat = START;
		break;
	case GOT_K:
		if(buffer[index_buffer - 1] == 'K'){
			stat = HALT;
		}
		else stat = START;
		break;
	case SEND:
		if(buffer[index_buffer - 1] == END_STRING){
			cmd_flag = SENDING;
			stat = START;
		}
		else stat = START;
		break;
	case HALT:
		if(buffer[index_buffer - 1] == END_STRING){
			cmd_flag = STOP_SEND;
			stat = START;
		}
		break;

	default:
		stat = START;
		break;
	}
}

void uart_communication_fsm(){
	char writeStr[50];
	if(timer1_flag == 1){
		if(cmd_flag == SENDING){
			HAL_UART_Transmit(&huart2, (void*)writeStr, sprintf( writeStr, "!ADC=%ld#", ADC_Value), 1000);
			setTimer1(300);
		}
	}
}
