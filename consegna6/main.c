// Quando ho scritto questo codice solo io e Dio sapevamo cosa fa. Ora lo sa solo Dio.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gestionefile.h"
#include "operazioni.h"

void smista(char[], FILE *, int *, char[]);

int main(int argc, char **argv)
{
  // Apro il file input in read
  FILE *vmach;
  vmach = fopen(argv[1], "r");
  int len = strlen(argv[1]);
  // E creo il file destinazione
  char assm[len + 1];
  rename_file(argv[1], assm);
  FILE *output;
  output = fopen(assm, "w");

  // Inizializzo il file
  bootstrap(output);

  char riga[128];

  // Traduzione delle istruzioni
  int i = 16;
  int * conditioncounter = malloc(sizeof(int));
  *conditioncounter = 0;
  while(fgets(riga, 127, vmach)) {    // Itero per ogni riga del file asm
    clear(riga);
    if (riga[0] != '/' && riga[0] != '\r' && riga[0] != '\n' && riga[0] != '\0')
    {
      smista(riga, output, conditioncounter, assm);
    }
  }

  fprintf(output, "(END)\n@END\n0;JMP");

  fclose(vmach);
  fclose(output);
  return 0;
}

void smista(char riga[], FILE * output, int * conditioncounter, char assm[]) {
  fprintf(output, "//%s\n", riga);     // commento con il nome del blocco di codice che sto per inserire, per leggibilità
  char cmd[10];

  identificacmd(riga, cmd);           // from operazioni.c

  exec_cmd(riga, cmd, output, conditioncounter, assm);

  //write(codifica, output);
}