/*********************************************************************/
/*                       S V G A . C P P                                     */
/*-----------------------------------------------------------------------------------*/
/*     Управление на изхода (екрана) в текстов режим      */
/*********************************************************************/

/*==Включени заглавни файлове==================*/
#include <graph.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <bios.h>
#include <malloc.h>
#include "..\include\svga.h"
#include "..\include\videoc.h"
#include "..\include\memnew.h"

VP vptr; /* указател към структура VP,която описва двойката ASCII
                                                символ и атрибут  */
#define VOFS(x,y) (2*(80*(x) + (y)))  /*отместване в паметта на левия горен ъгъл
 на прозореца спрямо левия горен ъгъл на екрана */
#define SVGA_START ((unsigned  far *)MK_FP(0x40,0x4E)) /* отместване спрямо 0 видеостраница*/
#define ADDR_6845  ((unsigned  far *)MK_FP(0x40,0x63))  /* адрес на бодеопаметта */
#define VPOS(x,y) (VP) ((byte far *)vptr + *SVGA_START+ VOFS(x,y)) /* позиция на екрана */
#define VPOSP(p,x,y) (VP) ((byte far  *)vptr + p*4096 + VOFS(x,y)) /* позиция на екрана */
/*== Глобални променливи  ============================*/


/************************************************************************/
/* disp_init : Намира се адреса на видео RAM,определя се */
/*                от вида на платката                                          */
/* Вход     : Няма                                                                */
/* Изход   : Няма                                                                */
/*********************************************************************/
flag disp_init(void)
{
   int brb=0;
   if(*ADDR_6845==0x3b4) /* начален адрес на видео RAM,
                                           от BIOS променлива          */
   vptr=(VP) MK_FP(0xb000,0x0);
  else
   vptr=(VP) MK_FP(0xb800,0x0);
    return TRUE;
}

/************************************************************************/
/* Save_scr :Съхранява прозорец от екрана в оперативната  */
/*                 памет с адрес sptr                                            */
/* Вход     : Екранна страница,координати на левия горен ъгъл,*/
/*                височина и ширина на прозореца и място за        */
/*                съхраняване на прозореца                                  */
/* Изход   : Няма                                                                */
/*********************************************************************/
void Save_scr(int page,int row,int col,int height,int width,union vel   *sptr)
{
   int i,j;  /* помощни променливи */
   register  VP lptr; /* указател за работа с екранната памет */
   for(i=0;i<=height;i++)
   { 
     lptr=VPOSP(page,row+i,col); /* формиране на начален адрес на прозореца
                                          и изчисляване на новите начални адреси  */
     for(j=0;j<=width;j++)
       (sptr++)->x.content=(lptr++)->x.content;/* прехвърляне на един ред */
   } 
}


/************************************************************************/
/* Allocate_scr :Резервира памет с определен обем и съхранява */
/*                     в него прозореца от екрана                                  */
/* Вход     : Екранна страница,координати на левия горен ъгъл,*/
/*                височина и ширина на прозореца                          */
/* Изход   : Указател към  мястото  за съхраняване на прозореца */
/*********************************************************************/
union vel  * Allocate_scr(int page,int row,int col,int height,int width)
{
   VP sptr=NULL;           
   int brb=0;/* брой байтове */
   brb=(height+1) * (width+1) *sizeof(union vel); /* изчисляване на 
                необходимия обем памет в байтове    */
   if(fNewMemory((void **)(&sptr),brb,&"SaveSCREEN")!=FALSE )
   {
      Save_scr(page,row,col,height,width,sptr);/* съхранява прозореца */
       return sptr;
    }
     else
       return NULL;
}
/*********************************************************************************/
/*Unallocate_scr:Освобождаване на паметта  за изображение */
/* Входни данни:Указател към изображението в паметта         */
/* Изходни данни:Няма                                                                                       */
/*********************************************************************************/
void Unallocate_scr(union vel  * sptr)
{                
  if( FreeMemory(sptr)!=FALSE)
    sptr=NULL;
  
}
  
/************************************************************************/
/* Load_scr :Извежда прозорец в работната екранна страница */
/*                или в буфера                                                      */
/* Вход     : Екранна страница,координати на левия горен ъгъл,*/
/*                височина и ширина на прозореца, мястото  на        */
/*                съхранения прозорец и булева променлива-          */
/*                страница от видеопаметта:TRUE ,буфер                 */
/*                (виртуален екран):FALSE                                       */
/* Изход   :  Няма                                                                     */
/*********************************************************************/
void Load_scr(int page,int row,int col,int height,int width,
                union vel   *sptr,int zero_page)
{
  register VP lptr,lptr1;    
  int  i,j;
  if(zero_page==0)
    lptr=vptr;
  else 
 {
        lptr=NULL;             
        printf("NULL lptr");
        return;
  }
  for(i=0;i<=height;i++)
  {
    lptr1=(VP)((byte   *)lptr +VOFS(row+i,col)); /* отмесване от началото на 
                                                                         буфера   */
    for(j=0;j<=width;j++)
       (lptr1++)->x.content=(sptr++)->x.content;  /* прехвърляне */
  }
}


/************************************************************************************/
/*dprint:Изобразяване на текст  във видеопаметта                                    */
/*Входни данни:видеостраница,колона ,ред,атрибут на текста -         */
/* комбинация от основен цвят и фон,указател към текста за изобразяване */
/*Изходни данна:Няма                                                                         */
/************************************************************************************/
void dprint(byte page,byte column,byte scrow,byte dcolor,char far *string)
{                                              
      VP lptr;//видео структура
      byte *strr=(byte *)string;//указател към текста
      lptr=VPOSP(page,scrow,column); /* позиция върху екрана */
      byte color=dcolor;//цвят
      __asm
      {
           push ds
           push es
           push si
           push di
           les di,lptr
           lds si,strr          
           cmp ds:[si],0 //край на низа
           je et2//да -преход на етикет ет2
et1:    mov al,ds:[si]//запис на поредния символ в регистър
           mov es:[di],al  //прехвърляне във видеопаметта
           mov al,color   //задаване на цвят
           inc di    //следващ символ
           mov es:[di],al  //запис
           inc di
           inc si
           cmp ds:[si],0 //проверка за карй на низа
            jne et1 //не-зацикляне
et2:      pop di
             pop si
             pop es
             pop ds
          }

}


void SaveFile(int page,int row,int col,int height,int width,char *namef)
{
   int i,j;  /* помощни променливи */
   register  VP lptr; /* указател за работа с екранната памет */
  
      FILE *stream;     /* поток на файла */
   if((stream=fopen(namef,"w+"))!=NULL) 
   {            /* не */
	   for(i=0;i<=height;i++)
   	   { 
     	lptr=VPOSP(page,row+i,col); /* формиране на начален адрес на прозореца
                                          и изчисляване на новите начални адреси  */
     		for(j=0;j<=width;j++)
     		{	
     	  		fputc((lptr)->h.thechar,stream); /* копиране на символа */
		  		fputc((lptr++)->h.attribute,stream);/* копиране на атрибута */
      		}                       
    	}   
    			fclose(stream);
    }
}


void LoadFile(int page,int row,int col,int height,int width,
                char *namef,int zero_page)
{
  register VP lptr,lptr1;    
  int  i,j;
  if(zero_page==0)
    lptr=vptr;
  else 
 {
        lptr=NULL;             
        printf("NULL lptr");
        return;
  }
     FILE *stream;     /* поток на файла */
  if((stream=fopen(namef,"r+"))!=NULL) /* има ли файл с изображение */
  {            /* не */
  	  for(i=0;i<=height;i++)
      {
    		lptr1=(VP)((byte   *)lptr +VOFS(row+i,col)); /* отмесване от началото на 
                                                                         буфера   */
             for(j=0;j<=width;j++)
             {
             		(lptr1)->h.thechar=fgetc(stream);/* символ */
		  			(lptr1++)->h.attribute=fgetc(stream);/* атрибут */
             }
       }                                   
       		fclose(stream);
   }
}
