/***************************************************

	File: GlobalVar.h

	Author: PinYo

***************************************************/
#ifndef GlobalVar_h
#define GlobalVar_h

#include <map>
#include <vector>

using namespace std;

class Instruction{
public:
	int Word, opcode, rs, rt, rd, shamt, funct;
	short C;
	char type;
	Instruction(){
		Word = opcode = rs = rt = rd = shamt = funct = 0;
		C = 0;
		type = '\0';
	}
};

class Buffer{
public:
	Instruction ins;
	int ALU_result, Data;
};

static map< int,char > Memory;
static int reg[32], PC, Branch_PC;
static bool Halt;
static bool Branch_taken;
static bool error_type[4];
static Buffer IF_ID, ID_EX, EX_MEM, MEM_WB;

#endif