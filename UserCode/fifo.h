#ifndef __FIFO_H__
#define __FIFO_H__

#define MAX_SIZES 500
#define FIFO_Init(__ptr,__cap) do{\
	(__ptr)->bottom = 0;\
         (__ptr)->sizes = 0; \
	(__ptr)->top = 0;}while(0)
#define FIFO_isFull(__fifo) ( (__fifo)->sizes == 0 ? 1 : 0 )
#define FIFO_isNull(__fifo) ( (__fifo)->sizes ? 0 : 1 )
#define FIFO_GetSizes(__fifo) ( (__fifo)->sizes )

typedef long long FIFO_Data_t;

typedef struct{
	int top;/*top of this fifo*/
	int bottom;/*bottom of this fifo*/
        int sizes;
	int cap;
	FIFO_Data_t *buf;
}fifo_t;

extern fifo_t *FIFO_NewFIFO(unsigned int cap);
extern int FIFO_PushData(fifo_t *FIFO, FIFO_Data_t data);
extern FIFO_Data_t FIFO_PopData(fifo_t *FIFO);

#endif
