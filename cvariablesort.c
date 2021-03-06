//a program that lexicographically sorts all the variables in a c program 
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "pgetline.c"
#define MAXWORD 100 //maximum length of a word
#define MAXLINE 1000
#define sizeofvt 4 //number of valid types
#define BREAK -1
#define CONTINUE -2

int pgetline(char *, int);
struct tnode *processline(char *, struct tnode *);

//functions used in processline
int istype(char *);
int ignoreend(char *);
int specialchar(char *line);
int getnextword(char *line);
int getnextvar(char *line);

//binary tree functions implemented in K&R
void treeprint(struct tnode *);
struct tnode *addtree(struct tnode *, char *);
struct tnode *talloc(void);

char *validtypes[sizeofvt] = {"char", "int", "float", "double"}; //basic data types in C

struct tnode {
  char *word;
  int count;
  struct tnode *left;
  struct tnode *right;
};

main()
{
  char line[MAXWORD];
  struct tnode *root;

  root = NULL;
  while(pgetline(line, MAXLINE) > 0) {//while not EOF
    root = processline(line, root); //process line, keep track of binary tree
  }
  treeprint(root);
  return 0;
}

int ignore = 0; //tells processline to ignore characters that are part of comments
//it is global because comments can span multiple lines,
//thus ignore has to retain its value across multiple calls of processline

//the following variables are made global so getnextword and getnextvar can use them
char word[MAXWORD];
char var[MAXWORD];
char *pword = word;
char *pvar = var;

//process a line at a time
//this is the heart of the program
//it takes a line and adds all declared variables to a binary tree
//comments and strings are ignored
struct tnode *processline(char *line, struct tnode *root)
{
  while(*line != '\0' && *line != '\n') {
    while(isspace(*line)) //skip whitespace
      line++;

    if(ignore)
      line += ignoreend(line); //process if ignore should be switched to 0

    else {
      int c = specialchar(line); //see if line has any special characters
      if(c == BREAK)
	break;
      else if(c == CONTINUE) {
	line++;
	continue;
      }

      if(isalnum(*line)) {
	line += getnextword(line);
	if(istype(word)) {
	  line += getnextvar(line);
	  if(*line == '(') { //if var is actually a function
	    line++;
	    while(*line != ')' && *line != '\0')
	      line++;
	  }
	  else //var is a variable not a function
	    if(var[0] != '\0')
	      root = addtree(root, var); //add variable to binary tree
	}
      }
      else //if character is not alphanumeric
	line++;
    }
  }
  return root;
}

//test if word is in validtypes i.e. if word is a type
int istype(char *word)
{
  int i;
  for(i = 0; i < sizeofvt; i++)
    if(strcmp(word, validtypes[i]) == 0)
      return 1;
  return 0;
}

//modified treeprint, does not print out count
void treeprint(struct tnode *p)
{
  if(p != NULL) {
    treeprint(p->left);
    printf("%s\n", p->word);
    treeprint(p->right);
  }
}

struct tnode *addtree(struct tnode *p, char *w)
{
  int cond;

  if(p == NULL) {
    p = talloc();
    p->word = strdup(w);
    p->count = 1;
    p->left = p->right = NULL;
  } else if((cond = strcmp(w, p->word)) == 0)
    p->count++;
  else if(cond < 0)
    p->left = addtree(p->left, w);
  else
    p->right = addtree(p->right, w);
  return p;
}

struct tnode *talloc(void)
{
  return (struct tnode *) malloc(sizeof(struct tnode));
}

int ignoreend(char *line)
{
  if(*line == '*' && *(line+1) == '/') {
    ignore = 0;
    return 2;
  }
  else if(*line == '\"') {
    ignore = 0;
    return 1;
  }
  else
    return 1;
}

int specialchar(char *line) //process any special characters
{
  switch(*line) {
  case '#':
    return BREAK;
  case '\"':
    ignore = 1;
    return CONTINUE;
  case '/':
    if(*(line + 1) == '/')
      return BREAK;
    else if(*(line + 1) == '*') {
      ignore = 1;
      return CONTINUE;
    }
    break;
  default:
    break;
  }
  return 1;
}

int getnextword(char *line) //get next word in line
{
  pword = word; //reset pword to point at beginning of word array
  while(*line != '\0' && *line != '\n' && !isspace(*line)) { //get the next word in line
    *pword = *line;
    pword++;
    line++;
  }
  *pword = '\0'; //pword is a string, so must add '\0'
  return (pword - word);
}

int getnextvar(char *line) //get next variable in line
{
  int gap = 0;
  while(*line != '\0' && *line != '\n' && !isalnum(*line)) //skip to when the variable's name starts
    line++, gap++;
  pvar = var; //reset pvar to point at beginning of var array
  while(isalnum(*line)) { //copy variable to var
    *pvar = *line;
    pvar++;
    line++;
    gap++;
  }
  *pvar = '\0'; //pvar is a string, so must add '\0'
  return gap;
}
