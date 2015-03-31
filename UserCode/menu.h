#ifndef __MENU_H__
#define __MENU_H__

typedef void *(*actFunc_t)(void *);
typedef void *(*updataFunc_t)(void *);
typedef unsigned short ID_t;

typedef struct
{
    actFunc_t Func;
    void *Arg;
}actFuncAndArg_t;

typedef __menu menu_t;
struct __menu
{
  /*进入菜单时被调用的函数链*/
  actFuncAndArg_t EnterActList;
  /*退出菜单时被调用的函数链*/
  actFuncAndArg_t BackActList;
  /*updata建议做的事情是将所有该菜
    单条目下的子菜单显示出来*/
  actFuncAndArg_t Updata;
  /*在使用UCOSII时，这个ID表示调用该菜单的进程*/
  ID_t IDNum;
  /*菜单条目的名字*/
  char *menuString;
  /*指向父菜单*/
  menu_t *Parent;
  /*指向子菜单*/
  menu_t *subMenuList;
  /*留给用户自行使用的私有数据指针*/
  void pri[0];
};

/*头指针，指向当前所在菜单条目*/
extern menu_t *head;

#define Menu_Enter()\
do{\
  head = head->subMenuList;\
  while(head->EnterActList){\
      head->EnterActList->Func(head->EnterActList->Arg);\
  }\
  if (head->Updata)\
      head->Updata->Func( head->updata->Arg);\
}while(0)

#define Menu_Back()\
do{\
  head = head->Parent;\
  while(head->BackActList){\
      head->BackActList->Func(head->BackActList->Arg);\
  }\
  if (head->Updata)\
      head = head->Updata->Func(head->Updata->Arg);\
}while(0)
#endif