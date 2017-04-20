/*****************************************************************************/
/*                            K E Y . H                                               */
/*  Деклариране на класове FuncKey,Key,ListKey                */
/*   и на структура KEYBUFFER                                            */
/*******************************************************************************/
 #ifndef _INC_KEY    
 #define _INC_KEY
 #pragma comment(lib,"..\\lib\\key.lib")
 #include "..\include\comv.h"
 int handle_program_memory_depletion( size_t ); 

// описание на структура  KEYBUFFER
typedef struct KEYBUFFER
{
   unsigned char *scanbuf;//начало на буфера
   unsigned char *scanend;//край на буфера
   unsigned int scannext;//последен за запис
   unsigned int scanlast; //последен прочетен
} KeyBuffer;

 void IniBuffer(KeyBuffer *,int);//заделяне на памет за клавиатурен буфер,инициализация
 void EndBuffer(KeyBuffer  *);//освобождаване на памет

 //описание на клас FuncKey - описание на функц. кл.
 class FuncKey 
 {
   friend class Key;
   public:        
     char *name; //име
     TC activf; //указател към функция на клавиша
     TC crfunc;//указател към CR функция на клавиша
     FuncKey(char *,TC,TC);   //конструктор
      ~FuncKey () {delete name;} //деструктор
};

// описание на клас Key  - функц. кл. като елемент
class Key
{          
   friend class ListKey;
  public:
    FuncKey *funck;  //указател към функц. кл.
    int number;//номер на функц. кл. в списъка
    Key *next;  //указател към следващия елемент
    Key(FuncKey *);//конструктор
};

//описание на клас ListKey -списък на функц. клавиши
class ListKey
{
   public:
   Key *top,**cur; //указатели към началото и текущия елемент
   int nm;  //брой елементи - функц. кл.
   ListKey();    //конструктор
   ~ListKey() {removeall(&top); } //деструктор
   void removeall(Key **);//изтриване на списъка
   flag add(FuncKey *); //добавяне на елемент
   flag add(FuncKey *,int ); //добавяне на елемент с посочен номер
   void find(FuncKey *);//търсене на елемент по указател към функц. кл.
   void find(int );//търсене на елемент по номер на функц. кл..
   void eraseKey(FuncKey *);
};  
  
       
 #endif
