/******************************************************************************************/
/*                               C O M  V. C P P                                                 */
/*   �����������  �� ���� �������� ���������� ��� ������ � �������������     */
/*     �������  �� "DISP"                                                                          */
/******************************************************************************************/
#ifndef __comv_h
#define __comv_h         
#include <new.h>

typedef unsigned char byte;  //���������� �� BYTE
typedef unsigned int word;     //���������� �� WORD
#ifndef     prom
#define  prom
 enum flag{FALSE,TRUE};//���������� �� ������ ����������
 typedef flag (*FCNC)(char *);//���������� �� �������� ��� ������� 
 #endif
 
 #ifndef  vstr
#define vstr
//����� ��������� 
struct velb { 
  byte thechar,attribute; //������ � �������
   };
struct velw{
  word content;//.������ � ���� ���� -������ � �������
  };
union vel {           //����������
      struct velb h; 
      struct velw x;
      };
#endif      
typedef union vel   *VP;/* ��������� VP,����� ������ �������� ASCII
                                                ������ � �������  */

 typedef struct  BLOCKINFO //��������� �� ���������� �� ���������� ����� ,��������� �� � "MEMNEW.CPP"
 {   
    public:
     BLOCKINFO  *pbiNext;  //�������� ��� ��������� ���. �����
     byte *pb;//���� � ����������
     size_t size;//������
     char * typedata;//��� �� ���������� �����
  }blockinfo;      
  typedef  void ( *TC)(void);     //���������� �� �������� ��� ������� 
  typedef  void ( *TCNC)(byte );     //���������� �� �������� ��� ������� 
  typedef  flag ( *TCC)(void  );     //���������� �� �������� ��� ������� 


#endif
