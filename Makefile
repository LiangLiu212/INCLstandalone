IDIR = include
CC=g++
#CFLAGS=-I$(IDIR) -I/root/inclxx/install-v6.33.1/include \
#	`inclxx-config --cflags --libs` \
#	-L/root/inclxx/install-v6.33.1/lib \
#	-DG4ThreadLocal="" -DHAS_BOOST_PROGRAM_OPTIONS \
#	`root-config --cflags --libs` \
#	-DINCL_DEEXCITATION_ABLA07 \
#	-DINCL_DEEXCITATION_ABLAXX \
#	-DINCL_DEEXCITATION_GEMINIXX \
#	-DINCL_DEEXCITATION_SMM \
#	-DINCL_ROOT_USE -lboost_program_options \
#	-DINCL_REGENERATE_AVATARS  -lboost_filesystem -DHAS_BOOST_TIMER -DINCL_DEBUG_LOG
CFLAGS=-I$(IDIR) -I/root/inclxx/install-v6.33.1/include \
	`inclxx-config --cflags --libs` \
	-L/root/inclxx/install-v6.33.1/lib \
	-DG4ThreadLocal="" \
	`root-config --cflags --libs` \
	-DINCL_DEEXCITATION_ABLA07 \
	-DINCL_DEEXCITATION_ABLAXX \
	-DINCL_DEEXCITATION_GEMINIXX \
	-DINCL_DEEXCITATION_SMM \
	-DINCL_ROOT_USE \
	-DINCL_REGENERATE_AVATARS \
	-DINCL_DEBUG_LOG




ODIR=obj
BDIR=bin
LDIR=lib
SDIR=src

LIBS=-lm

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
_TARGET=main
TARGET = $(patsubst %,$(BDIR)/%,$(_TARGET))

ALL: ${TARGET}

obj/main.o: src/main.cxx 
	$(CC) -c -o $@ $< $(CFLAGS)
obj/MYConfigParser.o: src/MYConfigParser.cc
	$(CC) -c -o $@ $< $(CFLAGS)


${TARGET}: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) -lboost_program_options

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
