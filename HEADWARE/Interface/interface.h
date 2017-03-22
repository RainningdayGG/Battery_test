#ifndef _INTERFACE_H
#define _INTERFACE_H

#ifdef INTERFACE_GLOBALS
#define INTERFACE_EXT 
#else
#define INTERFACE_EXT extern
#endif
#include "includes.h"
INTERFACE_EXT u8 Str_Count;
INTERFACE_EXT char String_key[7];
INTERFACE_EXT u8 Max_StrCnt;
void fwd(void);
void bwd(void);
void add(void);
void minus(void);
void clear_str(void);
void Hanle_NumKey(char numkey);
void Open_Window(char Str_key);
void Colse_Window(u8 x1, u8 y1, u8 x2, u8 y2);

#endif
