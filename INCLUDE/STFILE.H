/*******************************************************************************/
/*                            S T R U C T    F I L E                                     */
/*------------------------------------------------------------------------------------------------*/
/*          ����������� ��  ���� STRUCT FILE                   */
/*********************************************************************/
 #ifndef _INC_STFILE           
 #define _INC_STFILE
 #pragma comment(lib,"..\\lib\\file.lib")
 

 #ifdef __cplusplus
  extern "C"{
 #endif

 #if( _MSC_VER<=600)
 #define __cdecl _cdecl
 #define __far   _far
 #define __near  _near
 #endif
 
 #include "c:\msvc\dani\include\comv.h"    
 #include <stdio.h>
   #include <vmemory.h>
//�������� �� ���������� ���� BaseFile
class  BaseFile
 {
    public:
     int  numb;  /* ����� �� ��� � CNC ����� */
     int nmzapis; /* ����� �� ������ � ������� */
     BaseFile *next; /*��������� */
     BaseFile *back;/* ������������� */
     char *ptext;  /* �������� ��� ������ �� ��� �� CNC ����� */ 
     BaseFile(int,BaseFile*,char *); /* ����������� */
     ~BaseFile();   
     void change(char *);
   };   
 //�������� �� ���� ListFile  -������      
class ListFile
 {     
   public:
   BaseFile *top,**cur,*bcur;  /* ��������� ��� ���������,�������� � ���������� �������� */ 
   int nb;  /* ���� ����� */
   ListFile(); /* ����������� */
   ~ListFile();/* ���������� */                                                
   ListFile operator =(ListFile&);
	void find(int );  /* ������� �� ����-����� */
	void finds(int );  /* ������� �� ����-����� */
	void findnz (int ); //������� �� ������� ����� � �������
	void eraseFile(int); /* ��������� �� ������� � ��������� ���� -����� � �����*/
	void eraseFilenz(int); /* ��������� �� ������� � ��������� ����  -������� ����� � �������*/
	void removeall(BaseFile **);/* ��������� �� ������� */
	flag add(int ,char * =NULL,unsigned =0); /* �������� �� ����� */
	flag addn(int ,char * =NULL,unsigned =0); /* �������� �� ����� */
	void L_get_last(void);//������� �� ���������
	unsigned char load(char *);
	void  saveall( char *);  
	void saveList(FILE *,BaseFile *);
  };
     
     
  
     
 //�������� �� ���� RecFile - ������������� ������� 
 class RecFile
 {
   friend class ListRecFile ;
   public:
     char *nmfiledisk; /* ��� �� ����� ,����� � ������� �� ����� */
     char *nmfilemem; /* ��� �� ����� ,����� � ������� � �� */
     char *memfile;/* �������� ��� ������ � �� */      
    // _vmhnd_t memfile;
     RecFile *next; /* ������� ����� */
     RecFile(char *,byte); /* ���������� */
     ~RecFile();//����������
 };
 
 //�������� �� ���� ListRecFile - ������������� �������
 class ListRecFile
 { 
   public:
   RecFile *top,**cur;/* ��������� ��� ����� � ������� ������� � ������� */
   int nm; /* ���� �������� */
   ListRecFile(); /* ����������� */
   ~ListRecFile();/* ���������� */
   void finddisk(char * );  /* ������� �� ����-����� */
   void findmem(char * );  /* ������� �� ����-����� */
   void eraseRecFile(char *); /* ��������� �� ������� � ��������� ���� */ 
   void removeall(RecFile **);/* ��������� �� ������� */
   flag adddisk(char *); /* �������� �� ����� */
   flag addmem(char *); /* �������� �� ����� */
} ;  
 
 #ifdef __cplusplus
  }
 #endif
 
#endif
