/***************************************************

	File: GlobalVar.cpp

	Author: PinYo

***************************************************/

#include "GlobalVar.h"


int Global::Address[1024];
map< int,char > Global::Memory;
int Global::reg[32], Global::PC, Global::Branch_PC;
bool Global::Halt, Global::Stall;
bool Global::Branch_taken;
bool Global::error_type[4];
Buffer Global::IF_ID, Global::ID_EX, Global::EX_MEM, Global::MEM_WB;