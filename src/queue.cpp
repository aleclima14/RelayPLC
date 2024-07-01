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

/* INCLUDES */
#include "queue.h"

/* LOCAL FUNCTIONS */
bool QueueIsEmpty(Queue *queue); 
bool QueueIsFull(Queue *queue);

/* PUBLIC FUNCTIONS */
/**
 * @brief Initialize Queue
 * 
 * @param queue 
 */
void InitializeQueue(Queue *queue)
{
   queue->front = -1;
   queue->rear = -1;
}

/**
 * @brief Add item into queue
 * 
 * @param queue 
 * @param value 
 */
void EnqueueItem(Queue *queue, const char *value)
{
   if (QueueIsFull(queue) == false) 
   {
      if (QueueIsEmpty(queue) == true) 
      {
         queue->front = 0;
      }
      queue->rear++;
      strcpy(queue->data[queue->rear], value);
   }
}

/**
 * @brief Remove item from queue
 * 
 * @param queue 
 * @return const char* 
 */
char *DequeueItem(Queue *queue)
{
   char *value = NULL;
   if (QueueIsEmpty(queue) == false) 
   {
      value = queue->data[queue->front];

      if (queue->front == queue->rear) 
      {
         InitializeQueue(queue);
      } 
      else 
      {
         queue->front++;
      }
   }

   return value;
}

/**
 * @brief Return size of queue
 * 
 * @param queue 
 * @return uint8_t 
 */

uint8_t QueueSize(Queue *queue)
{
   uint8_t queueSize;
   if(queue->rear == -1)
   {
      queueSize = 0;
   }
   else
   {
      queueSize = queue->rear + 1;
   }
   return queueSize;
}

/* LOCAL FUNCTIONS */
/**
 * @brief 
 * 
 * @param queue 
 * @return bool 
 */
bool QueueIsEmpty(Queue *queue)
{
   bool bReturn = false;
   if(queue->front == -1) 
   {
      bReturn = true;
   }
   return bReturn;
}

/**
 * @brief 
 * 
 * @param queue 
 * @return bool 
 */
bool QueueIsFull(Queue *queue)
{
   bool bReturn = false;
   if(queue->rear == (QUEUE_MAX_SIZE - 1))
   {
      bReturn = true;
   }
   return bReturn;
}