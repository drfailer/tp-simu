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
 * Calcule une valeur approchée de pi à l'aide de la methode de Monte Carlo.
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
 *  Effectue plusieur simulation avec `simPi` et `nbPoints`, puis calcule la
 * moyenne et la précision sur pi.
 *
 * PARAMS:
 * nbPoints - nombre de points à calculer.
 * nbSimulations - nombre de simulation.
 *
 * OUTPUT:
 * - affiche la précision
 */
void simPiPrecisison(unsigned int nbPoints, int nbSimulations) {
  int    i;
  double mean;
  double sum = 0;

  initMT();

  // calcul des pis
  for (i = 0; i < nbSimulations; ++i) {
    sum += simPi(nbPoints);
  }
  mean = sum / nbSimulations;

  printf("mean: %lf\n", mean);
  printf("precision: %lf\n", fabs(mean - M_PI) / M_PI);
}

/**
 * NAME: intervalleConfience
 *
 *  Calcul et affiche l'intervalle de confience à 95% pour pi.
 *
 * PARAMS:
 * nbPoints - precision de la simulation de pi.
 *
 * OUTPUT:
 * - affiche l'intervalle de confience.
 */
void intervalleConfience(unsigned int nbPoints, int n) {
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

  printf("intervalle de confience: [%lf; %lf].\n", mean - r, mean + r);
}

int main(void) {
  initMT();

  /* printf("pi 1000: %lf\n", simPi(1000)); */
  /* printf("pi 1000000: %lf\n", simPi(1000000)); */
  /* printf("pi 1000000000: %lf\n", simPi(1000000000)); */
  simPiPrecisison(1000, 40);
  simPiPrecisison(1000000, 40);
  simPiPrecisison(1000000000, 40);
  /* intervalleConfience(1000000, 30); */

  return 0;
}
