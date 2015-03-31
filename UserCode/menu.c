#include "menu.h"

/*头指针，指向当前所在菜单条目*/
static menu_t *head;

 menu_t *Menu_CurMenu(void)
{
    return head;
}

 menu_t* Menu_Enter(void)
{
  if (head->subMenuList){/*如果当前菜单下还有子菜单，那就调用updata来显示子菜单名字*/
      if (head->Updata)
          head->Updata->Func(head->Updata->Arg);
      head = head->subMenuList;
  }else{
      while(head->EnterActList){/*如果当前菜单下没有子菜单，那就执行进入菜单时的动作*/
          head->EnterActList->Func(head->EnterActList->Arg);
          head->EnterActList = head->EnterActList->NextFunc;
      }
  }
  
  return head;
}

 menu_t* Menu_Back(void)
{
  head = head->Parent;
  if (head->BackActList)
    while(head->BackActList)
        head->BackActList->Func(head->BackActList->Arg);
  if (head->Updata)
    head->Updata->Func(head->Updata->Arg);
  
  return head;
}


 menu_t* Menu_Next(void)
{
  if (!head->NextBrother)
    return NULL;
  else
      head = head->NextBrother;
    return head;
}

 menu_t* Menu_Pre(void)
{
    if (!head->PreBrother)
        return NULL;
    else
        head = head->PreBrother;
    return head;
}

menu_t *Menu_Del(menu_t *srcMenu)
{
    if (!srcMenu) return NULL;

    if (srcMenu->NextBrother && srcMenu->NextBrother){
        srcMenu->NextBrother->PreBrother = srcMenu->PreBrother;
        srcMenu->PreBrother->NextBrother = srcMenu->NextBrother;
    }else if(srcMenu->NextBrother && !srcMenu->NextBrother){
        srcMenu->NextBrother->PreBrother = NULL;
    }else if (!srcMenu->NextBrother && srcMenu->PreBrother){
        srcMenu->PreBrother->NextBrother = NULL;
    }else  return srcMenu;
}

 menu_t* Menu_AddBrotherAfter(menu_t *dstMenu, menu_t *srcMenu)
{
    menu_t *tmpMenu;
    if ((!dstMenu) || (!srcMenu))
        return NULL;
    
    Menu_Del(srcMenu);
    
    if (!dstMenu->NextBrother)
        dstMenu->NextBrother = srcMenu;
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
    while(tmpMenu->NextBrother)
      tmpMenu = tmpMenu->NextBrother;
    
    Sub->Parent = Parent;
    return Menu_AddBrotherAfter(tmpMenu,Sub);
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