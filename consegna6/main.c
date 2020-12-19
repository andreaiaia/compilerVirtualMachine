// Quando ho scritto questo codice solo io e Dio sapevamo cosa fa. Ora lo sa solo Dio.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gestionefile.h"
#include "operazioni.h"



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
  while(fgets(riga, 127, vmach)) {    // Itero per ogni riga del file asm
    clear(riga);
    if (riga[0] != '\r' && riga[0] != '\n' && riga[0] != '\0') {
      
    }
  }

  fclose(vmach);
  fclose(output);
  return 0;
}
