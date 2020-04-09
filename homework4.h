/*
 * homework4.h
 *
 *  Created on:
 *      Author:
 */

#ifndef HOMEWORK4_H_
#define HOMEWORK4_H_

// This function initializes the board by turning off the Watchdog Timer.
void initBoard();
bool charFSM(char rChar);
void UART_transmitData(uint32_t moduleInstance, uint_fast8_t transmitData);
bool UARTHasChar(uint32_t moduleInstance);
void UARTPutChar(uint32_t moduleInstance, uint8_t transmittedChar);
void UARTPutString(uint32_t moduleInstance, char *str);
bool UARTCanSend(uint32_t moduleInstance);
// TODO: Define any constants that are local to homework.c using #define


#endif /* HOMEWORK4_H_ */
