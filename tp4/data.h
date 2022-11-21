#ifndef __DATA_T__
#define __DATA_T__

/* STRUCTURE: */

/* Permet de stocker des statistiques sur la population de lapins chaque années */
typedef struct data {
  unsigned long int nbF[15];   // nombre de femelles de 0 à 14 ans
  unsigned long int nbM[15];   // nombre de mâles de 0 à 14 ans
  unsigned long int f;         // nombre de femelles
  unsigned long int m;         // nombre de mâles
} data_t;

#endif
