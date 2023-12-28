programm: main.o gate.o connection.o type.o function.o simulation.o
	gcc -o programm main.o gate.o connection.o type.o function.o simulation.o

main.o: main.c
	gcc -c main.c

gate.o: gate.c
	gcc -c gate.c

connection.o: connection.c
	gcc -c connection.c

type.o: type.c
	gcc -c type.c

function.o: function.c
	gcc -c function.c

simulation.o: simulation.c
	gcc -c simulation.c

clean:
	del *.o output.exe
