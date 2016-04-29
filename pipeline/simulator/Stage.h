/***************************************************

	File: Stage.h

	Author: PinYo

***************************************************/
#ifndef Stage_h
#define Stage_h

int Instruction_Fetch(int);
int Instruction_Decode();
int Execute();
int Memory_Access();
int Write_Back();

#endif