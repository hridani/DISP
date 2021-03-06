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
byte poten[4];//4 ��������������
ListPia  listpia;
extern List input;
extern  List output;                         
char name[10]="p0.dat";
int ff;

int adresc;
byte numboutput[8]={0,0,0,0,0,0,0,0};//���� �� �������� ��������� ������ �� ������/������� ����� �� ���/ ,����������� � �������
unsigned l_value;              

void InitPiaLpt ( unsigned adres , unsigned tp,byte typebind )
{
        //��� ������ � LPT - ��������������� � ��������� � �� ���� :�-������, C1-������, B ot 3-8 �������)  �� ���-2 (������ 0-�����, 1-������� , 2� 3 �� �� Lpt)
        //��� ��� 3-�-����, C1-����, B ot 3-8 ����� 
      if(tp==2)
      {
     		//	byte noutput=4;/* ���� ������ �� ������� C1 */        
     		byte   noutput=12;/* ���� ������ �� ������� C1+A */       
    			byte ninput=5;  /* ���� ������� �� ������� B3-B8 */
     			listpia.add('p',adres,1,0,1,2,0,0);
      }
      else  //ne e testwan
      {      
     			byte noutput=5;   /* ���� ������ �� ������� B3-B8 */
    			byte ninput=12;   /* ���� ������� �� �������-A, C1 */ 
     			listpia.add('p',adres,0,1,0,2,0,0);     
      }
       
}    
        
void InitPiaLptMatrix ( unsigned adres1)
{
        //��� ������ � LPT - ��������������� � ��������� � �� ���� :�-������, C1-������, B ot 3-8 �������)  �� ���-2 (������ 0-�����, 1-������� , 2� 3 �� �� Lpt)
        //��� ��� 3-�-����, C1-����, B ot 3-8 �����      
        listpia.add('c', adres1,0,0,0,0,0,0);//��������� �� ����� MatrixLpt 'c' -��������� �������
}            
        
void InitPia(unsigned adres,byte typeA,byte typeB,byte typeC1,byte typeC2,
                 byte typebind,byte mi,byte mo )
{
    byte control_word=0x80;
    byte noutput=0;/* ���� ������ �� ������� */
    byte ninput=0;  /* ���� ������� �� ������� */
    if(typeA==1)   /* ����� - ����� */
       noutput=noutput + 8;
    else    
    { 
      control_word=control_word |  0x10;
      ninput=ninput + 8;
    }
    if(typeB==1) /* ���� B -����� */
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
    
    _outp(adres+3,control_word);//��������� ���� �� ���
    _outp(adres,0xff);//���� ����� � 0 -�����,1- �������
    listpia.add(typebind,adres,typeA,typeB,typeC1,typeC2,mi,mo);
}       

void EndPIA(void)
{  
  listpia.cur=&listpia.top;
   while(*listpia.cur)
   {
       _outp((*listpia.cur)->baseadres,0xff);//���� ����� � 0 -�����,1- �������    
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
   byte *hinput;/* ������� �������� ��� ������� �� PIA */
   unsigned cur_number=1; /* ����� ����� �� ���� � ����� */
   Base *cur_input=NULL;
   unsigned i=0; /* ����� ����� �� �� ��� �� PIA */\
   input.cur=&input.top;
   while(*listpia.cur)
   {
      (*listpia.cur)->read();   
      switch ((*listpia.cur)->kind())
      {
         case 0:
           hinput=((Matrix *)(*listpia.cur))->pinput; /* ��������� �� ������� �� PIA ��� ������� */
           break;
         case 1:
            hinput=((Linea *)(*listpia.cur))->pinput;/* ��������� �� ������� �� PIA ��� �����  */
           break;  
          case 2:
           hinput=((LineaLpt *)(*listpia.cur))->pinput;/* ��������� �� ������� �� PIA ��� �����  */
           break;         
          case 3:
           hinput=((MatrixLpt *)(*listpia.cur))->pinput;/* ��������� �� ������� �� PIA ��� �����  */
           break;               
         default:
          perror("Nodifaned Pia");  /* ������������ ��������� */
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
           les si,hinput //�������� �� ���������
           add si,i
           lds di,cur_input//������ �������
           add di,3
           mov al,ds:[di]//�����
           xor al,es:[si]// ��������� �� ������ ��������-  ��������� "���"
           dec di   //di-->�������� �� �����
           shr al,1 //����� � CF �� 0���
           jnb et1
           mov ds:[di],0x1//���� �������� = ��������� � ���������
           jmp end
      et1:mov ds:[di],0x0//���� �������� = ��������� � ���������
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
    unsigned numbPIA=0;/* ���� PIA - ������� */
    unsigned adres=0; /* ����� �� PIA */
    byte tA=0,tB=0,tC1=0,tC2=0;/* ��� �� ������� A ,B ,C1, C2 */
    byte nA=0,nB=0,nC1=0,nC2=0;  /* ���� ������� �� ��������� �� ��� ������� */
    byte ni=0;/* ��� ���� ������ ������� ��� ������� */
    byte no=0;/* ��� ���� ������� ������� ��� ������� */
    byte type=0;/* ��� �� ��������� 0- �����,1-������� */
    if((ff=_open(name,_O_TEXT|O_RDONLY))==-1)
    {
         perror("NO fail");
         return;
    }
    numbPIA=(unsigned)ReadParam(1);
    for( i=1;i<=numbPIA;i++)
    {  
       adres=(unsigned)ReadParam(10*i +1);       /* ���� ����� �� PIA */ 
       tA=(byte)(ReadParam(10*i+2)); /* ���� ��� �� ���� � */
       tB=(byte)(ReadParam(10*i+3)); /* ���� ��� �� ���� B */
       tC1=(byte)(ReadParam(10*i+4));/* ���� ��� �� ���� C1 */
       tC2=(byte)(ReadParam(10*i+5));/* ���� ��� �� ���� C2 */
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
    InitPia(adres,tA,tB,tC1,tC2,'m',ni,no);/* ������������� �� PIA ,��������� ��� ������� */
    DiscrMatrix(adres ,nA,nB ,nC1,nC2); /* �������� �� ��������� - ���� ������� �� ����� ��� */           
    }       
    else
      if (type==0)
      		InitPia(adres,tA,tB,tC1,tC2,'l');/* ������������� �� PIA ,��������� ��� ����� */   
        
    
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
  byte mask=0;  /* �������� �� �������� ����� */
  listpia.cur=&listpia.top;
   if((ff=_open("p1000.dat",_O_TEXT|O_RDONLY))==-1)
   {
         perror("NO fail");
         return;
    }
   input.cur=&input.top;
   for (unsigned i=1;i<=input.nm;i++)
   {
         mask=(byte)ReadParam(i);       /* ���� ��������� ����� */ 
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
         mask=(byte)ReadParam(i);       /* ���� ��������� ����� */ 
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
   byte *pmask=NULL;/* �������� ��� ������ � ����� � ����� �� ���������� ������� */
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
    unsigned numbPIA=0;/* ���� PIA - ������� */
    unsigned adres=0; /* ����� �� PIA */
    byte tA=0,tB=0,tC1=0,tC2=0;/* ��� �� ������� A ,B ,C1, C2 */
    byte nA=0,nB=0,nC1=0,nC2=0;  /* ���� ������� �� ��������� �� ��� ������� */
    byte ni=0;/* ��� ���� ������ ������� ��� ������� */
    byte no=0;/* ��� ���� ������� ������� ��� ������� */
    byte type=0;/* ��� �� ��������� 0- �����,1-������� */
    ReadParamMem(40,'u',&l_value);
    ReadParamMem(50,'u',&numbPIA);    
    int j;//������� ����������
    for( i=0;i<numbPIA;i++)
    {  
       ReadParamMem(60 +20 *i +1,'u',&adres);/* ���� ����� �� PIA */ 
       ReadParamMem(60+20*i +2,'u',&tA);/* ���� ��� �� ���� A*/
       ReadParamMem(60+ 20*i +3,'u',&tB);      /* ���� ��� �� ���� B */
       ReadParamMem(60+ 20*i +4,'u',&tC1);/* ���� ��� �� ���� C1 */
       ReadParamMem(60 + 20*i +5,'u',&tC2);/* ���� ��� �� ���� C2 */
       ReadParamMem(60 +20*i +6,'u',&type);             
       
       switch (type)
       {
         case 0:  //�����
           InitPia(adres,tA,tB,tC1,tC2,'l');/* ������������� �� PIA ,��������� ��� ����� */
           break;
         case 1:     //�������
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
               ReadParamMem(71+20*i + j,'u',&numboutput[j]);  //������ �� ����� ,���������� �������� ������              
          InitPia(adres,tA,tB,tC1,tC2,'m',ni,no);/* ������������� �� PIA ,��������� ��� ������� */
          DiscrMatrix(adres ,nA,nB ,nC1,nC2); /* �������� �� ��������� - ���� ������� �� ����� ��� */           
          break;      
          
          case 2:      
          case 3:
            InitPiaLpt(adres,type,'p');/*LPT */
          break;
           
           case 11://��������� �� �������- ���������� lpt1 i lpt2 -������ �� �� ����� ���� 2 ����� -���������� � �������� "������"
              InitPiaLptMatrix(adres);/*LPT */  
              DiscrMatrixLpt(adres);
       break;
        
         default:
          perror("Nodifaned Pia");  /* ������������ ��������� */
      } 
   }                      
   IniIOMask();
    numbinpPIA=input.nm;//����� �� ���� ������� �� PIA
    for(i=0;i<output.nm;i++)
     output(i,0);//�������� �� ��������  
     //�������� �������� �� 26.02.2013
    output(7,1);//7 ����� �� �����      �� ����
     
}


void IniIOMask(void)
{     
  byte mask=0;  /* �������� �� �������� ����� */
  listpia.cur=&listpia.top;
   input.cur=&input.top;
   for (unsigned i=1;i<=input.nm;i++)
   {
         ReadParamMem(200+i,'i',&mask);/* ���� ��������� ����� �� ����*/ 
         (*input.cur)->mask=mask;
         input.cur=&(*input.cur)->next;     
         mask=0;
    }
   output.cur=&output.top;
   for ( i=1;i<=output.nm;i++)
   {
         ReadParamMem(300+i,'i',&mask);/* ���� ��������� ����� �� �����*/ 
         (*output.cur)->mask=mask;
         output.cur=&(*output.cur)->next;
         mask=0;
    }
   listpia.cur=&listpia.top;
   byte *pmask=NULL;/* �������� ��� ������� � ����� � ����� �� ���������� ������ -����� �� ������,�����,����� */
   while(*listpia.cur)
   {
      if((*listpia.cur)->kind()==0)
      {
        pmask=((Matrix *)(*listpia.cur))->potable;
        for( i=0;i< ((Matrix *)(*listpia.cur))->moutput;i++)
        { 
          ReadParamMem(400+i,'i',&mask);/* ���� ��������� ����� �� �������� ����� */ 
          *(pmask + 3+4*i)=mask;//������ 3 ����� � ��������� � ����� �� ����. ������
          mask=0;
         }
       }
       listpia.cur=&(*listpia.cur)->next;
    }
}

void IniSerialInput(void)
{
  numbinpPIA=input.nm;//����� �� ���� ������� �� PIA
  //��������� � ��������  �� ��������� �� ASI
  unsigned numb;
  unsigned numbASIA=0;
  byte mask=0;
  int j=0;
  int adres;
  ReadParamMem(41,'i',&numb);  //����� �� COM : 1-COM1;2-COM2
  if(numb==1)
     adresc=0x3f8;
  if(numb==2)
     adresc=0x2f8;
  numb=input.nm+1;    
  ReadParamMem(42,'i',&numbASIA);  //���� ������� �� ASIA
  for(unsigned i=numb;i<numb+numbASIA;i++)//���� ������� �� ASI� � 63
  { 
    j++;
    input.add();
    ReadParamMem(100+numbinpPIA +j,'i',&mask);/* ���� ��������� ����� �� ����, ���� ������� �� PIA*/ 
    (*input.cur)->mask=mask;//�����
    (*input.cur)->adres=adres;//����� �� ����������� �������
  }
  
}
/******************************************************************************/
/*  ReadSerInput: ������� �� ��������� �� ��������� ������� � ������������� */
/* ������ �����: ����                                                                               */
/* ������� �����: �������� �� ���������:                                                   */
/*                         0-���� �������� ������ ����                                             */
/*                          1- ����                                                                              */
/*                          2-�������������                                                               */
/****************************************************************************************/
byte ReadSerInput(void)
{
 //COM 1 ->0x3f8
  input.cur=&input.top;
  byte n=0,c=0,dd=0,d=0;
//  int adres= input.top->adres;
  int numb=numbinpPIA;//������ �� ��������� �������
  if((_inp(adresc+5) & 1)==1)//��������� �� ��������� �� ��������� �� ������� ���0=1,��� ��� ����� �� ������
  {      
   n=_inp(adresc);//������� ��������� �� �������� �����
   if((n & 0x80)==0) // �����
   {   
     if((n & 0x40)==0)// ��������
     {
        c=n;//������ �� ���������� �����
         input.find(numb+c);
        (*input.cur)->value=(1 ^ (*input.cur)->mask);
     }
     else //�������� �����
     {
        n=n<<2;
        c=n>>2;
       input.find(numb+c);
        (*input.cur)->value=(0 ^ (*input.cur)->mask);
      }
      return 1;
   } 
   	else //�������������
	{
					dd=c=n;
					d=c<<1;//���������� �� 7 ���
					d=d>>6;//��������� �� ����� �� ������������� ���5 � ���6
					dd=dd<<3;//���������� �� ������ 7,6 � 5
					dd=dd>>3;//�������� �� ��������������
					poten[d]=dd;//�������� �� ��������������
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