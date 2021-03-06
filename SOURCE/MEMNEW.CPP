/************************************************************************************************************/
/*                                                      M E M N E W . C P P                                                */
/*       ������� �� �������� � ������������� �� �����.�������� �� ������� ���               */
/*                      ������  � "DEBUG"                                                                                    */
/************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>     
#include <malloc.h>
#include <dos.h>
#include "..\include\memnew.h"     
union REGS regs;
struct SREGS sregs;
flag permission=TRUE; //���� �� ������� �� �������

/*********************************************************************************/
/* Assert : ���������� ������������ �� �������� ,��� ����������� � */
/*              �� ���������                                                                     */
/* ����: ��� �� �����,����� �� ��������                                         */
/* �����:����                                                                                    */
/************************************************************************************/
void _Assert(char *strFile,unsigned uLine)
{
   fflush(NULL);//����������� �� ������������ � ��������
   fprintf(stderr,"ASSERT:%s,red %u  \n ADRESS :%lp \n",strFile,uLine);//���������
   fflush(stderr);//����� �� �����������
   abort();//�����
}   

  //  � � � � � � � � � � � � �      � � � � � � � 
  
/*********************************************************************************/
/* fNewMemory: ������ �����                                                       */
/* ����: ������� ��� ��������� �� ���������� �����,������,��� �� ���. ���� */
/* �����:����                                                                                  */
/*            FALSE:��������� �������� �� ����� ,����� �� ��           */
/*             TRUE:�����                                                                       */
/************************************************************************************/
flag fNewMemory(void **ppv,size_t size,char *td)
{
   byte  **ppb=(byte **)ppv;  
   if(permission!=FALSE)  //���� �� ������ � �������
     permission=FALSE; //��������� �� ������ � �������
   else 
     return FALSE; //�����, ����� ������ � �������
   ASSERT(ppv !=NULL && size !=0); //�������� �� �����������
   *ppb=(byte *)malloc(size);             //�������� �� ���� �� �������
//   if(*ppb==NULL)                                                     
//     NewMemoryUMB(ppv,size);
   permission=TRUE;  //���� �� �� ������ � �������
   return (flag) (*ppb!=NULL); //�������� �� ���������� �� ���� �� �������
}  

/*********************************************************************************/
/* FreeMemory: ����������� ����������  �����                            */
/* ����: ����a��� ��� ���������� �����                                         */
/* �����:����                                                                                 */
/*            FALSE:��������� ������������� �� �����                     */
/*             TRUE:�����                                                                       */
/************************************************************************************/
flag FreeMemory( void *pv)
{  
 ASSERT(pv!=NULL);  //�������� �� ���������
  if(permission!=FALSE) //������ �� �� � �������
     permission=FALSE; //�� - ���� �� ������ � �������
   else 
     return FALSE; // ��- �����
  unsigned int pr=_FP_SEG(pv);  
  if(pr >0xA000)
   FreeUMB(pv);
  else
  {
      free(pv);//������������� �� ������� 
  }
  permission=TRUE;//���� �� �� ������ � �������
  return TRUE;
}

int ReadUMB(void)
{ 
   regs.x.ax=0x5802;
   intdos(&regs,&regs);
   return regs.h.al;
}

void SetUMB(unsigned int UMB)
{
    regs.x.ax=0x5803;
    regs.x.bx=UMB;
    intdos(&regs,&regs);
}


void DOS_GetMem( unsigned int Ps,
		 unsigned int *Adr,
		 unsigned int *Res )
{
 regs.h.ah = 0x48  ;                           /* Function number */
 regs.x.bx = Ps;              /* Number of paragraphs to be reserved */
 intdos( &regs, &regs );                           /* Interrupt call */
 if ( !regs.x.cflag )                            /* Call successful? */
 {
  *Adr = regs.x.ax;               /* Yes --> Return address and size */
  *Res = Ps;
 }
 else                                                /* No --> Error */
 {
  *Adr = 0;                                    /* No memory reserved */
  *Res = regs.x.bx;                        /* Maximum available size */
 }
}

/*********************************************************************/
/* DOS_FreeMem: Releases reserved memory.                            */
/* Input      : Memory segment address                               */
/* Output     : None                                                 */
/*********************************************************************/

void DOS_FreeMem( unsigned int Adr )
{
 regs.h.ah = 0x49;                            /* Function number */
 sregs.es = Adr;                 /* Address of memory to be released */
 intdosx( &regs, &regs, &sregs );                  /* Interrupt call */
}

void SetDosStrategy( unsigned int Strategy )
{
 regs.x.ax = 0x5801;                    /* Set function number */
 regs.x.bx = Strategy;
 intdos( &regs, &regs );
}

void NewMemoryUMB(void **ppv,size_t size)
{
    byte  **ppb=(byte **)ppv;  
   ASSERT(ppv !=NULL && size !=0); //�������� �� �����������

     unsigned int oldUMB=ReadUMB();
    SetUMB(1);
    SetDosStrategy(1 |0x80 );
    int r=size/16 +1;      
    unsigned int rr;
    unsigned int pr1;
    DOS_GetMem(r,&pr1,&rr);    
	if(ReadUMB()==1)
	    SetUMB(0);
     *ppb=(byte *)_MK_FP(pr1,0x0);

}


void FreeUMB(void *ppv)
{
     unsigned int oldUMB=ReadUMB();
    SetUMB(1);
    SetDosStrategy(1 |0x80 );

   regs.h.ah=0x49;
  sregs.es=_FP_SEG(ppv);
  intdosx(&regs,&regs,&sregs);
	if(ReadUMB()==1)
	    SetUMB(0);
  ppv=NULL;
 }

void FreeMem(void *ppv)
{
   regs.h.ah=0x49;
   sregs.es=_FP_SEG(ppv);
   intdosx(&regs,&regs,&sregs);
 }

//�������� �� ����� ���� ������� 
flag NewMemory(void **ppv,size_t size)
{
    byte  **ppb=(byte **)ppv;  
   ASSERT(ppv !=NULL && size !=0); //�������� �� �����������

    int r=size/16 +1;      
    unsigned int rr;
    unsigned int pr1;
    DOS_GetMem(r,&pr1,&rr);    
     *ppb=(byte *)_MK_FP(pr1,0x0);
    return (flag) (*ppb!=NULL); //�������� �� ���������� �� ���� �� �������
}

