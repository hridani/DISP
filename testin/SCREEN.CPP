/******************************************************************************************/
/*                               S C R E E N . C P P                                                 */
/*   ���������� �� ������� �� ������ � ������ ,���� ������ �� "DISP"     */
/******************************************************************************************/
#include <stdio.h>
#include <dos.h>
#include "c:\msvc\dani\comst.h"
#include "c:\msvc\dani\screen.h"
#include "c:\msvc\dani\string.h"
#include "c:\msvc\dani\include\svga.h"
#include "c:\msvc\dani\include\videoc.h"
#include "c:\msvc\dani\irq9new.h"
//#include "c:\msvc\dani\strug\errfail.h"
//#include "c:\msvc\dani\mancnc4.h"
#include "dispnew.h"
#include <string.h>
      byte atrlfunc=71; /* �������� �� ������������� - ������� ������ */
      byte atrlfunc1=39;/* �������� �� ������������� - ��������� ������ */
      byte activfunc=207;

/***************************************************************************************/
/* IniEkran: ������������ ������  � ������� �� ������                            */
/* ������ �����: ����                                                                           */
/* ������� �����:����                                                                           */
/***************************************************************************************/
void IniEkran(byte Page)                                           
{     
   unsigned helppar=0;
   activpage=Page;
    if(disp_init()==FALSE) /* �������������� �� ������� */
      ExitSys();
    SetScreenPage(activpage);   /* ����������� �� ��������� �������� */     
    ClearScreen();/* ���������� �� ������ */ 
   for ( int i=0;i<5;i++)       /* ������ �� ������������ �� �������������� ������� �� 1 ��� */
     lfunc.add(i,23,1+16*i,14,0x7,NULL);  
   for ( i=0;i<5;i++)       /* ������ �� ������������ �� �������������� ������� �� 2 ���  */
     lfunc1.add(i,22,1+16*i,14,0x7,NULL);  
   if(ReadParamMem(3500,'u',&helppar)==FALSE)
      printf("ERROR");
   else
     atrlfunc=(byte)helppar;
   if(ReadParamMem(3501,'u',&helppar)==FALSE)
      printf("ERROR");
   else
     atrlfunc1=(byte)helppar;
   if(ReadParamMem(3502,'u',&helppar)==FALSE)
      printf("ERROR");
   else
     activfunc=(byte)helppar;
  
}
     unsigned brsk;
/***********************************************************************************/
/* SlowPrint:����� ������ �� "DISP" �� ����������� ������������� �� ������  */
/* ������ �����:����                                                                           */
/* ������� �����: ����                                                                       */
/************************************************************************************/
void SlowPrint(void)
{       
  while(TRUE)
  {  
       Delay(); /* �������������� */
      flag_change=FALSE;
    if( PrintEnterFields!=NULL && ReadSem(501)==1)
    { 
      PrintEnterFields(); /* ������������ �� ������-��������� ���� */
    //  NoSignalSem(501);
  }
   if( ReadSem(76)==0)  //ReadSem(70)==0  &&
   {
	if (PrintExitFields!=NULL)
		PrintExitFields();/* ������������ �� ��������� ������ */
	if(PrintUserFields!=NULL)
		PrintUserFields();
   }                           
   if(ReadSem(500)==1)
   {
      ScrFK();/* ������������ �� �������������� ������� */
      NoSignalSem(500);
   }

   }
}    

/***********************************************************************************/
/* DrawACScreen:��������� ������ � ������������ �� ������� ����������� �� ������� */
/* ������ �����:�������� ��� ����������� ������� �����������     */
/* ������� �����: ����                                                                       */
/************************************************************************************/
void DrawACScreen(ListVF *pl)
{                            
  activ=pl; /* ������������� �� ��������� �� ������� ����������� */
  if(activ->sptr!=NULL)     
    Load_scr(activpage,0,0,24,79,activ->sptr,0); /* ��������� � ��������� �������� �� ����������� �� ������� */

} 

/***********************************************************************************/
/* DrawField :��������� ����������� �������� ������                        */
/* ������ �����:�������������,��� �� ����� � ������������ ������,��� �� �������*/
/* ������� �����: ����                                                                       */
/************************************************************************************/
void DrawField( byte Page,char *name,ListVF *pl)
{
       pl->load(name); /* ���� � ������,����� �� ����� ������ �� */
       pl->cur=&pl->top;/* ����. �� ������� �������� */
       while(*pl->cur) /* ������������ �� ����������� �������� ������ ��� ������������ */
       {             
	    dprint(Page,(*pl->cur)->col,(*pl->cur)->row,(*pl->cur)->atr,(*pl->cur)->ptext); 
	    pl->cur=&(*pl->cur)->next; /* ������� ������� */   
	 }
}         

/***********************************************************************************/
/* DrawScreen:��������� ������ � ����� �� �������������� ������� �   */
/* ����������� �������� ������, ������� ��������� ���������� � ���� � ���. BUF */
/* ������ �����:�������������,��� �� ����� � ������������ ������,��� �� �������*/
/* ������� �����: ����                                                                       */
/************************************************************************************/
void DrawScreen(char *name,ListVF *pl)
{    
   FILE *stream;     /* ����� �� ����� */
   VP psptr; /* ������� �������� ��� ������������������ */
    /* ��� �� ����� � �������� ����������� */
   char *newname;    
   int c=26;
   int len=strlen(name ) ;
   newname=new char [len +1];  
   strcpy(newname,name);
   sprintf(newname + len -3,"buf"); 
   
   if((stream=fopen(newname,"r"))==NULL) /* ��� �� ���� � ����������� */
   {            /* �� */
   //   DrawFuncFrame(activpage); /* ��������� �� ������� �� �����. ������� */
      DrawField(activpage,name,pl);/* ���������� �� ����������� �������� ������  */
      SetCursorPos(activpage,90,30); /* ���� ������ */
	pl->sptr=Allocate_scr(activpage,0,0,24,79); /* ����� �� ������������� � �� */
      fclose(stream);
      if((stream=fopen(newname,"w+"))!=NULL) /* ��������� �� ���� �� ������������� */
      {
	psptr=pl->sptr; /* ������� �������� ��� ������������� � �� */
      /* �������� �� �� ��� ����� � ���������� BUF */
	for(int i=0;i<=24;i++)  /* ������ */
	{
		for(int j=0;j<80;j++) /* ������ */
		{       
		  fputc((psptr)->h.thechar,stream); /* �������� �� ������� */
		  fputc((psptr++)->h.attribute,stream);/* �������� �� �������� */
		}
		fputc(0xd,stream);/* ���� �� ���� */
		fputc(0xa,stream);
	}
	fclose(stream);
     }
     ClearScreen(); /* ���������� �� ������ */
   }
   else /* ��� ���� � ������������� */
   {                   
      /* �������� �� ����� �� ������������� �� ����� � ������� */
      if(fNewMemory((void **)(&pl->sptr),4000,&"SaveSCREEN")==NULL)
	    ExitSys();
      psptr=pl->sptr; /* ������� �������� ��� ������������� � �� */
      /* �������� �� ����� � ������������� � �� */
      for(int i=0;i<=24;i++) /* ������ */
      {
      for(int j=0;j<80;j++)         /* ������ */
      { 
		  (psptr)->h.thechar=fgetc(stream);/* ������ */
		  (psptr++)->h.attribute=fgetc(stream);/* ������� */
      }
      fgetc(stream);
      fgetc(stream);
     }
     fclose(stream);
    }      
    delete newname;
}
     


/***********************************************************************************/
/* ScrFK: ������������ �� ������������ ������������ �������   */
/* ������ �����:����                                                          */
/* ������� �����: ����                                                                       */
/************************************************************************************/
void ScrFK(void)             
{
      char atr=0;
      char atr1=0;  
      int c=16;
      int c1=17;         
      int c2=26;
      int c3=27;
      char buffer[20]; /* ������� ����� */
      int numb=0;  /* ����� �� �������� ������������ ������ */
      int len=0; /* ������� �� ����� �� ������������� ������ */
      if(posl==NULL || post==NULL)
	return;
      if(*posl>=10)
     {
	  sprintf(buffer,"%c",c3);
	  dprint(activpage,2,24,0x07,buffer); /* ��������� �� ������ � 10 ������ */
		 sprintf(buffer,"%c",' ');
       dprint(activpage,75,24,0x07,buffer); /* ��������� �� ������ � 10 ������ */
     }
     else
     {      
       sprintf(buffer,"%c",' ');
       dprint(activpage,2,24,0x07,buffer); /* ��������� �� ������ � 10 ������ */
	if((*dk).nm<10)           
	   sprintf(buffer,"%c",' ');
       else
	   sprintf(buffer,"%c",c2);                                                                                                          
	  dprint(activpage,75,24,0x07,buffer); /* ��������� �� ������ � 10 ������ */
	}
	if(*post==0) /* ������� ����� ������� � �. ��. */
      {        /* �������� �� �������� ����� � ���������� ����� �������� */
		  atr=atrlfunc;
		  atr1=atrlfunc1;
		  sprintf(buffer,"%c",c);
		  dprint(activpage,0,22,0x07," "); /* ��������� �� ������ � 10 ������ */
		  dprint(activpage,0,23,0x07,buffer); /* ��������� �� ������ � 10 ������ */
		  sprintf(buffer,"%c",c1);
		  dprint(activpage,79,22,0x07," "); /* ��������� �� ������ � 10 ������ */
		  dprint(activpage,79,23,0x07,buffer); /* ��������� �� ������ � 10 ������ */

	}
	else   /* ������� ����� ������� � �. ��. */
	{         /* �������� �� ���������� ����� � �������� ����� �������� */
		   atr=atrlfunc1;
		  atr1=atrlfunc;  
				  sprintf(buffer,"%c",c);
		  dprint(activpage,0,22,0x07,buffer); /* ��������� �� ������ � 10 ������ */
		  dprint(activpage,0,23,0x07," "); /* ��������� �� ������ � 10 ������ */
				  sprintf(buffer,"%c",c1);
		  dprint(activpage,79,22,0x07,buffer); /* ��������� �� ������ � 10 ������ */
		  dprint(activpage,79,23,0x07," "); /* ��������� �� ������ � 10 ������ */

	}

//����������� �� ������ �� �����. ��. �� ����� ������� �������������� �� ������ ��������
	if(*post==1)
	     numb=*posl + 5*(*post-1) +1;
	else
	     numb=*posl + 5*(*post) +1;
       if(dk->top==NULL)
	 return;
	dk->cur=&dk->top; /* ����. �� ������� � ����� �� �����.�� */
	dk->find(numb);     //������� �� ����� � ��������� ������� �����
	lfunc.cur=&lfunc.top; // ����. �� �������� �� �����.�� �� ������ ��������
	while(*lfunc.cur)    //��������� �� ��������� �����. ������               
       {         
		  STRCPY(buffer,"        ",14);/* ��������� �� ������ � 10 ������ */
		  dprint(activpage,(*lfunc.cur)->col,(*lfunc.cur)->row,atr,buffer); /* ��������� �� ������ � 10 ������ */
		 if(*dk->cur) /* ��� ��� �� �������� �����. ��.*/
		 {    
		   //     len=strlen((*dk->cur)->funck->name);/* ������� �� ����� */
		   len=14;
		      STRCPYCEN(buffer,(*dk->cur)->funck->name,len);/* �������� �� ����� � ������ � ���������� ������� */
			if(*aKey==(*dk->cur)) // ������� ������ -��
			 dprint(activpage,(*lfunc.cur)->col,(*lfunc.cur)->row,activfunc,buffer); /* ���� ����������� */
			else // �� � ������� ������
			{
			      dprint(activpage,(*lfunc.cur)->col,(*lfunc.cur)->row,atr,buffer); //�������� �����������
			}
			dk->cur=&(*dk->cur)->next;  /* �������� ��� */
		 }
		 lfunc.cur=&(*lfunc.cur)->next;    //�������� ���� 
	    }    
//������������ �� �����. ��. �� ������ ��������                                    
	  lfunc1.cur=&lfunc1.top; //����. �� ��������� ������ �� ������ ��������
	  /* ����������� �� ������ �� �����. �� ,�� ����� ������� �������������� */
	  if(*post==1)
	     numb=*posl + 5*(*post) +1;
	   else
	     numb=*posl + 5*(*post+1) +1;
	   dk->cur=&dk->top;// ����. �� ������� � �����
	   dk->find(numb);     /* ������� �� ������ � ��������� ������� ����� */
	     while(*lfunc1.cur)   //��������� �� ������� � ������      
	     {                          
		 STRCPY(buffer,"       ",14);
		 dprint(activpage,(*lfunc1.cur)->col,(*lfunc1.cur)->row,atr1,buffer); 
		 if(*dk->cur)/* ��� ��� �� �������� �����. ��.*/
		 {
			//len=strlen((*dk->cur)->funck->name);/* ������� �� ����� */
			len=14;
			STRCPYCEN(buffer,(*dk->cur)->funck->name,len);/* �������� �� ����� � ������ � ���������� ������� */
			if(*aKey==(*dk->cur))// ������� ������ -��
			      dprint(activpage,(*lfunc1.cur)->col,(*lfunc1.cur)->row,activfunc,buffer); /* ���� ����������� */
			else
			      dprint(activpage,(*lfunc1.cur)->col,(*lfunc1.cur)->row,atr1,buffer); //�������� �����������
			dk->cur=&(*dk->cur)->next;  /* �������� ��� */
		   }  
		   lfunc1.cur=&(*lfunc1.cur)->next;     //�������� ���� 
	    }                   
			
 }

   
/***********************************************************************************/
/* PExitFields: ������������ ��������� ������                                */
/* ������ �����:�������� ��� ������� � �������� �� �������� �     */
/*                        �������� ��� ������� � ������� ������                 */
/* ������� �����: ����                                                                   */
/************************************************************************************/
void PExitField(ListFile *pv,ListVF *lvf)
{
  int re=0;  /* ������ �� ������������  �� ������� � ��������*/
  if(lvf->top!=NULL && pv->top!=NULL)
      re=lvf->nb; 
  int ri=pv->nb;;  /* ��� ���� ������ �� ������� */
  int br=0; /* ������ �� ������������� */
  int ci=1;
  if(ReadSem(120)==1)
    ci=ReadSem(1005); /* ������� ����������� */
  //���������: ������� � �������� ������� ����� - �������� �� ������ , � ����� ����� ����
  // �� ������� ������� ������ - �����,���� wi 
  int ar=re/2 +1 ; /* ������ ��� �� ������������ */
  char buffer[80]; //������� �����
  pv->cur=&pv->top;
  if(pv->top)  /* ��� ��� ������ � �������� */
  {             
    //������� �� ������ �� 1 ������� ������� ���
   if(ci>ri)   
     ci=ri; 
   if(ri<=re)
     br=1;
   else
  {    
      if(ci>0 && ci< ar)
	    br=1;
      if(ri -ci <= re -ar)
	  br=ri -re +1;
      if( ci >=ar && ci <= (ri - ar +1) )
	    br=ci-ar+1;
   }
  // pv->cur=&pv->top;//����. �� ������� �������� �� ������� � ��������
   pv->findnz(br); /* ������� �� ������-������ */
   if(!*pv->cur)
    return;
   }
   lvf->cur=&lvf->top; /* ����. �� ��������� ������ */
   
   while(*lvf->cur) //��������� �� ������� � ������� ������
   {
	 if(ReadSem(72)==1 && (*lvf->cur)->row>15 && (*lvf->cur)->row<20)
	 {
	  lvf->cur=&(*lvf->cur)->next;//�������� ������� ���� �� �����������
	  continue;
	 }

     if(!(*pv->cur)) /* ���� ����� �� ��������� ������� ���� */
     {
       memset(&buffer[0],' ',(*lvf->cur)->len); /* ����� �� ������ � ������ */
       *(buffer+(*lvf->cur)->len)='\0'; /* ���� �� ������ */
       dprint(activpage,(*lvf->cur)->col,(*lvf->cur)->row,0x70,buffer); /* ����� �� ������ */
       lvf->cur=&(*lvf->cur)->next;//�������� ������� ���� �� ��������
     }
     else /* ��� ����� �� ��������� ���� */
     {
		  STRCPY(&buffer[1],(*pv->cur)->ptext,(*lvf->cur)->len-1);//�������� �� ������ � ����� � �������� �������
		  if(br==ci) //������� ��� -��
		  {
			buffer[posm]=16; //������������ �� �������� ������
			dprint(activpage,(*lvf->cur)->col,(*lvf->cur)->row,0x2f,buffer);//������������ �� ������
		  }       
		  else  //�� � �������
		  {
			buffer[0]=' '; //���������� �� 1 ������� �� ��������� ����
			dprint(activpage,(*lvf->cur)->col,(*lvf->cur)->row,0x70,buffer);//������������ �� ������
		  } 
		  lvf->cur=&(*lvf->cur)->next;//�������� ������� ���� �� �����������
	  pv->cur=&(*pv->cur)->next;//������� �����
	  br++;
      } //else ��� �����
   }
}


/***********************************************************************************/
/* PrintField: ����� �� ������ �� ���� � ��������� �� ��������� �� �� */
/*                  ����� � �������� �����                                                 */
/* ������ �����:������ �� ������,����� ,��� �� ��������������� ,����� */
/* ������� �����: ����                                                                       */
/************************************************************************************/
void PrintField(ListVF * vf,int nm,char type,void * data)
{ 
  char buffer[80];// ������� ����� ,�������� �� 80 �������
  vf->cur=&vf->top; //����. �� �������
  vf->find(nm);  //������� �� ������
  if(*vf->cur) //��� ����
  {   
	      Number_convert(type,data,(*vf->cur)->ptext); //��������y���� �� ������� � �����
	  STRCPY(&buffer[0],(*vf->cur)->ptext,(*vf->cur)->len);//�������� �� ������ � ������� ����� � �������=����. �� ��������� ����
	  dprint(activpage,(*vf->cur)->col,(*vf->cur)->row,(*vf->cur)->atr,buffer);//������������      
   }
}

/***********************************************************************************/
/* PEnterFields: ������������ �� ������  ������                                */
/* ������ �����:�������� ��� ������� � �������� �� �������� 1  � 2    */
/*                        ������� ��� ���� � �������/��2 ������/                  */
/* ������� �����: ����                                                                   */
/************************************************************************************/
void PEnterField(VField *vf1,VField *vf2,int nms)
{            
  char simwol;//������,�������� �� ASCII  ������
  int len=vf1->len; //���������� �� ������� 
  simwol=ReadASCII();//������ �� �������� ASCII ������
  if(ReadSem(150)!=1)//����-������
  { //��
	if(*col >vf1->len - 3)//������ -3 ������� ����� ����
		return;
  }
  else
  {       
     if(*col >=vf1->len )//����� ��������� �� ������
		return;
  }
  while(simwol!=0 && *col < vf1->len)  //��� ������
  {
     *(*wASCII + *col)=simwol; //����� �� ������� � �������  �����
     (*col)=(*col +1); //���������� � 1 �� ������������� ��������� �� ������� 
     if(*col>0 && *(*wASCII +*col)!=' ')  //����������� ��������� >0 � ������� �� ������ !=' ' */
     {
	    memset(*wASCII + *col,' ',len- *col);//��������� ��� ' ' �� �������� ���. �� ������� �� ���� �� ������ */
     }
     simwol=ReadASCII();//������ �� �������� ASCII ������
  }
  if(nms==0) //��� �� ������� //����� // �� ���������� �� 2 ������-������� ������- ��
  {
	if(ReadSem(123)==1)//����� "MAN"
	{
	  if(*col>1 && *(*wASCII + *col-2)!=' ' && *(*wASCII + *col-1)>='A' && *(*wASCII + *col-1)<='Z' )
	  {   
	    Insertstr(*wASCII,' ',*col-1)  ;//��������� �� ' ' ����� ������
	    (*col)++;
	  }
	}

    dprint(activpage,vf1->col,vf1->row,vf1->atr,vf1->ptext);//����������� 
    SetCursorPos(activpage,vf1->col + *col,vf1->row);//������� �� �������
  }
  else//��� ������� �.� ������ �� ���� ������-������� ����
  {
     if(ReadSem(nms)==0)  //������ �� ���������� �� �������� -�� � ��������� �������
     {
	      if(*(*wASCII)!=' ')
				STRCPY(vf1->ptext,*wASCII,vf1->len-1);//��������� �� ������ � ������� ����� � �������=����. �� ��������� ����
		 dprint(activpage,vf1->col,vf1->row,vf1->atr,vf1->ptext);//������������
		 dprint(activpage,vf2->col,vf2->row,vf2->atr,vf2->ptext);//������������
	     SetCursorPos(activpage,vf1->col + *col,vf1->row);//������� �� �������
     }
     else  //��������� �������
     {    
      if(*col >vf2->len - 3)
	       return;
	if(ReadSem(122)==1 || ReadSem(129)==1)//����� "EDIT","EDIT-MEM"
	{
	  if(*col>1 && *(*wASCII + *col-2)!=' ' && *(*wASCII + *col-1)>='A' && *(*wASCII + *col-1)<='Z' )
	  {   
	    Insertstr(*wASCII,' ',*col-1)  ;//��������� �� ' '����� ������
	    (*col)++;
	  }
	}
      if(*(*wASCII)!=' ')//��� ������
	       STRCPY(vf2->ptext,*wASCII,vf2->len-1);//��������� �� ������ � ������� ����� � �������=����. �� ��������� ����
	  dprint(activpage,vf2->col,vf2->row,vf2->atr,vf2->ptext);//������������
	  dprint(activpage,vf1->col,vf1->row,vf1->atr,vf1->ptext);//������������
	  SetCursorPos(activpage,vf2->col + *col,vf2->row);//������� �� �������
     }
   }
}


