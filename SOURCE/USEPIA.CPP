#include <conio.h>      
#include <io.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <memory.h>                      
#include <string.h>
#include <malloc.h>
#include "..\include\pia.h"
#include "..\include\usepia.h"
#include "..\include\comv.h"
#include "..\include\table.h"
#include <stdio.h>     
 unsigned numbinpPIA;
byte poten[4];//4 потенциометъра
ListPia  listpia;
extern List input;
extern  List output;                         
char name[10]="p0.dat";
int ff;

int adresc;
byte numboutput[8]={0,0,0,0,0,0,0,0};//само за матрично свързване номера на изходи/пореден номер на бит/ ,участвуващи в матрица
unsigned l_value;              

void InitPiaLpt ( unsigned adres , unsigned tp,byte typebind )
{
        //при работа с LPT - инициализацията е направена и се знае :А-изходи, C1-изходи, B ot 3-8 входове)  за тип-2 (защото 0-линия, 1-матрица , 2и 3 са за Lpt)
        //при тип 3-А-вхос, C1-вход, B ot 3-8 изход 
      if(tp==2)
      {
     		//	byte noutput=4;/* брой изходи по портове C1 */        
     		byte   noutput=12;/* брой изходи по портове C1+A */       
    			byte ninput=5;  /* брой входове по портове B3-B8 */
     			listpia.add('p',adres,1,0,1,2,0,0);
      }
      else  //ne e testwan
      {      
     			byte noutput=5;   /* брой изходи по портове B3-B8 */
    			byte ninput=12;   /* брой входове по портове-A, C1 */ 
     			listpia.add('p',adres,0,1,0,2,0,0);     
      }
       
}    
        
void InitPiaLptMatrix ( unsigned adres1)
{
        //при работа с LPT - инициализацията е направена и се знае :А-изходи, C1-изходи, B ot 3-8 входове)  за тип-2 (защото 0-линия, 1-матрица , 2и 3 са за Lpt)
        //при тип 3-А-вхос, C1-вход, B ot 3-8 изход      
        listpia.add('c', adres1,0,0,0,0,0,0);//създаване на обект MatrixLpt 'c' -кръстосна матрица
}            
        
void InitPia(unsigned adres,byte typeA,byte typeB,byte typeC1,byte typeC2,
                 byte typebind,byte mi,byte mo )
{
    byte control_word=0x80;
    byte noutput=0;/* брой изходи по портове */
    byte ninput=0;  /* брой входове по портове */
    if(typeA==1)   /* портА - изход */
       noutput=noutput + 8;
    else    
    { 
      control_word=control_word |  0x10;
      ninput=ninput + 8;
    }
    if(typeB==1) /* порт B -изход */
       noutput=noutput + 8;
    else
    {
      ninput=ninput + 8;
      control_word=control_word | 0x2;
    }
    if(typeC1==1)
       noutput=noutput + 4;
    else
    {
      ninput=ninput + 4;
       control_word=control_word | 0x1;
     }
    if(typeC2==1)
       noutput=noutput + 4;
    else
    {
      ninput=ninput + 4;
        control_word=control_word | 0x8;
      }
    
    _outp(adres+3,control_word);//контролна дума за ПИА
    _outp(adres,0xff);//тази схема е 0 -свати,1- изгасва
    listpia.add(typebind,adres,typeA,typeB,typeC1,typeC2,mi,mo);
}       

void EndPIA(void)
{  
  listpia.cur=&listpia.top;
   while(*listpia.cur)
   {
       _outp((*listpia.cur)->baseadres,0xff);//тази схема е 0 -свати,1- изгасва    
       listpia.cur=&(*listpia.cur)->next;
   }
}
void   DiscrMatrix(unsigned adres,byte nA,byte nB,byte nC1,byte nC2)
{
  listpia.cur=&listpia.top;
  listpia.find(adres);
  if(!*listpia.cur )
    return ;
  if((*listpia.cur)->kind()==0)
    ( (Matrix*)(*listpia.cur))->TMatrix(nA,nB,nC1,nC2);   
}
    

void ReadInput(void)
{
   listpia.cur=&listpia.top;
   byte *hinput;/* помощен указател към данните на PIA */
   unsigned cur_number=1; /* текущ номер на вход и маска */
   Base *cur_input=NULL;
   unsigned i=0; /* текущ номер на на бит ит PIA */\
   input.cur=&input.top;
   while(*listpia.cur)
   {
      (*listpia.cur)->read();   
      switch ((*listpia.cur)->kind())
      {
         case 0:
           hinput=((Matrix *)(*listpia.cur))->pinput; /* насочване на данните от PIA тип матрица */
           break;
         case 1:
            hinput=((Linea *)(*listpia.cur))->pinput;/* насочване на данните от PIA тип линия  */
           break;  
          case 2:
           hinput=((LineaLpt *)(*listpia.cur))->pinput;/* насочване на данните от PIA тип линия  */
           break;         
          case 3:
           hinput=((MatrixLpt *)(*listpia.cur))->pinput;/* насочване на данните от PIA тип линия  */
           break;               
         default:
          perror("Nodifaned Pia");  /* недефинирано свързване */
      } 
      i=0;  
      if(hinput!=NULL)
      {
      while(*(hinput+i)!=0xA5)
      {  
         cur_input=(*input.cur);
         __asm
         {
           push ds
           push es
           push si
           push di
           les si,hinput //стойност на входовете
           add si,i
           lds di,cur_input//обекти входове
           add di,3
           mov al,ds:[di]//маска
           xor al,es:[si]// москиране на новата стойност-  излючващо "или"
           dec di   //di-->стойност на входа
           shr al,1 //запис в CF на 0бит
           jnb et1
           mov ds:[di],0x1//нова стойност = прочетена и маскирана
           jmp end
      et1:mov ds:[di],0x0//нова стойност = прочетена и маскирана
      end:
          inc i
           pop di
           pop si
           pop es
           pop ds
      }
      input.cur=&(*input.cur)->next;
      }
   }
      listpia.cur=&(*listpia.cur)->next;
   }
}



void ReadParam(void)
{      
    unsigned i=0;
    unsigned numbPIA=0;/* брой PIA - активни */
    unsigned adres=0; /* адрес на PIA */
    byte tA=0,tB=0,tC1=0,tC2=0;/* туп на портове A ,B ,C1, C2 */
    byte nA=0,nB=0,nC1=0,nC2=0;  /* брой крачета за свързване от тип матрица */
    byte ni=0;/* общ брой входни крачета тип матрица */
    byte no=0;/* общ брой изходни крачета тип матрица */
    byte type=0;/* тип на свъпзване 0- линия,1-матрица */
    if((ff=_open(name,_O_TEXT|O_RDONLY))==-1)
    {
         perror("NO fail");
         return;
    }
    numbPIA=(unsigned)ReadParam(1);
    for( i=1;i<=numbPIA;i++)
    {  
       adres=(unsigned)ReadParam(10*i +1);       /* чете адрес на PIA */ 
       tA=(byte)(ReadParam(10*i+2)); /* чете тип на порт А */
       tB=(byte)(ReadParam(10*i+3)); /* чете тип на порт B */
       tC1=(byte)(ReadParam(10*i+4));/* чете тип на порт C1 */
       tC2=(byte)(ReadParam(10*i+5));/* чете тип на порт C2 */
       type=(byte)(ReadParam(10*i+6));         
       
       
       if(type==1)
       {
         nA=(byte)(ReadParam(10*i+7));
         nB=(byte)(ReadParam(10*i+8));
         nC1=(byte)(ReadParam(10*i+9));
         nC2=(byte)(ReadParam(10*i+10));
         if(tA==0)
            ni=ni+nA;
         else
            no=no+nA;
         if(tB==0)
            ni=ni+nB;
         else
            no=no+nB;
         if(tC1==0)
            ni=ni+nC1;
         else
            no=no+nC1;
         if(tC2==0)
            ni=ni+nC2;
         else
            no=no+nC2;
    InitPia(adres,tA,tB,tC1,tC2,'m',ni,no);/* инициализациа на PIA ,свързване тип матрица */
    DiscrMatrix(adres ,nA,nB ,nC1,nC2); /* описание на матрицата - брой крачета от всеки тип */           
    }       
    else
      if (type==0)
      		InitPia(adres,tA,tB,tC1,tC2,'l');/* инициализациа на PIA ,свързване тип линия */   
        
    
   }
  _close(ff);
}

 double  ReadParam(long num)
 {
      char *buff1;
      buff1=(char*)(malloc)(15);
      *(buff1+14)='\0';
      memset(buff1,' ',14);
      long pos,ps;                    
      double value=0;
      ps=(num-1)*14;
      _lseek(ff,ps,SEEK_SET);
      pos=_read(ff,buff1,14);
      if(pos==0)
         value=0.0;         
      else
         sscanf(buff1,"%lf",&value);
       return value;
 }

void IniMask(void)
{     
  byte mask=0;  /* стойност на текущата маска */
  listpia.cur=&listpia.top;
   if((ff=_open("p1000.dat",_O_TEXT|O_RDONLY))==-1)
   {
         perror("NO fail");
         return;
    }
   input.cur=&input.top;
   for (unsigned i=1;i<=input.nm;i++)
   {
         mask=(byte)ReadParam(i);       /* чете поредната маска */ 
         (*input.cur)->mask=mask;
       input.cur=&(*input.cur)->next;
    }
    _close(ff);
   if((ff=_open("p2000.dat",_O_TEXT|O_RDONLY))==-1)
   {
         perror("NO fail");
         return;
    }
   output.cur=&output.top;
   for ( i=1;i<=output.nm;i++)
   {
         mask=(byte)ReadParam(i);       /* чете поредната маска */ 
         (*output.cur)->mask=mask;
       output.cur=&(*output.cur)->next;
    }
        _close(ff);
   if((ff=_open("p3000.dat",_O_TEXT|O_RDONLY))==-1)
   {
         perror("NO fail");
         return;
    }  
   listpia.cur=&listpia.top;
   byte *pmask=NULL;/* указател към мските в блока с данни за матричните входове */
   while(*listpia.cur)
   {
      if((*listpia.cur)->kind()==0)
      {
        pmask=((Matrix *)(*listpia.cur))->potable;
        for( i=0;i< ((Matrix *)(*listpia.cur))->moutput;i++)
        { 
          mask=(byte)ReadParam(i+1);
          *(pmask + 3+4*i)=mask;
         }
       }
       listpia.cur=&(*listpia.cur)->next;
    }
}

void ReadParamIniIO(void )
{
    unsigned i=0;
    unsigned numbPIA=0;/* брой PIA - активни */
    unsigned adres=0; /* адрес на PIA */
    byte tA=0,tB=0,tC1=0,tC2=0;/* туп на портове A ,B ,C1, C2 */
    byte nA=0,nB=0,nC1=0,nC2=0;  /* брой крачета за свързване от тип матрица */
    byte ni=0;/* общ брой входни крачета тип матрица */
    byte no=0;/* общ брой изходни крачета тип матрица */
    byte type=0;/* тип на свързване 0- линия,1-матрица */
    ReadParamMem(40,'u',&l_value);
    ReadParamMem(50,'u',&numbPIA);    
    int j;//помощна променлива
    for( i=0;i<numbPIA;i++)
    {  
       ReadParamMem(60 +20 *i +1,'u',&adres);/* чете адрес на PIA */ 
       ReadParamMem(60+20*i +2,'u',&tA);/* чете тип на порт A*/
       ReadParamMem(60+ 20*i +3,'u',&tB);      /* чете тип на порт B */
       ReadParamMem(60+ 20*i +4,'u',&tC1);/* чете тип на порт C1 */
       ReadParamMem(60 + 20*i +5,'u',&tC2);/* чете тип на порт C2 */
       ReadParamMem(60 +20*i +6,'u',&type);             
       
       switch (type)
       {
         case 0:  //линия
           InitPia(adres,tA,tB,tC1,tC2,'l');/* инициализациа на PIA ,свързване тип линия */
           break;
         case 1:     //матрица
          ReadParamMem(60+20*i +7,'u',&nA);
          ReadParamMem(60 +20*i +8,'u',&nB);
          ReadParamMem(60+20*i +9,'u',&nC1);
          ReadParamMem(60+20*i +10,'u',&nC2);
          if(tA==0)
            ni=ni+nA;
          else
            no=no+nA;
          if(tB==0)
            ni=ni+nB;
          else
            no=no+nB;
          if(tC1==0)
            ni=ni+nC1;
          else
            no=no+nC1;
          if(tC2==0)
            ni=ni+nC2;
          else
            no=no+nC2;
          for( j=0;j<8;j++)
               ReadParamMem(71+20*i + j,'u',&numboutput[j]);  //битове от порта ,обрезуващи матрични изходи              
          InitPia(adres,tA,tB,tC1,tC2,'m',ni,no);/* инициализациа на PIA ,свързване тип матрица */
          DiscrMatrix(adres ,nA,nB ,nC1,nC2); /* описание на матрицата - брой крачета от всеки тип */           
          break;      
          
          case 2:      
          case 3:
            InitPiaLpt(adres,type,'p');/*LPT */
          break;
           
           case 11://свързване на матрица- кръстосано lpt1 i lpt2 -трябва да се знаят само 2 адрес -останалото е заложено "твърдо"
              InitPiaLptMatrix(adres);/*LPT */  
              DiscrMatrixLpt(adres);
       break;
        
         default:
          perror("Nodifaned Pia");  /* недефинирано свързване */
      } 
   }                      
   IniIOMask();
    numbinpPIA=input.nm;//запис на броя входове от PIA
    for(i=0;i<output.nm;i++)
     output(i,0);//нулиране на изходите  
     //добавена поправка на 26.02.2013
    output(7,1);//7 изход да свети      за Киро
     
}


void IniIOMask(void)
{     
  byte mask=0;  /* стойност на текущата маска */
  listpia.cur=&listpia.top;
   input.cur=&input.top;
   for (unsigned i=1;i<=input.nm;i++)
   {
         ReadParamMem(200+i,'i',&mask);/* чете поредната маска на вход*/ 
         (*input.cur)->mask=mask;
         input.cur=&(*input.cur)->next;     
         mask=0;
    }
   output.cur=&output.top;
   for ( i=1;i<=output.nm;i++)
   {
         ReadParamMem(300+i,'i',&mask);/* чете поредната маска на изход*/ 
         (*output.cur)->mask=mask;
         output.cur=&(*output.cur)->next;
         mask=0;
    }
   listpia.cur=&listpia.top;
   byte *pmask=NULL;/* указател към маските в блока с данни за матричните изходи -адрес на изхода,номер,маска */
   while(*listpia.cur)
   {
      if((*listpia.cur)->kind()==0)
      {
        pmask=((Matrix *)(*listpia.cur))->potable;
        for( i=0;i< ((Matrix *)(*listpia.cur))->moutput;i++)
        { 
          ReadParamMem(400+i,'i',&mask);/* чете поредната маска на матричен изход */ 
          *(pmask + 3+4*i)=mask;//винаги 3 данна в таблицата с данни за матр. изходи
          mask=0;
         }
       }
       listpia.cur=&(*listpia.cur)->next;
    }
}

void IniSerialInput(void)
{
  numbinpPIA=input.nm;//запис на броя входове от PIA
  //създаване и описание  на входовете на ASI
  unsigned numb;
  unsigned numbASIA=0;
  byte mask=0;
  int j=0;
  int adres;
  ReadParamMem(41,'i',&numb);  //номер на COM : 1-COM1;2-COM2
  if(numb==1)
     adresc=0x3f8;
  if(numb==2)
     adresc=0x2f8;
  numb=input.nm+1;    
  ReadParamMem(42,'i',&numbASIA);  //брой входове от ASIA
  for(unsigned i=numb;i<numb+numbASIA;i++)//броя входове от ASIА е 63
  { 
    j++;
    input.add();
    ReadParamMem(100+numbinpPIA +j,'i',&mask);/* чете поредната маска на вход, след входове от PIA*/ 
    (*input.cur)->mask=mask;//маска
    (*input.cur)->adres=adres;//адрес на асинхранния адаптер
  }
  
}
/******************************************************************************/
/*  ReadSerInput: Функция за прочитене на серийните входове и потенциометри */
/* Входни данни: Няма                                                                               */
/* Изходни данни: Резултат от функцията:                                                   */
/*                         0-няма натиснат сериен вход                                             */
/*                          1- вход                                                                              */
/*                          2-потенциометър                                                               */
/****************************************************************************************/
byte ReadSerInput(void)
{
 //COM 1 ->0x3f8
  input.cur=&input.top;
  byte n=0,c=0,dd=0,d=0;
//  int adres= input.top->adres;
  int numb=numbinpPIA;//начало на серийните входове
  if((_inp(adresc+5) & 1)==1)//проверява се регистъра за състояние на линията бит0=1,ако има данни за четене
  {      
   n=_inp(adresc);//данната изпратена по серийния канал
   if((n & 0x80)==0) // бутон
   {   
     if((n & 0x40)==0)// натиснат
     {
        c=n;//номера на натиснатия бутон
         input.find(numb+c);
        (*input.cur)->value=(1 ^ (*input.cur)->mask);
     }
     else //отпуснат бутон
     {
        n=n<<2;
        c=n>>2;
       input.find(numb+c);
        (*input.cur)->value=(0 ^ (*input.cur)->mask);
      }
      return 1;
   } 
   	else //потенциометър
	{
					dd=c=n;
					d=c<<1;//изчистване на 7 бит
					d=d>>6;//извличане на номер на потенциометър бит5 и бит6
					dd=dd<<3;//изчистване на битове 7,6 и 5
					dd=dd>>3;//стойност на потенциометъра
					poten[d]=dd;//стойност на потенциометъра
					return 2;
	}		        
	
 }
 return 0; 
}

void   DiscrMatrixLpt(unsigned adres)
{
  listpia.cur=&listpia.top;
  listpia.find(adres);
  if(!*listpia.cur )
    return ;
  if((*listpia.cur)->kind()==3)
   // ( (MatrixLpt*)(*listpia.cur))->TMatrix();          
     ( (MatrixLpt*)(*listpia.cur))->TMatrixVersion2017();//04.2017
}