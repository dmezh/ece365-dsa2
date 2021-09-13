CXXFLAGS = -O3 --std=c++17

spell: spellcheck.o hash.o util.o
	g++ $(CXXFLAGS) -o spell spellcheck.o hash.o util.o

spellcheck.o: spellcheck.cpp hash.h
	g++ $(CXXFLAGS) -c spellcheck.cpp

hash.o: hash.cpp hash.h
	g++ $(CXXFLAGS) -c hash.cpp

util.o: util.cpp util.h
	g++ $(CXXFLAGS) -c util.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups