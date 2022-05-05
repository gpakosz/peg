/* Copyright (c) 2007 by Ian Piumarta
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the 'Software'),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, provided that the above copyright notice(s) and this
 * permission notice appear in all copies of the Software.  Acknowledgement
 * of the use of this Software in supporting documentation would be
 * appreciated but is not required.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS'.  USE ENTIRELY AT YOUR OWN RISK.
 *
 * Last edited: 2016-07-22 09:42:48 by piumarta on zora.local
 */

#include <stdio.h>

typedef enum { Unknown= 0, Rule, Variable, Name, Dot, Character, String, Class, Action, Inline, Predicate, Error, Alternate, Sequence, PeekFor, PeekNot, Query, Star, Plus } NodeType;

enum {
  RuleUsed	= 1<<0,
  RuleReached	= 1<<1,
};

typedef union Node Node;

struct Rule	 { NodeType type;  Node *next;   char *name;	 Node *variables;  Node *expression;  int id;  int flags;	};
struct Variable	 { NodeType type;  Node *next;   char *name;  Node *value;  int offset;					};
struct Name	 { NodeType type;  Node *next;   Node *rule;  Node *variable;						};
struct Dot	 { NodeType type;  Node *next;										};
struct Character { NodeType type;  Node *next;   char *value; char caseInsensitive; char quote;         		};
struct String	 { NodeType type;  Node *next;   char *value;  char caseInsensitive; char quote;			};
struct Class	 { NodeType type;  Node *next;   unsigned char *value;							};
struct Action	 { NodeType type;  Node *next;   char *text;	  Node *list;  char *name;  Node *rule;  int line;		};
struct Inline    { NodeType type;  Node *next;   char *text;									};
struct Predicate { NodeType type;  Node *next;   char *text;									};
struct Error	 { NodeType type;  Node *next;   Node *element;  char *text;							};
struct Alternate { NodeType type;  Node *next;   Node *first;  Node *last;							};
struct Sequence	 { NodeType type;  Node *next;   Node *first;  Node *last;							};
struct PeekFor	 { NodeType type;  Node *next;   Node *element;								};
struct PeekNot	 { NodeType type;  Node *next;   Node *element;								};
struct Query	 { NodeType type;  Node *next;   Node *element;								};
struct Star	 { NodeType type;  Node *next;   Node *element;								};
struct Plus	 { NodeType type;  Node *next;   Node *element;								};
struct Any	 { NodeType type;  Node *next;										};

union Node
{
  NodeType		type;
  struct Rule		rule;
  struct Variable	variable;
  struct Name		name;
  struct Dot		dot;
  struct Character	character;
  struct String		string;
  struct Class		cclass;
  struct Action		action;
  struct Inline		inLine;
  struct Predicate	predicate;
  struct Error		error;
  struct Alternate	alternate;
  struct Sequence	sequence;
  struct PeekFor	peekFor;
  struct PeekNot	peekNot;
  struct Query		query;
  struct Star		star;
  struct Plus		plus;
  struct Any		any;
};

extern Node *actions;
extern Node *rules;
extern Node *start;

extern int   ruleCount;

extern FILE *output;

extern Node *makeRule(char *name);
extern Node *findRule(char *name);
extern Node *beginRule(Node *rule);
extern void  Rule_setExpression(Node *rule, Node *expression);
extern Node *Rule_beToken(Node *rule);
extern Node *makeVariable(char *name);
extern Node *makeName(Node *rule);
extern Node *makeDot(void);
extern Node *makeCharacter(char *text);
extern Node *makeString(char *text, int quote);
extern void  setTopStrCharCaseInsensitive();
extern Node *makeClass(char *text);
extern Node *makeAction(int lineNumber, char *text);
extern Node *makeInline(char *text);
extern Node *makePredicate(char *text);
extern Node *makeError(Node *e, char *text);
extern Node *makeAlternate(Node *e);
extern Node *Alternate_append(Node *e, Node *f);
extern Node *makeSequence(Node *e);
extern Node *Sequence_append(Node *e, Node *f);
extern Node *makePeekFor(Node *e);
extern Node *makePeekNot(Node *e);
extern Node *makeQuery(Node *e);
extern Node *makeStar(Node *e);
extern Node *makePlus(Node *e);
extern Node *push(Node *node);
extern Node *top(void);
extern Node *pop(void);

extern void  Rule_compile_c_header(void);
extern void  Rule_compile_c(Node *node, int nolines);

extern void  Node_print(Node *node);
extern void  Rule_print(Node *node);
extern void  EBNF_print();
extern void  LEG_print(int naked);
extern void  PEG_print(int naked);
extern void  PEGJS_print(int naked);
