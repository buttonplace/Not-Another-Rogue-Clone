CC = gcc
CXX = g++
ECHO = echo
RM = rm -f	

CFLAGS = -Wall -Werror -ggdb
CXXFLAGS = -Wall -Werror -ggdb

BIN = main
OBJS = main.o heap.o generator.o map.o game.o

all: $(BIN) announce

$(BIN): $(OBJS)
	@$(ECHO) Linking $@
	@$(CC) $^ -o $@

-include $(OBJS:.o=.d)

%.o: %.c
	@$(ECHO) Compiling $<
	@$(CC) $(CFLAGS) -MMD -MF $*.d -c $<

%.o: %.cpp
	@$(ECHO) Compiling $<
	@$(CXX) $(CXXFLAGS) -MMD -MF $*.d -c $<

.PHONY: all clean clobber etags

clean:
	@$(ECHO) Removing all generated files
	@$(RM) *.o $(BIN) *.d TAGS core vgcore.* gmon.out

clobber: clean
	@$(ECHO) Removing backup files
	@$(RM) *~ \#* *pgm

announce:
	@$(ECHO) 
	@$(ECHO) Run with './main'
	@$(ECHO) 