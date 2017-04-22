//Програма за тест на входно-изходната платка 
//Вариант , в който се указва кое прекъсване (съответно кои функции да се използват) се използва за DISP    
//Пар. 30 указва прекъсването  (ако е 8 -системния таймер, всяк друга стойност -платка "Иван"
//1. От системния таймер
//2. От платка "Иван"

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
ListKey functest;/* списък от указатели към обекти FuncKey  - ListKey e клас */
void CreateListFuncKey(void)
{
    functest.add(&ftest1);
    functest.add(&ftest2);
    functest.add(&ftest3);
}
ListVF listst; //списък на постоянните текстови полета 
VField vfield(1,1,3,30,0x70,NULL,NULL); //входно - изходно поле
ListVF vfieldoutput; //списък на защитените полета

void Inivariable(void)
{
	    dk=&functest;  /* инициализация на указателя за активни фънкционални клавиши */
//            aKey= //  активно функционално поле
	  col=&colvfield;  // иниц. на текущата колона на входно-изходно поле
	  wASCII=&vfield.ptext; //иниц. на указателя за активно входно-изходно поле
//            MKBottom=  //иниц. на указателя за функция на клавиш "надалу" 
//            MKTop=     //иниц. на указателя за функция на клавиш "нагоре" 
//        MKRight=  //иниц. на указателя за функция на клавиш "надясно" 
//        MKLeft=   //иниц. на указателя за функция на клавиш "наляво" 
	  post=&postprimer1;
	  posl=&poslprimer1;
	  PrintEnterFields=PrintInput;   //иниц. на указателя за функция  дисплей на входно-изходно поле
	  PrintExitFields=PrintOutput ; //иниц. на указателя за функция дисплей на изходни полета 

	CRFUNC=CRF; //иниц. на указателя за функция CR 
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
       FILE *f;//указател към файл
    f=fopen("tablepar.dat","r"); //файл с име за файла с параметри
	if(f!=NULL)//има файл
	{
		fscanf(f,"%s",namef);//име 
		if(*namef!=' ')//има име
			listnode.load(namef);  //зарежда параметрите от файл в паметта
	    else
	    listnode.load("t1.par");  //зарежда параметрите от файл в паметта               
		 fclose(f);//затваряне на файла
   }     
   else   
	listnode.load("t1.par");  //зарежда параметрите от файл в паметта           
   
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
    *(tsem +i)=0;     /* нулиране  семафори */
    //НАЧАЛНА инициализация на системата
   /* инициализация на семафорите */  
    if(ReadParamMem(30,'i',&numbirq)==FALSE)//брoй семафори-входове>ЗАБ: да се сложи действителен номер на параметър
      printf("ERROR");  
      if(numbirq==8) //използва се системния таймер
      {
        ExitSys=ExitSys8;  
        Ini=Ini8;
      }
      else     
      {
       ExitSys=ExitSysT; 
       Ini=IniT;       
   
      }   
   
   NoSignalSem(1);//няма клавиш в кл.буфер
   SignalSem(500);     
   SignalSem(501);     
   TimeSem(2001,30);//време за повтаряне на задача SlowKey
   AritSem(1000,1);//номер на парамтър за дисплей
   AritSem(1001,0);  //вид на данната за дисплей
}    
  

void main (void)
{  
   IniSem();
   unsigned Time=0;
   if(ReadParamMem(1033,'u',&Time)==FALSE)
      Time=50;
    kboard=CreateTask(&"Key  ",Timed,15,Slow,Time,4000,SlowKey); // програма клавиатура  
    if(ReadParamMem(1034,'u',&Time)==FALSE)
	Time=8;
    inpi=CreateTask(&"Input",Timed,16,Slow,Time,3024,SlowReadInput); // програма клавиатура  
    if(ReadParamMem(1035,'u',&Time)==FALSE)
	Time=60;
    screen=CreateTask(&"Print",Timed,14,Slow ,Time,4024,SlowPrint); // обява екран 
    idle=CreateTask(&"Idle ",Ready,2,Slow ,0,2000,Idle); //празна задача
    CreateListFuncKey(); //MAN
    IniEkran(0); /* иниц. на екрана */
    Inivariable();
    IniKeyb();  
	DrawTestScreen()  ;
    Ini();       //за системния таймер
    whlt();
    exit(0);
}
  

void DrawTestScreen(void)  
{                             
  int Page=0;                             
  char buffer[10];
  int k=1,numb=0;
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
    PrintField( &vfieldoutput,i,'u',&value); //печат за работа по 1 начин     
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
