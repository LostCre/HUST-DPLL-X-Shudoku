object = cnfProcess.o algorithm.o  main.o 

SAT_Solver: main.o algorithm.o cnfProcess.o
	gcc -o SAT_Solver ${object}

main.o: main.c def.h
	gcc -g -c main.c

algorithm.o: algorithm.c def.h stack.h
	gcc -g -c algorithm.c

cnfProcess.o: cnfProcess.c def.h 
	gcc -g -c cnfProcess.c

.PHONY: clean
clean:
	@-del /f $(object) SAT_Solver.exe
	@-echo "Clean done"