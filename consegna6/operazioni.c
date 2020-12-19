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
  char pulita[128];
  while((riga[i] != '\0') && (riga[i] != '/')) {  // Ciclo la riga finché non trovo un commento o la riga finisce
    if (riga[i] != '\t') {      // Se il carattere "puntato" non è uno spazio o un tab
      pulita[j] = riga[i];                        // lo copio nella riga ripulita
      j++;
    }
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
