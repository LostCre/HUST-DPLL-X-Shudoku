object = stack.o cnfProcess.o Xsudoku.o \
		algorithm.o  main.o 

SAT_Solver: ${object}
	gcc -o SAT_Solver ${object}

main.o: main.c def.h stack.h
	gcc -g -c main.c

algorithm.o: algorithm.c def.h stack.h
	gcc -g -c algorithm.c

cnfProcess.o: cnfProcess.c def.h
	gcc -g -c cnfProcess.c

stack.o: stack.c stack.h
	gcc -g -c stack.c

Xsudoku.o: Xsudoku.c def.h Xsudoku.h
	gcc -g -c Xsudoku.c

.PHONY: clean
clean:
	@-del /f $(object) SAT_Solver.exe
	@-echo "Clean done"