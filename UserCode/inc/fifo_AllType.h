#ifndef __FIFO_ALLTYPE_H__
#define __FIFO_ALLTYPE_H__


#define FIFO_Init(__ptr,__cap) do{\
	(__ptr)->bottom = 0;\
         (__ptr)->sizes = 0; \
	(__ptr)->top = 0;}while(0)
#define FIFO_isFull(__fifo) ( (__fifo)->sizes ? 1 : 0 )
#define FIFO_isNull(__fifo) ( (__fifo)->sizes == (__fifo)->cap ? 1 : 0 )
#define FIFO_GetSizes(__fifo) ( (__fifo)->sizes )

typedef long long FIFO_Data_t;

typedef struct{
	char* top;/*top of this fifo*/
	char* bottom;/*bottom of this fifo*/
	char *buf;
    unsigned int sizes;/*FIFO可用的字节数*/
	unsigned int cap;/*FIFO的容量（字节）*/
}fifo_t;

extern fifo_t *FIFO_NewFIFO(unsigned int cap);
extern int __FIFO_PopData(fifo_t *FIFO,void *data, unsigned int len );
extern int __FIFO_PushData(fifo_t *FIFO, void* data, unsigned int len);

#define FIFO_POP(pfifo,data)   __FIFO_PopData(pfifo,&data,sizeof(data))
#define FIFO_PUSH(pfifo,data) __FIFO_PushData(pfifo,&data,sizeof(data))


/**************************************************************************
                            Demo
**************************************************************************
#include <stdio.h>
#include "test.h"

int main(void)
{
    fifo_t *fifo = FIFO_NewFIFO(100);

    int data = 0;
    int cnt = 0;

    for (cnt = 0; cnt < 3000; cnt++){
        FIFO_PUSH(fifo,cnt);
	FIFO_POP(fifo,data);
	printf("%d ",data);
    }

    return 0;
}
***************************************************************************
                            End
**************************************************************************/



#endif
