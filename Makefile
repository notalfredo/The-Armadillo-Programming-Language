PROJS  = tokens

#-----------------------------------------------------------------
CC       = g++

SANFLAGS =

# Add -Wno-write-strings to suppress string literal warnings
CFLAGS   = -g -Wall -Wextra -Wno-write-strings $(SANFLAGS)
LDFLAGS  = $(SANFLAGS)
LDLIBS   = -lm

#-----------------------------------------------------------------
%.o : %.c
	$(CC) $(CFLAGS) -c $^

%.c : %.l
	flex --outfile=$*.c $^

tokens : tokens.o
	$(CC) $(LDFLAGS) -o tokens tokens.o

#-----------------------------------------------------------------
clean:
	rm -f *.o $(PROJS) lex.yy.[ch] tokens.tab.[ch]

#-----------------------------------------------------------------

