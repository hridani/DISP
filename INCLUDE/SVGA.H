/*********************************************************************/
/*                       S V G A                                              */
/*-----------------------------------------------------------------------------------*/
/*     ���������� �� ������ (������) � ������� �����      */
/*********************************************************************/
 #ifndef _INC_SVGA         
 #define _INC_SVGA
 #pragma comment(lib,"..\\lib\\ekran.lib")
 #include "..\include\comv.h"                     
 #include "..\include\memnew.h"
 
 #define max_width  80  /* ���������� ������ */
 #define max_height 25  /* ���������� �������� */

//#define COLOR(VG,HG)  ((VG<<4) +HG)  /* ����������� �� �������� �� ������� */

/*======���������� �� ���� ������ �����==========================*/
extern VP vptr;
 /* �������� ��� ��������� VP,����� ������ �������� ASCII
                                                ������ � �������  */

/*======�������=========================*/
 flag disp_init(void);/* ������������� */
 void Save_scr(int,int,int,int,int,union vel   *);/* ���������� �� ��������� � RAM */
 void SaveFile(int,int,int,int,int,char *);/* ���������� �� ��������� � RAM */
 void Load_scr(int,int,int,int,int,union vel  *,int);/* ��������� �� �������� �� RAM ���
                                ������������ ��� � ������ */
 void LoadFile(int,int,int,int,int,char  *,int);/* ��������� �� �������� �� RAM ��� */
 void Move_buffer_to_screen(void);/* ����������� �� ������ ��� ������������ */
 void dprint (byte,byte,byte,byte,char *); /* ��������� �� ��� �� ������ */
 union vel  * Allocate_scr(int,int,int,int,int);//����������� �� ����� � ����������� �� �������� 
 void Unallocate_scr(union vel  *);
 byte Test_CGA(void);

 #endif
