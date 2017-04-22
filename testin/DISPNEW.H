/******************************************************************************************/
/*                               D I S P N E W . H                                                */
/*   Деклариране на функции за работа с "DISP"  и на "DISP"                       */
/******************************************************************************************/
#ifndef __dispnew_h
#define __dispnew_h 
#include "..\include\classth.h"
int handle_program_memory_depletion( size_t );
TaskHandle* CreateTask(char *,State,unsigned ,Kind ,unsigned long,unsigned ,TC,int * =NULL);
 //дефиниране на задача на "DISP" - потребителска */
           

 void __interrupt __far DISPT(); // функция "DISP" - избор на задача     
 void __interrupt __far DISP8(); // функция "DISP" - избор на задача  
 
 void ExitSysT(void);  //изход от ситемата -потребителска       
 void ExitSys8(void);  //изход от ситемата, когато се използва системния таймер -потребителска     
 void Ini8(void);//инициализация при изход от системата - системен таймер
  void IniT(void);  //инициализация при изход от системата -друг таймер
 TaskHandle* FindSlow(TaskHandle **);//търсене на бавна задача с най-висок проритет
 void MeasTime(void); //изчисляване времето на периодичните задачи 
 void StartFast(void);//стартиране на бързи задачи
 void FindSem(void);//проверка на лог.ъравнение за активност на чакащите задачи
 void CurWait(void);//променя състоянието на тек. задача в чакаща
 void CurDelay(void); //променя състоянието на тек. задача в периодична
 void Delay  (void); //потребителска функция - сигнализира "DISP" ,че тази задача ще чака условия 
void WaitCond(void);//потребителска функция - сигнализира "DISP" ,че тази задача ще чака време
void WaitTaskCond(TaskHandle *);//потребителска функция - сигнализира "DISP" ,че тази посочената задача ще чака условия
void whlt(void); //празен цикъл -изчаква се "DISP"
void InTimer(unsigned ,unsigned int ,unsigned int ); //иниц. на таймера
void EndTask(void);//подготва за изход от програмата 
void *dek(char *);  //кодиране на лог. уравнение за активност в паметта -потребителска 
void dek(char *,int **);//кодиране на логическото условие в паметта
 flag check_con(void *); //проверка на лог.уравнение за активност
void  Resume(TaskHandle *);  //активира избрана задача 
void  NewSuspend(TaskHandle *);//дадена задача става неактивна
  //работа със семафори  -потребителски функции
void SignalSem(unsigned ); //вдига лог. семафор и увеличава с 1 аритм. семафор
void NoSignalSem(unsigned );//свалч лог. семафор или намалява с 1 аритм.сем.
void AritSem(unsigned ,unsigned);//установава аритм. сем със посочената стойност
void TimeSem(unsigned ,unsigned);//иниц. семафор за време
unsigned ReadSem(unsigned ); // чете семафора с посочен номер
byte Input(unsigned );//връзка на физическите семафори с логическите
void IniV(void); 
#endif                      
