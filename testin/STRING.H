/*******************************************************************************************************/
/*                                         S T R I N G . H                                        */
/*Деклариране на функции за работа с низове и преобразуване на дан. от един тип в друг*/
/*****************************************************************************************/
 #ifndef __string_h
 #define __string_h                               
 #include <sys\types.h>
 #include <sys\stat.h>
 #include "c:\msvc\dani\include\comv.h"  
 
void UnsToStr(unsigned ,char *); //преобразуване на беззнаково число в стринг
void LongToStr(long,char *);//преобразуване на дилго цяло число в стринг
void IntToStr(int,char *);  //преобразуване на цяло число в стринг
void UnsLToStr(unsigned long ,char *);//преобразуване на беззнаково дълго цяло число в стринг
char *STRCPY(char *,char *,int ) ;//функцията копира стрингов низ в друг с посочен размер      
char * STRSRCH(char *,char *);//търсене на низ в друг низ             
char  * STRCHR(char *,unsigned char);//търсене на символ в  низ           
char * STRCOPY(char *,char *,int); //копиране на низ в друг низ от определена позиция    
char * STRCOPYEDIT(char *,char *,int); //копиране на низ в друг низ от определена позиция    
void Deletechar(char *,int);//изтриване на определен брой символи от даден низ от посочената позиция      
void Insertsr(char *,char,int);//добавя на определено место в символния низ посочения символ     
flag strFindUns(char *,unsigned*);//преобразува  стринг  в цяло беззнаково число /тип unsigned  /        
flag strFindInt(char *,int *);// преобразува  стринг  в цяло число /тип int  /
flag strFindLong(char *,long *); //преобразува  стринг  в цяло число /тип long / 
flag strFindUnsL(char *,unsigned long *);//преобразува  стринг  в цяло дълго беззнаково число /unsigned long /
void String_convert(char ,char *,void *);// преобразуване на низ в число (int,double и др ) 
void  Number_convert(char ,void *,char *); // преобразуване на число (int,double и др ) в низ 
void  StatDos( char *,struct _stat &); //връща информация за файла с посочено име -дата,размер и др.
char *STRCPYCEN(char *,char *,int );//копиране на един низ в друг с посочен размер
void Insertstr(char *,char ,int);//включване на посочен низ в друг от зададена позиция
#endif                      
   
