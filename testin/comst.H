/******************************************************************************************/
/*                               C O M S T . H                                                 */
/*   Деклариране на общи глобални променливи при работа с "DISP"       */
/******************************************************************************************/
#ifndef __comst_h
#define __comst_h         
#include <new.h>
#include "..\include\comv.h"
#include "..\include\classth.h"
#include "..\include\vfield.h"
#include "..\include\key.h"
#include "..\include\pia.h"
#include "..\include\table.h"
#define Tk 1 /* дефиниране  такта на системата - време на квантуване -0.001с */
          extern    unsigned numbinpPIA;
extern TaskHandle *kboard,*screen,*idle,*inpi; //задачи  на "DISP" -"Keyboard","Screen","Idle"
extern int activpage;//видеостраница на активното изображение
extern flag flagend;  /* флаг за край на задачите на диспечера */
extern flag flag_change; // флаг за избор на нова задача
extern StateList READYF,READYS,WAIT,DELAY,SUSPEND; /* списъци със задачи по групи */
extern TaskHandle *CURTASK; /* текуща задача,изпълнявана от диспечера */
//extern byte *tsem,*tsem1,*tsem2;  /* таблица на семафорите */ 
extern byte tsem[3200];
extern unsigned size; /* размер на заетата памет */
extern unsigned numbls;//брой логически семафори
extern unsigned numbas;// брой аритметични семафори
extern unsigned numbts;//брой семафори за време
extern unsigned numbis; //семафори-вход          
extern unsigned numbirq;//номер на прекъсване използвано от DISP
extern List input;//входове -номер и стойност
extern List output;//изходи
extern ListVF *activ; /* указател към активното изображение в текущия режим */
extern ListVF lfunc;/* списък на полетата на функционалните клавиши */ 
extern ListVF lfunc1;/* списък на полетата на функционалните клавиши */ 
extern KeyBuffer *infbuffer; /* буфер с ASCII кодове */
extern KeyBuffer *kb;   /* указател буфер с позиционни кодове */
extern ListKey *dk; /* списък  функционални клавиши в активния режим*/
extern Key **aKey; /* активен функционален клавиш */
extern char * Inpos;/* таблица с информация POS код- значение */
extern char **wASCII ;/* указател към указателя на входно-изходното поле в текущия режим */
extern  byte *col;//указател към текущата колона на входното поле
extern int *hr; /* указател към активния хоризонтален ред */
extern int *wr; /* указател към активната вертикална колона */
extern flag flag_wait;//флаг за изчакване извикването на "DISP"
extern int *posl; /* позиция на най-левия функц.клавиш */
extern int *post;/* позиция на реда функц.клавиши */
extern int posa; /* позиция на избрания функционален клавиш */
extern int posm;// относително положение на марк. курсор - за отбелязване на изходните полета
extern int adrescom;//адрес на асинхронния адаптер COM1(0x3F8-0x3FF),COM2(0x2F8-0x2FF)
//extern ListNode listnode; //списък на параметрите      
extern  TC ExitSys;//укзател към функция з аизход опт системата       
extern TC Ini;//указател към функция за инициализация
extern TC PrintEnterFields;//указател към функция за дисплей на входно/изходно поле
extern TC PrintExitFields; //указател към функция за дисплей на изходни полета
extern TC PrintUserFields;//указател към потребителска  функция за дисплей на изходни полета
extern TC MKTop;//указател към функция за движение нагоре 
extern TC MKBottom;//указател към функция за движение надолу
extern TC MKInsert; //указател към функция за вмъкване
extern TC MKHome;//указател към функция "HOME"
extern TC MKPgUp;//указател към функция за движение "страница-нагоре"
extern TC MKPgDn;//указател към функция за движение "страница-надолу"
extern TC MKDel;//указател към функция за изтриване
extern TC MKRight;//указател към функция за движение надясно
extern TC MKLeft;  //указател към функция за движение наляво
extern TC F12;//указател към функция F12
extern TC F10;//указател към функция F10
extern TC F11;//указател към функция F11
extern TC F9;//указател към функция F9
extern TC F8;//указател към функция F8
extern TC ESC;//указател към функция ESC
extern TC CRFUNC; /* указател към функцията на CR */
extern TC CRFUNCOLD; /* указател към прдходна функция на CR */
extern  TC WorkingInput; //указател към функция за обработка на входовете
extern ListVF vidnode; //списък на постоянните текстови полета в екран TABLE
extern TC EndUseFunc;//указател към потребителска функция за край 
extern TC PrintUseFunc;//указател към потребителсака функция за дисплей
extern TC UseSemafor;//указател към потребителсака функция за обработка на семафорите
#endif
