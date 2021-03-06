/*********************************************************************/
/*                             V I D E O C                           */
/*-------------------------------------------------------------------*/
/*   ������� ,��������� �� ������ BIOS ����� �������������,   */
/*        �������� ��� ������������ �� C++                .            */
/*********************************************************************/
 #ifndef _INC_VIDEOC  
 #define _INC_VIDEOC
 #pragma comment(lib,"..\\lib\\ekran.lib")
 #include <bios.h>
                                          
/*======���������================================================*/

#define NORMAL        0x07    /* ���������� �� �������� �������� */
#define BOLD          0x0F    /* �� ��������� �� Monochromen      */
#define INVERS        0x70    /* Display Adapter                     */
#define UNDERLINE     0x01
#define BLINK         0x80

/*=====�������=========================================*/
void GetVideoMode(int *,int *,int *);/* ���� ���. �� ������� �����,�������������  � ��.*/
void SetCursorType(int,int);/* ���������� ��������� � ������ ����� �� ������� */
void SetCursorPos(int,int,int);/* ���������� ��������� �� ������� */
void GetCursorPos(int,int *,int *,int *,int *); /* ���� ���������� �� ��������� �� ������� */
void SetScreenPage(int); /* ���������� ������������� */
void ScrollUp(int,int,int,int,int,int); /* ���������� �� ������ ������ */
void ScrollDown(int,int,int,int,int,int);/* ���������� �� ������ ������ */
void GetChar(int,int,int,char *,int*); /*���� ���������� �� ASCII ���� �� ������� � ���������� �� */
void WriteChar(int,char,int);/* ������� ������ �� ������ */
void WriteText(int,int,int,int,char *);/* ������� ����� �� ������ */
void ClearScreen(void);/* �������� ������ */
 
 #endif

