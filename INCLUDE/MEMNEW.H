/******************************************************************************************************/
/*                                                      M E M N E W . H                                                */
/*       ������� �� �������� � ������������� �� �����.�������� �� ������� ���               */
/*                      ������  � "DEBUG"                                                                                    */
/*************************************************************************************************/
#ifndef _INC_MEMNEW
#define _INC_MEMNEW
 #pragma comment(lib,"..\\lib\\memnew.lib")
 #endif

 #ifdef __cplusplus
  extern "C"{
 #endif

#if (_MSC_VER <= 600)
#define __cdecl     _cdecl
#define __far       _far
#endif 
 #include "c:\msvc\dani\include\comv.h"
 #define bNewGarbage   0xA3    //����. �� ���������� �����  ��� ������ � "DEBUG"
 #define bFreeGarbage   0xA5    //����. �� ������������ �����  ��� ������ � "DEBUG"
 
  #ifdef  _DEBUG
    void _Assert (char *,unsigned);
    #define ASSERT(f)      if(f) NULL;    else _Assert(__FILE__,__LINE__)
 #else
    #define ASSERT(f)   NULL
 #endif 
    
    void _Assert (char *,unsigned);
    flag fNewMemory(void **,size_t,char *);//������ ����� /���� / � ��������� ������
    flag FreeMemory( void *) ;//����������� ���� �� �������
    void FreeUMB(void *);
    void FreeMem(void *);
    void NewMemoryUMB(void **,size_t );
  #ifdef __cplusplus
   }
  #endif
 
