NAME = mcresponse
INCDIR = include
SRCDIR = src
OBJDIR = $(SRCDIR)/obj

CC = g++
CCFLAGS += -g -Wall
CCFLAGS += -Iinclude -Irootlibs
CCFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)
LDLIBS += $(shell $(ROOTSYS)/bin/root-config --libs)

TDICT = TObjDict
TDICTSRC = $(TDICT).cc
TDICTOBJ = $(TDICT).o
TDOLOC = $(OBJDIR)/$(TDICTOBJ)
TDSLOC = $(SRCDIR)/$(TDICTSRC)
TLINKDEFINITIONS = LinkDef.h

TDEP = TPulseIsland TGlobalData TTrackerHit TEvent
TINC = $(patsubst %,%.hh,$(TDEP))
TINCLOC = $(patsubst %,$(INCDIR)/%,$(TINC))

DEP = MCFileHandler ArgumentParser DetectorCharacteristics DetectorResponse $(TDEP)
SRC = $(patsubst %,%.cc,$(DEP))
INC = $(SRC:.cc=.hh)
OBJ = $(SRC:.cc=.o)
SRCLOC = $(patsubst %,$(SRCDIR)/%,$(SRC))
INCLOC = $(patsubst %,$(INCDIR)/%,$(INC))
OBJLOC = $(patsubst %,$(OBJDIR)/%,$(OBJ))


all: $(OBJLOC) $(TDOLOC) $(NAME)
	$(CC) $(CCFLAGS) $(OBJLOC) $(TDOLOC) $(NAME).o $(LDLIBS) -o $(NAME)

$(NAME): $(NAME).cc $(INCLOC)
	$(CC) -c $(CCFLAGS) $< -o $@.o

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	@mkdir -p $(OBJDIR)
	$(CC) -c $(CCFLAGS) $< -o $@

$(TDSLOC):
	@rootcint $@ -c -f $(CCFLAGS) $(TINCLOC) $(TLINKDEFINITIONS)

clean:
	-rm -f $(OBJDIR)/*.o
	-rm -f $(SRCDIR)/*Dict* $(SRCDIR)/*.d $(INCDIR)/*.d
	-rm -f $(NAME)
