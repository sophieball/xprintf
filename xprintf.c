#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "xprintf.h"

enum types = {t_int, t_char, t_float, t_long, t_unsigned_long, t_unsigned_int, t_string};

typedef struct valist{
	//void* item;
	long item;
	enum types type;
	struct valist* next;
} valist_t;

struct node{
	FILE* stream;
	char* format;
	struct valist* valist; 
	int valist_length;
	int valist_filled;
	struct node * next;
	// int totalsize;
	char* output;
	char* number;
};

struct queue{
	struct node * next;
	int filled;
	int length;
};

int get_length(queue_t queue){
	return queue->length;
}

/*allocates space for the queue, all nodes and all va_list items;
returns the pointer to the queue*/
queue_t xprintf_init(int message_num){
	int i,j;
	queue_t queue = (queue_t)malloc(sizeof(queue_t));
	struct node* temp;
	struct valist* vatemp;
	void* item;
	char* output;
	if(queue == NULL){
		return NULL;
	}
	queue->next = NULL;
	queue->length = 0;
	queue->filled = 0;

	/*malloc nodes*/
	for(i=0; i<message_num; i++){
		temp = (struct node*)malloc(sizeof(struct node));
		if(temp == NULL){
			xprintf_free(queue);
			return NULL;
		}
		/*add node to the queue*/
		temp->next = queue->next;
		queue->next = temp;
		queue->length++;

		output = (char*)malloc(500);
		temp->output = output;
		temp->filled = 0;
		temp->valist_length = 0;
		temp->number = (char*)malloc(30);
		if(temp->number == NULL){
			xprintf_free(queue);
		}

		/*malloc valist for each node*/
		for(j=0; j<20; j++){
			vatemp = (struct valist*)malloc(sizeof(struct valist));
			if(vatemp == NULL){
				xprintf_free(queue);
				return NULL;
			}
			/*add this item to node*/
			vatemp->next = temp->valist;
			temp->valist = vatemp;
			temp->valist_length++;
		}
	}
	// queue->next = NULL;
	// queue->length = 0;
	return queue;
}

/*stores va_list, all items are read and stored as long*/
void xprintf(queue_t queue, char* format, ...){
	int i;

	va_list ap;
	valist_t* valist;

	char st;
	int aint;
	unsigned int aui;
	char* astring;
	double afloat;
	unsigned long aul;
	long along;

	int* pint;
	unsigned int* pui;
	char* pstring;
	double* pfloat;
	unsigned long* pul;
	long* plong;
	struct node* node;

	// newNode = (struct node*) malloc(sizeof(struct node));
	// if(newNode == NULL){
	// 	return;
	// }

	/*find the first unfilled node*/
	node = queue->next; 
	for(i=1; i<queue->filled; i++){
		node = node->next;
	}
	queue->filled++;
	node->format = format;
	// node->totalsize = 0;
	// node->valist_length = 0;
	// node->valist = NULL;
	// node->next = queue->next;

	/*adds the new message to the beginning of the list	*/
	// queue->next = newNode;
	// (queue->length)++;

	va_start(ap, format);


	while(*format!='\0'){
		if(*format == '%'){
			// valist = (valist_t*) malloc(sizeof(valist_t));
			/*find the next unused valist node*/
			valist = node->valist;
			for(i=0; i<node->valist_filled; i++){
				valist = valist->next;
			}
			node->valist_filled++;

			// if(valist == NULL){
			// 	free(newNode);
			// 	return;
			// }

			valist->item = va_arg(ap, long);

			st = *(++format);
			switch(st){
				case 'i':
					// aint = va_arg(ap, int);
					// pint = (int*) malloc(sizeof(aint));
					// *pint = aint;
					// valist->item = pint;
					// newNode->totalsize += 10;
					valist->type = t_int;
					break;
				case 'u':
					// aui = va_arg(ap, unsigned int);
					// pui = (unsigned int*) malloc(sizeof(aui));
					// *pui = aui;
					// valist->item = pui;
					// newNode->totalsize += 10;
					valist->type = t_unsigned_int;
					break;
				case 'd':
					// aint = va_arg(ap, int);
					// pint = (int*) malloc(sizeof(aint));
					// *pint = aint;
					// valist->item = pint;
					// newNode->totalsize += 10;
					valist->type = t_int;
					break;
				case 'c':
					// aint = va_arg(ap, int);
					// pint = (int*) malloc(sizeof(aint));
					// *pint = aint;
					// valist->item = pint;
					// newNode->totalsize += 10;
					// printf("\n");
					valist->type = t_char;
					break;
				case 's':
					// astring = va_arg(ap, char*);
					// pstring = (char*) malloc(sizeof(astring));
					// strcpy(pstring, astring);
					// valist->item = pstring;
					// newNode->totalsize += sizeof(astring);
					valist->type = t_string;
					break;
				case 'l':
					if(*(format+1) == 'u'){
						// format++;
						// aul = va_arg(ap, unsigned long);
						// pul = (unsigned long*) malloc (sizeof(aul));
						// *pul = aul;
						// valist->item = pul;
						// newNode->totalsize += 20;
						valist->type = t_unsigned_long;
						break;
					}
					else if(*(format+1) == 'd'){
						// format++;
						// along = va_arg(ap, long);
						// plong = (long*) malloc(sizeof(along));
						// *plong = along;
						// valist->item = plong;
						// newNode->totalsize += 20;
						valist->type = t_long;
						break;
					}
					break;
				case 'f':
					// afloat = va_arg(ap, double);
					// pfloat = (double*) malloc(sizeof(afloat));
					// *pfloat = afloat;
					// valist->item = pfloat;
					// newNode->totalsize += sizeof(afloat);
					valist->type = t_float;
					break;
			}
			// valist->next = newNode->valist;
			// newNode->valist = valist;
			// newNode->valist_length++;
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
	int i, j;
	struct node* node = queue->next;
	struct valist* valist = node->valist;
	char* format;
	char* message;
	char* message_start;
	char* numbers;

	// if(node != NULL){
	// 	for(i=1; i<queue->length; i++){

	// 		node = node->next;
	// 	}
	// }
	// (queue->length)--;

	for(j=0; j<queue->filled; j++){
		format = node->format;
		// message_start = (char*)malloc(sizeof(format) + node->totalsize + 1);
		message_start = node->output;
		message = message_start;
		while(*format != '\0'){		
			if(*format == '%'){
				/*replace with the item. free the valist node.*/
				format++;
				if(*format == 'l'){
					format++;
					if(*format == 'd' || *format == 'u'){
						valist = node->valist;
						for(i=1; i<node->valist_filled; i++){
							valist = valist->next;
						}
						

						// numbers = (char*) calloc(21, 1);
						int10_to_str((long)valist->item, node->number);

						strcpy(message, node->number);
						message += strlen(node->number);
						// free(numbers);
						// free(temp->item);
						// free(temp);
						// node->valist_length--;
					}
				}
				else if(*format == 'c'){
					valist = node->valist;
					for(i=1; i<node->valist_filled; i++){
						valist = valist->next;
					}

					*message = (char)valist->item;	
					message += 1;
				}
				else if(*format == 'i' || *format == 'u'){
					valist = node->valist;
					for(i=1; i<node->valist_filled; i++){
						valist = valist->next;
					}

					int10_to_str((int)valist->item, node->number);

					strcpy(message, node->numbers);
					message += strlen(numbers);	
				}
				else if(*format == 's'){
					valist = node->valist;
					for(i=1; i<node->valist_filled; i++){
						valist = valist->next;
					}

					strcpy(message, (char*)valist->item);
					message += strlen((char*)valist->item);	
				}
				else if(*format == 'f'){
					valist = node->valist;
					for(i=1; i<node->valist_filled; i++){
						valist = valist->next;
					}

					sprintf(node->number, "%f", (double)valist->item);
					strcpy(message, node->number);
					message += strlen(node->number);	
				}
				format++;
			}
			else{
				*message = *format;
				format++;
				message++;
			}
		}
		*(message) = '\0';
		fprintf(stream, "%s", message_start);
		// free(node);
		// free(message_start);

		node = node->next;
		// if(queue->length == 0){
		// 	node = NULL;
		// }
		// else{
		// 	node = queue->next;
		// 	for(i=1; i<queue->length; i++){
		// 		node = node->next;
		// 	}
		// }
		// (queue->length)--;
	}
	// free(queue);
}

