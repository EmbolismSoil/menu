#include "fifo_AllType.h"

#include <stdlib.h>
#include <string.h>

#define DEBUG_CHECK(__condiction, __ret...) if (__condiction) return __ret

int __FIFO_PushData(fifo_t *FIFO, void* data, unsigned int len)
{
	DEBUG_CHECK(!FIFO, -1);
    if (FIFO->sizes < len) return -1;


	int cnt = 0;
	char *dp = (char *)data;

	for (cnt = 0; cnt < len; cnt++)
	{
	    FIFO->top[0] = dp[0];

		FIFO->top + 1 == FIFO->buf + FIFO->cap ?
		FIFO->top = FIFO->buf : ++(FIFO->top);

		++dp;
		--(FIFO->sizes);
	}

	return 0;
}

int __FIFO_PopData(fifo_t *FIFO,void *data, unsigned int len )
{
	DEBUG_CHECK(!FIFO, -1);
	if (FIFO->cap - FIFO->sizes < len)
		return -1;

	char *dp = (char *)data;

	int cnt = 0;
	for (cnt = 0; cnt <len; cnt ++){
		dp[0] = FIFO->bottom[0];

		FIFO->bottom + 1 == FIFO->buf + FIFO->cap ?
		FIFO->bottom = FIFO->buf : ++(FIFO->bottom);
		++dp;
		++(FIFO->sizes);
	}

	return 0;

}

 fifo_t *FIFO_NewFIFO(unsigned int cap)
{
    if (cap <= 0) return NULL;

	fifo_t *fifo = malloc(sizeof(fifo_t));
	if (!fifo) return NULL;
	memset(fifo,0,sizeof(fifo_t));

	fifo->buf = (char*)malloc(sizeof(char)*cap);
	if (!fifo->buf){
		free(fifo);
		return NULL;
	}
	//memset(fifo->buf,0,cap * sizeof(FIFO_Data_t));

	fifo->cap = cap;
    fifo->sizes = cap;
	fifo->top = fifo->buf;
	fifo->bottom = fifo->buf;

	return fifo;
}