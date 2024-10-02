FLAGS =  $(ARCH) -g -fPIC -ansi  -Wno-overloaded-virtual -Wno-long-long -fno-common  -pedantic -Wall -std=c++17 `root-config --libs --ldflags --cflags`
LINKER = g++ -g -shared $(OPTIMISATION)


dummy.o: dummy.cc
	$(CXX) $(FLAGS) -c dummy.cc -o dummy.o

dummyRead.o: dummyRead.cc
	$(CXX) $(FLAGS) -c dummyRead.cc -o dummyRead.o

Setup.o: Setup.cc Setup.h
	$(CXX) $(FLAGS) -I. -c Setup.cc -o Setup.o

Cint.cxx: Setup.h LinkDef.h
	rootcint -v -f Cint.cxx -c Setup.h LinkDef.h

Cint.o: Cint.cxx
	$(CXX) $(FLAGS) -c Cint.cxx -o Cint.o

libSetup.so: Cint.o Setup.o
	$(LINKER) -o libSetup.so Setup.o Cint.o `root-config --libs --cflags --ldflags`

dummy: Setup.o libSetup.so dummy.o
	$(CXX) dummy.o Setup.o libSetup.so $(FLAGS) -o dummy

dummyRead: Setup.o libSetup.so dummyRead.o
	$(CXX) dummyRead.o Setup.o libSetup.so $(FLAGS) -o dummyRead

clean:
	rm Setup.o libSetup.so Cint.cxx Cint.h Cint.o dummy dummy.o  dummyRead dummyRead.o Cint_rdict.pcm
