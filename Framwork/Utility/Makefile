#Makefile for test
# AIX version

.SUFFIXES:
.SUFFIXES: .cpp .obj .o

CPP = g++

INC=-I$(HOME)/include

CPPFLAGS =  -g -Wall  -O2 
CPPDEFS = -DLINUX -DUNIX -DNDEBUG -D_THREAD_SAFE -D_MT -DSA_USE_PTHREAD -D_AIX32_THREADS 

.cpp.o:
	$(CPP) $(CPPFLAGS) -c  $(CPPDEFS) $(KERNEL_INC) $< -o $@

OBJL =  ConfigImpl.o ConsoleLogDev.o FileLogDev.o GessDate.o GessTime.o GessDateTime.o LogDevice.o Logger.o NetLogDev.o strutils.o Encode.o ThreadInterface.o GessTimerMgr.o GessTimerPosix.o

#ALL:   libutility.a  clean
ALL:   libutility.a

libutility.a: $(OBJL)
	ar rv $@ $(OBJL)
	mv $@ $(HOME)/lib
clean:
	rm -f *.o

