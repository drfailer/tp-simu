#include <stdio.h>
#include <stdlib.h>
#include "mt19937ar.h"
#include "data.h"
#include "affichage.h"
#define NB_M_DEP 2 // nombre de mâles au départ
#define NB_F_DEP 2 // nombre de femelles au départ

/* FONCTIONS: */

/**
 * NAME: function name
 *
 * Model non réaliste de Fibonacci.
 *
 * PARAMS:
 * n - nombre de mois à traiter
 *
 * OUTPUT:
 * - affiche le nombre de lapins sur `n` mois
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

/* printf("age: %d, nb lapins: %ld, nbSurvivants: %ld\n", age, nbLapins, nbSurvivants); */

  return nbSurvivants;
}

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

void calculNaissance(data_t * populations, int annee) {
  unsigned long i;
  int j, k;
  int nbPortees, nbPetits;
  int estMale;

  for (i = 0; i < populations[annee - 1].f; ++i) {
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

  // calcul des naissances
  // TODO: optimize this (put it in the loop above)
  for (i = 0; i < 14; ++i) {
    nombreF += populations[annee].nbF[i];
    nombreM += populations[annee].nbM[i];
  }
  populations[annee].f = nombreF;
  populations[annee].m = nombreM;

  // calcul des naissances
  calculNaissance(populations, annee);
}

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

data_t* computeNbLapins(int n) {
  int i;
  data_t* populations;
  populations = initPopulations(n);

  for (i = 1; i < n; ++i) {
    calculNouvelleAnnee(populations, i);
  }

  return populations;
}

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
 * NAME: afficheLapinAn
 *
 * Permet d'afficher le nombre de lapins dans une population sur `n` années.
 *
 * PARAMS:
 * n - nombre d'années à traiter
 *
 * OUTPUT:
 * - affiche le nombre de lapins par ans
 *
 * SIDE-EFFECTS:
 * - sauvegarde ces données dans un tableau
 *
 * RETURN:
 * return what
 */
void afficheLapinAn(int n) {
  int annee;
  data_t *population = computeNbLapins(n);

  for (annee = 0; annee < n; ++annee) {
    printf("annee %d: ", annee + 1);
    afficheData("adultes: %nbf %nbm, petits: %pf %pm, total: %t\n", population[annee]);
    /* affichePopulation(population, annee); */
    /* afficheData("%nbm\n", population[annee]); */
    /* afficheData("males min %m[1] mal max %m[14]\n", population[annee]); */
    /* afficheData("femeles ages min: %f[2]\nfemelles age max: %f[14]\n\n", population[annee]); */
  }
}

int main(void)
{
  /* Model de Fibonacci: */
  /* modelFib(20); */

  /* Model réaliste: */
  initMT();
  /* for (int i = 0; i < 10; ++i){ */
    afficheLapinAn(20);
  /* } */
  return 0;
}
