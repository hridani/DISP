 #ifndef _INC_USEPIA 
 #define _INC_USEPIA
 #pragma comment(lib,"..\\lib\\pia.lib")
 #include "..\include\comv.h"    
 

void InitPia(unsigned ,byte ,byte ,byte ,byte , byte ,byte =0,byte =0);  /*������������� �� PIA */
void   DiscrMatrix(unsigned ,byte ,byte ,byte ,byte );/** �������� �� ��������� �� ��� �� ��������� � ���� ������ */      
void   DiscrMatrixLpt(unsigned adres);//�������� �� ���������� ������� �� 2 ���
void ReadInput(void);     /* �������������� ������ �� ������ PIA � ������a */
double  ReadParam(long);
void ReadParam(void);
void IniMask(void);/* �������������� �� ������� �� ��������� � �������� */
void ReadParamIniIO(void);     
void IniIOMask(void);   
byte ReadSerInput(void);//����� ��������� �� ������ �����
void IniSerialInput(void);//������������� �� ��������� ,��������� �� �������� �����  
void EndPIA(void);//��������������� �� PIA

#endif
