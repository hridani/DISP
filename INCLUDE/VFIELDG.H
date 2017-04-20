/*******************************************************************************/
/*                               V I D E O   F I E L D                                   */
/*------------------------------------------------------------------------------------------------*/
/*          Деклариране на  клас VIDEO FIELD     */
/*********************************************************************/
 #ifndef _INC_VFIELD      
 #define _INC_VFIELD
 #pragma comment(lib,"c:\\msvc\\dani\\lib\\gr.lib")    
 #include "c:\msvc\dani\include\comv.h"    
 #include <stdio.h>
 #define COLOR(VG,HG)  ((VG<<4) +HG)  /* изчисляване на атрибута на символа */
 class  VField 
 {
    friend class ListVF;
    public:
     byte numb;  /* номер на полето */
     byte row,col,len,atr;/* ред и колона,дължина ,цвят на символите и фона */
     char *ptext;  //указател към текста на полето                             
     VField *next; /*наследник */
     VField *back;/* предшественик */
     VField(byte,byte,byte,byte,byte,char*,VField*); /* конструктор */
     ~VField();
     };

/* Декларация на клас ListVF */    
 class ListVF
 {     
   public:
   VField *top,**cur,*bcur;  /* указатели към началният,текущият и последният елементи */
   byte nb;  /* брой възли */
   VP sptr;//указател към изображението в паметта
   ListVF(); /* конструктор */
   ~ListVF();/* деструктор */
   void find(byte );  /* търсене по ключ-номер */
   void removeall(VField **);/* изтриване на списъка */
   void L_get_last(void); /* намиране на последния  */
   void saveListVF(FILE *,VField *);/* запис във файл на един възел */
   void add(byte ,byte ,byte ,byte ,byte ,char *); /* добавяне на възел */
   void saveall(char *);/* запис във файл на списъка */
   unsigned char load (char *); /* четене от файла */
   void  eraseVField( byte ); /* изтриване на един елемент от списъка */
  };

#endif
 
