/*******************************************************************************/
/*                            T A B L E . H                                     */
/*------------------------------------------------------------------------------------------------*/
/*          Деклариране на  клас NodeT и ListNode               */
/*********************************************************************/
 #ifndef _INC_TABLE          
 #define _INC_TABLE
 #pragma comment(lib,"..\\lib\\table.lib")
 
 #include "..\include\comv.h"    
  #include <stdio.h>
 #ifdef __cplusplus
  extern "C"{
 #endif

 #if( _MSC_VER<=600)
 #define __cdecl _cdecl
 #define __far   _far
 #define __near  _near
 #endif
 

//описание на клас NodeT
class  NodeT
 {
    public:
     unsigned  numb;  /* номер на параметър*/
     unsigned numbnz;//пореден номер в списъка
     double value;                   //стойност
     double min;//минимално допустима стойност
     double max;//максимално дпустима стойност
     char *name;//име на параметъра
     NodeT *next; /*наследник */
     NodeT *back;//предшественик
     NodeT(unsigned,unsigned,double,double,double,char *,NodeT *); /* конструктор */
     NodeT(unsigned,unsigned,NodeT *); /* конструктор */
     ~NodeT();//деструктор
   };   
     
 
 //описание на клас ListNode  -списък      
class ListNode
 {     
   public:
   NodeT *top,**cur,*bcur;  /* указатели към началният,текущият и последният елементи */ 
   unsigned nb;  /* брой възли */
   ListNode(); /* конструктор */
   ~ListNode();/* деструктор */
   void find(unsigned );  /* търсене по ключ-номер */
   void findnz(unsigned );  /* търсене по ключ-номер */
   void eraseNodeT(unsigned); /* изтриване на еленент с посочения ключ */
   void removeall(NodeT **);/* изтриване на списъка */
   flag add(unsigned,double,double,double,char *);//добавяне на елемент
   flag add(unsigned);//добавяне на елемент
   void saveNodeT(FILE *,NodeT *);/* запис във файл на един възел */
   void saveall(char *);/* запис във файл на списъка */
   void load (char *); /* четене от файла */
  // void L_get_last(void);//търсене на последния
  };
flag ReadParamMem(unsigned , char,void *);//четене на параметър -номер,тип на данната
                                                    //указател към данната за запис на извлечената стойност
 
 #ifdef __cplusplus
  }
 #endif
 
#endif
