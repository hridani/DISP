/*****************************************************************************/
/*                            K E Y . H                                               */
/*  ����������� �� ������� FuncKey,Key,ListKey                */
/*   � �� ��������� KEYBUFFER                                            */
/*******************************************************************************/
 #ifndef _INC_KEY    
 #define _INC_KEY
 #pragma comment(lib,"..\\lib\\key.lib")
 #include "..\include\comv.h"
 int handle_program_memory_depletion( size_t ); 

// �������� �� ���������  KEYBUFFER
typedef struct KEYBUFFER
{
   unsigned char *scanbuf;//������ �� ������
   unsigned char *scanend;//���� �� ������
   unsigned int scannext;//�������� �� �����
   unsigned int scanlast; //�������� ��������
} KeyBuffer;

 void IniBuffer(KeyBuffer *,int);//�������� �� ����� �� ����������� �����,�������������
 void EndBuffer(KeyBuffer  *);//������������� �� �����

 //�������� �� ���� FuncKey - �������� �� �����. ��.
 class FuncKey 
 {
   friend class Key;
   public:        
     char *name; //���
     TC activf; //�������� ��� ������� �� �������
     TC crfunc;//�������� ��� CR ������� �� �������
     FuncKey(char *,TC,TC);   //�����������
      ~FuncKey () {delete name;} //����������
};

// �������� �� ���� Key  - �����. ��. ���� �������
class Key
{          
   friend class ListKey;
  public:
    FuncKey *funck;  //�������� ��� �����. ��.
    int number;//����� �� �����. ��. � �������
    Key *next;  //�������� ��� ��������� �������
    Key(FuncKey *);//�����������
};

//�������� �� ���� ListKey -������ �� �����. �������
class ListKey
{
   public:
   Key *top,**cur; //��������� ��� �������� � ������� �������
   int nm;  //���� �������� - �����. ��.
   ListKey();    //�����������
   ~ListKey() {removeall(&top); } //����������
   void removeall(Key **);//��������� �� �������
   flag add(FuncKey *); //�������� �� �������
   flag add(FuncKey *,int ); //�������� �� ������� � ������� �����
   void find(FuncKey *);//������� �� ������� �� �������� ��� �����. ��.
   void find(int );//������� �� ������� �� ����� �� �����. ��..
   void eraseKey(FuncKey *);
};  
  
       
 #endif
