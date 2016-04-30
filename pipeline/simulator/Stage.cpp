/***************************************************

	File: Stage.cpp

	Author: PinYo

***************************************************/
#include "GlobalVar.h"
#include "Stage.h"
#include <iostream>

Instruction Instruction_Fetch(){
	Instruction ins;
	ins.Word = Global::Address[Global::PC];
	Global::IF_ID.ins = ins;
	return ins;
}

Instruction Instruction_Decode(){
	Instruction ins = Global::IF_ID.ins;
	// Calculate opcode
	ins.opcode = ((unsigned int) ins.Word) >> 26;
	// Calculate rs
	ins.rs = ((unsigned int) (ins.Word << 6)) >> 27;
	// Calculate rt
	ins.rt = ((unsigned int) (ins.Word << 11)) >> 27;
	// Calculate rd
	ins.rd = ((unsigned int) (ins.Word << 16)) >> 27;
	// Calculate shamt
	ins.shamt = ((unsigned int) (ins.Word << 21)) >> 27;
	// Calculate funct
	ins.funct = ((unsigned int) (ins.Word << 26)) >> 26;
	// Calculate C
	ins.C = (short) ins.Word;
	int K = 0, DataRs, DataRt;

	// Stall
	if(Global::ID_EX.MemRead && ((Global::ID_EX.WriteDes == ins.rs) || (Global::ID_EX.WriteDes == ins.rt))){
		Global::Stall = true;
		Instruction NOP;
		Global::ID_EX.ins = NOP;
		return ins;
	}
	else
		Global::Stall = false;

	// Forwarding
	if(Global::EX_MEM.RegWrite && (Global::EX_MEM.WriteDes!=0) && (Global::EX_MEM.WriteDes == ins.rs))
		DataRs = Global::EX_MEM.ALU_result;
	else
		DataRs = Global::reg[ins.rs];
	if(Global::EX_MEM.RegWrite && (Global::EX_MEM.WriteDes!=0) && (Global::EX_MEM.WriteDes == ins.rt))
		DataRt = Global::EX_MEM.ALU_result;
	else
		DataRt = Global::reg[ins.rt];

	Global::ID_EX.RegWrite = false;
	Global::ID_EX.MemRead = false;

	switch(ins.opcode){
		case 0:
			ins.type = 'R';
			switch (ins.funct){
				case 32: // add
					ins.Name = "ADD";
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 33: // addu
					ins.Name = "ADDU";
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 34: // sub
					ins.Name = "SUB";
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 36: // and
					ins.Name = "AND";
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 37: // or
					ins.Name = "OR";
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 38: // xor
					ins.Name = "XOR";
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 39: // nor
					ins.Name = "NOR";
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 40: // nand
					ins.Name = "NAND";
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 42: // slt
					ins.Name = "SLT";
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 0: // sll
					ins.Name = "SLL";
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 2: // srl
					ins.Name = "SRL";
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 3: // sra
					ins.Name = "SRA";
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 8: // jr
					Global::Branch_taken = true;
					Global::Branch_PC = Global::reg[ins.rs];
					ins.Name = "JR";
					break;
			}
			break;
		case 8: // addi
			ins.type = 'I';
			ins.Name = "ADDI";
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 9: // addiu
			ins.type = 'I';
			ins.Name = "ADDIU";
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 35: // lw
			ins.type = 'I';
			ins.Name = "LW";
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.MemRead = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 33: // lh
			ins.type = 'I';
			ins.Name = "LH";
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.MemRead = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 37: // lhu
			ins.type = 'I';
			ins.Name = "LHU";
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.MemRead = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 32: // lb
			ins.type = 'I';
			ins.Name = "LB";
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.MemRead = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 36: // lbu
			ins.type = 'I';
			ins.Name = "LBU";
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.MemRead = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 43: // sw
			ins.type = 'I';
			ins.Name = "SW";
			break;
		case 41: // sh
			ins.type = 'I';
			ins.Name = "SH";
			break;
		case 40: // sb
			ins.type = 'I';
			ins.Name = "SB";
			break;
		case 15: // lui
			ins.type = 'I';
			ins.Name = "LUI";
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 12: // andi
			ins.type = 'I';
			ins.Name = "ANDI";
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 13: // ori
			ins.type = 'I';
			ins.Name = "ORI";
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 14: // nori
			ins.type = 'I';
			ins.Name = "NORI";
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 10: // slti
			ins.type = 'I';
			ins.Name = "SLTI";
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 63: // Halt
			Global::Halt = true;
			break;
		case 4: // beq
			Global::Branch_taken = (DataRs == DataRt);
			Global::Branch_PC = Global::PC + 4 + 4 * (int)ins.C;
			ins.Name = "BEQ";
			break;
		case 5: // bne
			Global::Branch_taken = (DataRs != DataRt);
			Global::Branch_PC = Global::PC + 4 + 4 * (int)ins.C;
			ins.Name = "BNE";
			break;
		case 7: // bgtz
			Global::Branch_taken = (DataRs > 0);
			Global::Branch_PC = Global::PC + 4 + 4 * (int)ins.C;
			ins.Name = "BGTZ";
			break;
		case 2: // j
			ins.C = (unsigned)(ins.Word << 6) >> 6;
			K = ((Global::PC+4) >> 28) << 28;
			K += (ins.C << 2);
			Global::Branch_taken = true;
			Global::Branch_PC = K;
			ins.Name = "J";
			break;
		case 3: // jal
			Global::reg[31] = Global::PC + 4;
			ins.C = (unsigned)(ins.Word << 6) >> 6;
			K = ((Global::PC+4) >> 28) << 28;
			K += (ins.C << 2);
			Global::Branch_taken = true;
			Global::Branch_PC = K;
			ins.Name = "JAL";
			break;
	}
	Global::ID_EX.ins = ins;
	Global::ID_EX.RegRs = DataRs;
	Global::ID_EX.RegRt = DataRt;
	return ins;
}

Instruction Execute(){
	Instruction ins = Global::ID_EX.ins;
	int ALU_result = 0, DataRs = 0, DataRt = 0;
	// Forwarding
	if(Global::EX_MEM.RegWrite && (Global::EX_MEM.WriteDes!=0) && (Global::EX_MEM.WriteDes == ins.rs))
		DataRs = Global::EX_MEM.ALU_result;
	else
		DataRs = Global::ID_EX.RegRs;
	if(Global::EX_MEM.RegWrite && (Global::EX_MEM.WriteDes!=0) && (Global::EX_MEM.WriteDes == ins.rt))
		DataRt = Global::EX_MEM.ALU_result;
	else
		DataRt = Global::ID_EX.RegRs;

	switch (ins.opcode){
		case 0:
			switch (ins.funct){
				case 32: // add
					ALU_result = DataRs + DataRt;
					break;
				case 33: // addu
					ALU_result = DataRs + DataRt;
					break;
				case 34: // sub
					ALU_result = DataRs - DataRt;
					break;
				case 36: // and
					ALU_result = DataRs & DataRt;
					break;
				case 37: // or
					ALU_result = DataRs | DataRt;
					break;
				case 38: // xor
					ALU_result = DataRs ^ DataRt;
					break;
				case 39: // nor
					ALU_result = ~(DataRs | DataRt);
					break;
				case 40: // nand
					ALU_result = ~(DataRs & DataRt);
					break;
				case 42: // slt
					ALU_result = (DataRs < DataRt);
					break;
				case 0: // sll
					ALU_result = DataRt << ins.shamt;
					break;
				case 2: // srl
					ALU_result = (unsigned int)DataRt >> ins.shamt;
					break;
				case 3: // sra
					ALU_result = DataRt >> ins.shamt;
					break;
			}
			break;
		case 8: // addi
			ALU_result = DataRs + (int) ins.C;
			break;
		case 9: // addiu
			ALU_result = DataRs + (int) ins.C;
			break;
		case 35: // lw
			ALU_result = DataRs + (int) ins.C;
			break;
		case 33: // lh
			ALU_result = DataRs + (int) ins.C;
			break;
		case 37: // lhu
			ALU_result = DataRs + (int) ins.C;
			break;
		case 32: // lb
			ALU_result = DataRs + (int) ins.C;
			break;
		case 36: // lbu
			ALU_result = DataRs + (int) ins.C;
			break;
		case 43: // sw
			ALU_result = DataRs + (int) ins.C;
			break;
		case 41: // sh
			ALU_result = DataRs + (int) ins.C;
			break;
		case 40: // sb
			ALU_result = DataRs + (int) ins.C;
			break;
		case 15: // lui
			ALU_result = (int) ins.C << 16;
			break;
		case 12: // andi
			ALU_result = DataRs & (unsigned int) ins.C;
			break;
		case 13: // ori
			ALU_result = DataRs | (unsigned int) ins.C;
			break;
		case 14: // nori
			ALU_result = ~(DataRs | (unsigned int) ins.C);
			break;
		case 10: // slti
			ALU_result = (DataRs < ins.C);
			break;
		case 63: // Halt
			Global::Halt = true;
			break;
	}
	Global::EX_MEM.MemRead = Global::ID_EX.MemRead;
	Global::EX_MEM.ALU_result = ALU_result;
	Global::EX_MEM.ins = ins;
	Global::EX_MEM.RegRt = DataRt;
	Global::EX_MEM.RegWrite = Global::ID_EX.RegWrite;
	Global::EX_MEM.WriteDes = Global::ID_EX.WriteDes;
	return ins;
}

Instruction Memory_Access(){
	Instruction ins = Global::EX_MEM.ins;
	int Data = 0;
	switch(ins.opcode){
		case 35: // lw
			for(int i=0; i<4; i++)
				Data = (Data << 8) | (unsigned char)Global::Memory[Global::EX_MEM.ALU_result + i];
			break;
		case 33: // lh
			Data = Global::Memory[Global::EX_MEM.ALU_result];
			for(int i=1; i<2; i++)
				Data = (Data << 8) | (unsigned char)Global::Memory[Global::EX_MEM.ALU_result + i];
			break;
		case 37: // lhu
			for(int i=0; i<2; i++)
				Data = (Data << 8) | (unsigned char)Global::Memory[Global::EX_MEM.ALU_result + i];
			break;
		case 32: // lb
			Data = Global::Memory[Global::EX_MEM.ALU_result];
			break;
		case 36: // lbu
			Data = (unsigned char)Global::Memory[Global::EX_MEM.ALU_result];
			break;
		case 43: // sw
			for(int i=0; i<4; i++)
				Global::Memory[Global::EX_MEM.ALU_result + i] = (char)(Global::EX_MEM.RegRt >> (8*(3-i)));
			break;
		case 41: // sh
			for(int i=0; i<4; i++)
				Global::Memory[Global::EX_MEM.ALU_result + i] = (char)(Global::EX_MEM.RegRt >> (8*(1-i)));
			break;
		case 40: // sb
			Global::Memory[Global::EX_MEM.ALU_result] = (char)Global::EX_MEM.RegRt;
			break;
		default: // Store Load else
			Data = Global::EX_MEM.ALU_result;
			break;
	}
	Global::MEM_WB.Data = Data;
	Global::MEM_WB.ALU_result = Global::EX_MEM.ALU_result;
	Global::MEM_WB.ins = ins;
	Global::MEM_WB.RegRt = Global::EX_MEM.RegRt;
	Global::MEM_WB.WriteDes = Global::EX_MEM.WriteDes;
	return ins;
}

Instruction Write_Back(){
	Instruction ins = Global::MEM_WB.ins;
	if(ins.type=='R')
		Global::reg[Global::EX_MEM.WriteDes] = Global::MEM_WB.ALU_result;
	else if(ins.type=='I')
		Global::reg[Global::EX_MEM.WriteDes] = Global::MEM_WB.Data;
	return ins;
}