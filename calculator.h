/*
 * calculator.h
 *
 *  Created on: Sep 23, 2023
 *  Author: omar
 */

#ifndef INC_CALCULATOR_H_
#define INC_CALCULATOR_H_

#define PLUS   10
#define MINUS  11
#define MUL    12
#define DIV    13
#define EQUAL  14
#define DOT   15



void Run_Calculator(void);
void Calc_Get_Operation (u8 operation);
void Reset_Calc (void);

#endif /* INC_CALCULATOR_H_ */
