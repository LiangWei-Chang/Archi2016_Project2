CC = g++ -std=c++11 -Wall -Wextra

pipeline: pipeline.o Stage.o GlobalVar.o
	${CC} -o pipeline pipeline.o Stage.o  GlobalVar.o
pipeline.o: pipeline.cpp Stage.h GlobalVar.h
	${CC} -c -g pipeline.cpp
GlobalVar.o: GlobalVar.cpp GlobalVar.h
	${CC} -c -g GlobalVar.cpp
Stage.o: Stage.cpp Stage.h GlobalVar.h
	${CC} -c -g Stage.cpp
clean:
	rm -f Stage.o pipeline.o GlobalVar.o

