/*******************************************************************************/
/*                            T A B L E . H                                     */
/*------------------------------------------------------------------------------------------------*/
/*          ����������� ��  ���� NodeT � ListNode               */
/*********************************************************************/
 #ifndef _INC_TABLE          
 #define _INC_TABLE
 #pragma comment(lib,"..\\lib\\table.lib")
 
 #include "..\include\comv.h"    
  #include <stdio.h>
 #ifdef __cplusplus
  extern "C"{
 #endif

 #if( _MSC_VER<=600)
 #define __cdecl _cdecl
 #define __far   _far
 #define __near  _near
 #endif
 

//�������� �� ���� NodeT
class  NodeT
 {
    public:
     unsigned  numb;  /* ����� �� ���������*/
     unsigned numbnz;//������� ����� � �������
     double value;                   //��������
     double min;//��������� ��������� ��������
     double max;//���������� �������� ��������
     char *name;//��� �� ����������
     NodeT *next; /*��������� */
     NodeT *back;//�������������
     NodeT(unsigned,unsigned,double,double,double,char *,NodeT *); /* ����������� */
     NodeT(unsigned,unsigned,NodeT *); /* ����������� */
     ~NodeT();//����������
   };   
     
 
 //�������� �� ���� ListNode  -������      
class ListNode
 {     
   public:
   NodeT *top,**cur,*bcur;  /* ��������� ��� ���������,�������� � ���������� �������� */ 
   unsigned nb;  /* ���� ����� */
   ListNode(); /* ����������� */
   ~ListNode();/* ���������� */
   void find(unsigned );  /* ������� �� ����-����� */
   void findnz(unsigned );  /* ������� �� ����-����� */
   void eraseNodeT(unsigned); /* ��������� �� ������� � ��������� ���� */
   void removeall(NodeT **);/* ��������� �� ������� */
   flag add(unsigned,double,double,double,char *);//�������� �� �������
   flag add(unsigned);//�������� �� �������
   void saveNodeT(FILE *,NodeT *);/* ����� ��� ���� �� ���� ����� */
   void saveall(char *);/* ����� ��� ���� �� ������� */
   void load (char *); /* ������ �� ����� */
  // void L_get_last(void);//������� �� ���������
  };
flag ReadParamMem(unsigned , char,void *);//������ �� ��������� -�����,��� �� �������
                                                    //�������� ��� ������� �� ����� �� ����������� ��������
 
 #ifdef __cplusplus
  }
 #endif
 
#endif
