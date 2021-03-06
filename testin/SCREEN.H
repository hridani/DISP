/******************************************************************************************/
/*                               S C R E E N . H                                                */
/*   ����������� �� ������� �� ������ � ������ ,���� ������ �� "DISP"     */
/******************************************************************************************/
 #ifndef __screen_h                                   
 #define __screen_h  

 #include "..\include\stfile.h" 
 #include "..\include\comv.h" 
 #include "..\include\vfield.h" 

 void IniEkran(byte); //������������� �� ������  -�������������
 void DrawField(  byte ,char *,ListVF *); //����������� �� ����������� �������� ������
 void DrawACScreen(ListVF *);// ������������ �� ������ �� �������� ����� � �� -�������������*/
 void DrawScreen(char *,ListVF *);//������������ �� ������ �� ����� �� ������� ���� � �������� ��� �� ������ -�������������
 void DrawFuncFrame(byte );//����������� ������� �� �����. ��.
 void ScrFK(void);//���������� �� �����. ������ �� ������
 void PExitField(ListFile *,ListVF *);  //���������� �� ��������� ������ �� ������
  void PExitField(ListFile *,ListVF *,int,int);  //���������� �� ��������� ������ �� ������
    void PExitField(ListFile *,ListVF *,int);  //���������� �� ��������� ������ �� ������
void PEnterField(VField *vf1,VField * =NULL,int =0);//���������� �� ������-��������� �������/�� 2/ �� ������
void PrintField(ListVF * ,int,char ,void *);//������������ �� ���� �� ������ � ��������� �����
 void SlowPrint(void); //������ �� "DISP" -���������� �� ������ ���� ���������� �����
 void PrintInf(void);     
 #endif                      
 