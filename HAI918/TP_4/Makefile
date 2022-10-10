CXX		= g++
CXX_FLAGS	= -Wall -g -O0 -ansi -pedantic -std=c99 

LD_FLAGS 	=

PROGS		= run_nn #test1 run_nn #histogramme #couleur #histogram3c #seuillage4p 

SOURCES		= main.cpp AES.cpp#test1 main.cpp histogramme.cpp  #test.cpp test_couleur.cpp #histogram3c.cpp # seuillage4p.cpp 

HEADERS		= imglib.h image_ppm.h AES.h# 

DEPFILE		= .depend

ifeq ($(wildcard $(DEPFILE)), )

all: $(DEPFILE)
	make

else

all: $(PROGS)

include $(DEPFILE)

endif

include $(DEPFILE)

.SUFFIXES: .c .h .o 

$(PROGS): $(SOURCES:.c=.o)
	$(CXX) $(LD_FLAGS) $^ -o $@

.c.o:
	$(CXX) $(CXX_FLAGS)$< -c

$(DEPFILE): $(SOURCES)
	$(CXX) -MM $^ > $@ 

clean:
	rm -f *~ $(SOURCES:.c=.o) $(PROGS)
