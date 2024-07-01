/**
 * @file queue.h
 * @author aleclima14@gmail.com
 * @brief 
 * @version 0.1
 * @date 2024-06-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __queue_h__
#define __queue_h__

/* INCLUDES */
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* DEFINES */
#define QUEUE_MAX_SIZE 256
#define BUFFER_MAX_SIZE 100

/* ENUMS */

/* STRUCTS */
typedef struct
{
   char data[QUEUE_MAX_SIZE][BUFFER_MAX_SIZE];
   int front;
   int rear;
}Queue;

/* PUBLIC FUNCTIONS */
void InitializeQueue(Queue *queue);
void EnqueueItem(Queue *queue, const char *value);
char *DequeueItem(Queue *queue);
uint8_t QueueSize(Queue *queue);


#endif /*END __queue_h__*/