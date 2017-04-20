/*************************************************************************************************/
/*                           C A G E . H                                                                                 */
/* Описание на базов клас BaseCage, производни класове Matrix и Linea .            */
/* Описание на клас списък на Pia.Клас Base -описание на вход или изход       */
/* ListBase-списък на входовете или изходите                                                   */
/*************************************************************************************************/
 #ifndef _INC_CAGE   
 #define _INC_CAGE
 #pragma comment(lib,"c:\\msvc\\dani\\lib\\cagesm.lib")
 #include "c:\msvc\dani\include\comv.h"
 int handle_program_memory_depletion( size_t ); 



//Базов клас BaseCage -общо описание на клетка
 class BaseCage
 {
     friend class ListCage;
     protected:
          byte number; /* пореден номер на клатеката в списъка */

     public:     
     byte numb_money;//брой монети             
     byte nwork; //текуща операция          
     unsigned tcur;   //текъша време за операция
     unsigned tbegin;    //чачално заребдане в арит. семафор за време
             //входове
     byte input_money; //вход за монета                      
     byte input_stop;    //          СТОП бутон     
     
     byte input_cold; /* вход- студена вода */
     byte input_hot; /* вход топла вода */ 
     byte input_soap; /* вход сапун*/
     
     //изход
     byte output_money[6];//изход -6 броя
     
     byte output[3]; /* изход сапун*/  /*  изход- студена вода */      /* изход топла вода */   
               
     //параметри
     unsigned time[3]; /* време -студена водаза 1кредит  */    /* време - топла вода за 1  кредит */      /* време - сапун за 1 кредит  */
   
    double kredit; //kredit -1 монета = пар.кредити, управлението на времрто и изходите за пари е чрез кредит
    double cur_kr1,cur_kr2;
    double cur_kredit;//текущ кредит
     
   double kredit_s[3];//кредит за 1 с за операция(ст.вода, топла вода , сапун)  в селунди
   double kredit_op[3];//кредит за опреция         
   double sum_kredit[3];//сума от закупени кредити по операции
     BaseCage();/* конструктор */       
     BaseCage(byte, byte,double,byte,byte ,byte ,byte,unsigned , byte,byte,unsigned , byte,byte,unsigned, byte * , double ,double, double); /* конструктор */      
        BaseCage(byte, byte,double,byte,byte ,byte ,byte,unsigned , byte,byte,unsigned , byte,byte,unsigned); 

     ~BaseCage();
};

#endif
