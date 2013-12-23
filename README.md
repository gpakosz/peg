# peg/leg &mdash; recursive-descent parser generators for C

`peg` and `leg` are tools for generating recursive-descent parsers: programs that perform pattern matching on
text.  They processes a Parsing Expression Grammar (PEG)[Ford 2004] to produce a program that recognises legal sentences of that grammar.

`peg` processes PEGs written using the original syntax described by Ford.

`leg` processes PEGs written using slightly different syntax and conventions that are intended to make it an attractive replacement for parsers built with `lex` and `yacc`.

Unlike `lex` and `yacc`, `peg` and `leg` support unlimited backtracking, provide ordered choice as a means for disambiguation, and can combine scanning (lexical analysis) and parsing (syntactic analysis) into a single activity.

`peg` is distributed under the MIT license.  It will not infect your project with a contagious <strike>license</strike> disease if you
decide to modify it for your own use.  The parser generators that `peg` creates are unencumbered and you are free to use and/or
distribute them any way you like.

`peg`/`leg` is copyright (c) 2007 by Ian Piumarta.

## References

* `peg`/`leg` manual page: [peg.1.html][1]

* [Ford 2004] Bryan Ford, [*Parsing Expression Grammars: A Recognition-Based Syntactic Foundation*][2]. ACM SIGPLAN Symposium on Principles of Programming Languages (POPL), 2004.

[1]: http://piumarta.com/software/peg/peg.1.html "peg/leg manual"
[2]: http://bford.info/pub/lang/peg "Parsing Expression Grammars: A Recognition-Based Syntactic Foundation"

## Version history

* **0.1.14** ([zip](../../archive/0.1.14.zip), [tar.gz](../../archive/0.1.14.tar.gz)) &mdash; 2013-12-01  
Documentation typos fixed (thanks to Giulio Paci).
* **0.1.13** ([zip](../../archive/0.1.13.zip), [tar.gz](../../archive/0.1.13.tar.gz)) &mdash; 2013-08-16  
Predicate actions can refer to `yytext` (thanks to Grégory Pakosz).
Hexadecimal character escapes are supported by `leg` (thanks to Hugo Etchegoyen).
* **0.1.12** ([zip](../../archive/0.1.12.zip), [tar.gz](../../archive/0.1.12.tar.gz)) &mdash; 2013-07-20  
Use BSD-licensed `getopt()` in Windows build.  
Verbose mode handles Variable nodes.  
* **0.1.11** ([zip](../../archive/0.1.11.zip), [tar.gz](../../archive/0.1.11.tar.gz)) &mdash; 2013-06-03  
Add error actions via `"~"` operator.  
Support declaration of local variables at the top level of semantic actions.  
Dynamically grow data structures to remove artificial limits on rule recursion (thanks to Alex Klinkhamer).  
Many small changes to better support C++.  
Add build files for Win32 and MacOS (thanks to Fyodor Sheremetyev).  
Update manual page to describe new features.  
* **0.1.10** &mdash; missing in upstream
* **0.1.9** ([zip](../../archive/0.1.9.zip), [tar.gz](../../archive/0.1.9.tar.gz)) &mdash; 2012-04-29  
Move global state into a structure to facilitate reentrant and thread-safe parsers (thanks to Dmitry Lipovoi).
* **0.1.8** ([zip](../../archive/0.1.8.zip), [tar.gz](../../archive/0.1.8.tar.gz)) &mdash; 2012-03-29  
Allow nested, matched braces within actions.
* **0.1.7** ([zip](../../archive/0.1.7.zip), [tar.gz](../../archive/0.1.7.tar.gz)) &mdash; 2011-11-25  
Fix matching of 8-bit chars to allow utf-8 sequences in matching expressions (thanks to Grégory Pakosz).
* **0.1.6** ([zip](../../archive/0.1.6.zip), [tar.gz](../../archive/0.1.6.tar.gz)) &mdash; 2011-11-24  
Allow octal escapes in character classes.
* **0.1.5** ([zip](../../archive/0.1.5.zip), [tar.gz](../../archive/0.1.5.tar.gz)) &mdash; 2011-11-24  
Remove dwarf sym dirs when cleaning.  
Fix size calculation when resizing text buffers.  
Backslash can be escaped.  
* **0.1.4** ([zip](../../archive/0.1.4.zip), [tar.gz](../../archive/0.1.4.tar.gz)) &mdash; 2009-08-26  
Fix match of a single single quote character.  
Rename `getline` -> `nextline` to avoid C namespace conflict.  
* **0.1.3** ([zip](../../archive/0.1.3.zip), [tar.gz](../../archive/0.1.3.tar.gz)) &mdash; 2007-09-13  
Allow matched braces inside `leg` actions.  
Handle empty rules.  
Handle empty grammars.  
* **0.1.2** ([zip](../../archive/0.1.2.zip), [tar.gz](../../archive/0.1.2.tar.gz)) &mdash; 2007-08-31  
Grow buffers while (not if) they are too small.  
Remove dependencies on grammar files.  
Add more basic examples.  
* **0.1.1** ([zip](../../archive/0.1.1.zip), [tar.gz](../../archive/0.1.1.tar.gz)) &mdash; 2007-05-15  
First public release.
