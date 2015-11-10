/****************************************************************************/
/* LikeREXXstr.lib (c) 2015, Amixgris/RT, Russia.                           */
/*                                                                          */
/* Эта библиотека содержит реализацию некоторых функций языка REXX для ис-  */
/* пользования с проектом SDCC. Все функции написаны, исключительно, на ас- */
/* семблере Z80. Подробное описаниее функций можно найти в соответствующей  */
/* документации для REXX/ARexx.                                             */
/*                                                                          */
/* Примечания:                                                              */
/* 1) для функций, возвращающих текстовую стороку, в качестве первого аргу- */
/* мента добавлен указатель на буфер. /* Далее следуют аргументы в обычном  */
/* порядке REXX.                                                            */
/* 2) все функции самомодифицирующиеся: некоторые переменные хранятся в     */
/* коде команд.                                                             */
/* 3) функция SPACE временно изменяет исходную строку, обрезая конечные про-*/
/* белы. Знаю, это неправильно. :(                                          */
/*                                                                          */
/* This library contains the implementation of certain REXX programming     */
/* language functions for use with the SDCC-project.                        */
/* All functions are written on Z80-assembler.                              */
/* Detailed description of the functions can be found in the documentation  */
/* for REXX / ARexx.                                                        */
/*                                                                          */
/* Notes:                                                                   */
/* 1) For functions that return a text string, a pointer to the buffer is   */
/* added for the first argument.                                            */
/* 2) All are functions is self modifying: some variables are stored in     */
/* the instruction code.                                                    */
/* 3) SPACE-function temporarily modifies the original string, trimming     */
/* trailing spaces. It's wrong, I know. :(                                  */
/****************************************************************************/
void CHANGESTR(char* dest, char* needle, char* haystack, char* newneedle);
int  COMPARE(char* s1, char* s2, char pad);
void COMPRESS(char* d, char* s, char* list);
void COPIES(char* d, char* s, unsigned char num);
int  COUNTSTR(char* needle, char* haystack);
void DELSTR(char* d, char* s, int start, int lenght);
void LEFT(char* d, char* s, unsigned char lenght, char pad);
int  LENGHT(char* string);
void REVERSE(char* d, char* s);
void SPACE(char* d, char* s, unsigned char len, char pad);
void TRIM(char* d, char* s);
void UPPER(char* d, char* s);
int  WORDINDEX(char* string, int WordNum);
int  WORDS(char* string);
void WORD(char* dst, char* string, int WordNum);
int  WORDLENGHT(char* string, int WordNum);
int  WORDPOS(char* phrase, char* string, int startpos);

/****************/
/* TODO:	*/
/* INSERT       */
/* DELWORD      */
/* SUBSTR       */
/* SUBWORD      */
/* TRANSLATE    */
/* CENTER       */
/* CENTRE       */
/* JUSTIFY      */
/* LASTPOS      */
/* LOWER        */
/* POS          */
/* RIGHT        */
/* STRIP        */
/****************/
