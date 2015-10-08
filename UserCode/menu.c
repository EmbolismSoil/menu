#include "menu.h"
#include <string.h>
#include <stdlib.h>

/*ͷָ�룬ָ��ǰ���ڲ˵���Ŀ*/
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
* Description : �˵�ˢ�º�����ˢ��ʱ����˵����������ݻ����Ӳ˵��б�
*
* Argument(s) : none
*
* Return(s)   : Menu_coord ��ʱ���Ӧ�����ڵ�����
*
* Caller(s)   : Menu_Back Menu_Enter
*
* Note(s)     : none.
*********************************************************************************************************/
menu_t* Menu_Updata(void)
{
      printBuffer_t *tmpBuffer = head->Buffer;

  if (tmpBuffer){/*����������Ϊ�գ��������*/
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
  }else{/*����������Ϊ�գ��Ǿ�����Ӳ˵�������*/
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
* Description : ������ˢ�º���������û��ı��˻������ݣ�Ӧ����ʱ���������������ˢ��
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
  if (tmpBuffer)/*����������Ϊ�գ��������*/
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
* Description : ��ȡ��ǰ���ڲ˵���Ŀ
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
* Description : ���뵱ǰ�˵���Ŀ�����ҽ�headָ��ǰ�˵���Ŀ�ĵ�һ���Ӳ˵�
*
* Argument(s) : void
*
* Return(s)   : head
*
* Caller(s)   : none
*
* Note(s)     : �ڽ���˵�֮�����Ȼ����ִ�н��붯���������ǰ�˵���Ŀ������������������ݣ���������Ӳ˵�����
*********************************************************************************************************/
menu_t*  Menu_Enter(void)
{
  actFuncAndArg_t *tmpAct = head->EnterActList;
   while(tmpAct){/*���Ȉ����M�����*/
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
* Description : �˳���ǰ�˵���Ŀ�����ҽ�headָ����뵱ǰ�˵�֮ǰ�Ĳ˵���Ŀ
*
* Argument(s) : void
*
* Return(s)   : head
*
* Caller(s)   : none
*
* Note(s)     : �ڽ���˵�֮�����Ȼ����ִ�н��붯���������ǰ�˵���Ŀ������������������ݣ���������Ӳ˵�����
*********************************************************************************************************/
menu_t* Menu_Back(void)
{

   if (!lastHead || !lastHead->Parent) return NULL;

   actFuncAndArg_t *tmpAct = head->BackActList;
   while(tmpAct){/*����ִ���˳�����*/
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
* Description : headָ��ָ��ǰ�˵�����һ���ֵܲ˵�
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
* Description : ��headָ��ָ��ǰ�˵�����һ���ֵܲ˵�
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
* Description : �Ƴ�һ���˵�
*
* Argument(s) : srcMenu           ��Ҫ���Ƴ��Ĳ˵�
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
* Description : Ϊһ���˵����һ���ֵܲ˵�������ֵܲ˵����ᱻ��ӵ��ò˵��ĺ���
*
* Argument(s) : dstMenu   ��Ҫ����ֵܲ˵��Ĳ˵���
                srcMenu   ����ӵ��ֵܲ˵���
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
* Description : Ϊһ���˵����һ���ֵܲ˵�������ֵܲ˵����ᱻ��ӵ��ò˵���ǰ��
*
* Argument(s) : dstMenu   ��Ҫ����ֵܲ˵��Ĳ˵���
                srcMenu   ����ӵ��ֵܲ˵���
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
* Description : ��headָ���ʼ��
*
* Argument(s) : root   headָ��ᱻ��ʼ��Ϊ���rootָ��
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
* Description : ��һ���Բ˵���ӵ��˵����Բ˵��б�β��
*
* Argument(s) : Parent   ��Ҫ�Ӳ˵��ĸ��˵�
*               Sub      ����ӵ�β�����Ӳ˵�
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
* Description : ��һ���Բ˵���ӵ��˵����Բ˵��б��ײ�
*
* Argument(s) : Parent   ��Ҫ�Ӳ˵��ĸ��˵�
*               Sub      ����ӵ��ײ����Ӳ˵�
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
* Description : �½�һ���˵���Ŀ
*
* Argument(s) : menuString   �˵�������
*               ID           �˵���ID�����ڱ�ʶ�˵���ݡ�
                Parent       �ò˵����ᱻ��ӵ�Parent���¡����ParentΪNULL���ò˵��ͻᱻ��ӵ�head��
                EnterActList ����ò˵�ʱ�����õĺ�����
                BackActList  �˳��ò˵�ʱ�����õĺ�����
                Updata       ����ˢ�²˵�����ĺ��������������
                bufferList   �˵�������������������������б�Ϊ�գ�ˢ�²˵�ʱ������Ӳ˵�����

* Return(s)   : Sub
*
* Caller(s)   : none
*
* Note(s)     : Updata ������Ҫʵ�ֶ�����ʾ���ܣ�����ѡ��ܡ�������һ������
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

