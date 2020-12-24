#include "operazioni.h"

// OPERAZIONI GENERICHE
// Funzione che legge una stringa e restituisce un intero
int a_to_i(char riga[]) {
  int num = 0;
  for(int i = 1; riga[i] >= '0' && riga[i] <= '9'; i++)
    num = num*10 + riga[i] - '0';

  return num;
}

void to_bin(int i, int out[]) {
  for (int k = 14; k >= 0; k--)    // Converte ogni decimale in binario con il modulo della divisione per 2
  {
    out[k] = i % 2;
    i /= 2;
  }
}

// Rimuove tutte le tabulazioni, i commenti e i tab dalla riga ca codificare
void clear(char riga[]) {
  int i = 0;
  int j = 0;
  int stop = 0;
  while(riga[i] == ' ' || riga[i] == '\t') i++;
  char pulita[128];
  while((riga[i] != '\0') && (riga[i] != '/')) {  // Ciclo la riga finché non trovo un commento o la riga finisce    // Se il carattere "puntato" non è uno spazio o un tab
    pulita[j] = riga[i];                        // lo copio nella riga ripulita
    j++;
    i++;
  }
  pulita[j] = '\0';            // Aggiungo il terminatore
  strcpy(riga, pulita);        // Copio il risultato nella riga che poi codificherò
}

void bootstrap(FILE * output) {
  fprintf(output, "//bootstrap");
  fprintf(output, "@256");
  fprintf(output, "D=A");
  fprintf(output, "@SP");
  fprintf(output, "M=D");
}

void identificacmd(char riga[], char cmd[]) {
  int i = 0, j = 0;
  int stop = 0;
  while (riga[i] != '\0' && !stop) {
    if ((riga[i] >= 'A' && riga[i] <= 'Z') || (riga[i] >= 'a' && riga[i] <= 'z')) {
      cmd[j] = riga[i];
      j++;
      if (riga[i + 1] == ' ')
        stop = 1;
    }
    i++;
  }
  cmd[j] = '\0';
}

void identificaseg(char riga[], char seg[]) {
  int i = 0, j = 0;
  while (riga[i] != ' ') i++;
  i++;
  while(riga[i] != ' ' && riga[i] != '\0') {
    seg[j] = riga[i];
    j++;
    i++;
  }
  seg[j] = '\0';
}

void estrainum(char riga[], char num[])
{
  num[0] = '@';
  int i = 0, j = 1;
  while (riga[i] != ' ')
    i++;
  i++;
  while (riga[i] != ' ')
    i++;
  i++;
  while (riga[i] != ' ' && riga[i] != '\0' && riga[i] != '\n' && riga[i] != '\r')
  {
    num[j] = riga[i];
    j++;
    i++;
  }
  num[j] = '\0';
}

void exec_cmd(char riga[], char cmd[], FILE * output, int * conditioncounter) {
  if (!strcmp(cmd, "push"))
  {
    char segmento[strlen(riga)];
    identificaseg(riga, segmento);
    exec_seg(riga, segmento, output);
  }
  else if (!strcmp(cmd, "pop")) 
  {
    printf("è un pop\n");
  }
  else if (!strcmp(cmd, "add"))
  {
    fprintf(output, "@SP\n");
    fprintf(output, "M=M-1\n");
    fprintf(output, "A=M\n");
    fprintf(output, "D=M\n");
    fprintf(output, "@SP\n");
    fprintf(output, "M=M-1\n");
    fprintf(output, "A=M\n");
    fprintf(output, "M=M+D\n");
    fprintf(output, "@SP\n");
    fprintf(output, "M=M+1\n");
  }
  else if (!strcmp(cmd, "sub"))
  {
    fprintf(output, "@SP\n");
    fprintf(output, "M=M-1\n");
    fprintf(output, "A=M\n");
    fprintf(output, "D=M\n");
    fprintf(output, "@SP\n");
    fprintf(output, "M=M-1\n");
    fprintf(output, "A=M\n");
    fprintf(output, "M=M-D\n");
    fprintf(output, "@SP\n");
    fprintf(output, "M=M+1\n");
  }
  else if (!strcmp(cmd, "neg"))
  {
    fprintf(output, "@SP\n");
    fprintf(output, "M=M-1\n");
    fprintf(output, "A=M\n");
    fprintf(output, "M=-M\n");
    fprintf(output, "@SP\n");
    fprintf(output, "M=M+1\n");
  }
  else if (!strcmp(cmd, "eq") || !strcmp(cmd, "gt") || !strcmp(cmd, "lt"))
  {
    fprintf(output, "@SP\n");
    fprintf(output, "M=M-1\n");
    fprintf(output, "A=M\n");
    fprintf(output, "D=M\n");
    fprintf(output, "@SP\n");
    fprintf(output, "M=M-1\n");
    fprintf(output, "A=M\n");
    fprintf(output, "M=M-D\n");
    fprintf(output, "D=M\n");
    fprintf(output, "@IF_TRUE%d\n", *conditioncounter);

    if (!strcmp(cmd, "eq"))
      fprintf(output, "D;JEQ\n");
    else if (!strcmp(cmd, "gt"))
      fprintf(output, "D;JGT\n");
    else if (!strcmp(cmd, "lt"))
      fprintf(output, "D;JLT\n");

    fprintf(output, "@SP\n");
    fprintf(output, "A=M\n");
    fprintf(output, "M=0\n");
    fprintf(output, "@END%d\n", *conditioncounter);
    fprintf(output, "0;JMP\n");
    fprintf(output, "(IF_TRUE%d)\n", *conditioncounter);
    fprintf(output, "@SP\n");
    fprintf(output, "A=M\n");
    fprintf(output, "M=-1\n");
    fprintf(output, "(END%d)\n", *conditioncounter);
    fprintf(output, "@SP\n");
    fprintf(output, "M=M+1\n");
    *conditioncounter = *conditioncounter + 1;
  }
  else if (!strcmp(cmd, "and"))
  {
    fprintf(output, "@SP\n");
    fprintf(output, "M=M-1\n");
    fprintf(output, "A=M\n");
    fprintf(output, "D=M\n");
    fprintf(output, "@SP\n");
    fprintf(output, "M=M-1\n");
    fprintf(output, "A=M\n");
    fprintf(output, "M=M&D\n");
    fprintf(output, "@SP\n");
    fprintf(output, "M=M+1\n");
  }
  else if (!strcmp(cmd, "or"))
  {
    fprintf(output, "@SP\n");
    fprintf(output, "M=M-1\n");
    fprintf(output, "A=M\n");
    fprintf(output, "D=M\n");
    fprintf(output, "@SP\n");
    fprintf(output, "M=M-1\n");
    fprintf(output, "A=M\n");
    fprintf(output, "M=M|D\n");
    fprintf(output, "@SP\n");
    fprintf(output, "M=M+1\n");
  }
  else if (!strcmp(cmd, "not"))
  {
    fprintf(output, "@SP\n");
    fprintf(output, "M=M-1\n");
    fprintf(output, "A=M\n");
    fprintf(output, "M=!M\n");
    fprintf(output, "@SP\n");
    fprintf(output, "M=M+1\n");
  }/*
  else if (!strcmp(cmd, "label"))
  {
    printf("è un label\n");
  }
  else if (!strcmp(cmd, "goto"))
  {
    printf("è un goto\n");
  }
  else if (!strcmp(cmd, "ifgoto"))
  {
    printf("è un if-goto\n");
  }
  else if (!strcmp(cmd, "function"))
  {
    printf("è un function\n");
  }
  else if (!strcmp(cmd, "call"))
  {
    printf("è un call\n");
  }
  else if (!strcmp(cmd, "return"))
  {
    printf("è un return\n");
  }*/
}

void exec_seg(char riga[], char seg[], FILE * output) {
  char num[strlen(riga)];
  estrainum(riga, num);
  fprintf(output, "%s\n", num);
  fprintf(output, "D=A\n");
  if (!strcmp(seg, "constant")) 
  {
    fprintf(output, "@SP\n");
    fprintf(output, "A=M\n");
    fprintf(output, "M=D\n");
    fprintf(output, "@SP\n");

  }
  else if (!strcmp(seg, "local")) 
  {
    fprintf(output, "@LCL\n");
    fprintf(output, "A=M\n");
    fprintf(output, "M=D\n");
    fprintf(output, "@LCL\n");
  }
  else if (!strcmp(seg, "static")) 
  {

  }
  else if (!strcmp(seg, "argument")) 
  {
    fprintf(output, "@ARG\n");
    fprintf(output, "A=M\n");
    fprintf(output, "M=D\n");
    fprintf(output, "@ARG\n");
  }
  fprintf(output, "M=M+1\n");
}