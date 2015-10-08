#ifndef __KEY_H__
#define __KEY_H__

typedef enum{Pressed,Clicked}Key_Status_t;
typedef unsigned short Key_ID_t;
typedef struct
{
    Key_Status_t  Status;/*����״̬*/
    Key_ID_t      ID;/*������*/
}Key_t;

#define ENTER 1
#define BACK 2
#define PRE 3
#define NEXT 4

#endif