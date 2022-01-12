SHELL=/bin/bash

P2.out : Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o
	g++ -g -o P2.out Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o

Project2.o : Project2.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ -g -c Project2.cpp

SetLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ -g -c SyntacticalAnalyzer.cpp

clean : 
	rm -f [SP]*.o P2.out *.gch *.p1 *.p2 *.dbg *.lst
	rm -f P2Tests/*.dbg P2Tests/*.lst P2Tests/*.p1 P2Tests/*.p2
	rm -f P2TestsPickup/*.dbg P2TestsPickup/*.lst P2TestsPickup/*.p1 P2TestsPickup/*.p2

run: P2.out
	./P2.out P2Tests/P2-481-782.ss
	./P2.out P2Tests/P2-51-60.ss
	./P2.out P2Tests/P2-50-82.ss
	./P2.out P2Tests/P2-48-78.ss
	./P2.out P2Tests/P2-46-56.ss
	./P2.out P2Tests/P2-45-63.ss
	./P2.out P2Tests/P2-44-85.ss
	./P2.out P2Tests/P2-43-74.ss
	./P2.out P2Tests/P2-41-70.ss
	./P2.out P2Tests/P2-40-59.ss
	./P2.out P2Tests/P2-37-77.ss
	./P2.out P2Tests/P2-35-73.ss
	./P2.out P2Tests/P2-33-84.ss
	./P2.out P2Tests/P2-30-69.ss
	./P2.out P2Tests/P2-28-85.ss
	./P2.out P2Tests/P2-28-65.ss
	./P2.out P2Tests/P2-27-54.ss
	./P2.out P2Tests/P2-23-61.ss
	./P2.out P2Tests/P2-20-79.ss
	./P2.out P2Tests/P2-18-75.ss
	./P2.out P2Tests/P2-17-64.ss
	./P2.out P2Tests/P2-11-52.ss
	./P2.out P2Tests/P2-11-52e.ss
	./P2.out P2TestsPickup/P2-1.ss
	./P2.out P2TestsPickup/P2-2.ss
	./P2.out P2TestsPickup/P2-3.ss
	./P2.out P2TestsPickup/P2-4.ss
	./P2.out P2TestsPickup/P2-5.ss
	./P2.out P2TestsPickup/P2-6.ss
	./P2.out P2TestsPickup/P2-7.ss
	./P2.out P2TestsPickup/P2-8.ss
	./P2.out P2TestsPickup/P2-9.ss

submit : Project2.cpp LexicalAnalyzer.h SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt
	rm -rf marxP2
	mkdir marxP2
	cp Project2.cpp marxP2
	cp SyntacticalAnalyzer.h marxP2
	cp SyntacticalAnalyzer.cpp marxP2
	cp makefile marxP2
	cp README.txt marxP2
	tar cfvz marxP2.tgz marxP2 P2Tests P2TestsPickup
	cp marxP2.tgz ~shubbhi/cs460drop
