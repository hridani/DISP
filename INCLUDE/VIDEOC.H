/*********************************************************************/
/*                             V I D E O C                           */
/*-------------------------------------------------------------------*/
/*   Функции ,създадени на базата BIOS видео прекъсванията,   */
/*        работещи под управлението на C++                .            */
/*********************************************************************/
 #ifndef _INC_VIDEOC  
 #define _INC_VIDEOC
 #pragma comment(lib,"..\\lib\\ekran.lib")
 #include <bios.h>
                                          
/*======КОНСТАНТИ================================================*/

#define NORMAL        0x07    /* дефиниране на символни атрибути */
#define BOLD          0x0F    /* по отношение не Monochromen      */
#define INVERS        0x70    /* Display Adapter                     */
#define UNDERLINE     0x01
#define BLINK         0x80

/*=====ФУНКЦИИ=========================================*/
void GetVideoMode(int *,int *,int *);/* дава инф. за текущия режим,видеостраница  и др.*/
void SetCursorType(int,int);/* установява началната и крайна линия на курсора */
void SetCursorPos(int,int,int);/* установява позицията на курсора */
void GetCursorPos(int,int *,int *,int *,int *); /* дава информация за позицията на курсора */
void SetScreenPage(int); /* установява видеостраница */
void ScrollUp(int,int,int,int,int,int); /* превъртане на екрана нагоре */
void ScrollDown(int,int,int,int,int,int);/* превъртане на екрана надолу */
void GetChar(int,int,int,char *,int*); /*дава информация за ASCII кода на символа и атрибутите му */
void WriteChar(int,char,int);/* изписва символ на екрана */
void WriteText(int,int,int,int,char *);/* изписва текст на екрана */
void ClearScreen(void);/* почиства екрана */
 
 #endif

