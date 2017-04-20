/****************************************************************************************/
/*                                 C L A S S T H . H                                          */
/*     Дефинирене на клас TaskHandle ,ItemTask,StateList                                         */
/****************************************************************************************/
 #ifndef _INC_CLASSTH
 #define _INC_CLASSTH
 #pragma comment(lib,"..\\lib\\classth.lib")
 #include "..\include\memnew.h "
 #include "..\include\comv.h"
 #include <stdio.h>
   enum State{Current,Ready,Wait,Timed,Suspend};//състояние на задачите - текуща,
              // готова,чакаща,периодична
   enum Kind {Fast,Slow}; //вид -бързи ,бавни
   int handle_program_memory_depletion( size_t ); //функция за съобщение при недостиг на памет

 // Описание на клас TaskHandle -описание на задача на "DISP"
 class TaskHandle  
 {
   public:        
     char *name; //име
     State st;     //състояние
     unsigned ucurbp; //текущо състочние на рег. bp -дъно на стека
     unsigned priority; //приоритет
     Kind kind; //вид
     unsigned *bstack; //начало на стека на задачата
     unsigned ucurss; //текуща стойност на рег. ss
     unsigned ubeginbp;//начална стойност на рег. bp
     unsigned long time; //период ,ако задачата е периодична
     unsigned long timec;//брояч на периода
     int *taskcond;//указател към логическото уравнение за активност
     TC taskcode;  //указател към функцията на задачата
 //    void *operator new (size_t); //заделяне на памет
 //    void operator delete (void *); //освобождаване на памет
     TaskHandle( char *,State ,unsigned,Kind,unsigned long ,unsigned ,TC ) ; //конструктор
     ~TaskHandle () ;//деструктор
  };
  
 // Описание на клас ItemTask - възел от списъка със задачи
  class ItemTask
  {
    friend class StateList; //причтелски клас StateList
    public:
      TaskHandle *th; //указател към задачата
      ItemTask *next;  //указател към следващия възел
      ItemTask *back;//указател към предходния възел
      ItemTask(TaskHandle *,ItemTask *); //конструктор
      ~ItemTask();
 //     void *operator new (size_t ); //заделяне на памет за обект от този клас
 //     void operator delete (void *); //освобождаване на памет
   };
   
 // Описание на клас StateList - описание на списъка от задачи
   class StateList
   {           
    public:  
      ItemTask *top,**cur,*bcur; //указтели към началото ,текущ елемент и края на списъка
      int nm;    //брой елементи   
      StateList();   //конструктор      
      ~StateList() {removeall(&top); } //деструктор
      flag add(TaskHandle *);//добавяне на елемент /задача / в списъка
      void find(TaskHandle *); //турсене на елемент /задача / в списъка по посочен указател    
      void removeall(ItemTask **pit);//изтриване на целия списък
      flag eraseTask(TaskHandle *);//изтриване на елемент /задача/ по посочен указател
      void saveList(FILE *,ItemTask*); //запис на елемент във файл   
     void saveall( char * ); //запис на списъка във файл с посочено име
    // void L_get_last(); //търсене на последния елемент
   };

       
 #endif
