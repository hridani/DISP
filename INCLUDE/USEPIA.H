 #ifndef _INC_USEPIA 
 #define _INC_USEPIA
 #pragma comment(lib,"..\\lib\\pia.lib")
 #include "..\include\comv.h"    
 

void InitPia(unsigned ,byte ,byte ,byte ,byte , byte ,byte =0,byte =0);  /*инициализация на PIA */
void   DiscrMatrix(unsigned ,byte ,byte ,byte ,byte );/** описание на матрицата по тип на портовете и брой битове */      
void   DiscrMatrixLpt(unsigned adres);//описание на кръстосана матрица от 2 лпт
void ReadInput(void);     /* последователно четене на всички PIA в списъкa */
double  ReadParam(long);
void ReadParam(void);
void IniMask(void);/* инициализиране на маските на входовете и изходите */
void ReadParamIniIO(void);     
void IniIOMask(void);   
byte ReadSerInput(void);//етене входовете по сериен канал
void IniSerialInput(void);//инициализация на входовете ,прочетени по серийния канал  
void EndPIA(void);//деиницилизиране на PIA

#endif
