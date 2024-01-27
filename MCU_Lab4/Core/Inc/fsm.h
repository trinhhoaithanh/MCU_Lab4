/*
 * fsm.h
 *
 *  Created on: Nov 15, 2023
 *      Author: Hoai Thanh
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "main.h"
#include "string.h"
#include<stdio.h>
#include "software_timer.h"

#define MAX_BUFFER_SIZE 30

#define START 0
#define RECEIVING 1

#define SENDING 2


#define START_STRING '!'
#define END_STRING '#'

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;

extern uint8_t temp;
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t buffer_flag;

extern uint8_t cmd_flag;

extern int stat;
extern int comm_stat;

extern uint32_t ADC_Value;

extern uint8_t pkgSent;

void command_parser_fsm();

void uart_communication_fsm();


#endif /* INC_FSM_H_ */
