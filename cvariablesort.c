//a program that lexicographically sorts all the variables in a c program 
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "pgetline.c"
#include "addtree.c"
#define MAXWORD 100 //maximum length of a word
#define MAXLINE 1000
#define sizeofvt 4 //number of valid types

//int getword(char *, int);
int pgetline(char *, int);
struct tnode *processline(char *, struct tnode *);
int istype(char *); //return 1 if char * is a variable type
char *validtypes[sizeofvt] = {"char", "int", "float", "double"}; //basic data types in C

main()
{
  char line[MAXWORD];
  struct tnode *root;

  root = NULL;
  while(pgetline(line, MAXLINE) > 0) //while not EOF
    root = processline(line, root); //process line, keep track of binary tree
  treeprint(root);
  return 0;
}

int ignore = 0; //tells processline to ignore characters that are part of comments
//it is global because comments can span multiple lines,
//thus ignore has to retain its value across multiple calls of processline

//process a line at a time
//this is the heart of the program
//it takes a line and adds all declared variables to a binary tree
//comments and strings are ignored
struct tnode *processline(char *line, struct tnode *root)
{
  char word[MAXWORD];
  char var[MAXWORD];
  char *pword = word;
  char *pvar = var;

  while(*line != '\0' && *line != '\n') {
    while(isspace(*line)) //skip whitespace
      line++;

    if(ignore == 1 && *line == '*' && *(line + 1) == '/') { //end of comment
      ignore = 0;
      line += 2;
    }
    if(ignore == 1 && *line == '\"') { //end of string
      ignore = 0;
      line++;
    }

    if(ignore) //ignore character
      line++;
    else {
      if(*line == '\"') {//string
	ignore = 1;
	line++;
	continue;
      }
      if(*line == '/') { //comment
	if(*(line + 1) == '/') 
	  break;
	else if(*(line + 1) == '*') {
	  ignore = 1;
	  line++;
	  continue;
	}
      }

      if(isalnum(*line)) { //line is alphanumeric
	pword = word; //reset pword to point at beginning of word array
	while(*line != '\0' && *line != '\n' && !isspace(*line)) { //get the next word in line
	  *pword = *line;
	  pword++;
	  line++;
	} 
	*pword = '\0'; //pword is a string, so must add '\0'
	if(istype(word)) { //if word is a type, then there must be a variable declaration
	  //word is used instead of pword, because pword points to '\0' while word is the entire word
	  while(!isalnum(*line)) //skip to when the variable's name starts
	    line++;
	  pvar = var; //reset pvar to point at beginning of var array
	  while(isalnum(*line)) { //copy variable to var
	    *pvar = *line;
	    pvar++;
	    line++;
	  }
	  *pvar = '\0'; //pvar is a string, so must add '\0'
	  root = addtree(root, var); //add variable to binary tree
	  //var is used instead of pvar because pvar points to '\0' while var is the entire word
	}
      }
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
