#include "fifo.h"
#include <stdlib.h>
#include <string.h>

#define DEBUG_CHECK(__condiction, __ret...) if (__condiction) return __ret

int FIFO_PushData(fifo_t *FIFO, FIFO_Data_t data)
{
	DEBUG_CHECK(!FIFO, -1);

	if ((FIFO->top + 1) % FIFO->cap != FIFO->bottom)
		FIFO->top = (FIFO->top + 1) % FIFO->cap ;
	else return -1;

	FIFO->buf[FIFO->top] = data;
        FIFO->sizes++;
	return 0;
}

FIFO_Data_t FIFO_PopData(fifo_t *FIFO)
{
	DEBUG_CHECK(!FIFO, -1);
	if (FIFO->bottom != FIFO->top )
		FIFO->bottom = (FIFO->bottom + 1) % FIFO->cap;
	else return -1;
	FIFO->sizes--;
	return FIFO->buf[FIFO->bottom];
}

 fifo_t *FIFO_NewFIFO(unsigned int cap)
{
    if (cap <= 0) return NULL;

	fifo_t *fifo = malloc(sizeof(fifo_t));
	if (!fifo) return NULL;
	memset(fifo,0,sizeof(fifo_t));

	fifo->buf = (FIFO_Data_t*)malloc(sizeof(FIFO_Data_t)*cap);
	if (!fifo->buf){
		free(fifo);
		return NULL;
	}
	//memset(fifo->buf,0,cap * sizeof(FIFO_Data_t));

	fifo->cap = cap;
    fifo->top = 0;
	fifo->bottom = 0;

	return fifo;
}