#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "xprintf.h"

typedef struct valist{
	void* item;
	struct valist* next;
} valist_t;

struct node{
	FILE* stream;
	char* format;
	struct valist* valist; 
	int valist_length;
	struct node * next;
	int totalsize;
};

struct queue{
	struct node * next;
	int length;
};

queue_t printx_init(){
	struct queue* queue = (struct queue*)malloc(sizeof(struct queue));
	if(queue == NULL){
		return NULL;
	}
	queue->next = NULL;
	queue->length = 0;
	return queue;
}

void print_in(struct queue* queue, FILE *stream, char* format, ...){
	va_list ap;
	valist_t* valist;

	char st;
	int aint;
	char* astring;
	double afloat;
	unsigned long aul;
	long along;

	int* pint;
	char* pstring;
	double* pfloat;
	unsigned long* pul;
	long* plong;
	struct node* newNode;

	newNode = (struct node*) malloc(sizeof(struct node));
	if(newNode == NULL){
		return;
	}
	newNode->stream = stream;
	newNode->format = format;
	newNode->totalsize = 0;
	newNode->valist_length = 0;
	newNode->valist = NULL;
	newNode->next = queue->next;

	/*adds the new message to the beginning of the list	*/
	queue->next = newNode;
	(queue->length)++;

	va_start(ap, format);



	while(*format!='\0'){
		if(*format == '%'){
			valist = (valist_t*) malloc(sizeof(valist_t));
			if(valist == NULL){
				free(newNode);
				return;
			}
			st = *(++format);
			switch(st){
				case 'i':
					aint = va_arg(ap, int);
					pint = (int*) malloc(sizeof(aint));
					*pint = aint;
					valist->item = pint;
					newNode->totalsize += 10;
					break;

				case 'd':
					aint = va_arg(ap, int);
					pint = (int*) malloc(sizeof(aint));
					*pint = aint;
					valist->item = pint;
					newNode->totalsize += 10;

					break;
				case 'c':
					aint = va_arg(ap, int);
					pint = (int*) malloc(sizeof(aint));
					*pint = aint;
					valist->item = pint;
					newNode->totalsize += 10;
					printf("\n");
					break;
				case 's':
					astring = va_arg(ap, char*);
					pstring = (char*) malloc(sizeof(astring));
					strcpy(pstring, astring);
					valist->item = pstring;
					newNode->totalsize += sizeof(astring);

					break;
				case 'l':
					if(*(format+1) == 'u'){
						format++;
						aul = va_arg(ap, unsigned long);
						pul = (unsigned long*) malloc (sizeof(aul));
						*pul = aul;
						valist->item = pul;
						newNode->totalsize += 20;

						break;
					}
					else if(*(format+1) == 'd'){
						format++;
						along = va_arg(ap, long);
						plong = (long*) malloc(sizeof(along));
						*plong = along;
						valist->item = plong;
						newNode->totalsize += 20;

						break;
					}
					break;
				case 'f':
					afloat = va_arg(ap, double);
					pfloat = (double*) malloc(sizeof(afloat));
					*pfloat = afloat;
					valist->item = pfloat;
					newNode->totalsize += sizeof(afloat);
					break;
			}
			valist->next = newNode->valist;
			newNode->valist = valist;
			newNode->valist_length++;
		}
		format++;
	}
	va_end(ap);
}

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

void printx(struct queue* queue){
	int i = 0;
	struct node* node = queue->next;
	struct valist* temp;
	char* format;
	char* message;
	char* message_start;
	char* numbers;

	if(node != NULL){
		for(i=1; i<queue->length; i++){

			node = node->next;
		}
	}
	(queue->length)--;


	while(node != NULL){
		format = node->format;
		message_start = (char*)malloc(sizeof(format) + node->totalsize + 1);
		message = message_start;
		while(*format != '\0'){		
			if(*format == '%'){
				/*replace with the item. free the valist node.*/
				format++;
				if(*format == 'l'){
					format++;
					if(*format == 'd'){
						temp = node->valist;
						if(temp != NULL){
							for(i=1; i<node->valist_length; i++){
								temp = temp->next;
							}
						}
						if(numbers==NULL){
							return;
						}

						numbers = (char*) calloc(21, 1);
						int10_to_str(*(long*)temp->item, numbers);

						strcpy(message, numbers);
						message += sizeof(numbers);
						free(numbers);
						free(temp->item);
						free(temp);
						node->valist_length--;
					}
					else if(*format == 'u'){
						temp = node->valist;
						if(temp != NULL){
							for(i=1; i<node->valist_length; i++){
								temp = temp->next;
							}
						}
						if(numbers==NULL){
							return;
						}

						numbers = (char*) calloc(21, 1);
						int10_to_str(*(unsigned long*)temp->item, numbers);

						strcpy(message, numbers);
						message += sizeof(numbers);
						free(numbers);
						free(temp->item);
						free(temp);
						node->valist_length--;
					}
				}
				else if(*format == 'c'){
					temp = node->valist;
					if(temp != NULL){
						for(i=1; i<node->valist_length; i++){
							temp = temp->next;
						}
					}

					if(numbers==NULL){
						return;
					}
					*message = *(int*)temp->item;	
					message += 1;
					free(temp->item);
					free(temp);
					node->valist_length--;
				}
				else if(*format == 'i'){
					temp = node->valist;
					if(temp != NULL){
						for(i=1; i<node->valist_length; i++){
							temp = temp->next;
						}
					}

					if(numbers==NULL){
						return;
					}
					numbers = (char*)calloc(11, 1);
					int10_to_str(*(int*)temp->item, numbers);
					strcpy(message, numbers);
					message += strlen(numbers);
					free(numbers);
					free(temp->item);
					free(temp);
					node->valist_length--;
				}
				else if(*format == 's'){
					temp = node->valist;
					if(temp != NULL){
						for(i=1; i<node->valist_length; i++){
							temp = temp->next;
						}
					}

					if(numbers==NULL){
						return;
					}
					strcpy(message, temp->item);
					message += strlen(temp->item);
					free(temp->item);
					free(temp);
					node->valist_length--;
				}
				else if(*format == 'f'){
					temp = node->valist;
					if(temp != NULL){
						for(i=1; i<node->valist_length; i++){
							temp = temp->next;
						}
					}

					if(numbers==NULL){
						return;
					}
					numbers = (char*)calloc(35, 1);
					sprintf(numbers, "%f", *(double*)temp->item);
					strcpy(message, numbers);
					message += strlen(numbers);
					free(numbers);
					free(temp->item);
					free(temp);
					node->valist_length--;
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
		fprintf(node->stream, "%s", message_start);
		free(node);
		free(message_start);

		
		if(queue->length == 0){
			node = NULL;
		}
		else{
			node = queue->next;
			for(i=1; i<queue->length; i++){
				node = node->next;
			}
		}
		(queue->length)--;
	}
}


void printx_fini(struct queue* queue){
	free(queue);
}



