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
    write("D=M-D", output);
    fprintf(output, "@IF_TRUE%d\n", *conditioncounter);
    write("D;JEQ", output);
    write("M=0", output);
    fprintf(output, "@END%d\n", *conditioncounter);
    write("0;JMP", output);
    write("(IF_TRUE)", output);
    write("M=1", output);
    fprintf(output, "(END%d)\n", *conditioncounter);
    write("@SP", output);
    write("M=M+1", output);
    *conditioncounter++;
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
    write("D=D-M", output);
    fprintf(output, "@IF_TRUE%d\n", *conditioncounter);
    write("D;JGT", output);
    write("M=0", output);
    fprintf(output, "@END%d\n", *conditioncounter);
    write("0;JMP", output);
    write("(IF_TRUE)", output);
    write("M=1", output);
    fprintf(output, "(END%d)\n", *conditioncounter);
    write("@SP", output);
    write("M=M+1", output);
    *conditioncounter++;
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
    write("D=D-M", output);
    fprintf(output, "@IF_TRUE%d\n", *conditioncounter);
    write("D;JLT", output);
    write("M=0", output);
    fprintf(output, "@END%d\n", *conditioncounter);
    write("0;JMP", output);
    write("(IF_TRUE)", output);
    write("M=1", output);
    fprintf(output, "(END%d)\n", *conditioncounter);
    write("@SP", output);
    write("M=M+1", output);
    *conditioncounter++;
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
  write(num, output);
  write("D=A", output);
  if (!strcmp(seg, "constant")) 
  {
    write("@SP", output);
    write("A=M", output);
    write("M=D", output);
    write("@SP", output);

  }
  else if (!strcmp(seg, "local")) 
  {
    write("@LCL", output);
    write("A=M", output);
    write("M=D", output);
    write("@LCL", output);
  }
  else if (!strcmp(seg, "static")) 
  {

  }
  else if (!strcmp(seg, "argument")) 
  {
    write("@ARG", output);
    write("A=M", output);
    write("M=D", output);
    write("@ARG", output);
  }
  write("M=M+1", output);
}