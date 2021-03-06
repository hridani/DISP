/****************************************************************************************/
/*                                 C L A S S T H . H                                          */
/*     ���������� �� ���� TaskHandle ,ItemTask,StateList                                         */
/****************************************************************************************/
 #ifndef _INC_CLASSTH
 #define _INC_CLASSTH
 #pragma comment(lib,"..\\lib\\classth.lib")
 #include "..\include\memnew.h "
 #include "..\include\comv.h"
 #include <stdio.h>
   enum State{Current,Ready,Wait,Timed,Suspend};//��������� �� �������� - ������,
              // ������,������,����������
   enum Kind {Fast,Slow}; //��� -����� ,�����
   int handle_program_memory_depletion( size_t ); //������� �� ��������� ��� �������� �� �����

 // �������� �� ���� TaskHandle -�������� �� ������ �� "DISP"
 class TaskHandle  
 {
   public:        
     char *name; //���
     State st;     //���������
     unsigned ucurbp; //������ ��������� �� ���. bp -���� �� �����
     unsigned priority; //���������
     Kind kind; //���
     unsigned *bstack; //������ �� ����� �� ��������
     unsigned ucurss; //������ �������� �� ���. ss
     unsigned ubeginbp;//������� �������� �� ���. bp
     unsigned long time; //������ ,��� �������� � ����������
     unsigned long timec;//����� �� �������
     int *taskcond;//�������� ��� ����������� ��������� �� ���������
     TC taskcode;  //�������� ��� ��������� �� ��������
 //    void *operator new (size_t); //�������� �� �����
 //    void operator delete (void *); //������������� �� �����
     TaskHandle( char *,State ,unsigned,Kind,unsigned long ,unsigned ,TC ) ; //�����������
     ~TaskHandle () ;//����������
  };
  
 // �������� �� ���� ItemTask - ����� �� ������� ��� ������
  class ItemTask
  {
    friend class StateList; //���������� ���� StateList
    public:
      TaskHandle *th; //�������� ��� ��������
      ItemTask *next;  //�������� ��� ��������� �����
      ItemTask *back;//�������� ��� ���������� �����
      ItemTask(TaskHandle *,ItemTask *); //�����������
      ~ItemTask();
 //     void *operator new (size_t ); //�������� �� ����� �� ����� �� ���� ����
 //     void operator delete (void *); //������������� �� �����
   };
   
 // �������� �� ���� StateList - �������� �� ������� �� ������
   class StateList
   {           
    public:  
      ItemTask *top,**cur,*bcur; //�������� ��� �������� ,����� ������� � ���� �� �������
      int nm;    //���� ��������   
      StateList();   //�����������      
      ~StateList() {removeall(&top); } //����������
      flag add(TaskHandle *);//�������� �� ������� /������ / � �������
      void find(TaskHandle *); //������� �� ������� /������ / � ������� �� ������� ��������    
      void removeall(ItemTask **pit);//��������� �� ����� ������
      flag eraseTask(TaskHandle *);//��������� �� ������� /������/ �� ������� ��������
      void saveList(FILE *,ItemTask*); //����� �� ������� ��� ����   
     void saveall( char * ); //����� �� ������� ��� ���� � �������� ���
    // void L_get_last(); //������� �� ��������� �������
   };

       
 #endif
