/******************************************************************************************/
/*                               I R Q 9 N E W . C P P                                                 */
/*   ���������� �� ������� �� ������ � ������������ ,���� ������ �� "DISP"     */
/******************************************************************************************/
#include <dos.h>
#include <conio.h>
#include <string.h>
#include "dispnew.h"  
#include "irq9new.h"
#include "comst.h"
#include "string.h"
#include "..\include\usepia.h"
#include "..\include\svga.h"

#define kinf ((char far *) MK_FP(0x40,0x17))
void (__cdecl __interrupt __far *old9)();/* ���� ������ �� ���������� 0x9 */
void  __interrupt __far newi9(); /* ��� ������ �� ����������� ���������� */
KeyBuffer kbp; /* ����������� ����� �� ������������ ������ */
KeyBuffer ikb; /* �������������� ����� � ASCII ������ */
byte  *curtable,*table[3];//�������� ��� ������ ������� �� �����,3 ������� �� ����� �� ���������
byte numbtable;//����� �� ������� �������
byte inpar[80];
extern   unsigned  numbinpPIA;
/* ����� -POS ���,����������� ASCII ������ */
unsigned bri;
byte pASCII[][3]={ {16,81,113},
		     { 17,87,119},
		     { 18,69,101},
		     { 19,82,114},
		     { 20,84,116},
		     { 21,89,121},
		     { 22,85,117},
		     { 23,73,105},
		     { 24,79,111},
		     { 25,80,112},
		     { 30,65,97},
		     { 39,59,58},// : , ;
		     { 43,92,124},
		     {57,32,32},
		     { 31,83,115},
		     { 32,68,100},
		     { 33,70,102},
		     { 34,71,103},
		     { 35,72,104},
		     { 36,74,106},
		     { 37,75,107},
		     { 38,76,108},
		     { 44,90,122},
		     { 45,88,120},
		     { 46,67,99},
		     { 47,86,118},
		     { 48,66,98},
		     { 49,78,110},
		     { 50,77,109},

		     { 2,49,33},
		     { 3,50,64},
		     { 4,51,35},
		     { 5,52,36},
		     { 6,53,37},
		     { 7,54,94},
		     { 8,55,38},
		     { 9,56,42},
		     { 10,57,40},
		     { 11,48,41},
		     { 12,45,95},
		     { 13,61,43},
		     { 52,46,62},
		     { 51,44,60},
		    } ;

void (__interrupt __far*v)(void);

void __interrupt __far tsr(void)  
{  
  
   _enable();
   _outp(0x20,0x20);                  
  
}
		      

unsigned brd;
/*****************************************************************************/
/* IniKeyb:���������� ������������ ������������,�������        */
/*             �����  - ������� � �������                                         */
/* ������ �����:����                                                               */
/* ������� �����:����                                                              */
/*****************************************************************************/
void IniKeyb(void)
{         
     kb=&kbp;/* ����������� ����� =kbp -���������� */
     IniBuffer(kb,4); /* 32 ���. ��� */
     infbuffer=&ikb; /* ����. ���. �� ASCII ������ =ikb */
     IniBuffer(infbuffer,80);/* �������� �� ����� �� ����� ikb  */
     *(infbuffer->scanbuf)='\0';/* ������������� �� ����� ������ =0 */
     IniPos(); /* �������������� �� ��������� � POS ���- ��������  'F','M','S','CR' */
    if(CRFUNC==NULL)
     CRFUNC=CRF;/* �������� ��� ��������� CR �������- ���������� */
     #ifdef  TESTIN
     IniInputTable();    
    IniInputPar();
    #else 
     IniInputTable();    
    IniInputPar();
    
    #endif                         
    
     old9=_dos_getvect(0x09); /* ���� ������ �� ���������� */
    _dos_setvect(0x09,newi9);/* ��� ������ �� ����. ���������� */

}
   
/*****************************************************************************/
/* IniInputTable:���������� ������������ ������� � ������� ����������� */
/*             �����  - �������                                        */
/* ������ �����:����                                                               */
/* ������� �����:����                                                              */
/*****************************************************************************/
void IniInputTable(void)
{         
	ReadParamIniIO();//���� �� ��������� �  ����������� � ������������ PIA ,
			       //������� ������� ������� � ������,���� �����
	IniSerialPort();// ������������� �� �������� ����
	for(int i=0;i<3;i++)                                   
	{   
		if(fNewMemory((void **)(&table[i]),numbinpPIA,&"TableInput")==FALSE)      
	 	printf("No memory");
 	}
       
	curtable=table[0];
	numbtable=0; //������� � 0�������
     
}   

void EndTable(void)
{
  FreeMemory(table[0]);   
  FreeMemory(table[1]);   
  FreeMemory(table[2]);   
}
/***********************************************************************************/
/* IniPos :������� ������� � ���������� : POS ��� - ��� �� ������� */
/* ������ �����:����                                                                         */
/* ������� �����: ����                                                                       */
/************************************************************************************/
void IniPos(void)
{
     if(fNewMemory((void **)(&Inpos),90,&"InPos")!=FALSE)
     {        
	   *(Inpos+1)='M';
	   for(int i=59;i<64;i++)
	       *(Inpos+i)='F';  /* ������������ ������ */
	    for(i=64;i<89;i++)
	       *(Inpos +i)='M'; /* ���������� ������ */
	    for(i=13;i<16;i++)
	       *(Inpos +i)='M'; /* ���������� ������ */
	   for(i=2;i<=12;i++)
	       *(Inpos +i)='S';  /* �������� ������ */
	   for(i=16;i<28;i++)
	      *(Inpos +i)='S'; 
	   for(i=30;i<54;i++)
	      *(Inpos +i)='S'; 
     }
     *(Inpos +57)='S';  /* SPACE */
      *(Inpos +28)='C';  /* CR */
}

/******************************************************************/
/* EndPos : ����������� ������� �� ����� ��            */
/*                ������������                                             */
/* ������ ����� : ����                                                 */
/* ������� �����:����                                                 */
/********************************************************************/
void EndPos(char *pPos)
{
     FreeMemory(pPos);  
}


/****************************************************************************/
/* FindFKey : ������� �� ������������� ������ -���������� */
/*                 �� ��������� ����� ���� �������                         */
/* ������ �����: ���������� ��� �� �������                           */
/* ������� �����: ����                                                            */
/*****************************************************************************/
void FindFKey(byte pos)
{
    SignalSem(500);//���������� �� ������� �� ����� ��.
    posa=pos+ *posl - 58 +5*(*post);  /* ������� ������ = POS ��� - POSL- ��� ������ - POS ��� �� F1 +5*POST -�����*/
    dk->cur=&dk->top; /* �����k �� ������������ ������������ ������� */
    dk->find(posa); /* ������� -> ������� + 5(���������� ���� �� ���) *(������ ������ ) */
    if(*dk->cur)
    {
      *aKey=*dk->cur; /* ������� ������ */
      CRFUNCOLD=CRFUNC;
      if((*dk->cur)->funck->crfunc!=NULL)  /* ������� �� CR ������� �� ������� */
	   CRFUNC=(*dk->cur)->funck->crfunc; /* ���� CR ������� �� CR ������� */
      if((*dk->cur)->funck->activf!=NULL)  
	    (*dk->cur)->funck->activf();  
    }
}  
/***************************************************************************************/
/* FindMKey : �������� �� ���������� �� ������������� �������        */
/* ������ �����: ���������� ��� �� �������                           */
/* ������� �����: ����                                                            */
/*****************************************************************************/
void FindMKey(byte pos)
{        
   switch (pos)
   {
      case 1:
	       if(ReadSem(1003)!=0)
	       {
		 AritSem(1003,0);
		 SignalSem(74);//������� ���� ������
	       }

      break;
      case 64:    
	  SignalSem(500);//���������� �� ������� �� ����� ��.
	    FKRignt();//�������� ������� �� ���������� �� �����.��.
       break;
      case 65:
	  SignalSem(500);//���������� �� ������� �� ����� ��.
       FKLeft();//�������� ������ �� ���������� �� �����.��.
      break;    
      case 66 :            
	  SignalSem(500);//���������� �� ������� �� ����� ��.
       FKTop();//�������� ������ �� ���������� �� �����.��.
      break;
//       case 67://���������� �� ������� �� ����� ��.
//           SignalSem(500);
//       FKBottom();//�������� ������ �� ���������� �� �����.��.
//      break;   
       case 72:
      if(MKTop!=NULL)  //�������� ������ � �������
	      MKTop();
	 break;
       case 80:
       if(MKBottom!=NULL)
		  MKBottom();    //�������� ������ � �������
      break;                         
	 case 77:                     
	 if(MKRight!=NULL)//�������� ������� � �������
	    MKRight();
	    break;
	 case 75: 
	  if(MKLeft!=NULL )
	    MKLeft();  //�������� ������ ��� �������
	    break;
      case 82:
	if(MKInsert!=NULL)
	   MKInsert();   // INSERT
       break;
       case 71:    
	   SignalSem(500);
       if(MKHome!=NULL)
	 MKHome();   //HOME
       break;
       case 73:
       if(MKPgUp!=NULL)
	MKPgUp();      //PgUp
      break;
    case 81:
    if(MKPgDn!=NULL)
    MKPgDn();   //PgDn
      break;

	  case 83:
	  if(MKDel!=NULL)
	 MKDel();    // DEL
	break;
	 case 14:  //BS
	 SignalSem(501);
      if(*col>0)
       *col=*col -1 ;
      Deletechar(*wASCII,*col);
	 break;     
	 case 87:
	     SignalSem(500);//���������� �� ������� �� ����� ��.
	  if(F11!=NULL)  //F11
	    F11();
	    break; 
	 case 88:
	     SignalSem(500);//���������� �� ������� �� ����� ��.
	 if(F12!=NULL)    //F12
	  F12();
	      break; 
	 case 68:
	     SignalSem(500);//���������� �� ������� �� ����� ��.
	  if(F10!=NULL)   //F10
	    F10();
	       break; 
    }                 
}
       
/*****************************************************************************************/
/* FindSKey : ������� �� ASCII ���� �� ������ � ��������� POS ���      */
/*                   ����� � �������������� �����                                         */
/* ������ �����: ���������� ��� �� �������                           */
/* ������� �����: ����                                                            */
/*****************************************************************************/
void FindSKey(byte pos)
{                              
  byte k=0;
  SignalSem(501);
   for(int i=0;i<45;i++)
   {
      if(pASCII[i][0]==pos) /* ������� �� POS ��� ASCII 1,ASCII 2 */
      {   
       if((*kinf & 0x40)!=0)  //��������  Caps Lock
	    k=pASCII[i][2];     //����� �������� �� �������
       else  
	     k=pASCII[i][1]; //����� ��������� �� �������

       if(k!=0)
       {
	   __asm
	  {
	    push di
	    push es  
	    push ds
	    mov al,k
	    lds bx ,infbuffer  //����� �� ���. �����
	    les di,ds:[bx].scanbuf 
	    mov di,ds:[bx].scanlast
	    mov es:[di],al
	    inc di   
	    les si, ds:[bx].scanend
	    cmp di,si
	    jne i9nowrap
	    les si,ds:[bx].scanbuf
	    mov di,si
i9nowrap:mov ds:[bx].scanlast,di    
	    pop ds
	    pop es
	    pop di
	}
	break;
       }
      }                 
   }
}
unsigned brk;
/******************************************************************************/
/* FindKey : ������������� �� ��������� �� ���������� �������� */
/*               �� ������� POS ��� - �������� (F-������������,C - CR, */
/*                S- ��������, � - ���������� )                                      */
/* ������ �����: ���������� ��� �� �������                                   */
/* ������� �����: ����                                                                  */
/**********************************************************************************/
void FindKey(byte pos)
{     
   char pchar;// ���������� �� ���������� �������� �� ������� � ��������� ���. ���
   pchar=*(Inpos + pos); //��������� �� �������

   switch (pchar)  //��������
   {
     case 'F' : //������������
       FindFKey(pos);
      break;
     case 'M': //����������
       FindMKey(pos);
       break;
     case 'C':  //CR
     if(CRFUNC!=NULL)
       CRFUNC();
       break;
      case 'S': //��������


       FindSKey(pos);
       break;
    }                                  
}
int l=0;     
/******************************************************************************/
/* SlowKey : ����� ������ �� "DISP" - ��������� �� ������������ ������ */
/* ������ �����: ����                                                       */
/* ������� �����: ����                                                                  */
/**********************************************************************************/
void SlowKey(void)
{     
    byte k=0; //������� ����������
   int  numb=30;
    while(TRUE)
    {                                 

	    flag_change=FALSE;
	    k=0;
	do
	{
	      k=ReadKey();    //������ �� ���. ��� �� �������
	      if(k!=0)              //��� �� ������
	      {    
		FindKey(k); //��������� �� �������
		NoSignalSem(1);
	      }
	}
	while(k!=0); //  ������ ��� ����������� ���. ������ � ���. �����

       Filter();       //�������� ��������� � ������� �������� ��� ���������� ��������
//      SemInputCNC();//���������� �� �������� ������� ,���� 300 ���.��������
       SemInput();
	if( WorkingInput!=NULL)
	 WorkingInput();//��������� �� �������� ����������
		     Delay();
    }                    
}
  
/******************************************************************************/
/* EndKeyb :����������� ������� �� ������������� ������,     */
/*                 � �� ��������� � ���������� �� ��������� � ���.������.  */
/*                ����� ������ ������ �� ����. ����.                         */
/* ������ �����: ����                                                       */
/* ������� �����: ����                                                                  */
/**********************************************************************************/
void EndKeyb(void)
{
      EndBuffer(kb);//����������� ������� �� ���. �����
      EndBuffer(infbuffer);//����������� ������� �� ASCII ������ 
      EndPos(Inpos); //����������� ������� �� ��������� � ��������� �� ���.������
  //    EndTable();
      _dos_setvect(0x09,old9);//���� ������ �� ��.����������
}

/******************************************************************************/
/* ReadASCII:���� ASCII ���� �� ������ �� ������ � ASCII ������   */
/* ������ �����: ����                                                       */
/* ������� �����:���������� �� ASCII ����                                    */
/**********************************************************************************/
byte ReadASCII(void)
{
   byte k=0;//������� ����
       __asm
       {
       push es  
       push ds
       lds bx,infbuffer
       les di,ds:[bx].scanbuf
       mov di,ds:[bx].scannext
       cmp di,ds:[bx].scanlast
       je gsend
       mov al,es:[di]
       mov k,al    
       inc di    
       les si, ds:[bx].scanend
       cmp di,si
       jne gsd2
       les si,ds:[bx].scanbuf 
       mov di,si
gsd2:   mov ds:[bx].scannext,di                         
gsend: pop ds
	pop es
       }              
       return k;
}
       

/******************************************************************************/
/* new9: ��� ������ �� ���������� 0x9 - ����������� ���������� */
/* ������ �����: ����                                                       */
/* ������� �����:����                                                         */
/**********************************************************************************/
void __interrupt __far newi9(void)
{
  byte k=0;
   __asm
   {
	//    cli
	    sti
	    push ax 
	    push dx
	    mov dx,60h  
	    in al,dx //���� ���� 60H

	    cmp al,128
	    jb  i9begin
	    jmp i9end
i9begin:        push di
	    push es  
	    push ds
	    lds bx ,kb
	    les di,ds:[bx].scanbuf
	    mov di,ds:[bx].scanlast
	    mov es:[di],al
	    mov k,al
	    inc di   
	    les si, ds:[bx].scanend
	    cmp di,si
	    jne i9nowrap
	    les si,ds:[bx].scanbuf
	    mov di,si
i9nowrap:mov ds:[bx].scanlast,di
	    pop ds
	    pop es
	    pop di

	    push ds
	    push si
	    push 40h
	    pop ds
	    mov si,17h
	    xor ah,ah
	    mov ah,ds:[si]
	    //����� �� ����� �� ��������� �� ������������
	    cmp al,54  
	    je e1
	    and  ah,0feh
	    jmp e2                
e1:       or ah,1
e2:       cmp al,42
	    je e3
	    and ah,0fdh
	    jmp  e4
e3:       or ah,02h
e4:       cmp al,29
	    je e5
	    and ah,0fbh
	    jmp  e6
e5:       or ah,04h
e6:       cmp al,56
	    je e7
	    and ah,0f7h
	    jmp  e8
e7:       or ah,08h
e8:       cmp al,70
	    jne e11
	    mov bl,ah
	    and bl,10h
	    cmp bl,0
	    je e10
	    and ah,0efh
	    jmp e11
e10:      or ah,10h
e11:     cmp al,58
	       jne e13
	       mov bl,ah
	       and bl,40h
	 cmp bl,0
		je e12
		and ah,0bfh
		jmp e13
e12:      or ah,40h
e13:     cmp al,82
	       jne e9
	       mov bl,ah
	       and bl,80h
	 cmp bl,0
		je e14
		and ah,7fh
		jmp e9
e14:      or ah,80h
e9:       jmp gs4
gs4:     mov ds:[si],ah
	     pop  si
	 pop ds
i9end:  pop dx
	     pop ax
      }             
      if(k!=0)
       SignalSem(1);//������ ,�� ��� ���. ��� �� ������ � ���. �����
       __asm
       {
	    sti             //��������� �� �������������
	    mov al,20h  
	    out 20h,al
       }                            
}
       
  
 

/******************************************************************************/
/* ReadKey:���� ���. ���� �� ������ �� ������ �� ���.������   */
/* ������ �����: ����                                                       */
/* ������� �����:���������� �� ��� ����                                    */
/**********************************************************************************/
byte ReadKey(void)
{

    byte k=0;
      __asm
      {
       push es  
       push ds
       lds bx,kb
       les di,ds:[bx].scanbuf
	mov di,ds:[bx].scannext
	cmp di,ds:[bx].scanlast
	je gs1
	mov al,es:[di]
	mov k,al    
	inc di    
	les si, ds:[bx].scanend
	cmp di,si
	jne gs2
	les si,ds:[bx].scanbuf 
	mov di,si
gs2:   mov ds:[bx].scannext,di                         
gs1:   
      pop ds
      pop es
      }         
      return k;
}

/******************************************************************************/
/* CRF :����������  CR ������� -���������� �� ������-��������� ����*/
/* ������ �����: ����                                                       */
/* ������� �����:����                                                                  */
/**********************************************************************************/
void CRF(void)
{                                         
  if(*wASCII!=NULL) /* ��������� ���� �� ������-������� ���� */
  {
      int len=strlen(*wASCII);      /* ������� �� ������ */
      memset(*wASCII,' ',len); /* ���������� � ' ' */
      *(*wASCII+len)='\0'; /* ���� �� ������ - ���� ����� */
      *col=0; //������������� ��������� �� ������� ������ �������� �� ������ � 0
  }
  SignalSem(501);
}
/******************************************************************************/
/* FKRight :���������� ������� ��� ���������� �� �����. ������� -�������*/
/* ������ �����: ����                                                       */
/* ������� �����:����                                                                  */
/**********************************************************************************/
void FKRignt(void)
{
    if (*posl>=10 )    
   {
       *posl=*posl-10;   //�������� � 10 ������ 
       *aKey=NULL;//���� ������� ������������ ������ ������� ������ 10
    }
}        

/******************************************************************************/
/* FKLeft :���������� ������� ��� ���������� �� �����. ������� -������*/
/* ������ �����: ����                                                       */
/* ������� �����:����                                                                  */
/**********************************************************************************/
void FKLeft(void)
{
       if(*posl>=0 && *posl < (*dk).nm -10)
       {
	     *posl=*posl+10; /* ��� ��� �����. ��� ��������� � 10 ������ */
	     *aKey=NULL;//���� ������� ������������ ������ ������� ������ 10
       }
       if(*posl +5 >=(*dk).nm && *post==1)
	    *post=0;      //��� ���� �� �. ��. �� ������� �� ����� �������� -���� �����
}

/******************************************************************************/
/* FKTop:���������� ������� ��� ���������� �� �����. ������� -����� ��������*/
/* ������ �����: ����                                                       */
/* ������� �����:����                                                                  */
/**********************************************************************************/
void FKTop(void)
{       
   if(*post>=0 && *post<1 && (*dk).nm>5 && *posl +5 < (*dk).nm )
   {   // ��� ��� �����. ��. �� ����� ��������
	 *post=*post+1;
	 *aKey=NULL;//���� ������� ������������ ������ ������� ����  �� ����� ��������
      }
      else
      {
	if(*post<=1 && *post>0)
	{    
	*post=*post-1;
	  *aKey=NULL;//���� ������� ������������ ������ ������� ����  �� ����� ��������
	}
      }

}

/******************************************************************************/
/* FKBottom:���������� ������� ��� ���������� �� �����. ������� -����� ��������*/
/* ������ �����: ����                                                       */
/* ������� �����:����                                                                  */
/**********************************************************************************/
void FKBottom(void)
{       
    if(*post<=1 && *post>0)
      {    
	*post=*post-1;
	  *aKey=NULL;//���� ������� ������������ ������ ������� ����  �� ����� ��������
     }
}

void SlowReadInput(void)
{
    while(TRUE)
    {

      flag_change=FALSE;
      ReadInput();
      ReadSerInput();//������ �� ������� �� ASI
      for(unsigned i=0;i<numbinpPIA;i++)//���� �������
       *(curtable+i)=input(i+1) ;
      if(numbtable<2)
       numbtable++;
     else
       numbtable=0;
     curtable=table[numbtable];        
	 Delay();
    }
}   

void Filter(void)
{      
   //������ 2 �� 3 -������ �� ��� ������� ��������� �� 3
   unsigned binput=numbls +2*(numbas +numbts);
   for(unsigned i=0;i<numbinpPIA;i++)
   {
      if(*(table[0]+i)==*(table[1]+i)) //��������� �� �������� �� i ���� �� 1 � 2 �������
      *(tsem +binput +i)=*(table[0]+i);
      else
      {
	if(*(table[0]+i)==*(table[2]+i)) //��������� �� �������� �� i ���� �� 1 � 3 �������
	    *(tsem +binput +i)=*(table[0]+i);
	else
	    *(tsem +binput +i)=*(table[1]+i);//�������  ��������� �� i ���� �� 2 � 3 �������
       }
     }
	  input.cur=&input.top;
	  input.find(numbinpPIA+1);
     for(i=numbinpPIA;i<input.nm;i++)
     {               
	 *(tsem +binput +i)=(*input.cur)->value;
	 input.cur= &(*input.cur)->next;
     }
    

}

   

void SemInput(void)
{
   unsigned numb;        
   byte value;
   int par=0;
   for(int i=0; i<35;i++)
   {           
      numb=inpar[i];
      value=Input(numb);
       if(i==30 && value==0)
	  value=0;
      if(value==0x01)
	SignalSem(300+i);
      else
	  NoSignalSem(300+i);
    }
}

void IniInputPar(void)
{     
  unsigned numb;
  int par;
  for(int i=0;i<70;i++)
  {
       par=3000+i;
	if( ReadParamMem(par,'u',&numb)!=FALSE)
	       inpar[i]=numb;
  }
}
/**************************************************************************************/
/*IniSerialPort:������������� �� ����������� ������ ���������      */
/* ���� ������ ���������� � ��� ��������������                             */
/**************************************************************************************/
void IniSerialPort(void)
{ 
	unsigned numb=1;//�� ������������ COM1;
	union REGS regs;
	if( ReadParamMem(41,'u',&numb)==FALSE)
	       ExitSys();
	
	 if(numb==1)//COM1
	 {
	    adrescom=0x3f8;
	 regs.x.dx=0;      
      }
     else
     {
		if(numb==2)//COM2
		{
		adrescom=0x2f8;
		regs.x.dx=1;       
	}           
	else
	  return;
     }

  regs.h.ah=0;
  regs.h.al=0x83;//������������� -1200bps,��� �������,8 ���� ����� ,1 ���� ���
  int86(0x14,&regs,&regs);
  
  _outp(adrescom+1,0x1);//����������� �� ���������� ��� ���������� �� �����
  _outp(adrescom+4,0xb);//����������� �� ���������� �� ������ - ��� �� � ��������� �� ������� ������������ �� UART8250
  IniSerialInput();
}

