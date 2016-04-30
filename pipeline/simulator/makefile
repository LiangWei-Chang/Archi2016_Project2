CC = g++ -std=c++11

pipeline: pipeline.o Stage.o GlobalVar.o
	g++ -o pipeline pipeline.o Stage.o  GlobalVar.o
pipeline.o: pipeline.cpp Stage.h GlobalVar.h
	g++ -c -g pipeline.cpp
GlobalVar.o: GlobalVar.cpp GlobalVar.h
	g++ -c -g GlobalVar.cpp
Stage.o: Stage.cpp Stage.h GlobalVar.h
	g++ -c -g Stage.cpp
clean:
	rm -f Stage.o pipeline.o GlobalVar.o

