#include "mt19937ar.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************/
/*                            VARIABLES GLOBLALES                             */
/******************************************************************************/

/* 30 premiers coefficients de student (tableau de taille 31 car on compte à
 * partir de 1) */
static const double student[31] = {
    0,     12.71, 4.303, 3.182, 2.776, 2.571, 2.447, 2.365, 2.306, 2.262, 2.228,
    2.201, 2.179, 2.160, 2.145, 2.131, 2.120, 2.110, 2.101, 2.093, 2.086, 2.080,
    2.074, 2.069, 2.064, 2.060, 2.056, 2.052, 2.048, 2.045, 2.042,
};

/******************************************************************************/
/*                                 FONCTIONS                                  */
/******************************************************************************/

/**
 * NAME: simPi
 *
 * Calcule une valeur approchée de pi à l'aide de la méthode de Monte Carlo.
 *
 * PARAMS:
 * nbPoints - nombre de points à calculer
 *
 * RETURN:
 * valeur approchée de pi
 */
double simPi(unsigned int nbPoints) {
  unsigned int pointsInCircle = 0;
  unsigned int i;
  double       x, y;

  for (i = 0; i < nbPoints; ++i) {
    x = genrand_real1();
    y = genrand_real1();

    if (x * x + y * y < 1) {
      pointsInCircle++;
    }
  }

  return (double)(4 * pointsInCircle) / nbPoints;
}

/**
 * NAME: simPiPrecisison
 *
 * Effectue plusieurs simulation avec `simPi` et `nbPoints`, puis calcule la
 * moyenne et l'erreur sur pi.
 *
 * PARAMS:
 * nbPoints - nombre de points à calculer.
 * nbSimulations - nombre de simulation.
 *
 * OUTPUT:
 * - affiche: moyenne     erreur (affichage exploitable sur tableur)
 */
void simPiErreur(unsigned int nbPoints, int nbSimulations) {
  int    i;
  double mean;
  double sum = 0;

  initMT();

  // calcul des pis
  for (i = 0; i < nbSimulations; ++i) {
    sum += simPi(nbPoints);
  }
  mean = sum / nbSimulations;

  printf("%lf\t", mean);
  printf("%.10lf\n", fabs(mean - M_PI) / M_PI);
}

/**
 * NAME: precision10_40
 *
 * PARAMS:
 * nbPoints - nombre de points à générer pour la simulation de pi
 *
 * OUTPUT:
 * - moyenne et précisions pour 10, 11, ..., 40 expériences
 */
void erreur10_40(unsigned int nbPoints) {
  int i;

  for (i = 10; i <= 40; ++i) {
    simPiErreur(nbPoints, i);
  }
}

/**
 * NAME: intervalleConfiance
 *
 * Calcul et affiche l'intervalle de confiance à 95% pour pi. On ne peut faire
 * plus de 30 expériences avec cette fonction car on ne possède que les 30
 * premiers coefficients pour la loi de Student (voir 'VARIABLES GLOBALES').
 *
 * PARAMS:
 * nbPoints - précision de la simulation de pi.
 * n - nombre de pi à calculer pour la moyenne
 *
 * OUTPUT:
 * - affiche l'intervalle de confiance.
 */
void intervalleConfiance(unsigned int nbPoints, int n) {
  int    i;
  double pis[30] = {0};
  double mean;
  double sum = 0;
  double sumSn = 0, s2n;
  double tmp;
  double r;

  initMT();

  // calcul des pis
  for (i = 0; i < n; ++i) {
    pis[i] = simPi(nbPoints);
    sum += pis[i];
  }
  mean = sum / n; // calcul de la moyenne

  // Calcul de S²(n)
  for (i = 0; i < n; ++i) {
    tmp = pis[i] - mean;
    sumSn += tmp * tmp;
  }
  s2n = sumSn / (n - 1);

  r = student[n - 1] * sqrt(s2n / n);

  printf("Pour %d echantillons et %d points.\n", n, nbPoints);
  printf("taux d'erreur: %.10lf\n", r);
  printf("intervalle de confience: [%.10lf; %.10lf].\n", mean - r, mean + r);
}

int main(void) {
  initMT();

  // test `simPi`
  printf("pi 1000: %lf\n", simPi(1000));
  printf("pi 1000000: %lf\n", simPi(1000000));
  /* printf("pi 1000000000: %lf\n", simPi(1000000000)); */

  // test `simPiPrecisison`
  printf("\n=== Précision ===\n");
  simPiErreur(1000, 40);
  simPiErreur(1000000, 40);
  simPiErreur(1000000000, 40);

  // test `precision10_40`
  printf("\n=== Précision pour 10 à 40 expériences ===\n");
  erreur10_40(1000000);
  /* erreur10_40(1000000000); */

  // test `intervalleConfiance`
  printf("\n=== Intervalle de confiences ===\n");
  intervalleConfiance(1000, 30);
  intervalleConfiance(1000000, 30);
  /* intervalleConfiance(1000000000, 30); */

  return 0;
}
