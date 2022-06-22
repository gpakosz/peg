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
 * Last edited: 2016-07-15 10:25:14 by piumarta on zora
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef WIN32
# undef inline
# define inline __inline
#endif

#include "tree.h"

Node *actions= 0;
Node *rules= 0;
Node *thisRule= 0;
Node *start= 0;

FILE *output= 0;

int actionCount= 0;
int ruleCount= 0;
int lastToken= -1;

static inline Node *_newNode(int type, int size)
{
  Node *node= calloc(1, size);
  node->type= type;
  return node;
}

#define newNode(T)	_newNode(T, sizeof(struct T))

Node *makeRule(char *name)
{
  Node *node= newNode(Rule);
  node->rule.name= strdup(name);
  node->rule.id= ++ruleCount;
  node->rule.flags= 0;
  node->rule.next= rules;
  rules= node;
  return node;
}

Node *findRule(char *name)
{
  Node *n;
  char *ptr;
  for (ptr= name;  *ptr;  ptr++) if ('-' == *ptr) *ptr= '_';
  for (n= rules;  n;  n= n->any.next)
    {
      assert(Rule == n->type);
      if (!strcmp(name, n->rule.name))
	return n;
    }
  return makeRule(name);
}

Node *beginRule(Node *rule)
{
  actionCount= 0;
  return thisRule= rule;
}

void Rule_setExpression(Node *node, Node *expression)
{
  assert(node);
#ifdef DEBUG
  Node_print(node);  fprintf(stderr, " [%d]<- ", node->type);  Node_print(expression);  fprintf(stderr, "\n");
#endif
  assert(Rule == node->type);
  node->rule.expression= expression;
  if (!start || !strcmp(node->rule.name, "start"))
    start= node;
}

Node *makeVariable(char *name)
{
  Node *node;
  assert(thisRule);
  for (node= thisRule->rule.variables;  node;  node= node->variable.next)
    if (!strcmp(name, node->variable.name))
      return node;
  node= newNode(Variable);
  node->variable.name= strdup(name);
  node->variable.next= thisRule->rule.variables;
  thisRule->rule.variables= node;
  return node;
}

Node *makeName(Node *rule)
{
  Node *node= newNode(Name);
  node->name.rule= rule;
  node->name.variable= 0;
  rule->rule.flags |= RuleUsed;
  return node;
}

Node *makeDot(void)
{
  return newNode(Dot);
}

Node *makeCharacter(char *text)
{
  Node *node= newNode(Character);
  node->character.value= strdup(text);
  node->character.caseInsensitive= 0;
  node->character.quote = 0;
  return node;
}

Node *makeString(char *text, int quote)
{
  Node *node= newNode(String);
  node->string.value= strdup(text);
  node->string.caseInsensitive= 0;
  node->string.quote= quote;
  return node;
}

void setTopStrCharCaseInsensitive()
{
  Node *node= top();
  switch(node->type) {
      case String: node->string.caseInsensitive= 1; break;
      case Character: node->character.caseInsensitive= 1; break;
      default:
      fprintf(stderr, "\ncan not set CaseInsensitive on node type %d\n", node->type);
      exit(1);
  }
}

void setTopClassCharCaseInsensitive()
{
  Node *node= top();
  switch(node->type) {
      case Class: node->cclass.caseInsensitive= 1; break;
      default:
      fprintf(stderr, "\ncan not set CaseInsensitive on node type %d\n", node->type);
      exit(1);
  }
}

Node *makeClass(char *text)
{
  Node *node= newNode(Class);
  node->cclass.value= (unsigned char *)strdup(text);
  return node;
}

Node *makeAction(int lineNumber, char *text)
{
  Node *node= newNode(Action);
  char name[1024];
  assert(thisRule);
  sprintf(name, "_%d_%s", ++actionCount, thisRule->rule.name);
  node->action.name= strdup(name);
  node->action.text= strdup(text);
  node->action.list= actions;
  node->action.rule= thisRule;
  node->action.line= lineNumber;
  actions= node;
  {
    char *ptr;
    for (ptr= node->action.text;  *ptr;  ++ptr)
      if ('$' == ptr[0] && '$' == ptr[1])
	ptr[1]= ptr[0]= '_';
  }
  return node;
}

Node *makeInline(char *text)
{
  Node *node= newNode(Inline);
  node->inLine.text= strdup(text);
  return node;
}

Node *makePredicate(char *text)
{
  Node *node= newNode(Predicate);
  node->predicate.text= strdup(text);
  return node;
}

Node *makeError(Node *e, char *text)
{
  Node *node= newNode(Error);
  node->error.element= e;
  node->error.text= strdup(text);
  return node;
}

Node *makeAlternate(Node *e)
{
  if (Alternate != e->type)
    {
      Node *node= newNode(Alternate);
      assert(e);
      assert(!e->any.next);
      node->alternate.first=
	node->alternate.last= e;
      return node;
    }
  return e;
}

Node *Alternate_append(Node *a, Node *e)
{
  assert(a);
  a= makeAlternate(a);
  assert(a->alternate.last);
  assert(e);
  a->alternate.last->any.next= e;
  a->alternate.last= e;
  return a;
}

Node *makeSequence(Node *e)
{
  if (Sequence != e->type)
    {
      Node *node= newNode(Sequence);
      assert(e);
      assert(!e->any.next);
      node->sequence.first=
	node->sequence.last= e;
      return node;
    }
  return e;
}

Node *Sequence_append(Node *a, Node *e)
{
  assert(a);
  a= makeSequence(a);
  assert(a->sequence.last);
  assert(e);
  a->sequence.last->any.next= e;
  a->sequence.last= e;
  return a;
}

Node *makePeekFor(Node *e)
{
  Node *node= newNode(PeekFor);
  node->peekFor.element= e;
  return node;
}

Node *makePeekNot(Node *e)
{
  Node *node= newNode(PeekNot);
  node->peekNot.element= e;
  return node;
}

Node *makeQuery(Node *e)
{
  Node *node= newNode(Query);
  node->query.element= e;
  return node;
}

Node *makeStar(Node *e)
{
  Node *node= newNode(Star);
  node->star.element= e;
  return node;
}

Node *makePlus(Node *e)
{
  Node *node= newNode(Plus);
  node->plus.element= e;
  return node;
}

#define STACK_SIZE 1024
static Node  *stack[STACK_SIZE];
static Node **stackPointer= stack;


#ifdef DEBUG
static void dumpStack(void)
{
  Node **p;
  for (p= stack + 1;  p <= stackPointer;  ++p)
    {
      fprintf(stderr, "### %d\t", p - stack);
      Node_print(*p);
      fprintf(stderr, "\n");
    }
}
#endif

Node *push(Node *node)
{
  assert(node);
  assert(stackPointer < stack + (STACK_SIZE-1));
#ifdef DEBUG
  dumpStack();  fprintf(stderr, " PUSH ");  Node_print(node);  fprintf(stderr, "\n");
#endif
  return *++stackPointer= node;
}

Node *top(void)
{
  assert(stackPointer > stack);
  return *stackPointer;
}

Node *pop(void)
{
  assert(stackPointer > stack);
#ifdef DEBUG
  dumpStack();  fprintf(stderr, " POP\n");
#endif
  return *stackPointer--;
}


static void Node_fprint(FILE *stream, Node *node, int depth, int asEbnf, int asLeg, int naked)
{
  assert(node);
  switch (node->type)
    {
    case Rule:		fprintf(stream, " %s", node->rule.name);				break;
    case Variable:	fprintf(stream, " %s:", node->variable.name);				break;
    case Name:		if(node->name.variable && !naked)
                            Node_fprint(stream, node->name.variable, depth+1, asEbnf, asLeg, naked);
                        fprintf(stream, " %s", node->name.rule->rule.name);
                        break;
    case Dot:		fprintf(stream, " .");							break;
    case Character:	fprintf(stream, " '%s'", node->character.value);			break;
    case String:	fprintf(stream, " %c%s%c%s", node->string.quote,
                            node->string.value, node->string.quote,
                            node->string.caseInsensitive && !asEbnf ? "i" : "");		break;
    case Class:		fprintf(stream, " [%s]%s", node->cclass.value,
                            node->cclass.caseInsensitive && !asEbnf ? "i" : "");				break;
    case Action:	if(!naked) fprintf(stream, " { %s }", node->action.text);		break;
    case Predicate:	if(!naked) {
                            if(strcmp(node->predicate.text, "YY_BEGIN") == 0)
                                fprintf(stream, " <");
                            else if(strcmp(node->predicate.text, "YY_END") == 0)
                                fprintf(stream, " >");
                            else
                                fprintf(stream, " &{ %s }", node->predicate.text);
                        }
                        break;
    case Error: 	if(node->error.element)
                          Node_fprint(stream, node->error.element, depth+1, asEbnf, asLeg, naked);
                        if(!naked)
                          fprintf(stream, " ~{ %s }", node->error.text);
                        break;
    case Inline: 	if(!naked) fprintf(stream, " @{ %s }", node->inLine.text);		break;

    case Alternate:	node= node->alternate.first;
			if(depth > 0) fprintf(stream, " (");
			Node_fprint(stream, node, depth+1, asEbnf, asLeg, naked);
			while ((node= node->any.next))
			  {
			    fprintf(stream, asLeg ? "%s|" : "%s/", (depth == 0) ? "\n\t" : " ");
			    Node_fprint(stream, node, depth+1, asEbnf, asLeg, naked);
			  }
			if(depth > 0) fprintf(stream, " )");
			break;

    case Sequence:	node= node->sequence.first;
			if(depth > 0) fprintf(stream, " (");
			Node_fprint(stream, node, depth+1, asEbnf, asLeg, naked);
			while ((node= node->any.next))
			  Node_fprint(stream, node, depth+1, asEbnf, asLeg, naked);
			if(depth > 0) fprintf(stream, " )");
			break;

    case PeekFor:	fprintf(stream, asEbnf ? " _AND_ " : "&");  Node_fprint(stream, node->query.element, depth+1, asEbnf, asLeg, naked);	break;
    case PeekNot:	fprintf(stream, asEbnf ? " _NOT_ " : " !");  Node_fprint(stream, node->query.element, depth+1, asEbnf, asLeg, naked);	break;
    case Query:		Node_fprint(stream, node->query.element, depth+1, asEbnf, asLeg, naked);  fprintf(stream, "?");	break;
    case Star:		Node_fprint(stream, node->query.element, depth+1, asEbnf, asLeg, naked);  fprintf(stream, "*");	break;
    case Plus:		Node_fprint(stream, node->query.element, depth+1, asEbnf, asLeg, naked);  fprintf(stream, "+");	break;
    default:
      fprintf(stream, "\nunknown node type %d\n", node->type);
      exit(1);
    }
}

void Node_print(Node *node)	{ Node_fprint(stderr, node, 0, 0, 1, 0); }

static void Rule_fprint(FILE *stream, Node *node)
{
  assert(node);
  assert(Rule == node->type);
  fprintf(stream, "%s.%d =", node->rule.name, node->rule.id);
  if (node->rule.expression)
    Node_fprint(stream, node->rule.expression, 0, 0, 1, 0);
  else
    fprintf(stream, " UNDEFINED");
  fprintf(stream, " ;\n");
}

void Rule_print(Node *node)	{ Rule_fprint(stderr, node); }

static void EBNF_fprint(FILE *stream, Node *node)
{
  assert(node);
  assert(Rule == node->type);
  fprintf(stream, "%s ::=\n\t", node->rule.name);
  if (node->rule.expression)
    Node_fprint(stream, node->rule.expression, 0, 1, 1, 1);
  else
    fprintf(stream, " UNDEFINED");
  fprintf(stream, "\n\n");
}

static Node **getOrderedRules()
{
  int i;
  Node *n;
  Node **oderedRules = calloc(1, sizeof(Node*)*ruleCount);
  for (i=0, n= rules;  n;  n= n->any.next, ++i)
    oderedRules[i] = n;
  return oderedRules;
}

void EBNF_print() {
  int i;
  Node **oderedRules = getOrderedRules();
  fprintf(stdout, "\n//To be viewd at https://www.bottlecaps.de/rr/ui\n\n");
  for(i=ruleCount-1; i >= 0; --i)
    EBNF_fprint(stdout, oderedRules[i]);
  free(oderedRules);
  fprintf(stdout, "\n//Added tokens for railroad generation\n_NOT_ ::= '!'\n_AND_ ::= '&'\n\n");
}

typedef enum {asLeg, asPeg, asPegjs} eLegPegJs;

static void RuleLegPegJs_fprint(FILE *stream, Node *node, eLegPegJs outType, int naked)
{
  assert(node);
  assert(Rule == node->type);
  fprintf(stream, "%s %s", node->rule.name,
          (outType == asLeg) || (outType == asPegjs) ? "=\n\t" : "<-\n\t");
  if (node->rule.expression)
    Node_fprint(stream, node->rule.expression, 0, 0,
            (outType == asPegjs) ? 0 : (outType == asLeg), naked);
  else
    fprintf(stream, " UNDEFINED");
  if(asLeg) fprintf(stream, "\n\t;\n\n");
  else fprintf(stream, "\n\n");
}

void LEG_print(int naked) {
    int i;
    Node **oderedRules = getOrderedRules();
    for(i=ruleCount-1; i >= 0; --i)
        RuleLegPegJs_fprint(stdout, oderedRules[i], asLeg, naked);
    free(oderedRules);
}

void PEG_print(int naked) {
    int i;
    Node **oderedRules = getOrderedRules();
    for(i=ruleCount-1; i >= 0; --i)
        RuleLegPegJs_fprint(stdout, oderedRules[i], asPeg, naked);
    free(oderedRules);
}

void PEGJS_print(int naked) {
    int i;
    Node **oderedRules = getOrderedRules();
    for(i=ruleCount-1; i >= 0; --i)
        RuleLegPegJs_fprint(stdout, oderedRules[i], asPegjs, naked);
    free(oderedRules);
}

Header *makeHeader(Header *head, int line, char *text)
{
  Header *header= (Header *)malloc(sizeof(Header));
  header->line= line;
  header->text= strdup(text);
  header->next= head;
  return header;
}

Trailer * makeTrailer(int line, char *text)
{
  Trailer *trailer = (Trailer *)malloc(sizeof(Trailer)+strlen(text)+1);
  trailer->trailerLine= line;
  strcpy(trailer->trailer, text);
  return trailer;
}
