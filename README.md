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

* **0.1.7** ([zip](peg/zipball/0.1.7), [tar.gz](peg/tarball/0.1.7)) &mdash; 2011-11-25  
Fix matching of 8-bit chars to allow utf-8 sequences in matching expressions (thanks to Grégory Pakosz).
* **0.1.6** ([zip](peg/zipball/0.1.6), [tar.gz](peg/tarball/0.1.6)) &mdash; 2011-11-24  
Allow octal escapes in character classes.
* **0.1.5** ([zip](peg/zipball/0.1.5), [tar.gz](peg/tarball/0.1.5)) &mdash; 2011-11-24  
Remove dwarf sym dirs when cleaning.  
Fix size calculation when resizing text buffers.  
Backslash can be escaped.  
* **0.1.4** ([zip](peg/zipball/0.1.4), [tar.gz](peg/tarball/0.1.4)) &mdash; 2009-08-26  
Fix match of a single single quote character.  
Rename `getline` -> `nextline` to avoid C namespace conflict.  
* **0.1.3** ([zip](peg/zipball/0.1.3), [tar.gz](peg/tarball/0.1.3)) &mdash; 2007-09-13  
Allow matched braces inside `leg` actions.  
Handle empty rules.  
Handle empty grammars.  
* **0.1.2** ([zip](peg/zipball/0.1.2), [tar.gz](peg/tarball/0.1.2)) &mdash; 2007-08-31  
Grow buffers while (not if) they are too small.  
Remove dependencies on grammar files.  
Add more basic examples.  
* **0.1.1** ([zip](peg/zipball/0.1.1), [tar.gz](peg/tarball/0.1.1)) &mdash; 2007-05-15  
First public release.
