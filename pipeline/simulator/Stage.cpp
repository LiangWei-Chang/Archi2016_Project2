/***************************************************

	File: Stage.cpp

	Author: PinYo

***************************************************/
#include "GlobalVar.h"

int Instruction_Fetch(int Word){
	Instruction ins;
	ins.Word = Word;
	IF_ID.ins = ins;
	return Word;
}

int Instruction_Decode(){
	Instruction ins = IF_ID.ins;
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
	ID_EX.ins = ins;
	return ins.Word;
}

int Execute(){
	Instruction ins = ID_EX.ins;
	int ALU_result;
	switch (ins.opcode){
		case 0:
			switch (ins.funct){
				case 32: // add
					ALU_result = reg[ins.rs] + reg[ins.rt];
					break;
				case 33: // addu
					ALU_result = reg[ins.rs] + reg[ins.rt];
					break;
				case 34: // sub
					ALU_result = reg[ins.rs] - reg[ins.rt];
					break;
				case 36: // and
					ALU_result = reg[ins.rs] & reg[ins.rt];
					break;
				case 37: // or
					ALU_result = reg[ins.rs] | reg[ins.rt];
					break;
				case 38: // xor
					ALU_result = reg[ins.rs] ^ reg[ins.rt];
					break;
				case 39: // nor
					ALU_result = ~(reg[ins.rs] | reg[ins.rt]);
					break;
				case 40: // nand
					ALU_result = ~(reg[ins.rs] & reg[ins.rt]);
					break;
				case 42: // slt
					ALU_result = (reg[ins.rs] < reg[ins.rt]);
					break;
				case 0: // sll
					ALU_result = reg[ins.rt] << ins.shamt;
					break;
				case 2: // srl
					ALU_result = (unsigned int)reg[ins.rt] >> ins.shamt;
					break;
				case 3: // sra
					ALU_result = reg[ins.rt] >> ins.shamt;
					break;
				case 8: // jr
					ALU_result = reg[ins.rs];
					break;
			}
			break;
		case 8: // addi
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 9: // addiu
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 35: // lw
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 33: // lh
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 37: // lhu
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 32: // lb
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 36: // lbu
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 43: // sw
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 41: // sh
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 40: // sb
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 15: // lui
			ALU_result = (int) ins.C << 16;
			break;
		case 12: // andi
			ALU_result = reg[ins.rs] & (unsigned int) ins.C;
			break;
		case 13: // ori
			ALU_result = reg[ins.rs] | (unsigned int) ins.C;
			break;
		case 14: // nori
			ALU_result = ~(reg[ins.rs] | (unsigned int) ins.C);
			break;
		case 10: // slti
			ALU_result = (reg[ins.rs] < ins.C);
			break;
		/*case 4: // beq
			ALU_result = (reg[ins.rs] == reg[ins.rt]);
			break;
		case 5: // bne
			ALU_result = (reg[ins.rs] != reg[ins.rt]);
			break;
		case 7: // bgtz
			ALU_result = (reg[ins.rs] > 0);
			break;*/
		case 2: // j
			C = (unsigned)(Word << 6) >> 6;
			J_format("j", C);
			break;
		case 3: // jal
			C = (unsigned)(Word << 6) >> 6;
			J_format("jal", C);
			break;
		case 63: // Halt
			Halt = true;
			break;
	}
}

int Memory_Access(){
	Instruction ins = EX_MEM.ins;
}

int Write_Back(){
	Instruction ins = MEM_WB.ins;
}