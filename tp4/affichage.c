#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "affichage.h"

/******************************************************************************/
/*                                 FONCTIONS:                                 */
/******************************************************************************/

/**
 * NAME: parseAge
 *
 * Permet de récupérer l'age des lapins dans `age`. Consomme une partie de la
 * chaine de format en entrée (passage au prochain élément).
 *
 * PARAMS:
 * str - chaine à parser
 *
 * OUTPUT:
 * - display hello world
 *
 * SIDE-EFFECTS:
 * - modifie `age`
 *
 * RETURN:
 * pointeur sur la suite de la chaine.
 */
static inline char *parseAge(char *str, int *age) {
  *age = atoi(str);
  while (*str++ != ']');
  return str;
}

/**
 * NAME: parseFormat
 *
 * Permet de reconnaitre un élément de format.
 *
 * PARAMS:
 * str - chaine à parser
 *
 * RETURN:
 * retourne le numéro de l'élément reconnu
 */
int parseFormat(char *str) {
  int output = -1; // caractère

  if (strncmp(str, "%nbf", 4) == 0) { // nombre femelles
    output = 0;
  }
  else if (strncmp(str, "%nbm", 4) == 0) { // nombre mâles
    output = 1;
  }
  else if (strncmp(str, "%f[", 3) == 0) { // femelles age i
    output = 2;
  }
  else if (strncmp(str, "%m[", 3) == 0) { // mâles age i
    output = 3;
  }
  else if (strncmp(str, "%pf", 3) == 0) { // nombre de petites femelles
    output = 4;
  }
  else if (strncmp(str, "%pm", 3) == 0) { // nombre de petits mâles
    output = 5;
  }

  return output;
}

/**
 * NAME: afficheData
 *
 * Affiche un élément la population sur une année selon le fomat suivant:
 *
 * - %nbf: nombre de femelles
 * - %nbm: nombre de mâles
 * - %f[i]: nombre de femelles age i
 * - %m[i]: nombre de mâles age i
 * - %pf: nombre de petites femelles
 * - %pm: nombre de petits mâles
 *
 * PARAMS:
 * format - format de l'affichage
 * population - element de population à afficher
 *
 * OUTPUT:
 * - affiche la population sur une année suivant un format donné.
 */
void afficheData(char *format, data_t population) {
  char * e = format;
  int age;

  while (*e != 0) {
    switch (parseFormat(e)) {
      case 0:
        printf("%d", population.f);
        e += 4;
        break;
      case 1:
        printf("%d", population.m);
        e += 4;
        break;
      case 2:
        e = parseAge(e + 3, &age);
        printf("%d", population.nbF[age - 1]);
        break;
      case 3:
        e = parseAge(e + 3, &age);
        printf("%d", population.nbM[age - 1]);
        break;
      case 4:
        printf("%d", population.nbPetitsF);
        e += 3;
        break;
      case 5:
        printf("%d", population.nbPetitsM);
        e += 3;
        break;
      default:
        printf("%c", *e);
        ++e;
        break;
    }
  }
}
