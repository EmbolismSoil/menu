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
    Menu_Opt_t opt;/*选择Dis / nodis 显示或者不显示*/
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
              自动生成输出缓冲列表和函数链表
********************************************************/
#define Menu_GenPrintBufferList(String,Opt,x,y,Next) \
        (struct __printBuffer []){{String,Opt,x,y,Next}}
#define Menu_GenActList(Func,Arg,Next) \
        (struct __actFuncAndArg []){{Func,Arg,Next}}


typedef struct __menu
{
  /*进入菜单时被调用的函数链*/
  actFuncAndArg_t *EnterActList;
  actFuncAndArg_t *BackActList;
  /*updata建议做的事情是将所有该菜
    单条目下的子菜单显示出来*/
  updataFunc_t Updata;
  /*在使用UCOSII时，这个ID表示调用该菜单的进程*/
  ID_t IDNum;
  /*子菜单个数*/
  unsigned char subMenuNum;
  char *menuName;
  /*指向父菜单*/
  struct __menu *Parent;
  /*指向子菜单*/
  struct __menu *subMenuList;
  /*上一个兄弟菜单*/
  struct __menu *PreBrother;
  /*指向下一个兄弟菜单*/
  struct __menu *NextBrother;
  /*是否需要显示光标*/
  Menu_Opt_t Focus;
  /*显示缓冲区*/
  printBuffer_t *Buffer;
  Menu_coord coord;
  /*留给用户自行使用的私有数据指针*/
  void *pri;
}menu_t;


/*Menu模块实现的接口*/
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
