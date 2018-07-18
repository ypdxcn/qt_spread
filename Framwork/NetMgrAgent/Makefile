
#Makefile for 
# AIX version

.SUFFIXES:
.SUFFIXES: .cpp .obj .o

CPP = g++

KERNEL_INC=-I$(HOME)/include -I../Utility -I../NetMgr  -I../CommFramwork  -I../FramworkAp

CPPFLAGS =  -g -Wall  -O2 
CPPDEFS = -DLINUX -DUNIX -DNDEBUG -D_THREAD_SAFE -D_MT -DSA_USE_PTHREAD -D_AIX32_THREADS 

.cpp.o:
	$(CPP) $(CPPFLAGS) -c  $(CPPDEFS) $(KERNEL_INC) $< -o $@

OBJL = NetMgrModule.o Packet1911.o Packet1912.o Packet1913.o Packet1914.o Packet1915.o Packet1916.o Packet1921.o Packet1922.o Packet1923.o Packet1924.o Packet1925.o 

#ALL:   libnetmgragent.a  clean
ALL:   libnetmgragent.a

libnetmgragent.a : $(OBJL)
	ar rv $@ $(OBJL)
	mv $@ $(HOME)/lib
clean:
	rm -f *.o
