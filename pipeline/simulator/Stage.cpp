/***************************************************

	File: Stage.cpp

	Author: PinYo

***************************************************/
#include "GlobalVar.h"
#include "Stage.h"
#include <iostream>

bool isBranch(Instruction ins){
	return ((ins.Name=="BEQ") || (ins.Name=="BNE") || (ins.Name=="BGTZ") || (ins.Name=="J") || (ins.Name=="JAL") || (ins.Name=="JR"));
}

Instruction Decode(int Word){
	Instruction ins;
	ins.Word = Word;
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
		case 0:
			ins.type = 'R';
			switch (ins.funct){
				case 32: // add
					ins.Name = "ADD";
					break;
				case 33: // addu
					ins.Name = "ADDU";
					break;
				case 34: // sub
					ins.Name = "SUB";
					break;
				case 36: // and
					ins.Name = "AND";
					break;
				case 37: // or
					ins.Name = "OR";
					break;
				case 38: // xor
					ins.Name = "XOR";
					break;
				case 39: // nor
					ins.Name = "NOR";
					break;
				case 40: // nand
					ins.Name = "NAND";
					break;
				case 42: // slt
					ins.Name = "SLT";
					break;
				case 0: // sll
					ins.Name = "SLL";
					break;
				case 2: // srl
					ins.Name = "SRL";
					break;
				case 3: // sra
					ins.Name = "SRA";
					break;
				case 8: // jr
					ins.Name = "JR";
					break;
			}
			break;
		case 8: // addi
			ins.type = 'I';
			ins.Name = "ADDI";
			break;
		case 9: // addiu
			ins.type = 'I';
			ins.Name = "ADDIU";
			break;
		case 35: // lw
			ins.type = 'I';
			ins.Name = "LW";
			break;
		case 33: // lh
			ins.type = 'I';
			ins.Name = "LH";
			break;
		case 37: // lhu
			ins.type = 'I';
			ins.Name = "LHU";
			break;
		case 32: // lb
			ins.type = 'I';
			ins.Name = "LB";
			break;
		case 36: // lbu
			ins.type = 'I';
			ins.Name = "LBU";
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
			break;
		case 12: // andi
			ins.type = 'I';
			ins.Name = "ANDI";
			break;
		case 13: // ori
			ins.type = 'I';
			ins.Name = "ORI";
			break;
		case 14: // nori
			ins.type = 'I';
			ins.Name = "NORI";
			break;
		case 10: // slti
			ins.type = 'I';
			ins.Name = "SLTI";
			break;
		case 63: // Halt
			ins.Name = "HALT";
			break;
		case 4: // beq
			ins.Name = "BEQ";
			break;
		case 5: // bne
			ins.Name = "BNE";
			break;
		case 7: // bgtz
			ins.Name = "BGTZ";
			break;
		case 2: // j
			ins.Name = "J";
			break;
		case 3: // jal
			ins.Name = "JAL";
			break;
	}
	return ins;
}

void Instruction_Fetch(){
	Instruction ins;
	ins.Word = Global::Address[Global::PC];
	ins = Decode(ins.Word);
	if(!Global::Stall) Global::IF_ID.ins = ins;
}

void Instruction_Decode(){
	Instruction ins = Global::IF_ID.ins;
	int K = 0, DataRs, DataRt;

	if(Global::Stall){
		Instruction NOP;
		Global::ID_EX.ins = NOP;
		Global::ID_EX.MemRead = false;
		Global::ID_EX.RegWrite = false;
		return;
	}

	// Forwarding
	if(ins.fwdrs)
		DataRs = Global::MEM_WB.ALU_result;
	else
		DataRs = Global::reg[ins.rs];
	if(ins.fwdrt)
		DataRt = Global::MEM_WB.ALU_result;
	else
		DataRt = Global::reg[ins.rt];

	Global::ID_EX.RegWrite = false;
	Global::ID_EX.MemRead = false;

	switch(ins.opcode){
		case 0:
			switch (ins.funct){
				case 32: // add
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 33: // addu
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 34: // sub
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 36: // and
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 37: // or
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 38: // xor
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 39: // nor
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 40: // nand
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 42: // slt
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 0: // sll
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 2: // srl
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 3: // sra
					Global::ID_EX.RegWrite = true;
					Global::ID_EX.WriteDes = ins.rd;
					break;
				case 8: // jr
					Global::Branch_taken = true;
					Global::Branch_PC = Global::reg[ins.rs];
					break;
			}
			break;
		case 8: // addi
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 9: // addiu
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 35: // lw
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.MemRead = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 33: // lh
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.MemRead = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.MemRead = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 32: // lb
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.MemRead = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 36: // lbu
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.MemRead = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 15: // lui
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 12: // andi
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 13: // ori
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 14: // nori
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 10: // slti
			Global::ID_EX.RegWrite = true;
			Global::ID_EX.WriteDes = ins.rt;
			break;
		case 63: // Halt
			Global::Halt = true;
			break;
		case 4: // beq
			Global::Branch_taken = (DataRs == DataRt);
			Global::Branch_PC = Global::PC + 4 + 4 * (int)ins.C;
			break;
		case 5: // bne
			Global::Branch_taken = (DataRs != DataRt);
			Global::Branch_PC = Global::PC + 4 + 4 * (int)ins.C;
			break;
		case 7: // bgtz
			Global::Branch_taken = (DataRs > 0);
			Global::Branch_PC = Global::PC + 4 + 4 * (int)ins.C;
			break;
		case 2: // j
			ins.C = (unsigned)(ins.Word << 6) >> 6;
			K = ((Global::PC+4) >> 28) << 28;
			K += (ins.C << 2);
			Global::Branch_taken = true;
			Global::Branch_PC = K;
			break;
		case 3: // jal
			Global::reg[31] = Global::PC + 4;
			ins.C = (unsigned)(ins.Word << 6) >> 6;
			K = ((Global::PC+4) >> 28) << 28;
			K += (ins.C << 2);
			Global::Branch_taken = true;
			Global::Branch_PC = K;
			break;
	}
	if(!Global::Stall) Global::ID_EX.ins = ins;
	Global::ID_EX.RegRs = DataRs;
	Global::ID_EX.RegRt = DataRt;
}

void Execute(){
	Instruction ins = Global::ID_EX.ins;
	int ALU_result = 0, DataRs = 0, DataRt = 0;

	// Forwarding
	if(!isBranch(ins)){
		if(ins.fwdrs)
			DataRs = Global::MEM_WB.ALU_result;
		else
			DataRs = Global::ID_EX.RegRs;
		if(ins.fwdrt)
			DataRt = Global::MEM_WB.ALU_result;
		else
			DataRt = Global::ID_EX.RegRs;
	}
	else{
		ins.fwdrs = false;
		ins.fwdrt = false;
	}
	
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
	Global::EX_MEM.ALU_result = ALU_result;
	Global::EX_MEM.ins = ins;
	Global::EX_MEM.RegRt = DataRt;
	Global::EX_MEM.RegWrite = Global::ID_EX.RegWrite;
	Global::EX_MEM.MemRead = Global::ID_EX.MemRead;
	Global::EX_MEM.WriteDes = Global::ID_EX.WriteDes;
}

void Memory_Access(){
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
	Global::MEM_WB.RegWrite = Global::EX_MEM.RegWrite;
	Global::MEM_WB.MemRead = Global::EX_MEM.MemRead;
	Global::MEM_WB.WriteDes = Global::EX_MEM.WriteDes;
}

void Write_Back(){
	Instruction ins = Global::MEM_WB.ins;
	if(ins.type=='R')
		Global::reg[Global::MEM_WB.WriteDes] = Global::MEM_WB.ALU_result;
	else if(ins.type=='I')
		Global::reg[Global::MEM_WB.WriteDes] = Global::MEM_WB.Data;
}