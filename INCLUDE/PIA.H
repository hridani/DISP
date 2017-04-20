/*************************************************************************************************/
/*                           P I A . H                                                                                 */
/* Описание на базов клас BasePia, производни класове Matrix и Linea .            */
/* Описание на клас списък на Pia.Клас Base -описание на вход или изход       */
/* ListBase-списък на входовете или изходите                                                   */
/*************************************************************************************************/
 #ifndef _INC_PIA    
 #define _INC_PIA
 #pragma comment(lib,"..\\lib\\pia.lib")
 #include "..\include\comv.h"
 int handle_program_memory_depletion( size_t ); 
#ifndef prom  

typedef enum BOOL flag;
#endif
typedef struct typeport
{
  byte type; /* тип на порт */
  byte number; /* брой крачета от порт ,участвуващи в матрица */
}TypePort;     

   
//Базов клас BasePia -общо описание на PIA
 class BasePia
 {
     friend class ListPia;
     protected:
     byte typeA; /* тип на порт А*/
     byte typeB; /* тип на порт B */
     byte typeC1;/* тип на порт C1 */
     byte typeC2;/* тип на порт C2 */
    
     byte number; /* пореден номер на PIA в списъка */

     public:
      unsigned baseadres;/* базов адрес */
     BasePia *next;  /*указател към следващото PIA в списъка с PIA */
     BasePia(unsigned ,byte,byte,byte,byte,byte);/* конструктор */            
     virtual int kind()=0; /* виртуална функция */
     virtual void read()=0; /* четене на входове */
     virtual void write()=0; /* писане в изходи */
     virtual ~BasePia();
};

//Производен клас Matrix-матрично свързано PIA
class Matrix:public BasePia
{          
   friend class ListPIA;
   public:
   struct typeport tport[4]; /* тип и брой на матричните портове */
   byte minput; /* брой матрични входове */
   byte moutput; /* брой матрични изходи */
   byte vinput; /* брой свободни входове*/
   byte voutput; /* брой свободни изходи  */
   byte *pinput;/* указател към стойностите на входовете в паметта */
   byte *poutput;/* указател към стойностите на изходите в паметта */
   byte *pitable; /* указател към таблицата с данни за матричните портове */
   byte *potable; /* указател към таблицата с данни за матричните изходи */
   byte *pvtable; /* указател към таблицата с данни за свободните портове */
   Matrix(unsigned,byte,byte,byte,byte,byte,byte,byte);/* конструктор */
   void TMatrix(byte,byte,byte,byte);/* описание на матрицата */
   virtual int kind() {return 0;} 
   virtual void read(); /* четене на матрица и свободни входове */
     void reada(void);
   virtual void write(void); /* писане в изходи */
   ~Matrix();
  
};

//Производен клас Linea- линейно свързани PIA
class Linea:public BasePia
{          
   friend class ListPIA;
   public:
   unsigned linput; /* брой входове */
   unsigned loutput; /* брой изходи */
   byte *pinput; /* указател към стойностите на входовете в паметта */
   byte *poutput;/* указател към стойностите на изходите в паметта */
   byte *pitable; /* указател към таблицата с данни за  портове */
   Linea(unsigned,byte,byte,byte,byte,byte);/* конструктор */
    virtual int kind() {return 1;}
    virtual void read();  /* четене на входове */
    virtual void write(void); /* писане в изходи */
    ~Linea();//деструктор
};        

         //нов клас за LPT
class LineaLpt:public BasePia
{          
   friend class ListPIA;
   public:
   unsigned linput; /* брой входове */
   unsigned loutput; /* брой изходи */
   byte *pinput; /* указател към стойностите на входовете в паметта */
   byte *poutput;/* указател към стойностите на изходите в паметта */
   byte *pitable; /* указател към таблицата с данни за  портове */
   LineaLpt(unsigned,byte,byte,byte,byte,byte);/* конструктор */
    virtual int kind() {return 2;}
    virtual void read();  /* четене на входове */
    virtual void write(void); /* писане в изходи */
    ~LineaLpt();//деструктор
};
     //матрица от 2 пиа
class MatrixLpt:public BasePia
{          
   friend class ListPIA;
   public:
   unsigned baseadres1;
   unsigned baseadres2;
   byte minput; /* брой матрични входове */
   byte moutput; /* брой матрични изходи */
   byte vinput; /* брой свободни входове*/
   byte voutput; /* брой свободни изходи  */
   byte *pinput;/* указател към стойностите на входовете в паметта */
   byte *poutput;/* указател към стойностите на изходите в паметта */
   byte *pitable; /* указател към таблицата с данни за матричните портове */
   byte *potable; /* указател към таблицата с данни за матричните изходи */
   byte *pvtable; /* указател към таблицата с данни за свободните портове */
   MatrixLpt(unsigned,unsigned);/* конструктор */     
   void TMatrix();/* описание на матрицата */
    virtual int kind() {return 3;}
    virtual void read();  /* четене на входове */
    virtual void write(void); /* писане в изходи */
    ~MatrixLpt();//деструктор
};




//Списък на PIA 
class ListPia
{
   public:
   BasePia *top,**cur;//указател начало на списъка и реботен укзател
   int nm;//брой PIA
   ListPia();    //конструктор
   ~ListPia() {removeall(&top); }//деструктор
   void removeall(BasePia **);//освобождаване на паметта от списъка PIA
   flag add(char,unsigned,byte,byte,byte,byte,byte,byte);//добавяне на елементPIA към сп.
   void find(unsigned);//търсене на PIA 
  // void find(byte);//търсене на PIA 
};  

//Елемент от PIA-образуващи входове и изходи  
 class Base
 {
     friend class List;
     public:
     unsigned adres;/* адрес на порта ,на който е крачето */
     byte value; /* стойност на крачето */
     byte mask;/* маска на крачето */
     byte bit;/* номер на бит в порта */
     unsigned number; /* номер на краче */
     Base *next;  /* следващо краче */          
     Base(unsigned );  //конструктор - номер 
     Base(unsigned,byte);//конструктор-номер на елемент и стойност
     Base(unsigned,unsigned,byte);//конструктор-номер,адрес и бит от порта
 };
//Списък на елементи -изграждане на списък входове и списък изходи
class List
{
   public:
   Base *top,**cur;//указател -начало и текущ указател
   unsigned nm;//брой елементи
   List();    //конструктор на списъка
   ~List() {removeall(&top); }//деструктор-освобождаване на памет
   void removeall(Base**);//изтриване на списъка
   byte& operator()  (unsigned);//връща стойност за елемента със зададен номер
   byte& operator() (unsigned,byte);//инициализаира елемент със зададената стийност
   flag add();          //добавяне на елемент към списъка /ключ номер/
   flag add(unsigned,byte);//добавяне на елемент към списъка /ключ-номер/,задават се адрес и пореден бит в порта
   void find(unsigned);//търсене на елемент от списъка по номер
};  
       
 #endif
