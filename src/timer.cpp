/**
 * @file timer.cpp
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
#include "time.h"

boolean ElapsedTimer(ulong *timerPointer, ulong delay)
{
   boolean returnValue = false;

   if((millis() - *timerPointer) >= delay)
   {
      *timerPointer = millis();
      returnValue = true;
   }
   return returnValue;
}

void StartTimer(ulong *timerPointer)
{
   *timerPointer = millis();
}