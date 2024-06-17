/**
 * @file relay_control.cpp
 * @author aleclima14@gmail.com
 * @brief 
 * @version 0.1
 * @date 2024-06-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/* INCLUDES */
#include "relay_control.h"

/* DEFINES */


/* LOCAL FUNCTIONS */


/* GLOBAL VARIABLES */
ulong CurrentTimerRelay1 = 0;
ulong CurrentTimerRelay2 = 0;
ulong CurrentTimerRelay3 = 0;
ulong CurrentTimerRelay4 = 0;

ulong TimerOnRelay1;
ulong TimerOnRelay2;
ulong TimerOnRelay3;
ulong TimerOnRelay4;

ulong TimerOffRelay1;
ulong TimerOffRelay2;
ulong TimerOffRelay3;
ulong TimerOffRelay4;

EnRelayState Relay1State = RELAY_UNINITIALIZED;
EnRelayState Relay2State = RELAY_UNINITIALIZED;
EnRelayState Relay3State = RELAY_UNINITIALIZED;
EnRelayState Relay4State = RELAY_UNINITIALIZED;

/* TABLES */
static const StTableRelayPin stTableRelayPin[] = 
{
   {RELAY_1,   RELAY_1_PIN,    &Relay1State,    &TimerOnRelay1,    &TimerOffRelay1,   &CurrentTimerRelay1},
   {RELAY_2,   RELAY_2_PIN,    &Relay2State,    &TimerOnRelay2,    &TimerOffRelay2,   &CurrentTimerRelay2},
   {RELAY_3,   RELAY_3_PIN,    &Relay3State,    &TimerOnRelay3,    &TimerOffRelay3,   &CurrentTimerRelay3},
   {RELAY_4,   RELAY_4_PIN,    &Relay4State,    &TimerOnRelay4,    &TimerOffRelay4,   &CurrentTimerRelay4},
};
#define RELAY_TABLE_SIZE (int)(sizeof(stTableRelayPin)/sizeof(StTableRelayPin))

/* PUBLIC FUNCTIONS */
/**
 * @brief Init relays pins with initial states
 * 
 */
void InitRelay(void)
{
   for(int i = 0; i < RELAY_TABLE_SIZE; i++)
   {
      pinMode(stTableRelayPin[i].u8Pin, OUTPUT);
      digitalWrite(stTableRelayPin[i].u8Pin, *(stTableRelayPin[i].enStoreState));
   }
}

/**
 * @brief Dispatcher for the relay tables, with the configurations
 * 
 */
void RelayDispatcher(void)
{
   for(int i = 0; i < RELAY_TABLE_SIZE; i++)
   {
      if(*(stTableRelayPin[i].enStoreState) != RELAY_UNINITIALIZED)
      {
         if(*(stTableRelayPin[i].enStoreState) == RELAY_ON)
         {
            if(ElapsedTimer(stTableRelayPin[i].ulTimer, *(stTableRelayPin[i].ulTimerOn)))
            {
               *(stTableRelayPin[i].enStoreState) = RELAY_OFF;
               digitalWrite(stTableRelayPin[i].u8Pin, *(stTableRelayPin[i].enStoreState));
            }
         }
         else
         {
            if(*(stTableRelayPin[i].enStoreState) == RELAY_OFF)
            {
               if(ElapsedTimer(stTableRelayPin[i].ulTimer, *(stTableRelayPin[i].ulTimerOff)))
               {
                  *(stTableRelayPin[i].enStoreState) = RELAY_ON;
                  digitalWrite(stTableRelayPin[i].u8Pin, *(stTableRelayPin[i].enStoreState));
               }
            }
         }
      }
   }
}

void RelaySetStatus(EnSetCommand *enSetCommand)
{
   if(enSetCommand->enRelayIndex != RELAY_NULL) 
   {
      if(enSetCommand->enRelayState != RELAY_UNINITIALIZED)
      {
         digitalWrite (stTableRelayPin[enSetCommand->enRelayIndex].u8Pin, enSetCommand->enRelayState);
      }
      else
      {
         if(enSetCommand->enRelayState == RELAY_UNINITIALIZED)
         {
            digitalWrite (stTableRelayPin[enSetCommand->enRelayIndex].u8Pin, RELAY_OFF);
         }
      }
   }
   
}

void RelayRepeatStatus(EnRepeatCommand *enRepeatCommand)
{
   if((enRepeatCommand->enRelayIndex != RELAY_NULL) && (enRepeatCommand->enRelayState != RELAY_UNINITIALIZED) && (enRepeatCommand->ulTimerOn > 0) && (enRepeatCommand->ulTimerOff > 0))
   {
      *(stTableRelayPin[enRepeatCommand->enRelayIndex].enStoreState) = enRepeatCommand->enRelayState;
      *(stTableRelayPin[enRepeatCommand->enRelayIndex].ulTimerOn) = enRepeatCommand->ulTimerOn;
      *(stTableRelayPin[enRepeatCommand->enRelayIndex].ulTimerOff) = enRepeatCommand->ulTimerOff;
      
      if(enRepeatCommand->enRelayState == RELAY_ON)
      {
         digitalWrite (stTableRelayPin[enRepeatCommand->enRelayIndex].u8Pin, RELAY_ON);
      }
      
      StartTimer(stTableRelayPin[enRepeatCommand->enRelayIndex].ulTimer);
   }  
   else
   {
      *(stTableRelayPin[enRepeatCommand->enRelayIndex].enStoreState) = enRepeatCommand->enRelayState;
      digitalWrite(stTableRelayPin[enRepeatCommand->enRelayIndex].u8Pin, RELAY_OFF);
   }
}