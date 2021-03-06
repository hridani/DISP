/******************************************************************************************/
/*                               C O M S T . H                                                 */
/*   ����������� �� ���� �������� ���������� ��� ������ � "DISP"       */
/******************************************************************************************/
#ifndef __comst_h
#define __comst_h         
#include <new.h>
#include "..\include\comv.h"
#include "..\include\classth.h"
#include "..\include\vfield.h"
#include "..\include\key.h"
#include "..\include\pia.h"
#include "..\include\table.h"
#define Tk 1 /* ����������  ����� �� ��������� - ����� �� ���������� -0.001� */
          extern    unsigned numbinpPIA;
extern TaskHandle *kboard,*screen,*idle,*inpi; //������  �� "DISP" -"Keyboard","Screen","Idle"
extern int activpage;//������������� �� ��������� �����������
extern flag flagend;  /* ���� �� ���� �� �������� �� ��������� */
extern flag flag_change; // ���� �� ����� �� ���� ������
extern StateList READYF,READYS,WAIT,DELAY,SUSPEND; /* ������� ��� ������ �� ����� */
extern TaskHandle *CURTASK; /* ������ ������,����������� �� ��������� */
//extern byte *tsem,*tsem1,*tsem2;  /* ������� �� ���������� */ 
extern byte tsem[3200];
extern unsigned size; /* ������ �� ������� ����� */
extern unsigned numbls;//���� ��������� ��������
extern unsigned numbas;// ���� ����������� ��������
extern unsigned numbts;//���� �������� �� �����
extern unsigned numbis; //��������-����          
extern unsigned numbirq;//����� �� ���������� ���������� �� DISP
extern List input;//������� -����� � ��������
extern List output;//������
extern ListVF *activ; /* �������� ��� ��������� ����������� � ������� ����� */
extern ListVF lfunc;/* ������ �� �������� �� �������������� ������� */ 
extern ListVF lfunc1;/* ������ �� �������� �� �������������� ������� */ 
extern KeyBuffer *infbuffer; /* ����� � ASCII ������ */
extern KeyBuffer *kb;   /* �������� ����� � ���������� ������ */
extern ListKey *dk; /* ������  ������������ ������� � �������� �����*/
extern Key **aKey; /* ������� ������������ ������ */
extern char * Inpos;/* ������� � ���������� POS ���- �������� */
extern char **wASCII ;/* �������� ��� ��������� �� ������-��������� ���� � ������� ����� */
extern  byte *col;//�������� ��� �������� ������ �� �������� ����
extern int *hr; /* �������� ��� �������� ������������ ��� */
extern int *wr; /* �������� ��� ��������� ���������� ������ */
extern flag flag_wait;//���� �� ��������� ����������� �� "DISP"
extern int *posl; /* ������� �� ���-����� �����.������ */
extern int *post;/* ������� �� ���� �����.������� */
extern int posa; /* ������� �� �������� ������������ ������ */
extern int posm;// ����������� ��������� �� ����. ������ - �� ����������� �� ��������� ������
extern int adrescom;//����� �� ����������� ������� COM1(0x3F8-0x3FF),COM2(0x2F8-0x2FF)
//extern ListNode listnode; //������ �� �����������      
extern  TC ExitSys;//������� ��� ������� � ������ ��� ���������       
extern TC Ini;//�������� ��� ������� �� �������������
extern TC PrintEnterFields;//�������� ��� ������� �� ������� �� ������/������� ����
extern TC PrintExitFields; //�������� ��� ������� �� ������� �� ������� ������
extern TC PrintUserFields;//�������� ��� �������������  ������� �� ������� �� ������� ������
extern TC MKTop;//�������� ��� ������� �� �������� ������ 
extern TC MKBottom;//�������� ��� ������� �� �������� ������
extern TC MKInsert; //�������� ��� ������� �� ��������
extern TC MKHome;//�������� ��� ������� "HOME"
extern TC MKPgUp;//�������� ��� ������� �� �������� "��������-������"
extern TC MKPgDn;//�������� ��� ������� �� �������� "��������-������"
extern TC MKDel;//�������� ��� ������� �� ���������
extern TC MKRight;//�������� ��� ������� �� �������� �������
extern TC MKLeft;  //�������� ��� ������� �� �������� ������
extern TC F12;//�������� ��� ������� F12
extern TC F10;//�������� ��� ������� F10
extern TC F11;//�������� ��� ������� F11
extern TC F9;//�������� ��� ������� F9
extern TC F8;//�������� ��� ������� F8
extern TC ESC;//�������� ��� ������� ESC
extern TC CRFUNC; /* �������� ��� ��������� �� CR */
extern TC CRFUNCOLD; /* �������� ��� �������� ������� �� CR */
extern  TC WorkingInput; //�������� ��� ������� �� ��������� �� ���������
extern ListVF vidnode; //������ �� ����������� �������� ������ � ����� TABLE
extern TC EndUseFunc;//�������� ��� ������������� ������� �� ���� 
extern TC PrintUseFunc;//�������� ��� �������������� ������� �� �������
extern TC UseSemafor;//�������� ��� �������������� ������� �� ��������� �� ����������
#endif
