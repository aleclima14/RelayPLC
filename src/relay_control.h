/**
 * @file relay_control.h
 * @author aleclima14@gmail.com
 * @brief 
 * @version 0.1
 * @date 2024-06-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __relay_control_h__
#define __relay_control_h__

/* INCLUDES */
#include <Arduino.h>
#include "timer.h"
#include "pin.h"

/* DEFINES */

/* ENUMS */
typedef enum 
{
   RELAY_ON = 0,
   RELAY_OFF,
   RELAY_STOP,
}EnRelayState;

typedef enum 
{
   RELAY_1 = 0,
   RELAY_2,
   RELAY_3,
   RELAY_4,
   RELAY_NULL,
}EnRelayIndex;

/* STRUCTS */
typedef struct 
{
   EnRelayIndex enRelay;
   uint8_t u8Pin;
   EnRelayState *enStoreState;
   ulong *ulTimerOn;
   ulong *ulTimerOff;
   ulong *ulTimer;
   ulong *ulCycles;
   bool *bRepeatInfinite;
}StTableRelayPin;

typedef struct
{
   EnRelayIndex enRelayIndex;
   EnRelayState enRelayState;
   ulong ulTimerOn;
   ulong ulTimerOff;
   ulong ulCycles;
}EnRepeatCommand;

typedef struct
{
   EnRelayIndex enRelayIndex;
   EnRelayState enRelayState;
}EnSetCommand;

/* PUBLIC FUNCTIONS */
void InitRelay(void);
void RelayDispatcher(void);
void RelaySetStatus(EnSetCommand *enSetCommand);
void RelayRepeatStatus(EnRepeatCommand *enRepeatCommand);
bool FinishCommand();

#endif /*END __relay_control_h__*/