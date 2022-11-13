#ifndef __DATA_T__
#define __DATA_T__

/* STRUCTURE: */

/* Permet de stocker des statistiques sur la population de lapins chaque années */
typedef struct data {
  int nbF[14];   // nombre de femelles de 1 à 14 ans
  int nbM[14];   // nombre de mâles de 1 à 14 ans
  int nbPetitsF; // nombre de petites femelles (lapin de moins de 1 an)
  int nbPetitsM; // nombre de petits mâles
  int f;         // nombre de femelles
  int m;         // nombre de mâles
} data_t;

#endif
