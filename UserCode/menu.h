#ifndef __MENU_H__
#define __MENU_H__
#define MENU_BUFFER_SIZES 100

#include <stdio.h>


typedef void *(*actFunc_t)(void *);
typedef void *(*updataFunc_t)(void *);
typedef unsigned short ID_t;

typedef struct __actFuncAndArg actFuncAndArg_t;
struct __actFuncAndArg
{
    actFunc_t Func;
    void *Arg;
    actFuncAndArg_t *NextFunc;
};

typedef struct __menu
{
  /*进入菜单时被调用的函数链*/
  actFuncAndArg_t *EnterActList;
  actFuncAndArg_t *BackActList;
  /*updata建议做的事情是将所有该菜
    单条目下的子菜单显示出来*/
  actFuncAndArg_t *Updata;
  /*在使用UCOSII时，这个ID表示调用该菜单的进程*/
  ID_t IDNum;
  char *menuName;
  /*指向父菜单*/
  struct __menu *Parent;
  /*指向子菜单*/
  struct __menu *subMenuList;
  /*上一个兄弟菜单*/
  struct __menu *PreBrother;
  /*指向下一个兄弟菜单*/
  struct __menu *NextBrother;
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
extern menu_t* Menu_NewMenu(char *menuString, menu_t* Parent,
                     actFuncAndArg_t *EnteractList,
                      actFuncAndArg_t *BackActList,
                      actFuncAndArg_t *Updata);

/**********************************************************************************
                                   Demo
**********************************************************************************
void *updata(void *unused)
{
    printf("hello world");
    return NULL;
}

actFuncAndArg_t Updata =
{
   .Func = updata,
   .Arg = NULL,
};

int main(void)
{
   menu_t *menu =   Menu_NewMenu("Root",NULL,NULL,NULL,&Updata);
   menu_t *SubMenu = Menu_NewMenu("SubMenu",menu,NULL,NULL,&Updata);
   Menu_Enter();
}
***********************************************************************************
                                Demo End
**********************************************************************************/

#endif