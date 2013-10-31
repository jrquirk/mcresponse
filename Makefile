NAME = mcresponse
INCDIR = include
SRCDIR = src
OBJDIR = $(SRCDIR)/obj

CC = g++
CCFLAGS += -g -Wall
CCFLAGS += -Iinclude -Irootlibs
CCFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)
LDLIBS += $(shell $(ROOTSYS)/bin/root-config --glibs)

DEP = MCFileHandler ArgumentParser DetectorResponse TPulseIsland TGlobalData
SRC = $(patsubst %,%.cc,$(DEP))
INC = $(SRC:.cc=.hh)
OBJ = $(SRC:.cc=.o)
SRCLOC = $(patsubst %,$(SRCDIR)/%,$(SRC))
INCLOC = $(patsubst %,$(INCDIR)/%,$(INC))
OBJLOC = $(patsubst %,$(OBJDIR)/%,$(OBJ))

TDEP = TPulseIslandDict TGlobalDataDict
TSRC = $(patsubst %,%.cc,$(TDEP))
TOBJ = $(TSRC:.cc=.o)
TSRCLOC = $(patsubst %,$(SRCDIR)/%,$(TSRC))
TOBJLOC = $(patsubst %,$(OBJDIR)/%,$(TOBJ))

all: $(OBJLOC) $(TOBJLOC) $(NAME)
	$(CC) $(CCFLAGS) $(OBJLOC) $(TOBJLOC) rootlibs/TEventDict.o rootlibs/TEvent.o rootlibs/TTrackerHit.o $(NAME).o $(LDLIBS) -o $(NAME)

$(NAME): $(NAME).cc $(INCLOC)
	$(CC) -c $(CCFLAGS) $< -o $@.o

$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(INCLOC)
	$(CC) -c $(CCFLAGS) $< -o $@

$(SRCDIR)/T%Dict.cc:
	@rootcint $@ -c $(patsubst $(SRCDIR)/%Dict.cc,$(INCDIR)/%.hh,$@)

clean:
	-rm -f $(OBJLOC) *.o
	-rm -f $(SRCDIR)/*Dict* $(SRCDIR)/*.d $(INCDIR)/*.d
	-rm -f $(NAME)
