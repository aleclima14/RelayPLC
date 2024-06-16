/**
 * @file main.cpp
 * @author aleclima14@gmail.com
 * @brief 
 * @version 0.1
 * @date 2024-06-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/* INCLUDES */
#include <Arduino.h>
#include "timer.h"
#include "relay_control.h"
#include "serial_commands.h"

void setup() 
{ 
   InitRelay();
   SerialBegin();
}

void loop() 
{
   RelayDispatcher();
   ReadCommandSerial();
}


