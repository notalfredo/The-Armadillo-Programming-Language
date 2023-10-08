# Dalio, Brian A.
# dalioba
# 2023-04-10
#-----------------------------------------------------------------
CC        = gcc

SANFLAGS  =

CFLAGS    = -g -Wall -Wextra $(SANFLAGS) -DYYDEBUG
LDFLAGS   = $(SANFLAGS)
LDLIBS    = -lm

NODEFILES = nodeAllocFree.c nodeDump.c nodeExprs.c nodeLeafItems.c nodeScope.c nodeStmts.c
SRCFILES  = $(NODEFILES) semantic.c

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
