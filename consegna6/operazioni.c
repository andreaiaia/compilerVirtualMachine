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
  while((riga[i] != '\0') && (riga[i] != '/') && (riga[i] != '\r') && (riga[i] != '\n')) {  // Ciclo la riga finché non trovo un commento o la riga finisce    // Se il carattere "puntato" non è uno spazio o un tab
    pulita[j] = riga[i];                        // lo copio nella riga ripulita
    j++;
    i++;
  }
  pulita[j] = '\0';            // Aggiungo il terminatore
  strcpy(riga, pulita);        // Copio il risultato nella riga che poi codificherò
}

void bootstrap(FILE * output) {
  write("//bootstrap", output);
  write("@256", output);
  write("D=A", output);
  write("@SP", output);
  write("M=D", output);
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
  while(riga[i] != ' ' && riga[i] != '\0' && riga[i] != '\r' && riga[i] != '\n') {
    seg[j] = riga[i];
    j++;
    i++;
  }
  seg[j] = '\0';
}

void estrainum(char riga[], char num[])
{
  int i = 0, j = 0;
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

void exec_cmd(char riga[], char cmd[], FILE * output, int * conditioncounter, char assm[]) {
  if (!strcmp(cmd, "push"))
  {
    char segmento[strlen(riga)];
    identificaseg(riga, segmento);
    exec_seg_push(riga, segmento, output, assm);
  }
  else if (!strcmp(cmd, "pop")) 
  {
    char segmento[strlen(riga)];
    identificaseg(riga, segmento);
    exec_seg_pop(riga, segmento, output, assm);
  }
  else if (!strcmp(cmd, "add"))
  {
    write("@SP", output); 
    write("M=M-1", output);
    write("A=M", output);
    write("D=M", output);
    write("@SP", output);
    write("M=M-1", output);
    write("A=M", output);
    write("M=D+M", output);
    write("@SP", output);
    write("M=M+1", output);
  }
  else if (!strcmp(cmd, "sub"))
  {
    write("@SP", output);
    write("M=M-1", output);
    write("A=M", output);
    write("D=M", output);
    write("@SP", output);
    write("M=M-1", output);
    write("A=M", output);
    write("M=M-D", output);
    write("@SP", output);
    write("M=M+1", output);
  }
  else if (!strcmp(cmd, "neg"))
  {
    write("@SP", output);
    write("M=M-1", output);
    write("A=M", output);
    write("M=-M", output);
    write("@SP", output);
    write("M=M+1", output);
  }
  else if (!strcmp(cmd, "eq"))
  {
    write("@SP", output);
    write("M=M-1", output);
    write("A=M", output);
    write("D=M", output);
    write("@SP", output);
    write("M=M-1", output);
    write("A=M", output);
    write("M=M-D", output);
    write("D=M", output);
    fprintf(output, "@IF_TRUE%d\n", *conditioncounter);
    write("D;JEQ", output);
    write("@SP", output);
    write("A=M", output);
    write("M=0", output);
    fprintf(output, "@END%d\n", *conditioncounter);
    write("0;JMP", output);
    fprintf(output, "(IF_TRUE%d)\n", *conditioncounter);
    write("@SP", output);
    write("A=M", output);
    write("M=-1", output);
    fprintf(output, "(END%d)\n", *conditioncounter);
    write("@SP", output);
    write("M=M+1", output);
    *conditioncounter = *conditioncounter + 1;
  }
  else if (!strcmp(cmd, "gt"))
  {
    write("@SP", output);
    write("M=M-1", output);
    write("A=M", output);
    write("D=M", output);
    write("@SP", output);
    write("M=M-1", output);
    write("A=M", output);
    write("M=M-D", output);
    write("D=M", output);
    fprintf(output, "@IF_TRUE%d\n", *conditioncounter);
    write("D;JGT", output);
    write("@SP", output);
    write("A=M", output);
    write("M=0", output);
    fprintf(output, "@END%d\n", *conditioncounter);
    write("0;JMP", output);
    fprintf(output, "(IF_TRUE%d)\n", *conditioncounter);
    write("@SP", output);
    write("A=M", output);
    write("M=-1", output);
    fprintf(output, "(END%d)\n", *conditioncounter);
    write("@SP", output);
    write("M=M+1", output);
    *conditioncounter = *conditioncounter + 1;
  }
  else if (!strcmp(cmd, "lt"))
  {
    write("@SP", output);
    write("M=M-1", output);
    write("A=M", output);
    write("D=M", output);
    write("@SP", output);
    write("M=M-1", output);
    write("A=M", output);
    write("M=M-D", output);
    write("D=M", output);
    fprintf(output, "@IF_TRUE%d\n", *conditioncounter);
    write("D;JLT", output);
    write("@SP", output);
    write("A=M", output);
    write("M=0", output);
    fprintf(output, "@END%d\n", *conditioncounter);
    write("0;JMP", output);
    fprintf(output, "(IF_TRUE%d)\n", *conditioncounter);
    write("@SP", output);
    write("A=M", output);
    write("M=-1", output);
    fprintf(output, "(END%d)\n", *conditioncounter);
    write("@SP", output);
    write("M=M+1", output);
    *conditioncounter = *conditioncounter + 1;
  }
  else if (!strcmp(cmd, "and"))
  {
    write("@SP", output);
    write("M=M-1", output);
    write("A=M", output);
    write("D=M", output);
    write("@SP", output);
    write("M=M-1", output);
    write("A=M", output);
    write("M=M&D", output);
    write("@SP", output);
    write("M=M+1", output);
  }
  else if (!strcmp(cmd, "or"))
  {
    write("@SP", output);
    write("M=M-1", output);
    write("A=M", output);
    write("D=M", output);
    write("@SP", output);
    write("M=M-1", output);
    write("A=M", output);
    write("M=M|D", output);
    write("@SP", output);
    write("M=M+1", output);
  }
  else if (!strcmp(cmd, "not"))
  {
    write("@SP", output);
    write("M=M-1", output);
    write("A=M", output);
    write("M=!M", output);
    write("@SP", output);
    write("M=M+1", output);
  }
  else if (!strcmp(cmd, "label"))
  {
    char label[strlen(riga)];
    identificaseg(riga, label);
    fprintf(output, "(%s)\n", label);
  }
  else if (!strcmp(cmd, "goto"))
  {
    char label[strlen(riga)];
    identificaseg(riga, label);
    fprintf(output, "@%s\n", label);
    fprintf(output, "0;JMP\n");
  }
  else if (!strcmp(cmd, "ifgoto"))
  {
    char label[strlen(riga)];
    identificaseg(riga, label);
    fprintf(output, "@SP\n");
    fprintf(output, "M=M-1\n");
    fprintf(output, "A=M\n");
    fprintf(output, "D=M\n");
    fprintf(output, "@%s\n", label);
    fprintf(output, "D;JNE\n");
  }/*
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

void exec_seg_push(char riga[], char seg[], FILE *output, char assm[])
{
  char num[strlen(riga)];
  estrainum(riga, num);
  if (!strcmp(seg, "constant")) 
  {
    fprintf(output, "@%s\n", num);
    fprintf(output, "D=A\n");
  }
  else if (!strcmp(seg, "local")) 
  {
    fprintf(output, "@%s\n", num);
    fprintf(output, "D=A\n");
    fprintf(output, "@LCL\n");
    fprintf(output, "D=D+M\n");
    fprintf(output, "A=D\n");
    fprintf(output, "D=M\n");
  }
  else if (!strcmp(seg, "static")) 
  {
    fprintf(output, "@%s%s\n", assm, num);
    fprintf(output, "D=M\n");
  }
  else if (!strcmp(seg, "argument")) 
  {
    fprintf(output, "@%s\n", num);
    fprintf(output, "D=A\n");
    fprintf(output, "@ARG\n");
    fprintf(output, "D=D+M\n");
    fprintf(output, "A=D\n");
    fprintf(output, "D=M\n");
  }
  fprintf(output, "@SP\n");
  fprintf(output, "A=M\n");
  fprintf(output, "M=D\n");
  fprintf(output, "@SP\n");
  fprintf(output, "M=M+1\n");
}

void exec_seg_pop(char riga[], char seg[], FILE *output, char assm[])
{
  char num[strlen(riga)];
  estrainum(riga, num);
  if (!strcmp(seg, "local"))
  {
    fprintf(output, "@%s\n", num);
    fprintf(output, "D=A\n"); 
    fprintf(output, "@LCL\n"); 
    fprintf(output, "D=D+M\n"); 
    fprintf(output, "@R6\n");
    fprintf(output, "M=D\n"); 
    fprintf(output, "@SP\n");
    fprintf(output, "M=M-1\n");
    fprintf(output, "A=M\n");
    fprintf(output, "D=M\n");
    fprintf(output, "@R6\n");
    fprintf(output, "A=M\n");
    fprintf(output, "M=D\n");
  }
  else if (!strcmp(seg, "static"))
  {
    fprintf(output, "@SP\n");
    fprintf(output, "M=M-1\n");
    fprintf(output, "A=M\n");
    fprintf(output, "D=M\n");
    fprintf(output, "@%s%s\n", assm, num);
    fprintf(output, "M=D\n");
  }
  else if (!strcmp(seg, "argument"))
  {
    fprintf(output, "@%s\n", num);
    fprintf(output, "D=A\n");
    fprintf(output, "@ARG\n");
    fprintf(output, "D=D+M\n");
    fprintf(output, "@R6\n");
    fprintf(output, "M=D\n");
    fprintf(output, "@SP\n");
    fprintf(output, "M=M-1\n");
    fprintf(output, "A=M\n");
    fprintf(output, "D=M\n");
    fprintf(output, "@R6\n");
    fprintf(output, "A=M\n");
    fprintf(output, "M=D\n");
  }
}