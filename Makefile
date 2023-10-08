# Dalio, Brian A.
# dalioba
# 2023-04-10
#-----------------------------------------------------------------
<<<<<<< HEAD
CC        = gcc
=======
CC       = g++
>>>>>>> 2675d6a388412259b0ed0d7922b0cb6bcccc78e1

SANFLAGS  =

<<<<<<< HEAD
CFLAGS    = -g -Wall -Wextra $(SANFLAGS) -DYYDEBUG
LDFLAGS   = $(SANFLAGS)
LDLIBS    = -lm

NODEFILES = nodeAllocFree.c nodeDump.c nodeExprs.c nodeLeafItems.c nodeScope.c nodeStmts.c
SRCFILES  = $(NODEFILES) semantic.c
=======
# Add -Wno-write-strings to suppress string literal warnings
CFLAGS   = -g -Wall -Wextra -Wno-write-strings $(SANFLAGS)
LDFLAGS  = $(SANFLAGS)
LDLIBS   = -lm
>>>>>>> 2675d6a388412259b0ed0d7922b0cb6bcccc78e1

#-----------------------------------------------------------------
%.o : %.c
	$(CC) $(CFLAGS) -c $^

#-----------------------------------------------------------------
frontEnd : lexer.l parser.y main.c $(SRCFILES)
	flex --outfile=lexer.yy.c lexer.l
	bison -Werror=conflicts-sr -Werror=conflicts-rr --report=all --xml parser.y

	xsltproc /usr/share/bison/xslt/xml2xhtml.xsl parser.xml > parser.html
	rm -f parser.xml

	$(CC) $(CFLAGS) -o frontEnd  lexer.yy.c parser.tab.c main.c $(SRCFILES) $(LDLIBS)

#-----------------------------------------------------------------
clean:
	rm -f frontEnd *.o *.yy.[ch] *.tab.[ch] *.output *.html

#-----------------------------------------------------------------
