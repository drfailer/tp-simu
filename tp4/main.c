#include <stdio.h>
#include <stdlib.h>
#include "mt19937ar.h"
#include "data.h"
#include "affichage.h"
#define NB_M_DEP 2 // nombre de mâles au départ
#define NB_F_DEP 2 // nombre de femelles au départ

/******************************************************************************/
/*                           FONCTIONS UTILITAIRES                            */
/******************************************************************************/

static inline unsigned long minLapins(unsigned long a, unsigned long b) {
  int c = a;
  if (a < b)
    c = b;
  return c;
}

/******************************************************************************/
/*                                 FIBONACCI                                  */
/******************************************************************************/

/**
 * @brief  Simule le nombre de lapins chaque années en utilisant le modèle de
 *         Fibonacci.
 *
 * @param  n  nombre d'années à simuler.
 */
void modelFib(int n) {
  int i;
  int fn, fn1 = 1, fn2 = 1;

    for (i = 2; i <= n; ++i) {
      fn = fn1 + fn2;
      fn2 = fn1;
      fn1 = fn;
      printf("mois %d: %d lapins.\n", i, fn);
    }
}

/******************************************************************************/
/*                              MODÈLE RÉALISTE                               */
/******************************************************************************/

/**
 * @brief  Calcul à partir d'un nombre de lapins `nbLapins` agés de `age` à
 *         l'année N - 1 le nombre de survivants pour l'année N.
 *
 * @param  age       Age des lapins.
 * @param  nbLapins  Nombre de lapin l'année précedente.
 *
 * @return  Nombre de lapins survivants à la fin de l'année.
 */
int nbSurvivants(int age, unsigned long int nbLapins) {
  unsigned long int nbSurvivants = 0;
  unsigned long int i;
  double probaSurvie = 0.6;

  if (age >= 10) // la proba de survie diminue de 10%/ans au dela de 10 ans
    probaSurvie = 0.6 - 0.1 * (age - 10);
  else if (age == 0) // proba de survie pour les petits
    probaSurvie = 0.35;

  for (i = 0; i < nbLapins; ++i) {
    if (genrand_real1() < probaSurvie) {
      nbSurvivants++;
    }
  }

  return nbSurvivants;
}

/**
 * @brief  Calcul le nombre de portées pour une femelle sur une année suivant
 *         une loi normale (probabilités hard-codées).
 *
 * @return  Nombre de porté sur l'année en cours.
 */
int calculNbPortees() {
  int nbPortees = 8;
  int r = genrand_real1();

  /*  4  5  6  7  8
   * 10 20 40 20 10
   */
  if (r < 0.1) {
    nbPortees = 4;
  } else if (r < 0.3) {
    nbPortees = 5;
  } else if (r < 0.7) {
    nbPortees = 6;
  } else if (r < 0.9) {
    nbPortees = 7;
  }

  return nbPortees;
}

/**
 * @brief  Calcul le nombre de naissances entre l'année N - 1 et l'année N.
 *
 * @param  population  Population de lapins.
 * @param  annee       Année pour laquelle on calcul les naissances.
 */
void calculNaissance(data_t * populations, int annee) {
  unsigned long i;
  int j, k;
  int nbPortees, nbPetits;
  int estMale;
  unsigned long nbLapins = // on suppose qu'il faut minimum 1 mal pour 2 femelles
    minLapins(populations[annee - 1].f, 2 * populations[annee - 1].m);

  for (i = 0; i < nbLapins; ++i) {
    nbPortees = calculNbPortees();

    for (j = 0; j < nbPortees; ++j) {
      nbPetits = genrand_int32()%4 + 3;
      for (k = 0; k < nbPetits; ++k) {
        estMale = genrand_int32()%2;
        if (estMale) {
          populations[annee].nbM[0]++;
        } else {
          populations[annee].nbF[0]++;
        }
      }
    }
  }
}

/**
 * @brief  Calcul les naissances et les morts pour l'année N.
 *
 * @param  population  Population de lapins.
 * @param  annee       Année pour laquelle on calcul les naissances.
 */
void calculNouvelleAnnee(data_t *populations, int annee) {
  int i;
  int nombreF = 0;
  int nombreM = 0;

  // calcul des survivants chez les adultes
  for (i = 14; i >= 1; --i) {
    populations[annee].nbM[i] = nbSurvivants(i - 1, populations[annee - 1].nbM[i - 1]);
    populations[annee].nbF[i] = nbSurvivants(i - 1, populations[annee - 1].nbF[i - 1]);
  }
  // les petits deviennent adultes
  populations[annee].nbF[0] = 0;
  populations[annee].nbM[0] = 0;

  // calcul du nombre de mal et de femelles adultes pour pouvoir calculer les
  // naissances
  for (i = 1; i < 14; ++i) {
    nombreF += populations[annee].nbF[i];
    nombreM += populations[annee].nbM[i];
  }
  populations[annee].f = nombreF;
  populations[annee].m = nombreM;

  // calcul des naissances
  calculNaissance(populations, annee);
}

/**
 * @brief  Permet d'initialiser la population de lapins pour la première année.
 * Le nombre de mâles et de femelles sont des constantes pré-processeur.
 *
 * @param  n  Le nombre d'années à simuler (pour allocation dynamique du tableau
 *            des années).
 *
 * @return  Population à la première année.
 */
data_t* initPopulations(int n) {
  int i;
  data_t* populations = malloc(n * sizeof(data_t));

  if (populations == NULL) {
    fprintf(stderr, "ERREUR: allocation !\n");
    exit(1);
  }

  for (i = 0; i < 14; ++i) {
    populations[0].nbF[i] = 0;
    populations[0].nbM[i] = 0;
  }
  populations[0].nbF[1] = NB_F_DEP;
  populations[0].nbM[1] = NB_M_DEP;
  populations[0].f = NB_F_DEP;
  populations[0].m = NB_M_DEP;

  return populations;
}

/**
 * @brief  Calcul le nombre de lapins sur `n` années
 *
 * @param  n  nombre d'années à simuler.
 *
 * @return  Population sur n années.
 */
data_t* computeNbLapins(int n) {
  int i;
  data_t* populations;
  populations = initPopulations(n);

  for (i = 1; i < n; ++i) {
    calculNouvelleAnnee(populations, i);
  }

  return populations;
}

/******************************************************************************/
/*                           FONCTIONS D'AFFICHAGE                            */
/******************************************************************************/

/**
 * @brief  Affiche des statistiques sur la population à l'année `annee`
 *
 * @param  population  Population de lapins.
 * @param  annee       Année à afficher
 */
void affichePopulation(data_t *population, int annee) {
  int i;

  printf("\n\n===========================\n");
  printf("Population annee %d:\n", annee);

  printf("\nNombre de femelles: %ld\n", population[annee].f);
  printf("Nombre de mâles: %ld\n", population[annee].m);


  printf("\nFemelles adultes:\n");
  for (i = 0; i < 14; ++i) {
    printf("agée de %d ans: %ld\n", i + 1, population[annee].nbF[i]);
  }

  printf("\nMâles adultes:\n");
  for (i = 0; i < 14; ++i) {
    printf("agé de %d ans: %ld\n", i + 1, population[annee].nbM[i]);
  }

  printf("\nNombre de petites femelles: %ld\n", population[annee].nbF[0]);
  printf("Nombre de petits mâles: %ld\n", population[annee].nbM[0]);
}

/**
 * @brief  Permet d'afficher des statistiques sur la dernière année.
 *
 * @param  n  nombre d'années à simuler.
 */
void afficheLapin20(int n) {
  int annee;
  data_t *population = computeNbLapins(n);

  printf("==================\n");
  printf("nombre de males:\n");
  for (annee = 0; annee < n; ++annee) {
    printf("%d ", annee + 1);
    afficheData("%nbm\n", population[annee]);
  }
  printf("==================\n");
  printf("nombre de femelles:\n");
  for (annee = 0; annee < n; ++annee) {
    printf("%d ", annee + 1);
    afficheData("%nbf\n", population[annee]);
  }
  printf("==================\n");
  printf("nombre de petits (f m):\n");
  for (annee = 0; annee < n; ++annee) {
    printf("%d ", annee + 1);
    afficheData("%npf %npm\n", population[annee]);
  }
  printf("==================\n");
  printf("nombre de de lapins par ages (f m):\n");
  for (annee = 0; annee < n; ++annee) {
    printf("%d:\n", annee + 1);
    afficheData("    %pf %pm\n", population[annee]);
    for (int i = 0; i < 14; ++i) {
      printf("%d %ld %ld\n", i, population[annee].nbF[i], population[annee].nbM[i]);
    }
  }
  free(population);
}

/******************************************************************************/
/*                                    MAIN                                    */
/******************************************************************************/

int main(void)
{
  /* Modèle de Fibonacci: */
  /* modelFib(20); */

  /* Modèle réaliste: */
  initMT();
  afficheLapin20(20);
  return 0;
}
