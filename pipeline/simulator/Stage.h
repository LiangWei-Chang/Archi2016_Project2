/***************************************************

	File: Stage.h

	Author: PinYo

***************************************************/
#ifndef Stage_h
#define Stage_h

Instruction Instruction_Fetch(int);
Instruction Instruction_Decode();
Instruction Execute();
Instruction Memory_Access();
Instruction Write_Back();

#endif