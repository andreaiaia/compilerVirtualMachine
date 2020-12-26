#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gestionefile.h"

int a_to_i(char[]);
void to_bin(int, int[]);
void clear(char[]);
void bootstrap(FILE*);
void identificacmd(char[], char[]);
void exec_cmd(char[], char[], FILE *, int *, char[]);
void identificalabel(char[], char[]);
void exec_seg_push(char[], char[], FILE *, char[]);
void exec_seg_pop(char[], char[], FILE *, char[]);
void estrainum(char[], char[]);