/* DOC {{{
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
}}}*/

#include <math.h>
#include <stdio.h>

// ORIGINAL CODE {{{1
// PARAM & GLOBAL VARS {{{2
/* Period parameters */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt[N];       /* the array for the state vector  */
static int           mti = N + 1; /* mti==N+1 means mt[N] is not initialized */

// 2}}}

// ORIGINAL FUNCTIONS {{{2

/* initializes mt[N] with a seed */
void init_genrand(unsigned long s) {
  mt[0] = s & 0xffffffffUL;
  for (mti = 1; mti < N; mti++) {
    mt[mti] = (1812433253UL * (mt[mti - 1] ^ (mt[mti - 1] >> 30)) + mti);
    /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
    /* In the previous versions, MSBs of the seed affect   */
    /* only MSBs of the array mt[].                        */
    /* 2002/01/09 modified by Makoto Matsumoto             */
    mt[mti] &= 0xffffffffUL;
    /* for >32 bit machines */
  }
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
void init_by_array(unsigned long init_key[], int key_length) {
  int i, j, k;
  init_genrand(19650218UL);
  i = 1;
  j = 0;
  k = (N > key_length ? N : key_length);
  for (; k; k--) {
    mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1664525UL)) +
            init_key[j] + j; /* non linear */
    mt[i] &= 0xffffffffUL;   /* for WORDSIZE > 32 machines */
    i++;
    j++;
    if (i >= N) {
      mt[0] = mt[N - 1];
      i = 1;
    }
    if (j >= key_length)
      j = 0;
  }
  for (k = N - 1; k; k--) {
    mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1566083941UL)) -
            i;             /* non linear */
    mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
    i++;
    if (i >= N) {
      mt[0] = mt[N - 1];
      i = 1;
    }
  }

  mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long genrand_int32(void) {
  unsigned long        y;
  static unsigned long mag01[2] = {0x0UL, MATRIX_A};
  /* mag01[x] = x * MATRIX_A  for x=0,1 */

  if (mti >= N) { /* generate N words at one time */
    int kk;

    if (mti == N + 1)       /* if init_genrand() has not been called, */
      init_genrand(5489UL); /* a default initial seed is used */

    for (kk = 0; kk < N - M; kk++) {
      y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
      mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    for (; kk < N - 1; kk++) {
      y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
      mt[kk] = mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    y = (mt[N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
    mt[N - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

    mti = 0;
  }

  y = mt[mti++];

  /* Tempering */
  y ^= (y >> 11);
  y ^= (y << 7) & 0x9d2c5680UL;
  y ^= (y << 15) & 0xefc60000UL;
  y ^= (y >> 18);

  return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
long genrand_int31(void) { return (long)(genrand_int32() >> 1); }

/* generates a random number on [0,1]-real-interval */
double genrand_real1(void) {
  return genrand_int32() * (1.0 / 4294967295.0);
  /* divided by 2^32-1 */
}

/* generates a random number on [0,1)-real-interval */
double genrand_real2(void) {
  return genrand_int32() * (1.0 / 4294967296.0);
  /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
double genrand_real3(void) {
  return (((double)genrand_int32()) + 0.5) * (1.0 / 4294967296.0);
  /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
double genrand_res53(void) {
  unsigned long a = genrand_int32() >> 5, b = genrand_int32() >> 6;
  return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}
/* These real versions are due to Isaku Wada, 2002/01/09 added */
// 2}}}
// 1}}}

/* MY FUNCTIONS */

/******************************************************************************/
/*                      génération uniforme entre a et b                      */
/******************************************************************************/

/**
 * NAME: genrand_uniformAB
 *
 * Génère un nombre aléatoire compris entre `a` et `b`.
 *
 * PARAMS:
 * a - debut intervalle (double)
 * b - fin intervalle (double)
 *
 * RETURN:
 * nombre aléatoire dans [a,b]
 */
double genrand_uniformAB(double a, double b) {
  return a + (b - a) * genrand_real1();
}

/******************************************************************************/
/*                      distribution discrète empirique                       */
/******************************************************************************/

/**
 * NAME: genrand_distributionDiscreteABC
 *
 * Reproduction d'une distribution discrète empirique avec les 3 classes A, B,
 * C.
 *
 * RETURN:
 * entier compris entre 0 et 2 (0 -> classe A, 1 -> classe B, 3 -> classe C).
 */
int genrand_distributionDiscreteABC() {
  double r = genrand_real1();
  int    classe = 2; // classe C par défaut

  if (r <= 0.35) { // classe A
    classe = 0;
  } else if (r <= 0.80) { // classe B
    classe = 1;
  }
  return classe;
}

/**
 * NAME: genrand_distributionDiscrete
 *
 * Tirage d'une classe aléatoire dans une distribution empirique discrète avec
 * un nombre indéterminé de classes.
 *
 * PARAMS:
 * nbClasses - nombre de classes dans la distribution.
 * effectifs - tableau de taille `nbClasses` contenant les effectifs souhaités
 *             pour chaques classes.
 *
 * RETURN:
 * entier compris entre 0 et `nbClasses - 1` qui correspond à la classe tirée
 * aléatoirement.
 */
int genrand_distributionDiscrete(int nbClasses, int effectifs[]) {
  double r = genrand_real1();
  int    classe = nbClasses - 1;
  int    effectifTotal = 0;
  int    effectifCum = 0;
  int    i;

  // calcul effectif total
  for (i = 0; i < nbClasses; ++i) {
    effectifTotal += effectifs[i];
  }

  // calcul du resultat
  for (i = 0; i < nbClasses; ++i) {
    effectifCum += effectifs[i];
    if (r < (double)effectifCum / effectifTotal) {
      classe = i;
      i = nbClasses;
    }
  }
  return classe;
}

/**
 * NAME: statDistributionDiscreteABC
 *
 * Permet de tester `genrand_distributionDiscreteABC()` en calculant le
 * pourcentage d'individus trouvés pour chaque classes pour `n` tirages.
 *
 * PARAM:
 * `n` nombre de tirages à effectuer.
 *
 * SIDE EFFECT:
 * - affichage du pourcentage d'individus dans chaque classes.
 */
void statDistributionDiscreteABC(int n) {
  int t[3] = {0};
  int i;

  // Collecte les effectif des classes tirées
  for (i = 0; i < n; ++i) {
    t[genrand_distributionDiscreteABC()]++;
  }

  printf("A: %.10lf\n", (double)t[0] / n);
  printf("B: %.10lf\n", (double)t[1] / n);
  printf("C: %.10lf\n", (double)t[2] / n);
}

/**
 * NAME: statDistributionDiscrete
 *
 * Utilise `genrand_distributionDiscrete()` en calculant le pourcentage
 * d'individus trouvés pour chaque classes pour `n`tirages.
 *
 * PARAM:
 * n - nombre de tirage à effectuer.
 *
 * OUTPUT:
 * - affichage du pourcentage d'individus dans chaque classes.
 */
void statDistributionDiscrete(int n) {
  int t[5] = {0};
  int effectifs[5] = {20, 5, 50, 10, 15};
  int i;

  // experience
  for (i = 0; i < n; ++i) {
    t[genrand_distributionDiscrete(5, effectifs)]++;
  }

  // affichage des result
  for (i = 0; i < 5; ++i) {
    printf("%d: %.10lf\n", i, (double)t[i] / n);
  }
}

/******************************************************************************/
/*                          distributions continues                           */
/******************************************************************************/


/**
 * NAME: negExp
 *
 * Retourne des nombres aléatoires suivant une loi exponentielle négative.
 *
 * PARAM:
 * mean - moyenne de la loi exponentielle négative (double).
 *
 * RETURN:
 * nombre aléatoire suivant une loi exponentielle négative
 */
double negExp(double mean) { return -mean * log(1 - genrand_real2()); }

/******************************************************************************/
/*                      simulation de loi non réversible                      */
/******************************************************************************/

/**
 * NAME: genrand_gaussian
 *
 * RETURN:
 * some des 30 résultats de lancé de dé 6.
 */
int genrand_gaussian() {
  int i;
  int acc = 0;

  for (i = 0; i < 30; ++i) {
    acc += genrand_int32()%6 + 1;
  }

  return acc;
}

/**
 * NAME: gaussianAverage
 *
 * PARAM:
 * n - nombre de tirages à effectuer
 *
 * RETURN:
 * moyenne sur `n` tirages de `genrand_gaussian`.
 */
double gaussianAverage(int n) {
  int i;
  int acc = 0;

  for (i = 0; i < n; ++i) {
    acc += genrand_gaussian();
  }
  return (double) acc / n;
}

void simulateBellCurve(int testBins[], int numberOfBins, int n) {
  int i, j;
  double r;

  for (i = 0; i < n; ++i) {
    r = genrand_gaussian();

    if (r < numberOfBins) {
      // recherche de la position dans le tableau
      for (j = 0; j < numberOfBins - 1; ++j) {
        if (r >= j && r < j + 1) {
          testBins[j]++;
        }
      }
    } else {
      testBins[numberOfBins - 1]++;
    }
  }
}

/******************************************************************************/
/*                             fonctions de tests                             */
/******************************************************************************/

/**
 * NAME: testUniformeAB
 *
 * Effectue `n` tirages avec `genrand_uniformAB`.
 *
 * PARAM:
 * n - nombre de tirages à effectuer
 *
 * OUTPUT:
 * - affiche les `n` nombres trouvés.
 */
void testUniformeAB(int n) {
  int i;

  for (i = 0; i < n; ++i) {
    printf("%.2f\n", genrand_uniformAB(-89.2, 56.7));
  }
}

/**
 * NAME: testDistributionDiscreteABC
 *
 * Utilise `statDistributionDiscreteABC` avec les paramètres 1000, 10000 et
 * 100000.
 *
 * OUTPUT:
 * - statistiques d'une distribution discrète empirique à trois classes pour
 *   1000, 10000 et 100000 tirages.
 */
void testDistributionDiscreteABC() {
  printf("discEmpDist - 1000\n");
  statDistributionDiscreteABC(1000);
  printf("\ndiscEmpDist - 10000\n");
  statDistributionDiscreteABC(10000);
  printf("\ndiscEmpDist - 100000\n");
  statDistributionDiscreteABC(100000);
}

void testDistributionDiscrete() {
  printf("discEmpDist2 - 100000\n");
  statDistributionDiscrete(100000);
}

/* On vérifie que la suite de nombre retournée par `negExp()` après un certain
 * nombre de tirage a bien un moyenne de 11.
 */
void testExp() {
  double r;
  int    i;

  for (i = 0; i < 1000; ++i) {
    r += negExp(11);
  }
  printf("mean for 1000: %.10lf\n", (double)r / 1000);

  for (i = 0; i < 1000000; ++i) {
    /* r += negExp(11); */
  }
  printf("mean for 1000000: %.10lf\n", (double)r / 1000000);
}

/* Test du générateur `negExp()` en calculant des effectifs sur des intervalles
 * discrets de 0 à 22. Sachant que dernier case contiendra la fréquence
 * d'apparition des nombre plus grand que 22.
 */
void testDiscretisationNegExp(int n) {
  int    test22bins[23] = {0};
  int    i, j;
  double r;

  for (i = 0; i < n; ++i) {
    r = negExp(11);

    if (r < 22) {
      // recherche de la position dans le tableau
      for (j = 0; j < 22; ++j) {
        if (r >= j && r < j + 1) {
          test22bins[j]++;
        }
      }
    } else {
      test22bins[22]++;
    }
  }

  for (i = 0; i < 23; ++i) {
    printf("%d: %d\n", i + 1, test22bins[i]);
  }
}

/******************************************************************************/
/*                              ma fonction main                              */
/******************************************************************************/

void myMain() {
  testUniformeAB(1000);

  testDistributionDiscreteABC();
  testDistributionDiscrete();

  testExp();
  testDiscretisationNegExp(1000);
  testDiscretisationNegExp(1000000);
}

/******************************************************************************/
/*                          fonction main originale                           */
/******************************************************************************/

void originalMain() {
  int i;

  printf("1000 outputs of genrand_int32()\n");
  for (i = 0; i < 1000; i++) {
    printf("%10lu ", genrand_int32());
    if (i % 5 == 4)
      printf("\n");
  }
  printf("\n1000 outputs of genrand_real2()\n");
  for (i = 0; i < 1000; i++) {
    printf("%10.8f ", genrand_real2());
    if (i % 5 == 4)
      printf("\n");
  }
}

/******************************************************************************/
/*                                    MAIN                                    */
/******************************************************************************/

int main(void) {
  unsigned long init[4] = {0x123, 0x234, 0x345, 0x456}, length = 4;
  init_by_array(init, length);
  /* originalMain(); */
  myMain();
  return 0;
}
