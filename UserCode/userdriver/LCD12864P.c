#include "LCD12864P.h"
#include "stm32f4xx_hal_gpio.h"
/****************************************
              Your header
***************************************/
#include "User_GPIO.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdarg.h>


/**************************************
              Transplantation
**************************************/

/*
 *  RS = PA01 WR = PA05 E = PA07
 *
 */
#define cyCS_High() HAL_GPIO_WritePin (GPIOA, GPIO_PIN_1,GPIO_PIN_SET)
#define cyCS_Low() HAL_GPIO_WritePin (GPIOA, GPIO_PIN_1,GPIO_PIN_RESET)


#define cySID_High() HAL_GPIO_WritePin (GPIOA, GPIO_PIN_3,GPIO_PIN_SET)
#define cySID_Low()  HAL_GPIO_WritePin (GPIOA, GPIO_PIN_3,GPIO_PIN_RESET)

#define cyCLK_High() HAL_GPIO_WritePin (GPIOA, GPIO_PIN_7,GPIO_PIN_SET)
#define cyCLK_Low() HAL_GPIO_WritePin (GPIOA, GPIO_PIN_7,GPIO_PIN_RESET)



#define IO_Init()\
do{\
	User_GPIOInit(GPIOA,1,GPIO_MODE_OUTPUT_PP,FAST,PULLUP);\
	User_GPIOInit(GPIOA,3,GPIO_MODE_OUTPUT_PP,FAST,PULLUP);\
	User_GPIOInit(GPIOA,7,GPIO_MODE_OUTPUT_PP,FAST,PULLUP);\
}while(0)

/*******************************************
				Config in header
*******************************************/
#ifdef ucosii
#define delay_1ms() OSTimeDlyHMSM(0,0,0,1)
#define delay_nms(n) OSTimeDlyHMSM(0,0,0,n)
#else
#define delay_1ms() delay_1ms_pro()
#define delay_nms(n) delay_nms_pro(n)

static void delay_1ms_pro()
{
        uint i;
        for(i=0;i<22400;i++);
}

static void delay_nms_pro(uint n)
{
    uint i = 0;
    for(i = n;i > 0;i--)
        delay_1ms();
}
#endif

/***********************************************
				Encapsulate
***********************************************/
void Send(uchar type,uchar transdata);

static void lcd_pos(uchar x,uchar y)//��λ
{
         uchar pos;
         switch(x)
          {
            case 0:pos=0x80;break;
            case 1:pos=0x90;break;
            case 2:pos=0x88;break;
            case 3:pos=0x98;break;
          }
          pos += y;
          Send(0,pos);
}

void User_printf (uchar x, uchar y,uchar align, const char *fmt,...)
{
        va_list args;
	char  printbuffer[400];
        char *p = printbuffer;

         x %= 4;

        va_start (args, fmt);

        /* For this to work, printbuffer must be larger than
         * anything we ever want to print.
         */
        vsprintf(printbuffer, fmt, args);
        va_end (args);

//OS_ENTER_CRITICAL();

/* Print the string */
        lcd_pos(x,y);
        while(*p){
             Send(1,*(p++));
             if ( 16== p - printbuffer + y*2 ||
                  32== p - printbuffer + y*2 ||
                  48== p - printbuffer + y*2 ||
                  64== p - printbuffer + y*2)
                 if (align)  lcd_pos(++x,y);
                 else lcd_pos(++x,0);
        }
//OSTaskIdleHook();

}




void Init_Lcd(void)
{
        IO_Init();

        delay_nms(1);                 //��ʱ�ȴ�Һ����ɸ�λ
        Send(0,0x30);  /*��������:һ����8λ����,����ָ�*/
        delay_nms(1);
        Send(0,0x02);  /*DDRAM��ַ��λ*/
        delay_nms(1);
        Send(0,0x0c);  /*��ʾ�趨:����ʾ,����ʾ���,������ǰ��ʾλ��������*/
        delay_nms(1);
        Send(0,0x01);  /*��������DDRAM��λַ����������Ϊ��00H��*/
        delay_nms(1);
        Send(0,0x06);  /*�������ã����趨:��ʾ�ַ�/����������λ,DDRAM��ַ��1*/
        delay_nms(1);
}

static void Send(uchar type,uchar transdata)//0-д�������1-д����
{
        uchar firstbyte = 0xf8;
        uchar temp;
        uchar i,j = 3;

        if(type) firstbyte |= 0x02;

		cyCS_High();
		cyCLK_Low();

        while(j > 0)
        {
                if(j == 3) temp = firstbyte;
                else if(j == 2) temp = transdata&0xf0;
                else  temp = (transdata << 4) & 0xf0;

                for(i = 8;i > 0;i--)
                {
                        if(temp & 0x80)    cySID_High();
                        else    cySID_Low();

						cyCLK_High();
                        temp <<= 1;
						cyCLK_Low();
                }
                //�����ֽ�֮��һ��Ҫ���㹻����ʱ�������׳���ʱ������
                if(j == 3)        delay_nms(10);
                else               delay_nms(10);

                j--;
        }

		cySID_Low();
		cyCS_Low();

}


/*******************************************
�������ƣ�Draw_PM
��    �ܣ���������Ļ�ϻ�һ��ͼƬ
��    ����ptr--ָ�򱣴�ͼƬλ�õ�ָ��
����ֵ  ����
********************************************/
void Draw_PM(const uchar *ptr)
{
    uchar i,j,k;

	Send(0,0x34);        //����չָ�
	i = 0x80;
	for(j = 0;j < 32;j++)
	{
 	 	Send(0,i++);
 		Send(0,0x80);
  		for(k = 0;k < 16;k++)
  		{
  		    Send(1,*ptr++);
  		}
	}
	i = 0x80;
 	for(j = 0;j < 32;j++)
	{
 	    Send(0,i++);
  		Send(0,0x88);
  		for(k = 0;k < 16;k++)
  		{
   		    Send(1,*ptr++);
   		}
	}
    Send(0,0x36);		        //�򿪻�ͼ��ʾ
	Send(0,0x30);               //�ص�����ָ�
}
/*******************************************
�������ƣ�Draw_TX
��    �ܣ���Һ�������һ��16*16��ͼ��
��    ����Yaddr--Y��ַ��
          Xaddr--X��ַ
          dp--ָ�򱣴�ͼ�����ݵ�ָ��
����ֵ  ����
********************************************/
void Draw_TX(uchar Yaddr,uchar Xaddr,const uchar * dp)
{
    uchar j;
    uchar k = 0;

    Send(0,0x34);           //ʹ����չָ����رջ�ͼ��ʾ
    for(j = 0;j < 16;j++)
    {
        Send(0,Yaddr++);     //Y��ַ
        Send(0,Xaddr);       //X��ַ
        Send(1,dp[k++]);     //�������ֽڵ���ʾ����
        Send(1,dp[k++]);
    }
	Send(0,0x36);		    //�򿪻�ͼ��ʾ
	Send(0,0x30);		    //�ص�����ָ�ģʽ
}
