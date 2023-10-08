#-----------------------------------------------------------------
#-- Prerequisites ------------------------------------------------
#-----------------------------------------------------------------

* Ensure you have flex 2.6.4 (at least) installed.

  $ apt-cache policy flex
  flex:
    Installed: 2.6.4-6.2
    Candidate: 2.6.4-6.2
    Version table:
   *** 2.6.4-6.2 500
          500 http://archive.ubuntu.com/ubuntu focal/main amd64 Packages
          100 /var/lib/dpkg/status
  $ flex --version
  flex 2.6.4
  $

* Ensure that you have bison 3.5.1 (at least) installed.

  $ apt-cache policy bison
  bison:
    Installed: 2:3.5.1+dfsg-1
    Candidate: 2:3.5.1+dfsg-1
    Version table:
   *** 2:3.5.1+dfsg-1 500
          500 http://archive.ubuntu.com/ubuntu focal/main amd64 Packages
          100 /var/lib/dpkg/status
  $ bison --version
  bison (GNU Bison) 3.5.1
  Written by Robert Corbett and Richard Stallman.

  Copyright (C) 2020 Free Software Foundation, Inc.
  This is free software; see the source for copying conditions.  There is NO
  warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  $

* You don't *need* xsltproc installed, but it does make a much
    nicer looking parser automaton report if you do.

  $ apt-cache policy xsltproc
  xsltproc:
    Installed: 1.1.34-4ubuntu0.20.04.1
    Candidate: 1.1.34-4ubuntu0.20.04.1
    Version table:
   *** 1.1.34-4ubuntu0.20.04.1 500
          500 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 Packages
          500 http://security.ubuntu.com/ubuntu focal-security/main amd64 Packages
          100 /var/lib/dpkg/status
       1.1.34-4 500
          500 http://archive.ubuntu.com/ubuntu focal/main amd64 Packages
  $ xsltproc --version
  Using libxml 20914, libxslt 10135 and libexslt 820
  xsltproc was compiled against libxml 20914, libxslt 10135 and libexslt 820
  libxslt 10135 was compiled against libxml 20914
  libexslt 820 was compiled against libxml 20914
  $

  If you choose not to install xsltproc, comment out or remove
    line 25 of the Makefile.  (That's the line invoking xsltproc.)

#-----------------------------------------------------------------
#-- What to do ---------------------------------------------------
#-----------------------------------------------------------------

* In class, we added the WHILE statement to the frontEnd.  It
    wasn't hard.  Let's enumerate exactly what was changed.

  == LEXICAL
keywords.h :
  Added an entry to keywords[] for tok_WHILE.  [1 LOC]

  == SYNTAX
parser.y :
  Added %token line for tok_WHILE.  [1 LOC]
  Added %type line for whileStmt.  [1 LOC]
  Added whileStmt alternative to stmt rule.  [1 LOC]
  Added rule defining syntax of whileStmt.  The action routine
    for this rule returns a node structure built by a call to
    makeWhile().  [3 LOC]

  == SEMANTIC
semantic.c :
  Forward declaration for _processWhile().  [1 LOC]
  Added case KIND_WHILE to switch statement in _processStmt()
    routine.  [1 LOC]
  Definition of _processWhile().  Checks semantics of the test
    expression and complains if type isn't INTEGER.  Checks
    sematics of the body (a BLOCK).  [10 LOC]

  == General processing
node.h :
  Added KIND_WHILE to the enumeration of node kinds.  [1 LOC]

  Added fields test and block to hold the parts of a WHILE to
    the Node struct definition.  [2 LOC]

  Added an extern declaration for the makeWhile() routine
    (which is defined in nodeStmts.c).  [1 LOC]

nodeDump.c :
  Added "WHILE" to kindToStr[].  [1 LOC]

  Added case KIND_WHILE block to switch statement in
    _dumpNodeInternal().  [6 LOC]

nodeStmts.c :
  Added makeWhile() routine.  [8 LOC]

#-----------------------------------------------------------------
* Hmm, what did that cost?  (1) + (1 + 1 + 1 + 3) + (1 + 1 + 10)
    + (1 + 2 + 1 + 1 + 6 + 8), or 1 + 6 + 12 + 19, or 38 Lines Of
    Code in all.  Not very much when you think about what it
    accomplishes.

* Using the WHILE as an example, add the REPEAT statement.  This
    statement requires two keywords, 'repeat' and 'until'.  It's
    otherwise very close to the WHILE statement.  (You can even
    use the same test / block fields in the Node structure to
    hold its parts.)

* Remember, for each added construct, you have to supply LEXICAL
    details (any keywords, punctuation, token definitions, token
    attribute definitions, etc.), SYNTACTIC details (parsing
    rules with their action routines returning node structures),
    and SEMANTIC details (semantic checks for the parts of the
    syntactic structure and how these are altered/modified for
    insertion into the AST).  There also may be some General
    details such as additions to various enumerations, structure
    definitions, builder/dumper routines, and so forth.

    It's simpler than it sounds as many of these details /
    additions / modifications are only a line or two of code,
    sometimes perhaps as little as a single character.

* Once you are generating an IR file for uses of REPEAT, check
    your output against the Exercises/repeat*.asl examples.  Some
    of these are POSITIVE tests (designed to pass all lexical,
    syntactic, and semantic checks) and others are NEGATIVE tests
    (designed to fail one way or another).

    Ensure that you detect the same errors for the NEGATIVE tests
    and generate the same IR as given in the Result/repeat*.ir
    files for the POSITIVE tests.

* With that done, now add the IF statement.  There are another
    two keywords that have to be added, 'if' and 'else'.  You
    can reuse the test field in the Node structure but you'll
    have to add a couple of fields to hold the then block and the
    else block.  Again, check against the results of the given
    tests both POSITIVE and NEGATIVE.

* After REPEAT and IF, the only statements left are BREAK and
    CONTINUE.  Each has only one keyword but the semantic
    processing for each is a little tricky.  Why?

* There are some examples of WHILE, REPEAT, IF, BREAK, and
    CONTINUE in Exercises/.  Some are POSITIVE examples in the
    sense that they do not generate any errors at all.
    Some are NEGATIVE examples in that they do generate errors,
    usually (static) semantic errors.

    Run them through your front end as you're adding items and
    ensure that you detect the same (static) semantic errors (for
    the NEGATIVE examples) and (for the POSITIVE examples)
    generate the same IR representations given in Results/.

* To run these examples, you'll also have to add INTEGER and
    REAL literals and some operators.  The literals are trivial
    to add, requiring only some minor updates to the lexer so it
    has rules to recognize the forms of integer and real literals
    and return tok_INT_LIT and tok_REAL_LIT.

    Adding operators is a little trickier but not much.  Use the
    existing processing of '+' and '/' as examples of how to do
    it.  Remember, there's lexical, syntacic, and semantic
    components for adding each operator.  (Don't forget updating
    the precedence / associativity table! :)

#-----------------------------------------------------------------