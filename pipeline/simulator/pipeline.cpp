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
	fout << fiveStage[0] << "0x" << setw(8) << setfill('0') << hex << uppercase << fiveStageIns[0].Word;
	if(Global::Stall)
		fout << " to_be_stalled";
	fout << endl;
	for(int i=1; i<5; i++){
		int op = ((unsigned int)fiveStageIns[i].Word) >> 26;
		int fu = ((unsigned int)(fiveStageIns[i].Word << 26)) >> 26;
		bool check = (op==0 && fu==0);
		fout << fiveStage[i];
		if(check && (fiveStageIns[i].rt == 0) && (fiveStageIns[i].rd == 0) && (fiveStageIns[i].shamt == 0)) 
			fout << "NOP" << endl;
		else if(Global::Stall && i == 1)
			fout << fiveStageIns[i].Name << " to_be_stalled" << endl;
		else
			fout << fiveStageIns[i].Name << endl;
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
	Global::Halt = false;
	Global::Stall = false;

	//Start Instructions
	while(!Global::Halt){
		Global::Branch_taken = false;
		for(int i=0; i<4; i++) Global::error_type[i] = false;		
		fiveStageIns[4] = Write_Back();
		fiveStageIns[3] = Memory_Access();
		fiveStageIns[2] = Execute();
		fiveStageIns[1] = Instruction_Decode();
		fiveStageIns[0] = Instruction_Fetch();
		for(int i=0; i<4; i++){
			if(Global::error_type[i]==true)
				Errorout << "In cycle " << Cycle << Error_Message[i] << endl;
		}
		if(Global::Halt==true)
			break;
		cyclePrint(fout, Cycle);
		if(!Global::Stall && Global::Branch_taken){
			Instruction Emp;
			Global::PC = Global::Branch_PC;
			Global::IF_ID.ins = Emp;
		}
		else if(!Global::Stall)
			Global::PC += 4;
	}
	fout.close();
	Errorout.close();
	return 0;
}