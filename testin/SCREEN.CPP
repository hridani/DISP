/******************************************************************************************/
/*                               S C R E E N . C P P                                                 */
/*   Дефиниране на функции за работа с екрана ,като задача на "DISP"     */
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
      byte atrlfunc=71; /* атрибути на изображението - активен клавиш */
      byte atrlfunc1=39;/* атрибути на изображението - неактивен клавиш */
      byte activfunc=207;

/***************************************************************************************/
/* IniEkran: Инициализира екрана  и данните за работа                            */
/* Входни данни: Няма                                                                           */
/* Изходни данни:Няма                                                                           */
/***************************************************************************************/
void IniEkran(byte Page)                                           
{     
   unsigned helppar=0;
   activpage=Page;
    if(disp_init()==FALSE) /* инициализиране на дисплея */
      ExitSys();
    SetScreenPage(activpage);   /* превлючване на активната страница */     
    ClearScreen();/* почистване на екрана */ 
   for ( int i=0;i<5;i++)       /* полета за наименование на функционалните клавиши от 1 ред */
     lfunc.add(i,23,1+16*i,14,0x7,NULL);  
   for ( i=0;i<5;i++)       /* полета за наименование на функционалните клавиши от 2 ред  */
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
/* SlowPrint:бавна задача на "DISP" за опресняване изображението на екрана  */
/* Входни данни:Няма                                                                           */
/* Изходни данни: Няма                                                                       */
/************************************************************************************/
void SlowPrint(void)
{       
  while(TRUE)
  {  
       Delay(); /* времеизчакване */
      flag_change=FALSE;
    if( PrintEnterFields!=NULL && ReadSem(501)==1)
    { 
      PrintEnterFields(); /* изобразяване на входно-изходното поле */
    //  NoSignalSem(501);
  }
   if( ReadSem(76)==0)  //ReadSem(70)==0  &&
   {
	if (PrintExitFields!=NULL)
		PrintExitFields();/* изобразяване на изходните полета */
	if(PrintUserFields!=NULL)
		PrintUserFields();
   }                           
   if(ReadSem(500)==1)
   {
      ScrFK();/* изобразяване на функционалните клавиши */
      NoSignalSem(500);
   }

   }
}    

/***********************************************************************************/
/* DrawACScreen:Изчертава екрана с определеното за активно изображение от паметта */
/* Входни данни:Указател към опрделеното активно изображение     */
/* Изходни данни: Няма                                                                       */
/************************************************************************************/
void DrawACScreen(ListVF *pl)
{                            
  activ=pl; /* инизиализация на указателя за активно изображение */
  if(activ->sptr!=NULL)     
    Load_scr(activpage,0,0,24,79,activ->sptr,0); /* зареждане в актижната страница на изображение от паметта */

} 

/***********************************************************************************/
/* DrawField :Изчертава постоянните текстови полета                        */
/* Входни данни:Видеостраница,име на файла с дефинираните полета,име на списъка*/
/* Изходни данни: Няма                                                                       */
/************************************************************************************/
void DrawField( byte Page,char *name,ListVF *pl)
{
       pl->load(name); /* файл с полета,които не менят текста си */
       pl->cur=&pl->top;/* иниц. на текъщия указател */
       while(*pl->cur) /* изобразаване на постоянните текстови полета във видеопаметта */
       {             
	    dprint(Page,(*pl->cur)->col,(*pl->cur)->row,(*pl->cur)->atr,(*pl->cur)->ptext); 
	    pl->cur=&(*pl->cur)->next; /* следващ елемент */   
	 }
}         

/***********************************************************************************/
/* DrawScreen:Изчертава екрана с рамка за функционалните клавищи и   */
/* постоянните текстови полета, запазва екранната информация в файл с раз. BUF */
/* Входни данни:Видеостраница,име на файла с дефинираните полета,име на списъка*/
/* Изходни данни: Няма                                                                       */
/************************************************************************************/
void DrawScreen(char *name,ListVF *pl)
{    
   FILE *stream;     /* поток на файла */
   VP psptr; /* помощен указател към видеоизображението */
    /* име на файла с готовото изображение */
   char *newname;    
   int c=26;
   int len=strlen(name ) ;
   newname=new char [len +1];  
   strcpy(newname,name);
   sprintf(newname + len -3,"buf"); 
   
   if((stream=fopen(newname,"r"))==NULL) /* има ли файл с изображение */
   {            /* не */
   //   DrawFuncFrame(activpage); /* изчертава се рамката на функц. клавиши */
      DrawField(activpage,name,pl);/* изчертават се постоянните текстови полета  */
      SetCursorPos(activpage,90,30); /* няма курсор */
	pl->sptr=Allocate_scr(activpage,0,0,24,79); /* запис на изображението в ОП */
      fclose(stream);
      if((stream=fopen(newname,"w+"))!=NULL) /* създаване на файл за изображението */
      {
	psptr=pl->sptr; /* помощен указател към изображението в ОП */
      /* копиране от ОП във файла с разщирение BUF */
	for(int i=0;i<=24;i++)  /* редове */
	{
		for(int j=0;j<80;j++) /* колони */
		{       
		  fputc((psptr)->h.thechar,stream); /* копиране на символа */
		  fputc((psptr++)->h.attribute,stream);/* копиране на атрибута */
		}
		fputc(0xd,stream);/* край на реда */
		fputc(0xa,stream);
	}
	fclose(stream);
     }
     ClearScreen(); /* почистване на екрана */
   }
   else /* има файл с изабражението */
   {                   
      /* заделяне на памет за изображението от файла в паметта */
      if(fNewMemory((void **)(&pl->sptr),4000,&"SaveSCREEN")==NULL)
	    ExitSys();
      psptr=pl->sptr; /* помощен указател към изображението в ОП */
      /* копиране ит файла с изображението в ОП */
      for(int i=0;i<=24;i++) /* редове */
      {
      for(int j=0;j<80;j++)         /* колони */
      { 
		  (psptr)->h.thechar=fgetc(stream);/* символ */
		  (psptr++)->h.attribute=fgetc(stream);/* атрибут */
      }
      fgetc(stream);
      fgetc(stream);
     }
     fclose(stream);
    }      
    delete newname;
}
     


/***********************************************************************************/
/* ScrFK: Наименование на дефинираните функционални клавиши   */
/* Входни данни:Няма                                                          */
/* Изходни данни: Няма                                                                       */
/************************************************************************************/
void ScrFK(void)             
{
      char atr=0;
      char atr1=0;  
      int c=16;
      int c1=17;         
      int c2=26;
      int c3=27;
      char buffer[20]; /* помощен буфер */
      int numb=0;  /* номер на активния функционален клавиш */
      int len=0; /* дължина на името на функционалния клавиш */
      if(posl==NULL || post==NULL)
	return;
      if(*posl>=10)
     {
	  sprintf(buffer,"%c",c3);
	  dprint(activpage,2,24,0x07,buffer); /* попълване на буфера с 10 шпации */
		 sprintf(buffer,"%c",' ');
       dprint(activpage,75,24,0x07,buffer); /* попълване на буфера с 10 шпации */
     }
     else
     {      
       sprintf(buffer,"%c",' ');
       dprint(activpage,2,24,0x07,buffer); /* попълване на буфера с 10 шпации */
	if((*dk).nm<10)           
	   sprintf(buffer,"%c",' ');
       else
	   sprintf(buffer,"%c",c2);                                                                                                          
	  dprint(activpage,75,24,0x07,buffer); /* попълване на буфера с 10 шпации */
	}
	if(*post==0) /* активен долен ргистър с ф. кл. */
      {        /* атрибути на активния долен и неактивния горен регистри */
		  atr=atrlfunc;
		  atr1=atrlfunc1;
		  sprintf(buffer,"%c",c);
		  dprint(activpage,0,22,0x07," "); /* попълване на буфера с 10 шпации */
		  dprint(activpage,0,23,0x07,buffer); /* попълване на буфера с 10 шпации */
		  sprintf(buffer,"%c",c1);
		  dprint(activpage,79,22,0x07," "); /* попълване на буфера с 10 шпации */
		  dprint(activpage,79,23,0x07,buffer); /* попълване на буфера с 10 шпации */

	}
	else   /* активен долен ргистър с ф. кл. */
	{         /* атрибути на неактивния долен и активния горен регистри */
		   atr=atrlfunc1;
		  atr1=atrlfunc;  
				  sprintf(buffer,"%c",c);
		  dprint(activpage,0,22,0x07,buffer); /* попълване на буфера с 10 шпации */
		  dprint(activpage,0,23,0x07," "); /* попълване на буфера с 10 шпации */
				  sprintf(buffer,"%c",c1);
		  dprint(activpage,79,22,0x07,buffer); /* попълване на буфера с 10 шпации */
		  dprint(activpage,79,23,0x07," "); /* попълване на буфера с 10 шпации */

	}

//изчисляване на номера на функц. кл. от който започва изобразяването на долния регистър
	if(*post==1)
	     numb=*posl + 5*(*post-1) +1;
	else
	     numb=*posl + 5*(*post) +1;
       if(dk->top==NULL)
	 return;
	dk->cur=&dk->top; /* иниц. на списъка с имена на функц.кл */
	dk->find(numb);     //търсене на възел с посочения начален номер
	lfunc.cur=&lfunc.top; // иниц. на полетата на функц.кл от долния регистър
	while(*lfunc.cur)    //обхождане на екранните функц. полета               
       {         
		  STRCPY(buffer,"        ",14);/* попълване на буфера с 10 шпации */
		  dprint(activpage,(*lfunc.cur)->col,(*lfunc.cur)->row,atr,buffer); /* попълване на буфера с 10 шпации */
		 if(*dk->cur) /* има име за поредния функц. кл.*/
		 {    
		   //     len=strlen((*dk->cur)->funck->name);/* дължина на името */
		   len=14;
		      STRCPYCEN(buffer,(*dk->cur)->funck->name,len);/* копиране на името в буфера с посочената дължина */
			if(*aKey==(*dk->cur)) // активен клавиш -да
			 dprint(activpage,(*lfunc.cur)->col,(*lfunc.cur)->row,activfunc,buffer); /* ярко изображение */
			else // не е активен клавиш
			{
			      dprint(activpage,(*lfunc.cur)->col,(*lfunc.cur)->row,atr,buffer); //нормално изображение
			}
			dk->cur=&(*dk->cur)->next;  /* следващо име */
		 }
		 lfunc.cur=&(*lfunc.cur)->next;    //следващо поле 
	    }    
//изобразяване на функц. кл. от горния регистър                                    
	  lfunc1.cur=&lfunc1.top; //иниц. на екранните полета от горния регистър
	  /* изчисляване на номера на функц. кл ,от който започва изобразяването */
	  if(*post==1)
	     numb=*posl + 5*(*post) +1;
	   else
	     numb=*posl + 5*(*post+1) +1;
	   dk->cur=&dk->top;// иниц. на списъка с имена
	   dk->find(numb);     /* търсене на записа с посочения начален номер */
	     while(*lfunc1.cur)   //обхождане на списъкс с полета      
	     {                          
		 STRCPY(buffer,"       ",14);
		 dprint(activpage,(*lfunc1.cur)->col,(*lfunc1.cur)->row,atr1,buffer); 
		 if(*dk->cur)/* има име за поредния функц. кл.*/
		 {
			//len=strlen((*dk->cur)->funck->name);/* дължина на името */
			len=14;
			STRCPYCEN(buffer,(*dk->cur)->funck->name,len);/* копиране на името в буфера с посочената дължина */
			if(*aKey==(*dk->cur))// активен клавиш -да
			      dprint(activpage,(*lfunc1.cur)->col,(*lfunc1.cur)->row,activfunc,buffer); /* ярко изображение */
			else
			      dprint(activpage,(*lfunc1.cur)->col,(*lfunc1.cur)->row,atr1,buffer); //нормално изображение
			dk->cur=&(*dk->cur)->next;  /* следващо име */
		   }  
		   lfunc1.cur=&(*lfunc1.cur)->next;     //следващо поле 
	    }                   
			
 }

   
/***********************************************************************************/
/* PExitFields: Изобразяване изходните полета                                */
/* Входни данни:Указател към списъка с текстове на полетата и     */
/*                        указател към списъка с екранни полета                 */
/* Изходни данни: Няма                                                                   */
/************************************************************************************/
void PExitField(ListFile *pv,ListVF *lvf)
{
  int re=0;  /* редове за изобразяване  от списъка с текстове*/
  if(lvf->top!=NULL && pv->top!=NULL)
      re=lvf->nb; 
  int ri=pv->nb;;  /* общ брой редове на списъка */
  int br=0; /* начало на изображението */
  int ci=1;
  if(ReadSem(120)==1)
    ci=ReadSem(1005); /* активно изображение */
  //Забележка: списъка с текстове съдържа възли - текстове по редове , а всеки възел може
  // да съдържа няколко текста - избор,чрез wi 
  int ar=re/2 +1 ; /* среден ред за изобразяване */
  char buffer[80]; //помощен буфер
  pv->cur=&pv->top;
  if(pv->top)  /* ако има списък с текстове */
  {             
    //търсене на записа за 1 изходен екранен ред
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
  // pv->cur=&pv->top;//иниц. на текъшия указател на списъка с текстове
   pv->findnz(br); /* турсене на записа-начало */
   if(!*pv->cur)
    return;
   }
   lvf->cur=&lvf->top; /* иниц. на екранните полета */
   
   while(*lvf->cur) //обхождане на списъка с екранни полета
   {
	 if(ReadSem(72)==1 && (*lvf->cur)->row>15 && (*lvf->cur)->row<20)
	 {
	  lvf->cur=&(*lvf->cur)->next;//следващо екранно поле по хоризонтала
	  continue;
	 }

     if(!(*pv->cur)) /* няма текст за поредното екранно поле */
     {
       memset(&buffer[0],' ',(*lvf->cur)->len); /* запис на шпации в буфера */
       *(buffer+(*lvf->cur)->len)='\0'; /* край на буфера */
       dprint(activpage,(*lvf->cur)->col,(*lvf->cur)->row,0x70,buffer); /* печат на буфера */
       lvf->cur=&(*lvf->cur)->next;//следващо екранно поле по вертикал
     }
     else /* има текст за изходното поле */
     {
		  STRCPY(&buffer[1],(*pv->cur)->ptext,(*lvf->cur)->len-1);//копиране на текста в буфер с посочена дължина
		  if(br==ci) //активен ред -да
		  {
			buffer[posm]=16; //изобразяване на маркиращ курсор
			dprint(activpage,(*lvf->cur)->col,(*lvf->cur)->row,0x2f,buffer);//изобразяване на полето
		  }       
		  else  //не е активно
		  {
			buffer[0]=' '; //почистване на 1 позиция на екранното поле
			dprint(activpage,(*lvf->cur)->col,(*lvf->cur)->row,0x70,buffer);//изобразяване на полето
		  } 
		  lvf->cur=&(*lvf->cur)->next;//следващо екранно поле по хоризонтала
	  pv->cur=&(*pv->cur)->next;//следващ възел
	  br++;
      } //else има текст
   }
}


/***********************************************************************************/
/* PrintField: Печет на екрана на поле и свързване на указателя му за */
/*                  текст с посочени данни                                                 */
/* Входни данни:Списък на полето,номер ,тип на преобразуването ,данна */
/* Изходни данни: Няма                                                                       */
/************************************************************************************/
void PrintField(ListVF * vf,int nm,char type,void * data)
{ 
  char buffer[80];// помощен буфер ,максимум до 80 символа
  vf->cur=&vf->top; //иниц. на списъка
  vf->find(nm);  //търсене на полето
  if(*vf->cur) //име поле
  {   
	      Number_convert(type,data,(*vf->cur)->ptext); //преобразyване на даннота в текст
	  STRCPY(&buffer[0],(*vf->cur)->ptext,(*vf->cur)->len);//копиране на буфера в помощен буфер с дължина=дълж. на екранното поле
	  dprint(activpage,(*vf->cur)->col,(*vf->cur)->row,(*vf->cur)->atr,buffer);//изобразяване      
   }
}

/***********************************************************************************/
/* PEnterFields: Изобразяване на входни  полета                                */
/* Входни данни:Указател към списъка с текстове на полетата 1  и 2    */
/*                        семафор кое поле е активно/до2 входни/                  */
/* Изходни данни: Няма                                                                   */
/************************************************************************************/
void PEnterField(VField *vf1,VField *vf2,int nms)
{            
  char simwol;//символ,прочетен от ASCII  буфера
  int len=vf1->len; //променлива за дължина 
  simwol=ReadASCII();//четене на поредния ASCII символ
  if(ReadSem(150)!=1)//поле-отговр
  { //не
	if(*col >vf1->len - 3)//достъп -3 символа преди края
		return;
  }
  else
  {       
     if(*col >=vf1->len )//извън дължината на полето
		return;
  }
  while(simwol!=0 && *col < vf1->len)  //има символ
  {
     *(*wASCII + *col)=simwol; //запис на символа в посовен  буфер
     (*col)=(*col +1); //ъвеличение с 1 на относителното положение на курсора 
     if(*col>0 && *(*wASCII +*col)!=' ')  //относително положение >0 и наличие на символ !=' ' */
     {
	    memset(*wASCII + *col,' ',len- *col);//запулване със ' ' от текъщото пол. на курсора до края на буфера */
     }
     simwol=ReadASCII();//четвне на поредния ASCII символ
  }
  if(nms==0) //има ли семафор //номер // за управление на 2 входно-изходни полета- не
  {
	if(ReadSem(123)==1)//режим "MAN"
	{
	  if(*col>1 && *(*wASCII + *col-2)!=' ' && *(*wASCII + *col-1)>='A' && *(*wASCII + *col-1)<='Z' )
	  {   
	    Insertstr(*wASCII,' ',*col-1)  ;//въвеждане на ' ' между думите
	    (*col)++;
	  }
	}

    dprint(activpage,vf1->col,vf1->row,vf1->atr,vf1->ptext);//избразяване 
    SetCursorPos(activpage,vf1->col + *col,vf1->row);//позиция на курсора
  }
  else//има семафор т.е повече от едно входно-изходно поле
  {
     if(ReadSem(nms)==0)  //четене на стойността на семафора -не е настъпило събитие
     {
	      if(*(*wASCII)!=' ')
				STRCPY(vf1->ptext,*wASCII,vf1->len-1);//копоиране на буфера в помощен буфер с дължина=дълж. на екранното поле
		 dprint(activpage,vf1->col,vf1->row,vf1->atr,vf1->ptext);//изобразяване
		 dprint(activpage,vf2->col,vf2->row,vf2->atr,vf2->ptext);//изобразяване
	     SetCursorPos(activpage,vf1->col + *col,vf1->row);//позиция на курсора
     }
     else  //настъпило събитие
     {    
      if(*col >vf2->len - 3)
	       return;
	if(ReadSem(122)==1 || ReadSem(129)==1)//режим "EDIT","EDIT-MEM"
	{
	  if(*col>1 && *(*wASCII + *col-2)!=' ' && *(*wASCII + *col-1)>='A' && *(*wASCII + *col-1)<='Z' )
	  {   
	    Insertstr(*wASCII,' ',*col-1)  ;//въвеждане на ' 'между думите
	    (*col)++;
	  }
	}
      if(*(*wASCII)!=' ')//има символ
	       STRCPY(vf2->ptext,*wASCII,vf2->len-1);//копоиране на буфера в помощен буфер с дължина=дълж. на екранното поле
	  dprint(activpage,vf2->col,vf2->row,vf2->atr,vf2->ptext);//изобразяване
	  dprint(activpage,vf1->col,vf1->row,vf1->atr,vf1->ptext);//изобразяване
	  SetCursorPos(activpage,vf2->col + *col,vf2->row);//позиция на курсора
     }
   }
}


