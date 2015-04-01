#include "fifo.h"

#define DEBUG_CHECK(__condiction, __ret...) if (__condiction) return __ret 
int FIFO_PushData(fifo_t *FIFO, distance_t data)
{
	DEBUG_CHECK(!FIFO, -1);
	
	if ((FIFO->top + 1) % MAX_SIZES != FIFO->bottom)
		FIFO->top = (FIFO->top + 1) % MAX_SIZES ;
	else return -1;
	
	FIFO->buf[FIFO->top] = data;
        FIFO->sizes++;
	return 0;
}

distance_t FIFO_PopData(fifo_t *FIFO)
{
	DEBUG_CHECK(!FIFO, -1);

	if (FIFO->bottom != FIFO->top )
		FIFO->bottom = (FIFO->bottom + 1) % MAX_SIZES;
	else return -1;
	FIFO->sizes--;
	return FIFO->buf[FIFO->bottom];
}

