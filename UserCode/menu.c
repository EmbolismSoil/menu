#include "menu.h"
#include <string.h>
#include <stdlib.h>

/*头指针，指向当前所在菜单条目*/
static menu_t *head;
static menu_t *lastHead;


/*********************************************************************************************************
*                                          Menu_Updata
*
* Description : 菜单刷新函数，刷新时输出菜单缓冲区内容或者子菜单列表
*
* Argument(s) : none
*
* Return(s)   : none.
*
* Caller(s)   : Menu_Back Menu_Enter
*
* Note(s)     : none.
*********************************************************************************************************/
menu_t *Menu_Updata(void)
{
      printBuffer_t *tmpBuffer = head->Buffer;
    unsigned char cnt = 0;

  if (tmpBuffer){/*如果输出缓不为空，将其输出*/
      if (head->Updata){
          head->Updata(0,0,Align,CLEAR,NULL);/*clear*/
          while(tmpBuffer)
          {
               if (tmpBuffer->opt == Dis)
                   head->Updata(tmpBuffer->x,tmpBuffer->y,Align,Dis,tmpBuffer->printString);
               tmpBuffer = tmpBuffer->Next;
          }
      }
  }else{/*如果输出缓冲为空，那就输出子菜单的名字*/
      menu_t *tmpMenu = head->subMenuList;
      if (tmpMenu){
          if (head->Updata){
              head->Updata(0,0,Align,CLEAR,NULL);/*clear*/
              while(tmpMenu){
                  head->Updata(cnt,0,Align,Dis,tmpMenu->menuName);
                  tmpMenu = tmpMenu->NextBrother;
                  cnt++;
              }
          }
      }

  }

  return head;
}


 menu_t *Menu_CurMenu(void)
{
    return head;
}

 menu_t* Menu_Enter(void)
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

  return head;
}

 menu_t* Menu_Back(void)
{
   if (lastHead)
      head = lastHead->Parent;
   else return NULL;

   actFuncAndArg_t *tmpAct = head->BackActList;
   while(tmpAct){/*首先执行退出动作*/
          tmpAct->Func(head->BackActList->Arg);
          tmpAct = tmpAct->NextFunc;
      }

   Menu_Updata();
  head = lastHead;
  return head;
}


 menu_t* Menu_Next(void)
{
  if (!head->NextBrother)
    return NULL;
  else{
      lastHead = head;
      head = head->NextBrother;
  }
      return head;
}

 menu_t* Menu_Pre(void)
{
    if (!head->PreBrother)
        return NULL;
    else{
        head = head->PreBrother;
        lastHead = head;
    }
     return head;
}

menu_t *Menu_Del(menu_t *srcMenu)
{
    if (!srcMenu) return NULL;

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

 menu_t* Menu_AddBrotherAfter(menu_t *dstMenu, menu_t *srcMenu)
{
    menu_t *tmpMenu;
    if ((!dstMenu) || (!srcMenu))
        return NULL;

    Menu_Del(srcMenu);

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

    return srcMenu;
}

 menu_t* Menu_AddBrotherBefore(menu_t *dstMenu, menu_t *srcMenu)
{
    menu_t *tmpMenu;

    if ((!dstMenu) || (!srcMenu))
        return NULL;

     Menu_Del(srcMenu);

    if (!dstMenu->PreBrother)
        dstMenu->PreBrother = srcMenu;
    else{
        tmpMenu = dstMenu->PreBrother;
        dstMenu->PreBrother = srcMenu;
        srcMenu->NextBrother = dstMenu;
        dstMenu->PreBrother = tmpMenu;
        tmpMenu->NextBrother = srcMenu;
    }

    return srcMenu;
}

 menu_t* Menu_HeadInit(menu_t* root)
{
   if (!root) return NULL;

   head = root;
   return head;
}

menu_t* Menu_AddSubMenuTail(menu_t* Parent, menu_t* Sub)
{
    if (!Sub) return NULL;

    Menu_Del(Sub);

    menu_t *tmpMenu = Parent->subMenuList;

    if (tmpMenu)
        while(tmpMenu->NextBrother)
           tmpMenu = tmpMenu->NextBrother;

    if (tmpMenu)
       Menu_AddBrotherAfter(tmpMenu,Sub);
    else Parent->subMenuList = Sub;

    Sub->Parent = Parent;
    return Sub;
}

menu_t* Menu_AddSubMenuHead(menu_t* Parent, menu_t* Sub)
{
    if (!Sub) return NULL;

    Menu_Del(Sub);

    if (!Parent->subMenuList){
        Parent->subMenuList = Sub;
        return Sub;
    }

    Parent->subMenuList->PreBrother = Sub;
    Sub->NextBrother = Parent->subMenuList;
    return Sub;
}

menu_t* Menu_NewMenu(char *menuString, ID_t ID, menu_t* Parent,
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