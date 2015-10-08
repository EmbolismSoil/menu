#include "menu.h"
#include <string.h>
#include <stdlib.h>

/*头指针，指向当前所在菜单条目*/
static menu_t *head = NULL;
static menu_t *lastHead = NULL;
static int  cursorON = 1;
static Menu_coord curcorCoord = {.line = 0,.row = 0};

void Menu_cursorON(void)
{
   cursorON = 1;
}
void Menu_cursorOFF(void)
{
   cursorON = 0;
}

void Menu_cursorUpdata(void)
{
        if (  cursorON&& head->coord.row > 0){
            head->Updata(curcorCoord.line,0,Align,Dis," ");
            curcorCoord.line = head->coord.line;
            head->Updata(head->coord.line,0,Align,Dis,"*");
        }
}
/*********************************************************************************************************
*                                          Menu_Updata
*
* Description : 菜单刷新函数，刷新时输出菜单缓冲区内容或者子菜单列表
*
* Argument(s) : none
*
* Return(s)   : Menu_coord 此时光标应该所在的坐标
*
* Caller(s)   : Menu_Back Menu_Enter
*
* Note(s)     : none.
*********************************************************************************************************/
menu_t* Menu_Updata(void)
{
      printBuffer_t *tmpBuffer = head->Buffer;

  if (tmpBuffer){/*如果输出缓不为空，将其输出*/
      if (head->Updata){
          head->Updata(0,0,Align,CLEAR,NULL);/*clear*/
          while(tmpBuffer)
          {
               if (tmpBuffer->opt == Dis)
                   head->Updata(tmpBuffer->x,tmpBuffer->y,
                                   Align,Dis,tmpBuffer->printString);
               tmpBuffer = tmpBuffer->Next;
          }
      }
  }else{/*如果输出缓冲为空，那就输出子菜单的名字*/
      menu_t *tmpMenu = head->subMenuList;
      if (tmpMenu){
          if (head->Updata){
              head->Updata(0,0,Align,CLEAR,NULL);/*clear*/
              while(tmpMenu){
                  head->Updata(tmpMenu->coord.line,tmpMenu->coord.row,Align,Dis,tmpMenu->menuName);
                  tmpMenu = tmpMenu->NextBrother;
              }
          }
      }
  }
         return head;

}

/*********************************************************************************************************
*                                          Menu_RefreshBuffer
*
* Description : 缓冲区刷新函数，如果用户改变了缓冲内容，应该适时调用这个函数进行刷新
*
* Argument(s) : none
*
* Return(s)   :
*
* Caller(s)   :
*
* Note(s)     : none.
*********************************************************************************************************/
menu_t* Menu_RefreshBuffer(void)
{
  char clearBit = 1;
  printBuffer_t *tmpBuffer = head->Buffer;
  if (tmpBuffer)/*如果输出缓不为空，将其输出*/
      if (head->Updata){
          while(tmpBuffer)
          {
              if (tmpBuffer->opt == Dis){
                  if (clearBit){
                       head->Updata(0,0,Align,CLEAR,NULL);/*clear*/
                       clearBit = 0;
                   }
                   head->Updata(tmpBuffer->x,tmpBuffer->y,
                                   Align,Dis,tmpBuffer->printString);
              }
               tmpBuffer = tmpBuffer->Next;
          }
      }

     return head;
}

/*********************************************************************************************************
*                                          Menu_CurMenu
*
* Description : 获取当前所在菜单条目
*
* Argument(s) : void
*
* Return(s)   : none.
*
* Caller(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************/
 menu_t *Menu_CurMenu(void)
{
    return head;
}


/*********************************************************************************************************
*                                          Menu_Enter
*
* Description : 进入当前菜单条目，并且将head指向当前菜单条目的第一个子菜单
*
* Argument(s) : void
*
* Return(s)   : head
*
* Caller(s)   : none
*
* Note(s)     : 在进入菜单之后，首先会遍历执行进入动作，如果当前菜单条目输出缓冲就输出缓冲内容，否则输出子菜单名字
*********************************************************************************************************/
menu_t*  Menu_Enter(void)
{
  actFuncAndArg_t *tmpAct = head->EnterActList;
   while(tmpAct){/*首先執行進入動作*/
          tmpAct->Func(head->EnterActList->Arg);
          tmpAct = tmpAct->NextFunc;
      }

   Menu_Updata();
  if (head->subMenuList){
      lastHead = head;
      head = head->subMenuList;
  }

  Menu_cursorUpdata();
  return head;
}

/*********************************************************************************************************
*                                          Menu_Back
*
* Description : 退出当前菜单条目，并且将head指向进入当前菜单之前的菜单条目
*
* Argument(s) : void
*
* Return(s)   : head
*
* Caller(s)   : none
*
* Note(s)     : 在进入菜单之后，首先会遍历执行进入动作，如果当前菜单条目输出缓冲就输出缓冲内容，否则输出子菜单名字
*********************************************************************************************************/
menu_t* Menu_Back(void)
{

   if (!lastHead || !lastHead->Parent) return NULL;

   actFuncAndArg_t *tmpAct = head->BackActList;
   while(tmpAct){/*首先执行退出动作*/
          tmpAct->Func(head->BackActList->Arg);
          tmpAct = tmpAct->NextFunc;
      }

   head = lastHead->Parent;
    Menu_Updata();
   head = lastHead;
   lastHead = head->Parent;

  Menu_cursorUpdata();
  return head;

}


/*********************************************************************************************************
*                                          Menu_Next
*
* Description : head指针指向当前菜单的下一个兄弟菜单
*
* Argument(s) : void
*
* Return(s)   : head
*
* Caller(s)   : none
*
* Note(s)     : none
*********************************************************************************************************/
menu_t* Menu_Next(void)
{
  if (!head->NextBrother){
       if (!head->Parent)
           return NULL;
       else
           head = head->Parent->subMenuList;
  }
  else
      head = head->NextBrother;

  Menu_cursorUpdata();
  return head;
}

/*********************************************************************************************************
*                                          Menu_Pre
*
* Description : 将head指针指向当前菜单的上一个兄弟菜单
*
* Argument(s) : void
*
* Return(s)   : head
*
* Caller(s)   : none
*
* Note(s)     : None
*********************************************************************************************************/
 menu_t* Menu_Pre(void)
{
    if (!head->PreBrother){
        while(head->NextBrother)
            head = head->NextBrother;
    }
    else
        head = head->PreBrother;

    Menu_cursorUpdata();
     return head;
}

/*********************************************************************************************************
*                                          Menu_Remove
*
* Description : 移除一个菜单
*
* Argument(s) : srcMenu           需要被移除的菜单
*
* Return(s)   : head
*
* Caller(s)   : none
*
* Note(s)     : None
*********************************************************************************************************/
menu_t *Menu_Remove(menu_t *srcMenu)
{
    if (!srcMenu) return NULL;

    if (srcMenu->Parent){
        (srcMenu->Parent->subMenuNum)--;
        srcMenu->Parent = NULL;
    }

    if (srcMenu->NextBrother && srcMenu->PreBrother){
        srcMenu->NextBrother->PreBrother = srcMenu->PreBrother;
        srcMenu->PreBrother->NextBrother = srcMenu->NextBrother;
    }else if(srcMenu->NextBrother && !srcMenu->NextBrother){
        srcMenu->NextBrother->PreBrother = NULL;
    }else if (!srcMenu->NextBrother && srcMenu->PreBrother){
        srcMenu->PreBrother->NextBrother = NULL;
    };

    return srcMenu;
}


/*********************************************************************************************************
*                                          Menu_AddBrotherAfter
*
* Description : 为一个菜单添加一个兄弟菜单，这个兄弟菜单将会被添加到该菜单的后面
*
* Argument(s) : dstMenu   需要添加兄弟菜单的菜单。
                srcMenu   被添加的兄弟菜单。
*
* Return(s)   : srcMenu
*
* Caller(s)   : none
*
* Note(s)     : None
*********************************************************************************************************/
 menu_t* Menu_AddBrotherAfter(menu_t *dstMenu, menu_t *srcMenu)
{
    menu_t *tmpMenu;
    if ((!dstMenu) || (!srcMenu))
        return NULL;

    Menu_Remove(srcMenu);

    if (!dstMenu->NextBrother){
        dstMenu->NextBrother = srcMenu;
        srcMenu->PreBrother = dstMenu;
    }
    else{
        tmpMenu = dstMenu->NextBrother;
        dstMenu->NextBrother = srcMenu;
        srcMenu->PreBrother = dstMenu;
        dstMenu->NextBrother = tmpMenu;
        tmpMenu->PreBrother = dstMenu;
    }

    if (dstMenu->Parent){
        srcMenu->Parent = dstMenu->Parent;
        (srcMenu->Parent->subMenuNum)++;
    }

    return srcMenu;
}

/*********************************************************************************************************
*                                          Menu_AddBrotherBefore
*
* Description : 为一个菜单添加一个兄弟菜单，这个兄弟菜单将会被添加到该菜单的前面
*
* Argument(s) : dstMenu   需要添加兄弟菜单的菜单。
                srcMenu   被添加的兄弟菜单。
*
* Return(s)   : srcMenu
*
* Caller(s)   : none
*
* Note(s)     : None
*********************************************************************************************************/
 menu_t* Menu_AddBrotherBefore(menu_t *dstMenu, menu_t *srcMenu)
{
    menu_t *tmpMenu;

    if ((!dstMenu) || (!srcMenu))
        return NULL;

     Menu_Remove(srcMenu);

    if (!dstMenu->PreBrother){
        dstMenu->PreBrother = srcMenu;
    }
    else{
        tmpMenu = dstMenu->PreBrother;
        dstMenu->PreBrother = srcMenu;
        srcMenu->NextBrother = dstMenu;
        dstMenu->PreBrother = tmpMenu;
        tmpMenu->NextBrother = srcMenu;
    }

    if (dstMenu->Parent){
        srcMenu->Parent = dstMenu->Parent;
        (srcMenu->Parent->subMenuNum)++;
    }
    return srcMenu;
}


/*********************************************************************************************************
*                                          Menu_HeadInit
*
* Description : 将head指针初始化
*
* Argument(s) : root   head指针会被初始化为这个root指针
*
* Return(s)   : head
*
* Caller(s)   : none
*
* Note(s)     : None
*********************************************************************************************************/
 menu_t* Menu_HeadInit(menu_t* root)
{
   if (!root) return NULL;

   head = root;
   return head;
}

/*********************************************************************************************************
*                                          Menu_AddSubMenuTail
*
* Description : 将一个自菜单添加到菜单的自菜单列表尾部
*
* Argument(s) : Parent   需要子菜单的父菜单
*               Sub      被添加到尾部的子菜单
* Return(s)   : Sub
*
* Caller(s)   : none
*
* Note(s)     : None
*********************************************************************************************************/
menu_t* Menu_AddSubMenuTail(menu_t* Parent, menu_t* Sub)
{
    if (!Sub) return NULL;

    Menu_Remove(Sub);

    menu_t *tmpMenu = Parent->subMenuList;

    if (tmpMenu)
        while(tmpMenu->NextBrother)
           tmpMenu = tmpMenu->NextBrother;

    if (tmpMenu)
       Menu_AddBrotherAfter(tmpMenu,Sub);
    else {
       Parent->subMenuList = Sub;
       Parent->subMenuNum++;
    }

    Sub->Parent = Parent;
    return Sub;
}

/*********************************************************************************************************
*                                          Menu_AddSubMenuHead
*
* Description : 将一个自菜单添加到菜单的自菜单列表首部
*
* Argument(s) : Parent   需要子菜单的父菜单
*               Sub      被添加到首部的子菜单
* Return(s)   : Sub
*
* Caller(s)   : none
*
* Note(s)     : None
*********************************************************************************************************/
menu_t* Menu_AddSubMenuHead(menu_t* Parent, menu_t* Sub)
{
    if (!Sub) return NULL;

    Menu_Remove(Sub);

    if (!Parent->subMenuList){
        Parent->subMenuList = Sub;
        Parent->subMenuNum++;
        return Sub;
    }

    Parent->subMenuList->PreBrother = Sub;
    Sub->NextBrother = Parent->subMenuList;
    return Sub;
}

/*********************************************************************************************************
*                                          Menu_NewMenu
*
* Description : 新建一个菜单条目
*
* Argument(s) : menuString   菜单的名字
*               ID           菜单的ID，用于标识菜单身份。
                Parent       该菜单将会被添加到Parent底下。如果Parent为NULL，该菜单就会被添加到head下
                EnterActList 进入该菜单时被调用的函数链
                BackActList  退出该菜单时被调用的函数链
                Updata       用于刷新菜单界面的函数，与驱动相关
                bufferList   菜单的输出缓冲链表，如果这个缓冲列表为空，刷新菜单时就输出子菜单名字

* Return(s)   : Sub
*
* Caller(s)   : none
*
* Note(s)     : Updata 函数需要实现对齐显示功能，清屏选项功能。以下是一个例子
                 void Updata(unsigned char x, unsigned char y,uchar align,
                                        Menu_Opt_t opt, char *printString)
               {
                    if (opt == CLEAR)
                        LCD_CLEAR();
                    else if (opt == Dis)
                        User_printf(x,y,align,printString);
                };
*********************************************************************************************************/
menu_t* Menu_NewMenu(char *menuString, char line,char row,ID_t ID, menu_t* Parent,
                     actFuncAndArg_t *EnterActList,
                      actFuncAndArg_t *BackActList,
                      updataFunc_t Updata,printBuffer_t *bufferList)
{
    menu_t *menu = (menu_t*)malloc(sizeof(menu_t));

    if (menu){
        memset(menu,0,sizeof(menu_t));
        menu->NextBrother = NULL;
        menu->PreBrother = NULL;
        menu->Parent = NULL;
        menu->subMenuList = NULL;
        menu->IDNum = ID;
        menu->pri = NULL;
        menu->Updata = NULL;
        menu->subMenuNum = 0;
        menu->Focus = focus;

        if (line >= 0)
            menu->coord.line = line;
        if (row >= 0)
            menu->coord.row = row;

        if (!menuString)
            return NULL;
        else{
            menu->menuName = menuString;
            if (bufferList)
                menu->Buffer = bufferList;
            if (EnterActList)
              menu->EnterActList = EnterActList;
            if (BackActList)
              menu->BackActList = BackActList;
            if (!Updata)
              return NULL;
            else menu->Updata = Updata;
            if (Parent)
              Menu_AddSubMenuTail(Parent,menu);
            else if (head)
              Menu_AddSubMenuTail(head,menu);
            else
              head = menu;
        }

        return menu;
    }

    return NULL;
}

