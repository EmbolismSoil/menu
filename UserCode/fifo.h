#ifndef __FIFO_H__
#define __FIFO_H__

#define MAX_SIZES 500
#define FIFO_Init(__ptr) do{\
	(__ptr)->bottom = 0;\
         (__ptr)->sizes = 0; \
	(__ptr)->top = 0;}while(0)
#define FIFO_isFull(__fifo) ( (__fifo)->sizes == 0 ? 1 : 0 )
#define FIFO_isNull(__fifo) ( (__fifo)->sizes ? 0 : 1 )         
#define FIFO_GetSizes(__fifo) ( (__fifo)->sizes )

typedef long long distance_t;

typedef struct{
	int top;/*top of this fifo*/
	int bottom;/*bottom of this fifo*/
        int sizes;
	distance_t buf[MAX_SIZES];
}fifo_t;

int FIFO_PushData(fifo_t *FIFO, distance_t data);
distance_t FIFO_PopData(fifo_t *FIFO);

#endif
