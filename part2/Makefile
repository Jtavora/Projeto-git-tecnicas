programa: arq.o
	gcc func.o git.o -o prog.exe
	gcc func.o serv.o -o prog2.exe

arq.o: Git.h Git.c Functions.c serv.c 
	gcc -c -g Git.c -o git.o
	gcc -c -g Functions.c -o func.o
	gcc -c -g serv.c -o serv.o

run: programa
	./prog.exe

clean:
	rm func.o git.o prog.exe 

clean2:
	rm serv.o prog2.exe 

all: programa run clean
