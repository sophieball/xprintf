#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "xprintf.h"

typedef struct valist{
	long item;
	double f_item;

	struct valist* next;
} valist_t;

typedef struct node{
	FILE* stream;
	char* format;
	int valist_length;
	int valist_filled;

	struct node * next;

	char* output;
	char* number;
	struct valist* valist; 
} node_t;

struct queue{
	int filled;
	int length;

	struct node * next;
};

int get_length(queue_t queue){
	return queue->length;
}

/*allocates space for the queue, all nodes and all va_list items;
returns the pointer to the queue*/
queue_t xprintf_init(int message_num){

	int i,j;
	queue_t queue = (queue_t)malloc(sizeof(struct queue));
	struct node* temp;
	struct valist* vatemp;
	if(queue == NULL){
		return NULL;
	}
	queue->next = NULL;
	queue->length = message_num;
	queue->filled = 0;

	/*malloc nodes*/
	for(i=0; i<queue->length; i++){
		temp = (struct node*)malloc(sizeof(struct node));
		if(temp == NULL){
			xprintf_free(queue);
			return NULL;
		}
		/*add node to the queue*/
		temp->next = queue->next;
		queue->next = temp;

		temp->number = (char*)malloc(30);
		temp->output = (char*)malloc(500);
		temp->valist_filled = 0;
		temp->valist_length = 20;
		if(temp->number == NULL){
			xprintf_free(queue);
		}

		/*malloc valist for each node*/
		for(j=0; j<temp->valist_length; j++){
			vatemp = (struct valist*)malloc(sizeof(struct valist));
			if(vatemp == NULL){
				xprintf_free(queue);
				return NULL;
			}
			/*add this item to node*/
			vatemp->next = temp->valist;
			temp->valist = vatemp;
		}
	}
	return queue;
}

/*stores va_list, all items are read and stored as long*/
void xprintf(queue_t queue, char* format, ...){
	int i;
	char st;

	va_list ap;
	valist_t* valist;
	node_t* node;

	/*find the first unfilled node*/
	node = queue->next; 
	if(queue->filled < queue->length){
		for(i=1; i<=queue->filled; i++){
			node = node->next;
		}
	}
	else{
		return;
	}

	queue->filled++;
	node->format = format;

	va_start(ap, format);

	while(*format!='\0'){
		if(*format == '%'){
			/*find the next unused valist node*/
			valist = node->valist;
			if(node->valist_filled < node->valist_length){
				for(i=0; i<node->valist_filled; i++){
					valist = valist->next;
				}
			}
			else{
				/*no more available space*/
				return;
			}

			node->valist_filled++;

			st = *(++format);
			if(st == 'f'){
				valist->f_item = va_arg(ap, double);
			}
			else{
				valist->item = va_arg(ap, long);
			}
		}
		format++;
	}
	va_end(ap);
}

/*converts (unsigned) long/int to string*/
static char *int10_to_str(long int val, char *dst)
{
  char buffer[65];
  register char *p;
  long int new_val;


	if (val < 0)
	{
	  *dst++ = '-';
	  val = -val;
	}
  

  p = &buffer[sizeof(buffer)-1];
  *p = '\0';
  new_val= (long) ((unsigned long int) val / 10);
  *--p = '0'+ (char) ((unsigned long int) val - (unsigned long) new_val * 10);
  val = new_val;

  while (val != 0)
  {
    new_val=val/10;
    *--p = '0' + (char) (val-new_val*10);
    val= new_val;
  }
  while ((*dst++ = *p++) != 0) ;
  return dst-1;
}

/*start printing message from the first node to the last*/
void xprintf_fini(struct queue* queue, FILE *stream){
	int j, count;
	struct node* node;
	struct valist* valist;
	char* format;
	char* message;
	char* message_start;


	node = queue->next;


	for(j=0; j<queue->filled; j++){
		count = 0;
		format = node->format;
		valist = node->valist;
		message_start = node->output;
		message = message_start;
		while(*format != '\0'){		
			if(*format == '%'){
				format++;
				if(*format == 'l'){
					format++;
					if(*format == 'd' || *format == 'u'){
						int10_to_str((long)valist->item, node->number);

						strcpy(message, node->number);
						message += strlen(node->number);
					}
				}
				else if(*format == 'c'){
					*message = (char)valist->item;	
					message += 1;
				}
				else if(*format == 'i' || *format == 'u'){
					int10_to_str((int)valist->item, node->number);

					strcpy(message, node->number);
					message += strlen(node->number);	
				}
				else if(*format == 's'){

					strcpy(message, (char*)valist->item);
					message += strlen((char*)valist->item);	
				}
				else if(*format == 'f'){

					sprintf(node->number, "%f", (float)valist->f_item);
					strcpy(message, node->number);
					message += strlen(node->number);	
				}
				format++;
				valist = valist->next;
			}
			else{
				*message = *format;
				format++;
				message++;
			}
		}
		*(message) = '\0';
		fprintf(stream, "%s", message_start);
		fflush(stdout);
		node = node->next;

	
	}
}

void xprintf_free(struct queue* queue){
	int i,j;
	struct node* node;
	struct valist* valist;
	for(i=0; i<queue->length; i++){
		node = queue->next;
		free(node->number);
		free(node->output);
		for(j=0; j<node->valist_length; j++){
			valist = node->valist;
			node->valist = valist->next;
			free(valist);
		}
		queue->next = node->next;
		free(node);
	}
	free(queue);
}