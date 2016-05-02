/***************************************************

	File: Pipeline.cpp

	Author: PinYo

***************************************************/

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <iomanip>
#include "GlobalVar.h"
#include "Stage.h"

using namespace std;

string Error_Message[4] = {": Write $0 Error", ": Global::Address Overflow", ": Misalignment Error", ": Number Overflow"};
string fiveStage[5] = {"IF: ", "ID: ", "EX: ", "DM: ", "WB: "};
Instruction fiveStageIns[5];

bool isBranch2(Instruction ins){
	return ((ins.Name=="BEQ") || (ins.Name=="BNE") || (ins.Name=="BGTZ") || (ins.Name=="J") || (ins.Name=="JAL") || (ins.Name=="JR"));
}

void Test(){
	Instruction ins = Global::IF_ID.ins;

	// Stall
	if((Global::EX_MEM.MemRead && ((Global::EX_MEM.WriteDes == ins.rs) || (Global::EX_MEM.WriteDes == ins.rt))) ||
		(Global::ID_EX.MemRead && ((Global::ID_EX.WriteDes == ins.rs) || (Global::ID_EX.WriteDes == ins.rt))))
		Global::Stall = true;
	else 
		Global::Stall = false;

	// Forwarding in ID
	if(Global::EX_MEM.RegWrite && (Global::EX_MEM.WriteDes!=0) && (Global::EX_MEM.WriteDes == ins.rs))
		Global::IF_ID.ins.fwdrs = true;
	else
		Global::IF_ID.ins.fwdrs = false;
	if(Global::EX_MEM.RegWrite && (Global::EX_MEM.WriteDes!=0) && (Global::EX_MEM.WriteDes == ins.rt))
		Global::IF_ID.ins.fwdrt = true;
	else
		Global::IF_ID.ins.fwdrt = false;

	// Forwarding in EXE
	ins = Global::ID_EX.ins;
	if(!isBranch2(ins)){
		if(Global::EX_MEM.RegWrite && (Global::EX_MEM.WriteDes!=0) && (Global::EX_MEM.WriteDes == ins.rs))
			Global::EX_MEM.ins.fwdrs = true;
		else
			Global::EX_MEM.ins.fwdrs = false;
		if(Global::EX_MEM.RegWrite && (Global::EX_MEM.WriteDes!=0) && (Global::EX_MEM.WriteDes == ins.rt))
			Global::EX_MEM.ins.fwdrt = true;
		else
			Global::EX_MEM.ins.fwdrt = false;
	}
	else{
		Global::ID_EX.ins.fwdrs = false;
		Global::ID_EX.ins.fwdrt = false;
	}
}

void InitialReg(){
	for(int i=0; i<32; i++)
		Global::reg[i] = 0;
}

void cyclePrint(ofstream &fout, int &Cycle){
	fout << "cycle " << dec << Cycle++ << endl;
	for(int i=0; i<32; i++){
		fout << "$" << setw(2) << setfill('0') << dec << i;
		fout << ": 0x" << setw(8) << setfill('0') << hex << uppercase << Global::reg[i] << endl;
	}
	fout << "PC: 0x" << setw(8) << setfill('0') << hex << uppercase << Global::PC << endl;
	fout << fiveStage[0] << "0x" << setw(8) << setfill('0') << hex << uppercase << Global::Address[Global::PC];
	if(Global::Stall)
		fout << " to_be_stalled";
	fout << endl;
	if(Global::Address[Global::PC] != 0xFFFFFFFF)
			Global::Halt = false;
	Buffer B;
	for(int i=0; i<4; i++){
		if(i == 0) B = Global::IF_ID;
		else if(i == 1) B = Global::ID_EX;
		else if(i == 2) B = Global::EX_MEM;
		else B = Global::MEM_WB;
		if(B.ins.Name != "HALT")
			Global::Halt = false;
		int op = ((unsigned int)B.ins.Word) >> 26;
		int fu = ((unsigned int)(B.ins.Word << 26)) >> 26;
		bool check = (op==0 && fu==0);
		fout << fiveStage[i+1];
		if(check && (B.ins.rt == 0) && (B.ins.rd == 0) && (B.ins.shamt == 0)) 
			fout << "NOP" << endl;
		else if(Global::Stall && i == 0)
			fout << B.ins.Name << " to_be_stalled" << endl;
		else if(B.ins.fwdrs)
			fout << B.ins.Name << " fwd_EX-DM_rs_$" << B.ins.rs << endl;
		else if(B.ins.fwdrt)
			fout << B.ins.Name << " fwd_EX-DM_rt_$" << B.ins.rt << endl;
		else 
			fout << B.ins.Name << endl;
 	}
	fout << endl << endl;
}

int main(){
	char ch;
	int Word = 0, bytes = 4, Cycle = 0, idx = -2;

	// Initialize register;
	InitialReg();

	ofstream fout("snapshot.rpt", ios::out);
	ofstream Errorout("error_dump.rpt", ios::out);

	// Read iimage.bin
	ifstream fin("iimage.bin", ios::in | ios::binary);
	if(!fin){
		cout << "Error to load 'iimage.bin'!\n";
		return 0;
	}
	while(!fin.eof()){
		fin.get(ch);
		Word = (Word << 8) | (unsigned char)ch;
		bytes--;
		if(bytes==0){
			bytes = 4;
			if(idx==-2){
				Global::PC = Word;
				idx++;
				continue;
			}
			else if(idx==-1){
				idx++;
				continue;
			}
			Global::Address[Global::PC+idx*4] = Word;
			idx++;
		}
	}
	fin.close();

	// Read dimage.bin
	fin.open("dimage.bin", ios::in | ios::binary);
	// Read $sp
	for(int i=4; i>0; i--){
		fin.get(ch);
		Word = (Word << 8) + (unsigned char)ch;
	}
	Global::reg[29] = Word;
	// Numbers of words
	for(int i=4; i>0; i--){
		fin.get(ch);
		Word = (Word << 8) + (unsigned char)ch;
	}
	int NumbersOfWords = Word;
	for(int i=0; i<NumbersOfWords*4; i++){
		fin.get(ch);
		Global::Memory[i] = ch;
	}

	cyclePrint(fout, Cycle);
	Global::Halt = false;
	Global::Stall = false;

	//Start Instructions
	while(!Global::Halt){
		Global::Branch_taken = false;
		for(int i=0; i<4; i++) Global::error_type[i] = false;		
		Write_Back();
		Memory_Access();
		Execute();
		Instruction_Decode();
		Instruction_Fetch();
		//cyclePrint(fout, Cycle);
		for(int i=0; i<4; i++){
			if(Global::error_type[i]==true)
				Errorout << "In cycle " << Cycle << Error_Message[i] << endl;
		}
		if(!Global::Stall && Global::Branch_taken){
			Instruction Emp;
			Global::PC = Global::Branch_PC;
			Global::IF_ID.ins = Emp;
		}
		else if(!Global::Stall)
			Global::PC += 4;
		Test();
		cyclePrint(fout, Cycle);
		if(Global::Halt==true)
			break;
	}
	fout.close();
	Errorout.close();
	return 0;
}