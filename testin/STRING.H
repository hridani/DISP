/*******************************************************************************************************/
/*                                         S T R I N G . H                                        */
/*����������� �� ������� �� ������ � ������ � ������������� �� ���. �� ���� ��� � ����*/
/*****************************************************************************************/
 #ifndef __string_h
 #define __string_h                               
 #include <sys\types.h>
 #include <sys\stat.h>
 #include "c:\msvc\dani\include\comv.h"  
 
void UnsToStr(unsigned ,char *); //������������� �� ���������� ����� � ������
void LongToStr(long,char *);//������������� �� ����� ���� ����� � ������
void IntToStr(int,char *);  //������������� �� ���� ����� � ������
void UnsLToStr(unsigned long ,char *);//������������� �� ���������� ����� ���� ����� � ������
char *STRCPY(char *,char *,int ) ;//��������� ������ �������� ��� � ���� � ������� ������      
char * STRSRCH(char *,char *);//������� �� ��� � ���� ���             
char  * STRCHR(char *,unsigned char);//������� �� ������ �  ���           
char * STRCOPY(char *,char *,int); //�������� �� ��� � ���� ��� �� ���������� �������    
char * STRCOPYEDIT(char *,char *,int); //�������� �� ��� � ���� ��� �� ���������� �������    
void Deletechar(char *,int);//��������� �� ��������� ���� ������� �� ����� ��� �� ���������� �������      
void Insertsr(char *,char,int);//������ �� ���������� ����� � ��������� ��� ��������� ������     
flag strFindUns(char *,unsigned*);//�����������  ������  � ���� ���������� ����� /��� unsigned  /        
flag strFindInt(char *,int *);// �����������  ������  � ���� ����� /��� int  /
flag strFindLong(char *,long *); //�����������  ������  � ���� ����� /��� long / 
flag strFindUnsL(char *,unsigned long *);//�����������  ������  � ���� ����� ���������� ����� /unsigned long /
void String_convert(char ,char *,void *);// ������������� �� ��� � ����� (int,double � �� ) 
void  Number_convert(char ,void *,char *); // ������������� �� ����� (int,double � �� ) � ��� 
void  StatDos( char *,struct _stat &); //����� ���������� �� ����� � �������� ��� -����,������ � ��.
char *STRCPYCEN(char *,char *,int );//�������� �� ���� ��� � ���� � ������� ������
void Insertstr(char *,char ,int);//��������� �� ������� ��� � ���� �� �������� �������
#endif                      
   
