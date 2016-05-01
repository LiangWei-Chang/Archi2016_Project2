/***************************************************

	File: GlobalVar.h

	Author: PinYo

***************************************************/
#ifndef GlobalVar_h
#define GlobalVar_h

#include <map>
#include <vector>
#include <string>

using namespace std;

class Instruction{
public:
	int Word, opcode, rs, rt, rd, shamt, funct;
	short C;
	char type;
	bool fwdrs, fwdrt;
	string Name;
	Instruction(){
		Word = opcode = rs = rt = rd = shamt = funct = 0;
		C = 0;
		type = '\0';
		fwdrs = fwdrt = false;
		Name = "NOP";
	}
};

class Buffer{
public:
	Instruction ins;
	int ALU_result, Data, RegRs, RegRt, WriteDes;
	bool RegWrite, MemRead;
	Buffer(){
		RegRs = RegRt = Data = ALU_result = WriteDes = 0;
		RegWrite = MemRead = false;
	}
};

class Global{
public:
	static int Address[1024];
	static map< int,char > Memory;
	static int reg[32], PC, Branch_PC;
	static bool Halt, Stall;
	static bool Branch_taken;
	static bool error_type[4];
	static Buffer IF_ID, ID_EX, EX_MEM, MEM_WB;
};
#endif