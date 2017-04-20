/*************************************************************************************************/
/*                           C A G E . H                                                                                 */
/* Описание на базов клас BaseCage, производни класове Matrix и Linea .            */
/* Описание на клас списък на Pia.Клас Base -описание на вход или изход       */
/* ListBase-списък на входовете или изходите                                                   */
/*************************************************************************************************/
 #ifndef _INC_CAGE   
 #define _INC_CAGE
 #pragma comment(lib,"..\\lib\\cage.lib")
 #include "..\include\comv.h"
 int handle_program_memory_depletion( size_t ); 



//Базов клас BaseCage -общо описание на клетка
 class BaseCage
 {
     friend class ListCage;
     protected:
          byte number; /* пореден номер на клатеката в списъка */

     public:     
     byte numb_money;//брой монети   
     byte input_money; //вход за монета                      
     byte input_emsstop;
     byte output_money;//изход -приета монета    
     byte output_light;//изход осветления на клетката   
     BaseCage *next;  /*указател към следващото PIA в списъка с PIA */
     BaseCage(byte,byte,byte,byte,byte);/* конструктор */            
     BaseCage();/* конструктор */ 
     virtual int kind()=0; /* виртуална функция */
     virtual ~BaseCage();
};

//Производен клас Wash -описва миене със студена, топла вода и сапун
class Wash:public BaseCage
{          
   friend class ListCage;
   public:
   byte input_cold; /* вход- студена вода */
   byte input_hot; /* вход топла вода */ 
   byte input_soap; /* вход сапун*/
   byte output_soap; /* изход сапун*/      
   byte output_cold; /*  изход- студена вода */
   byte output_hot; /* изход топла вода */
   unsigned time_cold; /* време -студена вода  */       
   unsigned time_hot; /* време - топла вода  */
   unsigned time_soap; /* време - сапун  */
    unsigned m_cold;//сума -монети за студена вода
     unsigned m_hot;//сума- монети за т.вода
     unsigned m_soap;//сума- монети за сапун

   Wash(byte, byte,byte,byte ,byte,byte ,byte,unsigned , byte,byte,unsigned , byte,byte,unsigned  ); /* конструктор */      
   Wash();
   virtual int kind() {return 0;} 
    ~Wash();
  
};

//Производен клас Linea- линейно свързани PIA
class Vac:public BaseCage
{          
   friend class ListCage;
   public:
    byte output_vac;//изход -прахосм.
    unsigned time_vac;//време прахосм
    unsigned m_vac;//сума - монети прах.
    Vac(byte,byte,byte,byte,byte,byte,unsigned);/* конструктор */
    virtual int kind() {return 1;}
    ~Vac();//деструктор
};        

       
//Списък на клатки -Listcage
class ListCage
{
   public:
   BaseCage *top,**cur;//указател начало на списъка и реботен укзател
   int nm;//брой PIA
   ListCage();    //конструктор                         
   ~ListCage() {removeall(&top); }//деструктор
   void removeall(BaseCage **);//освобождаване на паметта от списъка PIA
   flag add(char,byte,byte,byte,byte,byte,byte,unsigned,byte,byte,unsigned,byte,byte,unsigned);//добавяне на елементPIA към сп.
   void find(byte);//търсене на cage

};  

 #endif
