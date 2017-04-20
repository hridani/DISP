/*                                P I D . C P P                                                                           */
/*Описание на PID чипа и функции-команди.Описание на осите    .                             */
/* Списък от PID чипове и оси                                                                                    */
/******************************************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <dos.h>       
#include <stdio.h>
#include <new.h>
#include <malloc.h>
//#include "c:\msvc\dani\string.h"
#include "..\include\pid.h"
/******************************************************************************************************/
/*PIDchip::PIDchip:Конструктор на клас PIDchip                                                         */
/*Входни данни:Номер на PIDchip,адрес,указател към прешестваника                       */
/*Изходни данни:Няма                                                                                               */
/******************************************************************************************************/
 PIDchip::PIDchip(int nm,unsigned adr,PIDchip *bc)
 {     
     number=nm;//номер
     adresc=adr;//адрес на контролната дума 
     adresd=adr+1;//адрес на данните
     kp=40;
     ki=5;
     kd=10000;
     il=1000;     
     ds=4;
     filter_control_word=0xf;
     next=NULL;
     back=bc;
  }

/******************************************************************************************************/
/*PIDchip::PIDchip:Конструктор на клас PIDchip                                                         */
/*Входни данни:Адрес                                                                                              */
/*Изходни данни:Няма                                                                                               */
/******************************************************************************************************/
PIDchip::PIDchip(unsigned adr)
 {     
     number=0;//номер
     adresc=adr;//адрес на контролната дума
     adresd=adr+1;//адрес на данните
     kp=0;//коеф.на пропорционалност
     ki=0;//интегрален коеф.
     kd=0;//дуференц. коеф
     il=0;     //сума за интегриране
     ds=0;
     filter_control_word=0x0;//филтърна контроилна дума
     //динамични коеф.
     kpdin=0;
     kidin=0;
     kddin=0;
     ildin=0;     
     dsdin=0;
     filter_control_worddin=0x0;
     next=NULL;//наследник
     back=NULL;//предшественик
  }
 
/******************************************************************************************************/
/*PIDchip::PIDchip:Конструктор на клас PIDchip                                                         */
/*Входни данни:Номер ,адрес,коефициенти-kp,ki,kd,il,ds, pred[estwenik                     */
/*Изходни данни:Няма                                                                                               */
/******************************************************************************************************/
 PIDchip::PIDchip(int nm,unsigned adr,int kkp,int kki,int kkd,int kil,int f_c_w,PIDchip *bc)
 {     
     number=nm;//номер
     adresc=adr;//адрес на командата
     adresd=adr +1;//адрес на данните
     kp=kkp;//kp
     ki=kki;//ki
     kd=kkd;//kd
     il=kil;//il
     ds=(f_c_w>>4) +1;//ds
     filter_control_word=f_c_w;//филтърна контролна дума
     //динамични коефиж=циенти
     kpdin=0;
     kidin=0;
     kddin=0;
     ildin=0;     
     dsdin=0;
     filter_control_worddin=0x0;
     next=NULL;//наследник
     back=bc;//предшественик
  }

/*************************************************************************************************/
/* PIDchip::PIDIniFil:Инициализация на коефициентите на филтъра -статични        */
/* Входни данни:Коефициенти-kd,ds,kp,ki,il                                                         */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void PIDchip::PIDIniFil(int kkd,int kds,int kkp,int kki,int kil)
{
  kd=kkd;
  ds=kds;
  kp=kkp;
  il=kil;
  ki=kki;
  filter_control_word=((ds -1)<<8) + 0xf;//филтърна контролна дума
  //динамични коеф.
     kpdin=0;
     kidin=0;
     kddin=0;
     ildin=0;     
     dsdin=0;
     filter_control_worddin=0x0;//филтърна контролна дума

}

/*************************************************************************************************/
/* PIDchip::PIDIniFil:Инициализация на коефициентите на филтъра -динамични     */
/* Входни данни:Коефициенти-kd,ds,kp,ki,il                                                         */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void PIDchip::PIDIniFilDin(int kkd,int kds,int kkp,int kki,int kil)
{
  kddin=kkd;
  dsdin=kds;
  kpdin=kkp;
  ildin=kil;
  kidin=kki;
  filter_control_worddin=((dsdin -1)<<8) + 0xf;//филтърна контролна дума
}

/*************************************************************************************************/
/* Axis::IniTr:Въвеждане на скорост и ускорение по оста.Инициализация на данни*/
/* Входни данни:Скорост и ускорение                                                         */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void Axis::IniTr(double vl,double al)
{
   vel=(unsigned long)(((vl*kfd*kv *341)/(1000000 *60)) * 65536);//преобразуване по изискване на PID чипа
   acl=(unsigned long)(al * kfd *kv *0.000341 *0.000341 * 65536);//преобразуване по изискване на PID чипа
}

/*************************************************************************************************/
/* Axis::IniTr:Въвеждане на скорост  по оста                                                      */
/* Входни данни:Скорост                                                                  */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
unsigned long Axis::Newvel(double vl)   
{             
   unsigned long newvel;
   newvel=(unsigned long)(((vl*kfd*kv *341)/(1000000 *60)) * 65536); //преобразуване по изискване на PID чипа
   return newvel;
}

/*************************************************************************************************/
/* Axis::IniTr:Зарежда въвежданите на скорост и ускорение по оста  в PID чипа   */
/* Входни данни:Няма                                                                                         */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void Axis::IniTr(void)
{
   Ltrj(0x28,acl,vel);//запис в буфери на PID чипа
}
   
/*************************************************************************************************/
/* PIDchip::INI:Инициализация на PID чипа                                                 */
/* Входни данни:Няма                          l                                                         */
/* изходни данни:0-успешно инициализиране ,-1 -не е успешно инициализирането      */
/**************************************************************************************************/
int  PIDchip::INI(void)
{
  int ad=adresc;
  unsigned val=0xffff;
  unsigned val1=0;   
  int k=0;
    __asm
  {  
    cli
e1:mov al,RESET//команда RESET
     mov dx,ad
     out dx,al
     
     in al,40h
     mov ah,al
     nop
     in al,40h
     xchg ah,al
     mov bx,ax
      
et3:  in al,40h
     mov ah,al
     nop
     in al,40h
     xchg ah,al
     cmp ax,bx
     jna et1
     mov val,0xffff
     sub val,ax
     mov ax,val
     add ax,bx
     jmp et2
et1: mov val1,bx
      sub  val1,ax
      mov ax,val1

et2:cmp ax,1785//изчакване
      jna et3
      mov cx,0
et4:cmp cx,8000 
      mov k,-1
       ja et8 
      inc cx
     in al,dx //тест - дали има такъв адрес и дали се възприемат командите
     test al,1
        jne et4 
        mov k,0
     cmp al,0xc4
     je et5
     cmp al,0x84
     jne e1
et5:mov al,RSTI
     out dx,al
ett2:  in al,dx
     test al,1
     jne ett2
     inc dx
     mov al,0
     out dx,al
     mov al,00
     out dx,al
     dec dx
ett4:  in al,dx
     test al,1
     jne ett4   
    cmp al,0xc0
     je e6
     cmp al,0x80
     jne e1
e6:mov al,PORT12
     out dx,al
e4:in al,dx
     test al,1
     jne e4
     jmp et8
     mov k,-1
et8:   sti
 }
 return k;
}    

/*************************************************************************************************/
/* PIDchip::LfiI:Зареждане на филтърните коефициенти от клас PIDchip            */
/* Входни данни:Няма                          l                                                         */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void PIDchip::Lfil(void)
{    
   unsigned ad=adresc;
   int fcw=filter_control_word;
   int  kpp=kp;  
   int kii=ki;
   int kdd=kd;
   int ill=il;
   __asm
   { 
     mov dx,ad
e11:in al,dx
     test al,1
     jne e11

     mov al,LFIL//команда
     out dx,al
     
et1:in al,dx
     test al,1//тест за готовност за запис на данни
     jne et1
     
     inc dx 
    mov ax,fcw//запис на ф.командна дума
    xchg ah,al
    out dx,al
    xchg ah,al
    out dx,al
    mov bl,al
    test bl,8
    je et2
    dec dx
et11:in al,dx
     test al,1
     jne et11
     inc dx
     mov ax,kpp
     xchg ah,al
     out dx,al
     xchg ah,al
     out dx,al
et2:test bl,4
      je et3
      dec dx
et12:in al,dx
     test al,1
     jne et12
     inc dx 
     mov ax,kii
     xchg ah,al
     out dx,al
     xchg ah,al
     out dx,al

et3: test bl,2
       je et4   
       dec dx
et13:in al,dx
     test al,1
     jne et13
       inc dx
       mov ax,kdd
       xchg ah,al
       out dx,al
       xchg ah,al
       out dx,al

et4:test bl,1
      je et5
      dec dx
et14:in al,dx
     test al,1
     jne et14
      inc dx
      mov ax,ill
      xchg ah,al
      out dx,al
      xchg ah,al
      out dx,al
      dec dx
et5:in al,dx
     test al,1
     jne et5          
   }
}      

/*************************************************************************************************/
/* PIDchip::LfiIDin:Зареждане на филтърните коефициенти от клас PIDchip-динамични   */
/* Входни данни:Няма                          l                                                         */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void PIDchip::LfilDin(void)
{    
   unsigned ad=adresc;
   int fcw=filter_control_worddin;
   int  kpp=kpdin;  
   int kii=kidin;
   int kdd=kddin;
   int ill=ildin;
   __asm
   { 
     mov dx,ad
e11:in al,dx
     test al,1
     jne e11

     mov al,LFIL
     out dx,al
     
et1:in al,dx
     test al,1
     jne et1
     
     inc dx 
    mov ax,fcw
    xchg ah,al
    out dx,al
    xchg ah,al
    out dx,al
    mov bl,al
    test bl,8
    je et2
    dec dx
et11:in al,dx
     test al,1
     jne et11
     inc dx
     mov ax,kpp
     xchg ah,al
     out dx,al
     xchg ah,al
     out dx,al
et2:test bl,4
      je et3
      dec dx
et12:in al,dx
     test al,1
     jne et12
     inc dx 
     mov ax,kii
     xchg ah,al
     out dx,al
     xchg ah,al
     out dx,al

et3: test bl,2
       je et4   
       dec dx
et13:in al,dx
     test al,1
     jne et13
       inc dx
       mov ax,kdd
       xchg ah,al
       out dx,al
       xchg ah,al
       out dx,al

et4:test bl,1
      je et5
      dec dx
et14:in al,dx
     test al,1
     jne et14
      inc dx
      mov ax,ill
      xchg ah,al
      out dx,al
      xchg ah,al
      out dx,al
      dec dx
et5:in al,dx
     test al,1
     jne et5          
   }
}      

/*************************************************************************************************/
/* PIDchip::LfiI:Зареждане на филтърните коефициенти от клас PIDchip            */
/* Входни данни:Филтърна командна дума,параметър,който се зарежда            */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void PIDchip::Lfil(unsigned f_c_w,unsigned par)
{    
   unsigned ad=adresc;
   unsigned fcw=f_c_w;
   unsigned  pr=par;  
   __asm
   { 
     mov dx,ad
e11:in al,dx
     test al,1
     jne e11

     mov al,LFIL
     out dx,al
     
et1:in al,dx
     test al,1
     jne et1
     
     inc dx 
    mov ax,fcw
    xchg ah,al
    out dx,al
    xchg ah,al
    out dx,al
    dec dx
et11:in al,dx
     test al,1
     jne et11
     inc dx
     mov ax,par
     xchg ah,al
     out dx,al
     xchg ah,al
     out dx,al
      dec dx
et12:in al,dx
     test al,1
     jne et12
   }
}      

/*************************************************************************************************/
/* PIDchip::Udf:Актуализация на филтърните коеф.                                            */
/* Входни данни:Няма                                                                                        */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void PIDchip::Udf(void)
{
    unsigned ad=adresc; 
   __asm
   {
     mov dx,ad
    et1:in al,dx
     test al,1
     jne et1
    
     mov al,UDF
     out dx,al
     
et6:in al,dx
     test al,1
     jne et6
    }  
}
  
/*************************************************************************************************/
/* PIDchip::Rdrv:Чете реалната скорост               .                                            */
/* Входни данни:Няма                                                                                        */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
int  PIDchip::Rdrv()
{    
   int pos=0;            
   unsigned ad=adresc;//адрес на командата
   __asm
   {
      mov dx,ad
e1:in al,dx
     test al,1//тест за готовност
     jne e1

     mov al,RDRV//команда
     out dx,al               
et1:in al,dx
     test al,1
     jne et1
     
     inc dx
     in al,dx
     mov ah,al
     in al,dx
     mov pos,ax
     dec dx
et2:in al,dx
     test al,1
     jne et2
  }
    return pos;
}                           

/*************************************************************************************************/
/* PIDchip::Rdrp:Чете реалната позиция               .                                            */
/* Входни данни:Няма                                                                                        */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
long  PIDchip::Rdrp()
{    
  long pos=0;    
  byte *ps=(byte *)&pos;   //указател за запис на реалната позиция
   unsigned ad=adresc;
   __asm
   {                     
      push ds
      lds di,ps
      add di,2
      mov dx,ad 
et3: in al,dx
      test al,1
      jne et3
      mov al,RDRP//команда
      out dx,al               
et1:in al,dx
     test al,1
     jne et1
     inc dx
     in al,dx
     mov ah,al
     in al,dx
     mov ds:[di],ax
     dec dx
et2:in al,dx
     test al,1
     jne et2
     inc dx
     in al,dx
     mov ah,al
     in al,dx
     mov word ptr pos,ax
     dec dx
     dec dx
     pop ds 
  }
    return pos;
} 
  
/*************************************************************************************************/
/* PIDchip::Rddp:Чете желаната позиция               .                                            */
/* Входни данни:Няма                                                                                        */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
long  PIDchip::Rddp()
{    
  long pos=0;    
  byte *ps=(byte *)&pos;
   unsigned ad=adresc;
   __asm
   {                     
      push ds
      lds di,ps
      add di,2
      mov dx,ad
et3:in al,dx
     test al,1
     jne et3
     mov al,RDDP
     out dx,al               
et1:in al,dx
     test al,1
     jne et1
     
     inc dx
     in al,dx
     mov ah,al
     in al,dx
     mov ds:[di],ax
     dec dx
et2:in al,dx
     test al,1
     jne et2
     inc dx
     in al,dx
     mov ah,al
     in al,dx
     mov word ptr pos,ax
     dec dx
    pop ds 
  }
    return pos;
} 
     
/*************************************************************************************************/
/* PIDchip::Rdip:Чете индекцната позиция               .                                            */
/* Входни данни:Няма                                                                                        */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
long  PIDchip::Rdip(void)
{    
  long pos=0;    
  byte *ps=(byte *)&pos;
   unsigned ad=adresc;
   __asm
   {                     
      push ds
      lds di,ps
      add di,2
      mov dx,ad
et3:in al,dx
     test al,1
     jne et3
     mov al,RDIP
     out dx,al               
et1:in al,dx
     test al,1
     jne et1
     
     inc dx
     in al,dx
     mov ah,al
     in al,dx
     mov ds:[di],ax
     dec dx
et2:in al,dx
     test al,1
     jne et2
     inc dx
     in al,dx
     mov ah,al
     in al,dx
     mov word ptr pos,ax
     dec dx
    pop ds 
  }
    return pos;
} 
     
/*************************************************************************************************/
/* PIDchip::Ltrj1:Въвежда траектория -позиция,скорост,ускорение -ком.дума          */
/* Входни данни:Командна дума,параметри до 3            */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void Ltrj1(unsigned tcw,unsigned long par1,unsigned long par2,unsigned long par3)
{    
   unsigned ad=0;//adresc;
   unsigned fcw=0;//filter_control_word;
   unsigned long acl=0,vl=0,pos=0;
   unsigned long *acp,*vlp,*posp;
   if((tcw & 48)!=0)
   {
      acl=par1;//ускорение
      if((tcw & 12)!=0)
      {
        vl=par2;           //скорост
        if((tcw & 3)!=0)
           pos=(unsigned long)par3;//позиция
           
      }
      else
        if((tcw & 3)!=0)
           pos=(unsigned long)par2;//позиция
   }
   else
   {
       if((tcw & 12)!=0)
       {
          vl=par1;//скорост
        if((tcw & 3)!=0)
           pos=(unsigned long)par2;//позиция
        }
        else
         if((tcw & 3)!=0)
          pos=(unsigned long)par1;//позиция
   }
   acp=&acl;
   vlp=&vl;
   posp=&pos;

   __asm
   {
     push ds
     push di
     mov dx,ad
e1:in al,dx
     test al,1
     jne e1

     mov al,LTRJ//команда
     out dx,al
     
et1:in al,dx
     test al,1
     jne et1
    inc dx  
    mov ax,tcw
    xchg ah,al
    out dx,al
    xchg ah,al
    out dx,al
    mov bl,al
    test bl,48
    je et2
    dec dx
et11:in al,dx
     test al,1
     jne et11
     inc dx
     lds di,acp
     add di,3
     mov al,ds:[di]
     out dx,al
     dec di
     mov al,ds:[di]
     out dx,al
     dec di
     dec dx
et12:in al,dx
     test al,1
     jne et12
     inc dx
     mov al,ds:[di]
     out dx,al
     dec di
     mov al,ds:[di]
     out dx,al
et2:test bl,12
      je et3
      dec dx
et13:in al,dx
     test al,1
     jne et13
     inc dx
     lds di,vlp
     add di,3
     mov al,ds:[di]
     out dx,al
     dec di
     mov al,ds:[di]
     out dx,al
     dec di
     dec dx
et14:in al,dx
     test al,1
     jne et14
     inc dx
     mov al,ds:[di]
     out dx,al
     dec di
     mov al,ds:[di]
     out dx,al

et3: test bl,3
       je et6
       dec dx
et15:in al,dx
     test al,1
     jne et15
        inc dx
     lds di,posp
     add di,3
     mov al,ds:[di]
     out dx,al
     dec di
     mov al,ds:[di]
     out dx,al
     dec di
     dec dx
et16:in al,dx
     test al,1
     jne et16
     inc dx
     mov al,ds:[di]
     out dx,al
     dec di
     mov al,ds:[di]
     out dx,al
    dec  dx
et6: in al,dx
     test al,1
     jne et6

     pop di 
     pop ds
   }
}      

/*************************************************************************************************/
/* PIDchip::Stt:Старт на движението                                                                   */
/* Входни данни:Няма                                                                                     */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void PIDchip::Stt(void)
{
    unsigned ad=adresc;
    __asm
    {
     mov dx,ad
    et1:in al,dx
     test al,1
     jne et1

     mov al,STT
     mov dx,ad
     out dx,al
     
et7:in al,dx
     test al,1
     jne et7
   }
}

/*************************************************************************************************/
/* PIDchip::Dfh:Дефинира начало                                                                    */
/* Входни данни:Няма                                                                                     */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void PIDchip::Dfh(void)
{
    unsigned ad=adresc;
    __asm
    {
     mov dx,ad
    et1:in al,dx
     test al,1
     jne et1

     mov al,DFH
     mov dx,ad
     out dx,al
     
et7:in al,dx
     test al,1
     jne et7
   }
}
/*************************************************************************************************/
/* PIDchip::Sip:Задава индексна позиция                                                         */
/* Входни данни:Няма                                                                                     */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void PIDchip::Sip(void)
{
    unsigned ad=adresc;
    __asm
    {
     mov dx,ad
    et1:in al,dx
     test al,1
     jne et1

     mov al,SIP
     mov dx,ad
     out dx,al
     
et7:in al,dx
     test al,1
     jne et7
   }
}
    
/*************************************************************************************************/
/* PIDchip::Rsti:Нулира прекъсванията                                                                   */
/* Входни данни:Няма                                                                                     */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void PIDchip::Rsti(int par)
{
  int ad=adresc;
  int *pr=&par;
  __asm
  {  
     push ds
     push di
     lds di,pr
     inc di
     mov dx,ad
et1:in al,dx
     test al,1
     jne et1
     mov al,RSTI
     out dx,al
et2:  in al,dx
     test al,1
     jne et2
     mov al, ds:[di]
     inc dx
     out dx,al
     dec di
     mov al, ds:[di]
     out dx,al
     dec dx
et4:  in al,dx
     test al,1
     jne et4
     pop di
     pop ds
 }
}

/*************************************************************************************************/
/* PIDchip::Lpes:Стоп при грешка                                                                  */
/* Входни данни:Стаойност на грешката ,над която се спира                               */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void PIDchip::Lpes(int par)
{
  int ad=adresc;
  int *pr=&par;
  __asm
  {  
     push ds
     push di
     lds di,pr
     inc di
     mov dx,ad
et1:in al,dx
     test al,1
     jne et1
     mov al,LPES
     out dx,al
et2:  in al,dx
     test al,1
     jne et2
     mov al, ds:[di]
     inc dx
     out dx,al
     dec di
     mov al, ds:[di]
     out dx,al
     dec dx
et4:  in al,dx
     test al,1
     jne et4
     pop di
     pop ds
 }
}

/*************************************************************************************************/
/* PIDchip::Mski:Маска на прекъсванията                                                                 */
/* Входни данни:Маска                                                                                     */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void PIDchip::Mski(int par)
{
  int ad=adresc;
  int *pr=&par;
  __asm
  {  
     push ds
     push di
     lds di,pr
     inc di
     mov dx,ad
et1:in al,dx
     test al,1
     jne et1
     mov al,MSKI
     out dx,al
et2:  in al,dx
     test al,1
     jne et2
     mov al, ds:[di]
     inc dx
     out dx,al
     dec di
     mov al, ds:[di]
     out dx,al
     dec dx
et4:  in al,dx
     test al,1
     jne et4
     pop di
     pop ds
 }
}


/*************************************************************************************************/
/* PIDchip::Rdstat:Чете статусния байт                                                                   */
/* Входни данни:Няма                                                                                     */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
 byte PIDchip::Rdstat(void)
 {
   byte val=0;
   unsigned ad=adresc;
   __asm
   {             
      mov dx,ad
      in al,dx
      mov val,al
   }
   return val;
}

/**********************************************************************************************/
/* PIDList::PIDList:Конструктор на класPIDList                                                  */
/* Вход : Няма                                                                                                */
/* Изход:Няма                                                                                                  */
/**************************************************************************************************/
  PIDList::PIDList()   
  {
     top=NULL;
     cur=&top;                
     bcur=top;
     nm=0;
   }   
           
/*********************************************************************/
/*PIDList::find:Търсене на елемент от списъка                  */
/* Вход  : Ключ за търсене                                               */
/* Изход : Няма                                                                 */
/**********************************************************************/
   void PIDList::find(int nm)
   {
      if(*cur==NULL)
         return;
      if((*cur)->number==nm)
         return;                   
      bcur=*cur;  /* указател към последния елемент */         
      cur=&(*cur)->next;
      find(nm);  
   } 

/*********************************************************************/
/* PIDList::add :Добавяне на възел в списъка                      */
/* Вход  :Номер ,адрес,коефициенти-kp,ki,kd,il и филт.контр.дума*/
/* Изход :Няма                                                                          */
/************************************************************************/
  void PIDList::add(int nm,unsigned adr,int kkp,int kki,int kkd,int kil,int f_c_w)
   {
      cur=&top;
      find(nm);//търсене по номер
      if(*cur)
           return ;
      else
      {
        *cur=new PIDchip(nm,adr,kkp,kki,kkd,kil,f_c_w,bcur);   //добавяне
        if(*cur!=NULL)               
         nm++;
      }
   }

/*********************************************************************/
/* PIDList::add :Добавяне на възел в списъка                      */
/* Вход  :Номер ,адрес                                                          */
/* Изход :Флаг за успешно добавяне                                   */
/************************************************************************/
flag PIDList::add(int nm,unsigned adr)
   {
      cur=&top;
      find(nm);//търсене по номер
      if(*cur)//има такъв елемент
           return FALSE;//връщане
      else
      {
        *cur=new PIDchip(nm,adr,bcur);   //добавяне
        if(*cur!=NULL)               
        {
         nm++;
         return TRUE;
         }
         else
               return FALSE;                        
      }
   }

/*********************************************************************/
/* PIDList::removeall:Изтриване (унищожаване) на списъка */
/* Вход  : Начало на списъка                                            */
/* Изход : Няма                                                                 */
/**********************************************************************/
   void PIDList::removeall(PIDchip **pch)
   {
     if(*pch==NULL)
       return;
     removeall(&((*pch)->next));
     delete *pch;
     nm--;
     *pch=NULL; 
   }


/*********************************************************************/
/* PIDList::saveall : Запис на списък във файл                      */
/* Вход  : Име на  файла                                               */
/* Изход     : Няма                                                    */
/******************************************************/
void  PIDList::saveall( char *nf )
{    
   FILE *f;
   if(top)
   {
      f=fopen(nf,"w+t");  /* отваряне на файла */
      if(f==NULL)
        return ;
      else
      {
        fprintf(f,"%d",nm);  /*  брой полета */
        saveList(f,top);    
        fclose (f);
         return ;
      }
   }
   else
    return ;
}
  
/*********************************************************************/
/* PIDList::saveList : Запис във файл на списъка с начало p */
/* Вход  :  Указател към файла , указател към възела  */
/* Изход     : Няма                                      */
/******************************************************/
void PIDList::saveList(FILE *f,PIDchip*p)
{
  if(p)
  {   
  /* запис на данните във файла по тип на полето */
    fprintf(f,"%u %lf %lf %lf %lf %d \n",p->adresc,p->kp,p->ki,p->kd,p->il,p->filter_control_word);
    saveList(f,p->next); /* рекурсивно извикване */
   }
}     
  
/***************************************************************************************/
/* PIDNode ::L_get_last : Търсeнe на последния елемент в списъка        */
/* Вход  :Няма                                                                                       */
/* Изход:Няма                                                                                         */
/****************************************************************************************/
void PIDList::L_get_last()
{
   if(*cur==NULL) /* ако няма елементи излиза */
    return ;
   if((*cur)->next==NULL) /* ако няма следващ излиза */
    return ;
   cur=&(*cur)->next; /* следващия */
   L_get_last();  /* рекурсивно извикване */
}

/*********************************************************************/
/* ListVF::load:Четене на списъка от файл                           */
/* Вход  : Име на  файла                                               */
/* Изход     : Резултат                                     */
/******************************************************/    
void PIDList::load(char *nf)
{
   FILE *f;  /* указател към файла */
   int numb=0; /* брой полета  */
   int nm=0;/* номер на поле */        
   unsigned adres=0;
   int kkp=0;
   int kki=0;
   int kkd=0;
   int kil=0;
   int f_c_w=0;
   f=fopen(nf,"r+t");  /*отваряне на файла */
   if(f==NULL)
    return ;
   else
   {
     fscanf(f,"%d ",&numb);/* прочитане на броя записи(полета) */
     for(int i1=0;i1<numb;i1++)
     {        
        fscanf(f,"%u %lf %lf %lf %lf %d ",&adres,&kkp,&kki,&kkd,&kil,&f_c_w);
        add(i1+1,adres,kkp,kki,kkd,kil,f_c_w);/* добавяне в списъка */
     }
     fclose(f);   
   }
}

/**********************************************************************************************/
/*  Axis::Axis :Конструктор на клас  Axis -ос           */
/* Вход : Адрес ,коефициенти на оста -кеф.на датчика,квадратурен коеф     */
/*            стъпка на винта,.номер на изход "ENABLE"                                         */
/* Изход:Няма                                                                                                  */
/**************************************************************************************************/
 Axis::Axis(unsigned adres,unsigned kid,unsigned kkv, double kstep,unsigned nou):
                PIDchip(adres)
{
  kfd=kid;//коеф. на датчика
  kv=kkv;//квадратурен коеф.
  pstep=kstep/1000;//сттъпка
  kmach=(double)(kfd*kv/pstep);//машинен коеф.
  acl=0;
  vel=0;
  numboutput=nou;//изход
}                


/*************************************************************************************************/
/* PIDchip::Ltrj:Въвежда траектория -позиция,скорост,ускорение -ком.дума          */
/* Входни данни:Командна дума,параметри до 3            */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void PIDchip::Ltrj(unsigned tcw,unsigned long par1,unsigned long par2,unsigned long par3)
{    
   unsigned ad=adresc;
   unsigned fcw=filter_control_word;//комндана дума -кой параметър се зарежда
   unsigned long acl=0,vl=0,pos=0;
   unsigned long *acp,*vlp,*posp;
   acl=par1;//ускорение
   vl=par2;//скорост
   pos=par3;//позиция
   acp=&acl;
   vlp=&vl;
   posp=&pos;

   __asm
   {
     push ds
     push di
     mov dx,ad
e1:in al,dx
     test al,1
     jne e1

     mov al,LTRJ//команда
     out dx,al
     
et1:in al,dx
     test al,1
     jne et1
    inc dx  
    mov ax,tcw
    xchg ah,al
    out dx,al
    xchg ah,al
    out dx,al
    dec dx
et11:in al,dx
     test al,1
     jne et11
     inc dx
     lds di,acp
     add di,3
     mov al,ds:[di]
     out dx,al
     dec di
     mov al,ds:[di]
     out dx,al
     dec di
     dec dx
et12:in al,dx
     test al,1
     jne et12
     inc dx
     mov al,ds:[di]
     out dx,al
     dec di
     mov al,ds:[di]
     out dx,al
      dec dx
et13:in al,dx
     test al,1
     jne et13
     inc dx
     lds di,vlp
     add di,3
     mov al,ds:[di]
     out dx,al
     dec di
     mov al,ds:[di]
     out dx,al
     dec di
     dec dx
et14:in al,dx
     test al,1
     jne et14
     inc dx
     mov al,ds:[di]
     out dx,al
     dec di
     mov al,ds:[di]
     out dx,al
       dec dx
et15:in al,dx
     test al,1
     jne et15
        inc dx
     lds di,posp
     add di,3
     mov al,ds:[di]
     out dx,al
     dec di
     mov al,ds:[di]
     out dx,al
     dec di
     dec dx
et16:in al,dx
     test al,1
     jne et16
     inc dx
     mov al,ds:[di]
     out dx,al
     dec di
     mov al,ds:[di]
     out dx,al
    dec  dx
et6: in al,dx
     test al,1
     jne et6

     pop di 
     pop ds
   }
}      

/*************************************************************************************************/
/* PIDchip::Ltrj:Въвежда траектория -позиция,скорост,ускорение -ком.дума          */
/* Входни данни:Командна дума,параметри до 2            */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void PIDchip::Ltrj(unsigned tcw,unsigned long par1,unsigned long par2)
{    
   unsigned ad=adresc;
   unsigned fcw=filter_control_word;//ком.дума-кои 2 параметъра се зареждат
   unsigned long pr1=0,pr2=0;
   unsigned long *ppr1,*ppr2;
   pr1=par1;
   pr2=par2;
   ppr1=&pr1;
   ppr2=&pr2;

   __asm
   {
     push ds
     push di
     mov dx,ad
e1:in al,dx
     test al,1
     jne e1

     mov al,LTRJ//команда
     out dx,al
     
et1:in al,dx
     test al,1
     jne et1
    inc dx  
    mov ax,tcw
    xchg ah,al
    out dx,al
    xchg ah,al
    out dx,al
    dec dx
et11:in al,dx
     test al,1
     jne et11
     inc dx
     lds di,ppr1
     add di,3
     mov al,ds:[di]
     out dx,al
     dec di
     mov al,ds:[di]
     out dx,al
     dec di
     dec dx
et12:in al,dx
     test al,1
     jne et12
     inc dx
     mov al,ds:[di]
     out dx,al
     dec di
     mov al,ds:[di]
     out dx,al
      dec dx
et13:in al,dx
     test al,1
     jne et13
     inc dx
     lds di,ppr2
     add di,3
     mov al,ds:[di]
     out dx,al
     dec di
     mov al,ds:[di]
     out dx,al
     dec di
     dec dx
et14:in al,dx
     test al,1
     jne et14
     inc dx
     mov al,ds:[di]
     out dx,al
     dec di
     mov al,ds:[di]
     out dx,al
       dec dx
et15:in al,dx
     test al,1
     jne et15
     pop di 
     pop ds
   }
}      

/*************************************************************************************************/
/* PIDchip::Ltrj:Въвежда траектория -позиция,скорост,ускорение -ком.дума          */
/* Входни данни:Командна дума,параметри до 1           */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void PIDchip::Ltrj(unsigned tcw,unsigned long par1)
{    
   unsigned ad=adresc;
   unsigned fcw=filter_control_word; //ком.дума -кой параметър се зарежда
   unsigned long pr1=0;
   unsigned long *ppr1;
   pr1=par1;
   ppr1=&pr1;

   __asm
   {
     push ds
     push di
     mov dx,ad
e1:in al,dx
     test al,1
     jne e1

     mov al,LTRJ//команда
     out dx,al
     
et1:in al,dx
     test al,1//проверка за готовност
     jne et1
    inc dx  
    mov ax,tcw
    xchg ah,al
    out dx,al//запис на контр. дума в чипа
    xchg ah,al
    out dx,al
    dec dx
et11:in al,dx
     test al,1//проверка за готовност
     jne et11
     inc dx
     lds di,ppr1
     add di,3
     mov al,ds:[di]
     out dx,al //запис на  параметъра в чипа
     dec di
     mov al,ds:[di]
     out dx,al
     dec di
     dec dx
et12:in al,dx
     test al,1  //проверка за готовност
     jne et12
     inc dx
     mov al,ds:[di]
     out dx,al
     dec di
     mov al,ds:[di]
     out dx,al
      dec dx
et13:in al,dx
     test al,1
     jne et13
     pop di 
     pop ds
   }
}      

/*************************************************************************************************/
/* PIDchip::Ltrj:Въвежда траектория -позиция,скорост,ускорение -ком.дума          */
/* Входни данни:Командна дума                                                                     */
/* изходни данни:Няма                                                                                         */
/**************************************************************************************************/
void PIDchip::Ltrj(unsigned tcw)
{    
   unsigned ad=adresc;
   unsigned fcw=filter_control_word;   //ком.дума 
   __asm
   {
     push ds
     push di
     mov dx,ad
e1:in al,dx
     test al,1
     jne e1
     mov al,LTRJ
     out dx,al
et1:in al,dx
     test al,1
     jne et1
    inc dx  
    mov ax,tcw
    xchg ah,al
    out dx,al
    xchg ah,al
    out dx,al
    dec dx
et11:in al,dx
     test al,1
     jne et11
     pop di 
     pop ds
   }
}      
