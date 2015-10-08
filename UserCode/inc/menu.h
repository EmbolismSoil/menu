#ifndef __MENU_H__
#define __MENU_H__
#define MENU_BUFFER_SIZES 100
#include <stdlib.h>
#include <string.h>

#define Align 1
#define unalign 0

typedef struct __actFuncAndArg actFuncAndArg_t;
typedef struct __printBuffer printBuffer_t;
typedef enum{CLEAR = 0,Dis,NODis,focus,unfocus}Menu_Opt_t;

typedef void *(*actFunc_t)(void *);
typedef void (*updataFunc_t)(unsigned char, unsigned char,
                             unsigned char ,Menu_Opt_t, char *);

typedef unsigned short ID_t;

struct __actFuncAndArg
{
    actFunc_t Func;
    void *Arg;
    actFuncAndArg_t *NextFunc;
};

struct __printBuffer
{
    char *printString;
    Menu_Opt_t opt;/*ѡ��Dis / nodis ��ʾ���߲���ʾ*/
    unsigned char x;
    unsigned char y;
    printBuffer_t *Next;
};

typedef struct
{
    char line;
    char row;
}Menu_coord;
/********************************************************
              �Զ�������������б�ͺ�������
********************************************************/
#define Menu_GenPrintBufferList(String,Opt,x,y,Next) \
        (struct __printBuffer []){{String,Opt,x,y,Next}}
#define Menu_GenActList(Func,Arg,Next) \
        (struct __actFuncAndArg []){{Func,Arg,Next}}


typedef struct __menu
{
  /*����˵�ʱ�����õĺ�����*/
  actFuncAndArg_t *EnterActList;
  actFuncAndArg_t *BackActList;
  /*updata�������������ǽ����иò�
    ����Ŀ�µ��Ӳ˵���ʾ����*/
  updataFunc_t Updata;
  /*��ʹ��UCOSIIʱ�����ID��ʾ���øò˵��Ľ���*/
  ID_t IDNum;
  /*�Ӳ˵�����*/
  unsigned char subMenuNum;
  char *menuName;
  /*ָ�򸸲˵�*/
  struct __menu *Parent;
  /*ָ���Ӳ˵�*/
  struct __menu *subMenuList;
  /*��һ���ֵܲ˵�*/
  struct __menu *PreBrother;
  /*ָ����һ���ֵܲ˵�*/
  struct __menu *NextBrother;
  /*�Ƿ���Ҫ��ʾ���*/
  Menu_Opt_t Focus;
  /*��ʾ������*/
  printBuffer_t *Buffer;
  Menu_coord coord;
  /*�����û�����ʹ�õ�˽������ָ��*/
  void *pri;
}menu_t;


/*Menuģ��ʵ�ֵĽӿ�*/
extern menu_t *Menu_CurMenu(void);
extern menu_t* Menu_Enter(void);
extern menu_t* Menu_Back(void);
extern menu_t* Menu_Next(void);
extern menu_t* Menu_Pre(void);
extern menu_t* Menu_AddBrotherAfter(menu_t *dstMenu, menu_t *srcMenu);
extern menu_t* Menu_AddBrotherBefore(menu_t *dstMenu, menu_t *srcMenu);
extern menu_t* Menu_HeadInit(menu_t* root);
extern menu_t* Menu_NewMenu(char *menuString, char line,char row,ID_t ID, menu_t* Parent,
                     actFuncAndArg_t *EnterActList,
                      actFuncAndArg_t *BackActList,
                      updataFunc_t Updata,printBuffer_t *bufferList);
extern menu_t *Menu_Updata(void);
extern Menu_coord Menu_CursorCood(void);
extern void Menu_cursorON(void);
extern  void Menu_cursorOFF(void);
extern menu_t* Menu_RefreshBuffer(void);

#endif
