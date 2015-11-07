
#include <stdio.h>

#ifndef XPRINTF
#define XPRINTF

typedef struct queue * queue_t;

/*Initializes a queue with space enough for message_num. Returns the pointer to the structure.*/
queue_t xprintf_init(int message_num);

/*Adds format and corresponding va_list to the queue.*/
void xprintf(queue_t, char*, ...);

/*Prints out all messages stored in queue in FIFO order to stream.*/
void xprintf_fini(queue_t, FILE* stream);

int get_length(queue_t);

#endif