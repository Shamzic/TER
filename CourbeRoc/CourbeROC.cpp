#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <sstream>
#include "../includes/image_ppm.h"

int main(int argc, char* argv[]) {

	char cNomImgVerite[250] ;
  int nH, nW, nTaille, nbImg ;
  int nbPos, nbVraiPos, nbNeg, nbVraiNeg ;
  double sensibilite, antispecificite ;

  if (argc < 3) {
    printf("Usage: <ImageVerite.ppm> <nbImages à comparer avec la verité>  \n"); 
    exit (1) ;
  }

  sscanf (argv[1],"%s",cNomImgVerite) ;
  sscanf (argv[2],"%d",&nbImg) ;

  OCTET *ImgVerite, *ImgLue ;

  lire_nb_lignes_colonnes_image_ppm(cNomImgVerite, &nH, &nW) ;
  nTaille = nH * nW ;

  allocation_tableau(ImgVerite, OCTET, 3*nTaille);
  lire_image_ppm(cNomImgVerite, ImgVerite, nH * nW);
  allocation_tableau(ImgLue, OCTET, nTaille);

  FILE* fichier = fopen("CourbeRoc.dat", "w+");
  // Pour chaque image avec un seuil différent :
  for (int i = 0; i < nbImg; ++i) {

	  // On lit l'image dans ImgLue
  	std::stringstream ss; 
	  ss << "./images/" << i << ".pgm" ;
	  std::string sNomImgLue = ss.str();
	  char* cNomImgLue = (char*)sNomImgLue.c_str();
  	lire_image_pgm(cNomImgLue, ImgLue, nH * nW);

  	// On réinitialise les variables
  	nbPos = 0 ;
	  nbVraiPos = 0 ;
	  nbNeg = 0 ;
	  nbVraiNeg = 0 ;

	  // On calcule les variables
  	for (int i = 1; i < nH - 1 ; i++) {
    	for (int j = 1; j < nW - 1; j++) {
    		if (ImgLue[i*nW+j] == 255) {
    			nbPos ++ ;
    			if (ImgVerite[3*i*nW+3*j] < 10) {
    				nbVraiPos ++ ;
    			}
    		} else {
    			nbNeg ++ ;
    			if (ImgVerite[3*i*nW+3*j] > 10) {
    				nbVraiNeg ++ ;
    			}
    		}
    	}
    }
    sensibilite = (double) nbVraiPos / nbPos ;
    antispecificite = (double) (nbNeg - nbVraiNeg) / nbNeg ;


    // On rajoute le point correspondant à l'image dans le fichier dat
    fprintf(fichier, "%f %f\n", antispecificite, sensibilite);

  }
  fclose(fichier);

}

// Pour afficher la courbe dans gnuplot
// plot 'CourbeRoc.dat' u 1:2 w l