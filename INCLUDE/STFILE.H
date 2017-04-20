/*******************************************************************************/
/*                            S T R U C T    F I L E                                     */
/*------------------------------------------------------------------------------------------------*/
/*          Деклариране на  клас STRUCT FILE                   */
/*********************************************************************/
 #ifndef _INC_STFILE           
 #define _INC_STFILE
 #pragma comment(lib,"..\\lib\\file.lib")
 

 #ifdef __cplusplus
  extern "C"{
 #endif

 #if( _MSC_VER<=600)
 #define __cdecl _cdecl
 #define __far   _far
 #define __near  _near
 #endif
 
 #include "c:\msvc\dani\include\comv.h"    
 #include <stdio.h>
   #include <vmemory.h>
//описание на абстрактен клас BaseFile
class  BaseFile
 {
    public:
     int  numb;  /* номер на ред в CNC файла */
     int nmzapis; /* номер на записа в списъка */
     BaseFile *next; /*наследник */
     BaseFile *back;/* предшественик */
     char *ptext;  /* указател към текста на ред от CNC файла */ 
     BaseFile(int,BaseFile*,char *); /* конструктор */
     ~BaseFile();   
     void change(char *);
   };   
 //описание на клас ListFile  -списък      
class ListFile
 {     
   public:
   BaseFile *top,**cur,*bcur;  /* указатели към началният,текущият и последният елементи */ 
   int nb;  /* брой възли */
   ListFile(); /* конструктор */
   ~ListFile();/* деструктор */                                                
   ListFile operator =(ListFile&);
	void find(int );  /* търсене по ключ-номер */
	void finds(int );  /* търсене по ключ-номер */
	void findnz (int ); //търсене по пореден номер в списъка
	void eraseFile(int); /* изтриване на еленент с посочения ключ -номер в файла*/
	void eraseFilenz(int); /* изтриване на еленент с посочения ключ  -пореден номер в списъка*/
	void removeall(BaseFile **);/* изтриване на списъка */
	flag add(int ,char * =NULL,unsigned =0); /* добавяне на възел */
	flag addn(int ,char * =NULL,unsigned =0); /* добавяне на възел */
	void L_get_last(void);//търсене на последния
	unsigned char load(char *);
	void  saveall( char *);  
	void saveList(FILE *,BaseFile *);
  };
     
     
  
     
 //описание на клас RecFile - информащионен елемент 
 class RecFile
 {
   friend class ListRecFile ;
   public:
     char *nmfiledisk; /* име на файла ,който е записан на диска */
     char *nmfilemem; /* име на файла ,който е записан в ОП */
     char *memfile;/* указател към записа в ОП */      
    // _vmhnd_t memfile;
     RecFile *next; /* следващ запис */
     RecFile(char *,byte); /* консруктор */
     ~RecFile();//деструктор
 };
 
 //описание на клас ListRecFile - информационна таблица
 class ListRecFile
 { 
   public:
   RecFile *top,**cur;/* указатели към върха и текущия елемент в списъка */
   int nm; /* брой елементи */
   ListRecFile(); /* конструктор */
   ~ListRecFile();/* деструктор */
   void finddisk(char * );  /* търсене по ключ-номер */
   void findmem(char * );  /* търсене по ключ-номер */
   void eraseRecFile(char *); /* изтриване на еленент с посочения ключ */ 
   void removeall(RecFile **);/* изтриване на списъка */
   flag adddisk(char *); /* добавяне на възел */
   flag addmem(char *); /* добавяне на възел */
} ;  
 
 #ifdef __cplusplus
  }
 #endif
 
#endif
