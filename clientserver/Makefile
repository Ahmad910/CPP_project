# Define the compiler and the linker. The linker must be defined since
# the implicit rule for linking uses CC as the linker. g++ can be
# changed to clang++.
CXX = g++
CC  = $(CXX)

# Generate dependencies in *.d files
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of GNU's libstdc++.
# -g is for debugging.
CPPFLAGS  = -std=c++11 -I..
CXXFLAGS =  -O0 -Wall -Wextra -pedantic-errors -Wold-style-cast
CXXFLAGS += -std=c++11
CXXFLAGS += -g -ggdb
CXXFLAGS += $(DEPFLAGS)
LDFLAGS   = -g -L..
#CPPFLAGS += -stdlib=libc++
#CXXFLAGS += -stdlib=libc++
#LDFLAGS +=  -stdlib=libc++

# Libraries
#LDLIBS = -lclientserver

# Targets
#<<<<<<< HEAD
#PROGS =  memorydbtest
#all: $(PROGS)


# Targets rely on implicit rules for compiling and linking
#memorydbtest: memorydbtest.o article.o newsgroup.o inmemory_db.o

#g++ -o main main.o memorydbtest.o -L. -llab1

# Phony targets
#.PHONY: all test clean distclean

# Standard clean
#clean:
#	rm -f *.o $(PROGS)

#distclean: clean
#	rm *.d
#=======
PROGS = myserver myclient

all: $(PROGS)
PREFIX = /h/d6/i/dat15aib/cpp/clientserver
.PHONY: install
install: myserver
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/
install: myclient
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/

# Targets rely on implicit rules for compiling and linking
# The dependency on libclientserver.a is not defined.
myserver: dbDisk.o connection.o messageHandler.o sessionHandler.o server.o myserver.o inmemory_db.o article.o newsgroup.o
myclient: myclient.o connection.o messageHandler.o

SRC = $(wildcard *.cc)

# Phony targets
.PHONY: all clean distclean

# Standard clean
clean:
	rm -f $(SRC:.cc=.o) $(PROGS)


distclean: clean
	-rm $(SRC:.cc=.d)
	-rm -r $(addsuffix .dSym,$(PROGS)) # for cleaning on Macs

#>>>>>>> 8b6fb76e58db01fcce4ac66c82853c0c80bb1197


# Include the *.d files
SRC = $(wildcard *.cc)
-include $(SRC:.cc=.d)
