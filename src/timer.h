/**
 * @file timer.h
 * @author aleclima14@gmail.com
 * @brief 
 * @version 0.1
 * @date 2024-06-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/* INCLUDES */
#include "Arduino.h"

/* DEFINES */
#ifndef __TIMER_H__
#define __TIMER_H__

/* STRUCTS */
typedef unsigned long int ulong;

/* PUBLIC FUNCTIONS */
boolean ElapsedTimer(ulong *timerPointer, ulong delay);
void StartTimer(ulong *timerPointer);

#endif /*END __TIMER_H__*/
