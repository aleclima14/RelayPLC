/**
 * @file serial_commands.cpp
 * @author aleclima14@gmail.com
 * @brief 
 * @version 0.1
 * @date 2024-06-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/* INCLUDES */
#include <string.h>
#include <stdlib.h>
#include "serial_commands.h"
#include "relay_control.h"
#include "queue.h"

/* DEFINES */


/* LOCAL FUNCTIONS */
EnCommand GetCommand(const char *command);
EnRelayIndex GetRelayIndex(const char *relay);
EnRelayState GetRelayState(const char *state);
ulong GetUlongValue(const char *ulongValue);
void ParserCommand(char *command);

/* GLOBAL VARIABLES */
EnCommand enCommand;
char SerialBuffer[BUFFER_LENGHT];
uint8_t BufferSize = 0;
Queue SerialQueue;
bool bFirstQueueInitialize = false;

/* CONST VARIABLES */

/* TABLES */

/* PUBLIC FUNCTIONS */
/**
 * @brief Init serial
 * 
 */
void SerialBegin(void)
{
   Serial.begin(BAUDRATE);
}

void ReadCommandSerial(void)
{  
   static unsigned int messagePosition = 0;
   static char bufferMessage[BUFFER_LENGHT];
   char readedByteSerial;
   bool validBuffer = false;

   if(bFirstQueueInitialize == false)
   {
      InitializeQueue(&SerialQueue);
      bFirstQueueInitialize = true;
   }

   while (Serial.available() > 0)
   {
      readedByteSerial = Serial.read();

      if((readedByteSerial != '\n') && (messagePosition < (BUFFER_LENGHT - 1)))
      {
         if((readedByteSerial != '\r'))
         {
            bufferMessage[messagePosition] = readedByteSerial;
            messagePosition++;
         }
      }
      else
      { 
         BufferSize = messagePosition;
         messagePosition = 0;

         /* Backup buffer into SerialBuffer */
         for(uint8_t i = 0; i <= BufferSize; i++)
         {
            SerialBuffer[i] = bufferMessage[i];
            bufferMessage[i] = 0;
         }
      }
   }

   /* Check if buffer is valid */
   for(uint8_t i = 0; i < BufferSize; i++)
   {
      if(SerialBuffer[i] != 0)
      {
         validBuffer = true;
      }
   }
   
   if(validBuffer == true)
   {
      EnqueueItem(&SerialQueue, SerialBuffer);

      /* Clear buffer */
      for(uint8_t i = 0; i <= BufferSize; i++)
      {
         SerialBuffer[i] = 0;
      }
      validBuffer = false;
   }

   if((FinishCommand() == true) && (QueueSize(&SerialQueue) > 0))
   {
      ParserCommand(DequeueItem(&SerialQueue));
   }
}

EnCommand GetCommand(const char *command)
{
   EnCommand enReturn = WRONG_COMMAND;
   if (strcmp(command, "set") == 0) enReturn = SET_COMMAND;
   if (strcmp(command, "test") == 0) enReturn = TEST_COMMAND;
   if (strcmp(command, "repeat") == 0) enReturn = REPEAT_COMMAND;
   if (strcmp(command, "help") == 0) enReturn = HELP_COMMAND;
   return enReturn;
}

EnRelayIndex GetRelayIndex(const char *relay) 
{
   EnRelayIndex enReturn = RELAY_NULL;
   if (strcmp(relay, "relay1") == 0) enReturn = RELAY_1;
   if (strcmp(relay, "relay2") == 0) enReturn = RELAY_2;
   if (strcmp(relay, "relay3") == 0) enReturn = RELAY_3;
   if (strcmp(relay, "relay4") == 0) enReturn = RELAY_4;
   return enReturn;
}

EnRelayState GetRelayState(const char *state) 
{
   EnRelayState enReturn = RELAY_STOP;
   if (strcmp(state, "on") == 0) enReturn = RELAY_ON;
   if (strcmp(state, "off") == 0) enReturn = RELAY_OFF;
   if (strcmp(state, "stop") == 0) enReturn = RELAY_STOP;
   return enReturn; 
}

ulong GetUlongValue(const char *ulongValue)
{
   return strtoul(ulongValue, NULL, 0);
}

void ParserCommand(char *command)
{
   char *token = strtok(command, ",");
   EnRepeatCommand enRepeatCommand;
   EnSetCommand enSetCommand;

   enCommand = GetCommand(token);
   
   switch (enCommand)
   {
      case HELP_COMMAND:
      {
         Serial.println(">>set command<<\r\n set,relayX,status\r\n Example: \r\n set,relay1,on\r\n for turn on relay1\r\n\r\n");
         Serial.println(">>repeat command<<\r\n repeat,relayX,initialStatus,timeOn,timeOff,cycles\r\n Example:\r\n repeat,relay1,on,5,10,4 \r\n for 5s on, 10s off and 4 cycles\r\nOBS: use cycles = 0 to infine cycles");
         break;
      }
      case SET_COMMAND:
      {
         /* set,relay1,on */
         token = strtok(NULL, ",");
         enSetCommand.enRelayIndex = GetRelayIndex(token);
         
         token = strtok(NULL, ",");
         enSetCommand.enRelayState = GetRelayState(token);

         RelaySetStatus(&enSetCommand);
         break;
      }

      case REPEAT_COMMAND:
      {
         /* repeat,relay1,off,1000,2000,0 */
         /* repeat,relay2,stop */
         token = strtok(NULL, ",");
         enRepeatCommand.enRelayIndex = GetRelayIndex(token);

         token = strtok(NULL, ",");
         enRepeatCommand.enRelayState = GetRelayState(token);

         if(enRepeatCommand.enRelayState != RELAY_STOP)
         {
            token = strtok(NULL, ",");
            enRepeatCommand.ulTimerOn = GetUlongValue(token) * 1000;

            token = strtok(NULL, ",");
            enRepeatCommand.ulTimerOff = GetUlongValue(token) * 1000;

            token = strtok(NULL, ",");
            enRepeatCommand.ulCycles = GetUlongValue(token);
         }
         
         RelayRepeatStatus(&enRepeatCommand);
         break;
      }

      default:
      break;
   }
}