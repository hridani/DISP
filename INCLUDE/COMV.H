/******************************************************************************************/
/*                               C O M  V. C P P                                                 */
/*   Деклариране  на общи глобални променливи при работа с библиотечните     */
/*     функции  на "DISP"                                                                          */
/******************************************************************************************/
#ifndef __comv_h
#define __comv_h         
#include <new.h>

typedef unsigned char byte;  //дефинирене на BYTE
typedef unsigned int word;     //дефинирене на WORD
#ifndef     prom
#define  prom
 enum flag{FALSE,TRUE};//дефиниране на булева променлива
 typedef flag (*FCNC)(char *);//дефиниране на указател кум функция 
 #endif
 
 #ifndef  vstr
#define vstr
//видео структури 
struct velb { 
  byte thechar,attribute; //символ и атрибут
   };
struct velw{
  word content;//.заедно в една дума -символ и атрибут
  };
union vel {           //обединение
      struct velb h; 
      struct velw x;
      };
#endif      
typedef union vel   *VP;/* структура VP,която описва двойката ASCII
                                                символ и атрибут  */

 typedef struct  BLOCKINFO //структура за информация за заделената памет ,използува се в "MEMNEW.CPP"
 {   
    public:
     BLOCKINFO  *pbiNext;  //указател към следващия инф. запис
     byte *pb;//блок с информация
     size_t size;//размер
     char * typedata;//име на заделената памет
  }blockinfo;      
  typedef  void ( *TC)(void);     //дефиниране на указател кум функция 
  typedef  void ( *TCNC)(byte );     //дефиниране на указател кум функция 
  typedef  flag ( *TCC)(void  );     //дефиниране на указател кум функция 


#endif
