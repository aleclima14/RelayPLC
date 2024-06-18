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

/* STRUCT */
typedef struct
{
   EnRelayState enState;
   ulong TimerOn;
   ulong TimerOff;
   ulong CurrentTimer;
   ulong Cycles;
   bool Repeat; 
}StRelayConfig;

/* LOCAL FUNCTIONS */


/* GLOBAL VARIABLES */
StRelayConfig stRelay1 = 
{
   .enState = RELAY_UNINITIALIZED,
   .TimerOn = 0,
   .TimerOff = 0,
   .CurrentTimer = 0,
   .Cycles = 0,
   .Repeat = false,
};

StRelayConfig stRelay2 = 
{
   .enState = RELAY_UNINITIALIZED,
   .TimerOn = 0,
   .TimerOff = 0,
   .CurrentTimer = 0,
   .Cycles = 0,
   .Repeat = false,
};

StRelayConfig stRelay3 = 
{
   .enState = RELAY_UNINITIALIZED,
   .TimerOn = 0,
   .TimerOff = 0,
   .CurrentTimer = 0,
   .Cycles = 0,
   .Repeat = false,
};

StRelayConfig stRelay4 = 
{
   .enState = RELAY_UNINITIALIZED,
   .TimerOn = 0,
   .TimerOff = 0,
   .CurrentTimer = 0,
   .Cycles = 0,
   .Repeat = false,
};

/* TABLES */
static const StTableRelayPin stTableRelayPin[] = 
{
   {RELAY_1, RELAY_1_PIN, &stRelay1.enState, &stRelay1.TimerOn, &stRelay1.TimerOff, &stRelay1.CurrentTimer, &stRelay1.Cycles, &stRelay1.Repeat},
   {RELAY_2, RELAY_2_PIN, &stRelay2.enState, &stRelay2.TimerOn, &stRelay2.TimerOff, &stRelay2.CurrentTimer, &stRelay2.Cycles, &stRelay2.Repeat},
   {RELAY_3, RELAY_3_PIN, &stRelay3.enState, &stRelay3.TimerOn, &stRelay3.TimerOff, &stRelay3.CurrentTimer, &stRelay3.Cycles, &stRelay3.Repeat},
   {RELAY_4, RELAY_4_PIN, &stRelay4.enState, &stRelay4.TimerOn, &stRelay4.TimerOff, &stRelay4.CurrentTimer, &stRelay4.Cycles, &stRelay4.Repeat},
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

               if(((*(stTableRelayPin[i].ulCycles)) > 0) && (*(stTableRelayPin[i].bRepeatInfinite) == false))
               {
                  *(stTableRelayPin[i].ulCycles) -= 1;
               }
               else
               {
                  if((*(stTableRelayPin[i].ulCycles) == 0) && (*(stTableRelayPin[i].bRepeatInfinite) == false))
                  {
                     *(stTableRelayPin[i].enStoreState) = RELAY_UNINITIALIZED;
                  }
               }
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
   if(enRepeatCommand->enRelayIndex != RELAY_NULL)
   {
      if((enRepeatCommand->enRelayState != RELAY_UNINITIALIZED) && (enRepeatCommand->ulTimerOn > 0) && (enRepeatCommand->ulTimerOff > 0))
      {
         *(stTableRelayPin[enRepeatCommand->enRelayIndex].enStoreState) = enRepeatCommand->enRelayState;
         *(stTableRelayPin[enRepeatCommand->enRelayIndex].ulTimerOn) = enRepeatCommand->ulTimerOn;
         *(stTableRelayPin[enRepeatCommand->enRelayIndex].ulTimerOff) = enRepeatCommand->ulTimerOff;
         
         if(enRepeatCommand->ulCycles > 0)
         {
            *(stTableRelayPin[enRepeatCommand->enRelayIndex].bRepeatInfinite) = false;
            *(stTableRelayPin[enRepeatCommand->enRelayIndex].ulCycles) = enRepeatCommand->ulCycles - 1;
         }
         else
         {
            if(enRepeatCommand->ulCycles == 0)
            {
               *(stTableRelayPin[enRepeatCommand->enRelayIndex].bRepeatInfinite) = true;
            }
         }

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
}