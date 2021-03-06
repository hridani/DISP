/*********************************************************************/
/*                       S V G A . C P P                                     */
/*-----------------------------------------------------------------------------------*/
/*     ���������� �� ������ (������) � ������� �����      */
/*********************************************************************/

/*==�������� �������� �������==================*/
#include <graph.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <bios.h>
#include <malloc.h>
#include "..\include\svga.h"
#include "..\include\videoc.h"
#include "..\include\memnew.h"

VP vptr; /* �������� ��� ��������� VP,����� ������ �������� ASCII
                                                ������ � �������  */
#define VOFS(x,y) (2*(80*(x) + (y)))  /*���������� � ������� �� ����� ����� ����
 �� ��������� ������ ����� ����� ���� �� ������ */
#define SVGA_START ((unsigned  far *)MK_FP(0x40,0x4E)) /* ���������� ������ 0 �������������*/
#define ADDR_6845  ((unsigned  far *)MK_FP(0x40,0x63))  /* ����� �� ������������ */
#define VPOS(x,y) (VP) ((byte far *)vptr + *SVGA_START+ VOFS(x,y)) /* ������� �� ������ */
#define VPOSP(p,x,y) (VP) ((byte far  *)vptr + p*4096 + VOFS(x,y)) /* ������� �� ������ */
/*== �������� ����������  ============================*/


/************************************************************************/
/* disp_init : ������ �� ������ �� ����� RAM,�������� �� */
/*                �� ���� �� ��������                                          */
/* ����     : ����                                                                */
/* �����   : ����                                                                */
/*********************************************************************/
flag disp_init(void)
{
   int brb=0;
   if(*ADDR_6845==0x3b4) /* ������� ����� �� ����� RAM,
                                           �� BIOS ����������          */
   vptr=(VP) MK_FP(0xb000,0x0);
  else
   vptr=(VP) MK_FP(0xb800,0x0);
    return TRUE;
}

/************************************************************************/
/* Save_scr :��������� �������� �� ������ � ������������  */
/*                 ����� � ����� sptr                                            */
/* ����     : ������� ��������,���������� �� ����� ����� ����,*/
/*                �������� � ������ �� ��������� � ����� ��        */
/*                ����������� �� ���������                                  */
/* �����   : ����                                                                */
/*********************************************************************/
void Save_scr(int page,int row,int col,int height,int width,union vel   *sptr)
{
   int i,j;  /* ������� ���������� */
   register  VP lptr; /* �������� �� ������ � ��������� ����� */
   for(i=0;i<=height;i++)
   { 
     lptr=VPOSP(page,row+i,col); /* ��������� �� ������� ����� �� ���������
                                          � ����������� �� ������ ������� ������  */
     for(j=0;j<=width;j++)
       (sptr++)->x.content=(lptr++)->x.content;/* ����������� �� ���� ��� */
   } 
}


/************************************************************************/
/* Allocate_scr :��������� ����� � ��������� ���� � ��������� */
/*                     � ���� ��������� �� ������                                  */
/* ����     : ������� ��������,���������� �� ����� ����� ����,*/
/*                �������� � ������ �� ���������                          */
/* �����   : �������� ���  �������  �� ����������� �� ��������� */
/*********************************************************************/
union vel  * Allocate_scr(int page,int row,int col,int height,int width)
{
   VP sptr=NULL;           
   int brb=0;/* ���� ������� */
   brb=(height+1) * (width+1) *sizeof(union vel); /* ����������� �� 
                ����������� ���� ����� � �������    */
   if(fNewMemory((void **)(&sptr),brb,&"SaveSCREEN")!=FALSE )
   {
      Save_scr(page,row,col,height,width,sptr);/* ��������� ��������� */
       return sptr;
    }
     else
       return NULL;
}
/*********************************************************************************/
/*Unallocate_scr:������������� �� �������  �� ����������� */
/* ������ �����:�������� ��� ������������� � �������         */
/* ������� �����:����                                                                                       */
/*********************************************************************************/
void Unallocate_scr(union vel  * sptr)
{                
  if( FreeMemory(sptr)!=FALSE)
    sptr=NULL;
  
}
  
/************************************************************************/
/* Load_scr :������� �������� � ��������� ������� �������� */
/*                ��� � ������                                                      */
/* ����     : ������� ��������,���������� �� ����� ����� ����,*/
/*                �������� � ������ �� ���������, �������  ��        */
/*                ���������� �������� � ������ ����������-          */
/*                �������� �� ������������:TRUE ,�����                 */
/*                (��������� �����):FALSE                                       */
/* �����   :  ����                                                                     */
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
    lptr1=(VP)((byte   *)lptr +VOFS(row+i,col)); /* ��������� �� �������� �� 
                                                                         ������   */
    for(j=0;j<=width;j++)
       (lptr1++)->x.content=(sptr++)->x.content;  /* ����������� */
  }
}


/************************************************************************************/
/*dprint:������������ �� �����  ��� ������������                                    */
/*������ �����:�������������,������ ,���,������� �� ������ -         */
/* ���������� �� ������� ���� � ���,�������� ��� ������ �� ������������ */
/*������� �����:����                                                                         */
/************************************************************************************/
void dprint(byte page,byte column,byte scrow,byte dcolor,char far *string)
{                                              
      VP lptr;//����� ���������
      byte *strr=(byte *)string;//�������� ��� ������
      lptr=VPOSP(page,scrow,column); /* ������� ����� ������ */
      byte color=dcolor;//����
      __asm
      {
           push ds
           push es
           push si
           push di
           les di,lptr
           lds si,strr          
           cmp ds:[si],0 //���� �� ����
           je et2//�� -������ �� ������ ��2
et1:    mov al,ds:[si]//����� �� �������� ������ � ��������
           mov es:[di],al  //����������� ��� ������������
           mov al,color   //�������� �� ����
           inc di    //������� ������
           mov es:[di],al  //�����
           inc di
           inc si
           cmp ds:[si],0 //�������� �� ���� �� ����
            jne et1 //��-���������
et2:      pop di
             pop si
             pop es
             pop ds
          }

}


void SaveFile(int page,int row,int col,int height,int width,char *namef)
{
   int i,j;  /* ������� ���������� */
   register  VP lptr; /* �������� �� ������ � ��������� ����� */
  
      FILE *stream;     /* ����� �� ����� */
   if((stream=fopen(namef,"w+"))!=NULL) 
   {            /* �� */
	   for(i=0;i<=height;i++)
   	   { 
     	lptr=VPOSP(page,row+i,col); /* ��������� �� ������� ����� �� ���������
                                          � ����������� �� ������ ������� ������  */
     		for(j=0;j<=width;j++)
     		{	
     	  		fputc((lptr)->h.thechar,stream); /* �������� �� ������� */
		  		fputc((lptr++)->h.attribute,stream);/* �������� �� �������� */
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
     FILE *stream;     /* ����� �� ����� */
  if((stream=fopen(namef,"r+"))!=NULL) /* ��� �� ���� � ����������� */
  {            /* �� */
  	  for(i=0;i<=height;i++)
      {
    		lptr1=(VP)((byte   *)lptr +VOFS(row+i,col)); /* ��������� �� �������� �� 
                                                                         ������   */
             for(j=0;j<=width;j++)
             {
             		(lptr1)->h.thechar=fgetc(stream);/* ������ */
		  			(lptr1++)->h.attribute=fgetc(stream);/* ������� */
             }
       }                                   
       		fclose(stream);
   }
}
