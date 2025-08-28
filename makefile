shell: main.o shell.o parser.o exe_cmd.o streams.o
	gcc main.o shell.o parser.o exe_cmd.o streams.o -o shell

main.o: main.c shell.h
	gcc -c  main.c -o main.o

shell.o: shell.c shell.h
	gcc -c shell.c -o shell.o

parser.o: parser.c shell.h parser.h
	gcc -c parser.c -o parser.o

exe_cmd.o: execute_commands.c shell.h parser.h 
	gcc -c  execute_commands.c -o exe_cmd.o

streams.o: streams.c shell.h parser.h
	gcc -c streams.c -o streams.o

clean:
	rm -f *.o shell