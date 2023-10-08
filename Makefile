CC        = gcc

SANFLAGS  =

CFLAGS    = -g -Wall -Wextra $(SANFLAGS) -DYYDEBUG
LDFLAGS   = $(SANFLAGS)
LDLIBS    = -lm

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
