/*******************************************************************************/
/*                                 O F F S E T                                               */
/*------------------------------------------------------------------------------------------------*/
/*          Деклариране на  клас OFFSET     */
/*********************************************************************/
 #ifndef _INC_OFFSET
 #define _INC_OFFSET
 #pragma comment(lib,"..\\lib\\offset.lib")    
 #include <stdio.h>

 class  Offset
 {
    friend class ListOF;
    public:
     int  numb;  /* номер на записа */
     int numbtool;//номер на инструмента
     int kdw; //координационна двойка
     int npar; //деоствителен брой параметри
     double par[8]; //стойност на параметрите -офсети по оси
     Offset *next; /*наследник */
     Offset *back;/* предшественик */  
     Offset(int ,int ,int,Offset *);  //конструктор
     Offset(int ,int ,int ,Offset *,double ,double );
     };

/* Декларация на клас ListOF */    
 class ListOF
 {     
   public:
   Offset *top,**cur,*bcur;  /* указатели към началният,текущият и последният елементи */
   int nb;  /* брой възли */
   ListOF(); /* конструктор */
   ~ListOF();/* деструктор */
   void find(int );  /* търсене по ключ-номер */
   void findnt (int ); //търсене по номер на нож
   void findtkw (int ,int );
   void removeall(Offset **);/* изтриване на списъка */
   void L_get_last(void); /* намиране на последния  */
   void saveListOF(FILE *,Offset *);/* запис във файл на един възел */
   void add(int ,int ,int ); /* добавяне на възел */                                           
   void add(int,int ,int ,double =NULL,double =NULL);
   void saveall(char *);/* запис във файл на списъка */
   void load (char *); /* четене от файла */
   void  eraseOffset( int ); /* изтриване на един елемент от списъка */
  };

#endif
 
