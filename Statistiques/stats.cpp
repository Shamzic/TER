#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include "./includes/image_ppm.h"

int main(int argc, char* argv[]) {

	char cNomImgVerite[250] ;
  char cNomImgLue[250] ;
  int nH, nW, nH2, nW2, nTaille, nTaille2 ;
  int nbPos, nbVraiPos, nbFauxPos, nbNeg, nbVraiNeg, nbFauxNeg ;
  double sensibilite, antispecificite ;

  if (argc < 3) {
    printf("Usage: <ImgLue.ppm> <ImageVerite.ppm> \n"); 
    exit (1) ;
  }
  sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%s",cNomImgVerite) ;

  OCTET *ImgVerite, *ImgLue ;
  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW) ;
  lire_nb_lignes_colonnes_image_ppm(cNomImgVerite, &nH2, &nW2) ;
  nTaille = nH * nW ;
  nTaille2 = nH2 * nW2 ;
  allocation_tableau(ImgLue, OCTET, nTaille);
  allocation_tableau(ImgVerite, OCTET, 3*nTaille2);

  lire_image_ppm(cNomImgVerite, ImgVerite, nTaille);
  lire_image_pgm(cNomImgLue, ImgLue, nTaille);


	// On initialise les variables
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
  nbFauxPos = nbPos - nbVraiPos;
  nbFauxNeg = nbNeg - nbVraiNeg;
  sensibilite = (double) nbVraiPos / nbPos;
  antispecificite = (double) (nbNeg - nbVraiNeg) / nbNeg;

  std::cout << "nbVraiPos : " << nbVraiPos << std::endl;
  std::cout << "nbFauxPos : " << nbFauxPos << std::endl;
  std::cout << "nbPos : " << nbPos << std::endl;
  std::cout << "nbVraiNeg : " << nbVraiNeg << std::endl;
  std::cout << "nbFauxNeg : " << nbFauxNeg << std::endl;
  std::cout << "nbNeg : " << nbNeg << std::endl;

}