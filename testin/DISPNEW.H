/******************************************************************************************/
/*                               D I S P N E W . H                                                */
/*   ����������� �� ������� �� ������ � "DISP"  � �� "DISP"                       */
/******************************************************************************************/
#ifndef __dispnew_h
#define __dispnew_h 
#include "..\include\classth.h"
int handle_program_memory_depletion( size_t );
TaskHandle* CreateTask(char *,State,unsigned ,Kind ,unsigned long,unsigned ,TC,int * =NULL);
 //���������� �� ������ �� "DISP" - ������������� */
           

 void __interrupt __far DISPT(); // ������� "DISP" - ����� �� ������     
 void __interrupt __far DISP8(); // ������� "DISP" - ����� �� ������  
 
 void ExitSysT(void);  //����� �� �������� -�������������       
 void ExitSys8(void);  //����� �� ��������, ������ �� �������� ��������� ������ -�������������     
 void Ini8(void);//������������� ��� ����� �� ��������� - �������� ������
  void IniT(void);  //������������� ��� ����� �� ��������� -���� ������
 TaskHandle* FindSlow(TaskHandle **);//������� �� ����� ������ � ���-����� ��������
 void MeasTime(void); //����������� ������� �� ������������ ������ 
 void StartFast(void);//���������� �� ����� ������
 void FindSem(void);//�������� �� ���.��������� �� ��������� �� �������� ������
 void CurWait(void);//������� ����������� �� ���. ������ � ������
 void CurDelay(void); //������� ����������� �� ���. ������ � ����������
 void Delay  (void); //������������� ������� - ����������� "DISP" ,�� ���� ������ �� ���� ������� 
void WaitCond(void);//������������� ������� - ����������� "DISP" ,�� ���� ������ �� ���� �����
void WaitTaskCond(TaskHandle *);//������������� ������� - ����������� "DISP" ,�� ���� ���������� ������ �� ���� �������
void whlt(void); //������ ����� -������� �� "DISP"
void InTimer(unsigned ,unsigned int ,unsigned int ); //����. �� �������
void EndTask(void);//�������� �� ����� �� ���������� 
void *dek(char *);  //�������� �� ���. ��������� �� ��������� � ������� -������������� 
void dek(char *,int **);//�������� �� ����������� ������� � �������
 flag check_con(void *); //�������� �� ���.��������� �� ���������
void  Resume(TaskHandle *);  //�������� ������� ������ 
void  NewSuspend(TaskHandle *);//������ ������ ����� ���������
  //������ ��� ��������  -������������� �������
void SignalSem(unsigned ); //����� ���. ������� � ��������� � 1 �����. �������
void NoSignalSem(unsigned );//����� ���. ������� ��� �������� � 1 �����.���.
void AritSem(unsigned ,unsigned);//���������� �����. ��� ��� ���������� ��������
void TimeSem(unsigned ,unsigned);//����. ������� �� �����
unsigned ReadSem(unsigned ); // ���� �������� � ������� �����
byte Input(unsigned );//������ �� ����������� �������� � �����������
void IniV(void); 
#endif                      
