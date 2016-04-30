/***************************************************

	File: Stage.cpp

	Author: PinYo

***************************************************/
#include "GlobalVar.h"

Instruction Instruction_Fetch(int Word){
	Instruction ins;
	ins.Word = Word;
	if(Branch_taken){
		PC = Branch_PC;
		IF_ID.ins = new Instruction();
	}
	else{
		PC += 4;
		IF_ID.ins = ins;
	}
	return ins;
}

Instruction Instruction_Decode(){
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
	switch(ins.opcode){
		case 4: // beq
			Branch_taken = (reg[ins.rs] == reg[ins.rt]);
			Branch_PC = PC + 4 + 4 * (int)ins.C;
			break;
		case 5: // bne
			Branch_taken = (reg[ins.rs] != reg[ins.rt]);
			Branch_PC = PC + 4 + 4 * (int)ins.C;
			break;
		case 7: // bgtz
			Branch_taken = (reg[ins.rs] > 0);
			Branch_PC = PC + 4 + 4 * (int)ins.C;
			break;
		case 2: // j
			ins.C = (unsigned)(ins.Word << 6) >> 6;
			int K = ((PC+4) >> 28) << 28;
			K += (C << 2);
			Branch_taken = true;
			Branch_PC = K;
			break;
		case 3: // jal
			reg[31] = PC + 4;
			ins.C = (unsigned)(ins.Word << 6) >> 6;
			int K = ((PC+4) >> 28) << 28;
			K += (C << 2);
			Branch_taken = true;
			Branch_PC = K;
			break;
		case 0:
			if(funct==8){ // jr
				Branch_taken = true;
				Branch_PC = reg[ins.rs];
			}
			break;
	}
	ID_EX.ins = ins;
	return ins;
}

Instruction Execute(){
	Instruction ins = ID_EX.ins;
	int ALU_result;
	switch (ins.opcode){
		case 0:
			ins.type = 'R';
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
			}
			break;
		case 8: // addi
			ins.type = 'I';
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 9: // addiu
			ins.type = 'I';
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 35: // lw
			ins.type = 'I';
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 33: // lh
			ins.type = 'I';
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 37: // lhu
			ins.type = 'I';
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 32: // lb
			ins.type = 'I';
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 36: // lbu
			ins.type = 'I';
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 43: // sw
			ins.type = 'I';
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 41: // sh
			ins.type = 'I';
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 40: // sb
			ins.type = 'I';
			ALU_result = reg[ins.rs] + (int) ins.C;
			break;
		case 15: // lui
			ins.type = 'I';
			ALU_result = (int) ins.C << 16;
			break;
		case 12: // andi
			ins.type = 'I';
			ALU_result = reg[ins.rs] & (unsigned int) ins.C;
			break;
		case 13: // ori
			ins.type = 'I';
			ALU_result = reg[ins.rs] | (unsigned int) ins.C;
			break;
		case 14: // nori
			ins.type = 'I';
			ALU_result = ~(reg[ins.rs] | (unsigned int) ins.C);
			break;
		case 10: // slti
			ins.type = 'I';
			ALU_result = (reg[ins.rs] < ins.C);
			break;
		case 63: // Halt
			Halt = true;
			break;
	}
	EX_MEM.ALU_result = ALU_result;
	EX_MEM.ins = ins;
	return ins;
}

Instruction Memory_Access(){
	Instruction ins = EX_MEM.ins;
	int Data = 0;
	switch(ins.opcode){
		case 35: // lw
			for(int i=0; i<4; i++)
				Data = (Data << 8) | (unsigned char)Memory[EX_MEM.ALU_result + i];
			break;
		case 33: // lh
			Data = Memory[EX_MEM.ALU_result];
			for(int i=1; i<2; i++)
				Data = (Data << 8) | (unsigned char)Memory[EX_MEM.ALU_result + i];
			break;
		case 37: // lhu
			for(int i=0; i<2; i++)
				Data = (Data << 8) | (unsigned char)Memory[EX_MEM.ALU_result + i];
			break;
		case 32: // lb
			Data = Memory[EX_MEM.ALU_result];
			break;
		case 36: // lbu
			Data = (unsigned char)Memory[EX_MEM.ALU_result];
			break;
		case 43: // sw
			for(int i=0; i<4; i++)
				Memory[EX_MEM.ALU_result + i] = (char)(reg[ins.rt] >> (8*(3-i)));
			break;
		case 41: // sh
			for(int i=0; i<4; i++)
				Memory[EX_MEM.ALU_result + i] = (char)(reg[ins.rt] >> (8*(1-i)));
			break;
		case 40: // sb
			Memory[EX_MEM.ALU_result] = (char)reg[ins.rt];
			break;
		default: // Store Load else
			Data = EX_MEM.ALU_result;
			break;
	}
	MEM_WB.Data = Data;
	MEM_WB.ALU_result = EX_MEM.ALU_result;
	MEM_WB.ins = ins;
	return ins;
}

Instruction Write_Back(){
	Instruction ins = MEM_WB.ins;
	if(ins.type=='R')
		reg[ins.rd] = MEM_WB.ALU_result;
	else if(ins.type=='I')
		reg[ins.rt] = MEM_WB.Data;
	return ins;
}