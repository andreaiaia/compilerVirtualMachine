#include "gestionefile.h"

void rename_file(char in_name[], char out_name[])
{
  int len = strlen(in_name);
  strcpy(out_name, in_name);
  int i = len - 2;            // Posiziono il "cursore" al . prima dell'estensione
  char est[4] = "asm";
  while (i < len + 1)         // Sostituisco i caratteri dell'estensione con l'est 'hack'
  {
    out_name[i] = est[i - len + 2];
    i++;
  }
  out_name[i] = '\0';         // Chiudo la stringa
}

void write(char riga[], FILE *output) {
  fprintf(output, "%s\n", riga);
}