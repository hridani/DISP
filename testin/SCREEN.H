/******************************************************************************************/
/*                               S C R E E N . H                                                */
/*   Деклариране на функции за работа с екрана ,като задача на "DISP"     */
/******************************************************************************************/
 #ifndef __screen_h                                   
 #define __screen_h  

 #include "..\include\stfile.h" 
 #include "..\include\comv.h" 
 #include "..\include\vfield.h" 

 void IniEkran(byte); //Инициализациа на екрана  -потребителска
 void DrawField(  byte ,char *,ListVF *); //Изчертаване на постоянните текстови полета
 void DrawACScreen(ListVF *);// Изобразяване на екрана от посочено копие в ОП -потребителска*/
 void DrawScreen(char *,ListVF *);//Изобразяване на екрана на полта от посочен файл и посочено име на списък -потребителска
 void DrawFuncFrame(byte );//Изчертаване рамката на функц. кл.
 void ScrFK(void);//Обноваване на функц. полета на екрана
 void PExitField(ListFile *,ListVF *);  //Обновяване на изходните полета на екрана
  void PExitField(ListFile *,ListVF *,int,int);  //Обновяване на изходните полета на екрана
    void PExitField(ListFile *,ListVF *,int);  //Обновяване на изходните полета на екрана
void PEnterField(VField *vf1,VField * =NULL,int =0);//Обноваване на входно-изходните поолета/до 2/ на екрана
void PrintField(ListVF * ,int,char ,void *);//изобразчване на поле от списък с определен номер
 void SlowPrint(void); //Задача на "DISP" -обновяване на екрана през определено време
 void PrintInf(void);     
 #endif                      
 