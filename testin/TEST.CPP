//�������� �� ���� �� ������-��������� ������ 
//������� , � ����� �� ������ ��� ���������� (��������� ��� ������� �� �� ���������) �� �������� �� DISP    
//���. 30 ������ ������������  (��� � 8 -��������� ������, ���� ����� �������� -������ "����"
//1. �� ��������� ������
//2. �� ������ "����"

#include <stdlib.h>  
#include <new.h>   
#include "comst.h"
#include "dispnew.h"   
#include "irq9new.h"                 
#include "screen.h"
#include "string.h"
#include "..\include\pia.h"    
#include "..\include\usepia.h"
#include "..\include\videoc.h"
#include "..\include\svga.h"
#define TESTIN
byte colvfield;  

int postprimer1;
int poslprimer1;
void PrintOutput(void);
void PrintInput(void);
void DrawTestScreen(void)  ;
void DarkCR(void);
void OutputCR(void);
extern ListNode listnode;
extern unsigned numbinpPIA;
  void CRQUIT(void)
{    
   flagend=FALSE;
}
FuncKey ftest1("OLight",NULL,OutputCR);
FuncKey ftest2("ODARK",NULL,DarkCR);
FuncKey ftest3("QUIT",NULL,CRQUIT);
ListKey functest;/* ������ �� ��������� ��� ������ FuncKey  - ListKey e ���� */
void CreateListFuncKey(void)
{
    functest.add(&ftest1);
    functest.add(&ftest2);
    functest.add(&ftest3);
}
ListVF listst; //������ �� ����������� �������� ������ 
VField vfield(1,1,3,30,0x70,NULL,NULL); //������ - ������� ����
ListVF vfieldoutput; //������ �� ���������� ������

void Inivariable(void)
{
	    dk=&functest;  /* ������������� �� ��������� �� ������� ������������ ������� */
//            aKey= //  ������� ������������ ����
	  col=&colvfield;  // ����. �� �������� ������ �� ������-������� ����
	  wASCII=&vfield.ptext; //����. �� ��������� �� ������� ������-������� ����
//            MKBottom=  //����. �� ��������� �� ������� �� ������ "������" 
//            MKTop=     //����. �� ��������� �� ������� �� ������ "������" 
//        MKRight=  //����. �� ��������� �� ������� �� ������ "�������" 
//        MKLeft=   //����. �� ��������� �� ������� �� ������ "������" 
	  post=&postprimer1;
	  posl=&poslprimer1;
	  PrintEnterFields=PrintInput;   //����. �� ��������� �� �������  ������� �� ������-������� ����
	  PrintExitFields=PrintOutput ; //����. �� ��������� �� ������� ������� �� ������� ������ 

	CRFUNC=CRF; //����. �� ��������� �� ������� CR 
}

void Idle(void)
{
  while (flagend!=FALSE)
  {
    flag_change=TRUE;
    whlt();                              
    
  }
   // ExitSys();
   __asm jmp EndTask;

} 
void IniSem(void)  
{            
   size_t razm;        
   char namef[20];
       FILE *f;//�������� ��� ����
    f=fopen("tablepar.dat","r"); //���� � ��� �� ����� � ���������
	if(f!=NULL)//��� ����
	{
		fscanf(f,"%s",namef);//��� 
		if(*namef!=' ')//��� ���
			listnode.load(namef);  //������� ����������� �� ���� � �������
	    else
	    listnode.load("t1.par");  //������� ����������� �� ���� � �������               
		 fclose(f);//��������� �� �����
   }     
   else   
	listnode.load("t1.par");  //������� ����������� �� ���� � �������           
   
   if(ReadParamMem(1,'i',&numbls)==FALSE)
      printf("ERROR");
   if(ReadParamMem(2,'i',&numbas)==FALSE)
      printf("ERROR");   
   if(ReadParamMem(3,'i',&numbts)==FALSE)
      printf("ERROR");
   if(ReadParamMem(4,'i',&numbis)==FALSE)
      printf("ERROR");
   razm=numbls +2*(numbas + numbts) +numbis;
  
   for(unsigned i=0;i<razm;i++)
    *(tsem +i)=0;     /* ��������  �������� */
    //������� ������������� �� ���������
   /* ������������� �� ���������� */  
    if(ReadParamMem(30,'i',&numbirq)==FALSE)//��o� ��������-�������>���: �� �� ����� ������������ ����� �� ���������
      printf("ERROR");  
      if(numbirq==8) //�������� �� ��������� ������
      {
        ExitSys=ExitSys8;  
        Ini=Ini8;
      }
      else     
      {
       ExitSys=ExitSysT; 
       Ini=IniT;       
   
      }   
   
   NoSignalSem(1);//���� ������ � ��.�����
   SignalSem(500);     
   SignalSem(501);     
   TimeSem(2001,30);//����� �� ��������� �� ������ SlowKey
   AritSem(1000,1);//����� �� �������� �� �������
   AritSem(1001,0);  //��� �� ������� �� �������
}    
  

void main (void)
{  
   IniSem();
   unsigned Time=0;
   if(ReadParamMem(1033,'u',&Time)==FALSE)
      Time=50;
    kboard=CreateTask(&"Key  ",Timed,15,Slow,Time,4000,SlowKey); // �������� ����������  
    if(ReadParamMem(1034,'u',&Time)==FALSE)
	Time=8;
    inpi=CreateTask(&"Input",Timed,16,Slow,Time,3024,SlowReadInput); // ��������  ������� 
    if(ReadParamMem(1035,'u',&Time)==FALSE)
	Time=60;
    screen=CreateTask(&"Print",Timed,14,Slow ,Time,4024,SlowPrint); // ����� ����� 
    idle=CreateTask(&"Idle ",Ready,2,Slow ,0,2000,Idle); //������ ������
    CreateListFuncKey(); //MAN
    IniEkran(0); /* ����. �� ������ */
    Inivariable();
    IniKeyb();  
	DrawTestScreen()  ;
    Ini();       //�� ��������� ������
    whlt();
    exit(0);
}
  

void DrawTestScreen(void)  
{                             
  int Page=0;                             
  char buffer[10];
  unsigned k=1,numb=0;
  dprint(Page,35 ,0,0x70,&"T E S T"); 
    for(unsigned i=1; i<=input.nm;i++)
    {                          
       if(i> 18*k)
	     k++;                            
	     if(i <= numbinpPIA)
	   sprintf(buffer,"InPIA%d",i);
	 else
	   sprintf(buffer,"InASI%d",i);
	 dprint(Page,20*(k-1) ,1+i -18*(k-1),0x07,buffer); 
	  vfieldoutput.add(i, 1 +i -18*(k-1),10 +20*(k-1),2,0x7,NULL);
    }
//     DrawFuncFrame(Page);
}  

void PrintOutput(void)
{                          
  unsigned value;
  for(unsigned i=1;i<=input.nm;i++)
  {
     value=(unsigned)input(i);
    PrintField( &vfieldoutput,i,'u',&value); //����� �� ������ �� 1 �����     
   }  
 }

void PrintInput(void) 
{
  PEnterField(&vfield);
}

void OutputCR(void)
{                   
   int outnumb;
    strFindInt(*wASCII,&outnumb);
    output(outnumb,1);
    CRFUNC=CRF;
    *aKey=NULL;
    CRFUNC();
}

void DarkCR(void)
{                   
   int outnumb;
    strFindInt(*wASCII,&outnumb);
    output(outnumb,0);
    CRFUNC=CRF;
    *aKey=NULL;
    CRFUNC();
}
