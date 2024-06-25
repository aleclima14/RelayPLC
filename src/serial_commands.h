/**
 * @file serial_commands.h
 * @author aleclima14@gmail.com
 * @brief 
 * @version 0.1
 * @date 2024-06-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __serial_commands_h__
#define __serial_commands_h__

/* INCLUDES */
#include <Arduino.h>
#include "relay_control.h"
#include "timer.h"

/* DEFINES */
#define BAUDRATE 115200
#define BUFFER_LENGHT 100

/* PUBLIC FUNCTIONS */
void SerialBegin(void);
void ReadCommandSerial(void);

/* ENUMS */
typedef enum
{
   SET_COMMAND = 0,
   TEST_COMMAND,
   REPEAT_COMMAND,
   HELP_COMMAND,
   WRONG_COMMAND,
}EnCommand;

/* STRUCTS */


#endif /*END __serial_commands_h__*/