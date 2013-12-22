#ifndef _RAFFLER_H_
#define _RAFFLER_H_

#include "RNGs.h"

extern void raffler(void);
extern void Raffle(int* , int ,RngInstance*);
extern int* pickUpOnlySelectedIds(int , int* , int  , int* );
#endif /* #ifndef _RAFFLER_H_*/
