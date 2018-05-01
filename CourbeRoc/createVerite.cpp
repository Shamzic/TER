#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "includes/image_ppm.h"

// Recopie une partie de l'image 2 dans l'image 1 et crée une verité de terrain

int main(int argc, char* argv[]) {
  char cNomImg1[250], cNomImg2[250], cNomImgOut[250], cNomImgVerite[250] ;
  int nH, nW, nTaille ;

	sscanf (argv[1],"%s",cNomImg1) ;
  sscanf (argv[2],"%s",cNomImg2) ;
  sscanf (argv[3],"%s",cNomImgOut) ;
  sscanf (argv[4],"%s",cNomImgVerite) ;

  OCTET *Img1, *Img2, *ImgOut, *ImgVerite ;
  lire_nb_lignes_colonnes_image_ppm(cNomImg1, &nH, &nW) ;
	nTaille = nH * nW ;

	allocation_tableau(Img1, OCTET, 3*nTaille);
	allocation_tableau(Img2, OCTET, 3*nTaille);
	allocation_tableau(ImgOut, OCTET, 3*nTaille);
	allocation_tableau(ImgVerite, OCTET, 3*nTaille);
  lire_image_ppm(cNomImg1, Img1, nH * nW);
  lire_image_ppm(cNomImg2, Img2, nH * nW);

	for (int i = 0; i < nH  ; i++) {
	  for (int j = 0; j < nW ; j++) {
	  	if (i > 256 && i < 400 && j > 256 && j < 400) {
	  		ImgOut[3*i*nW + 3*j] = Img2[3*i*nW + 3*j] ;
	  		ImgOut[3*i*nW + 3*j +1] = Img2[3*i*nW + 3*j +1] ;
	  		ImgOut[3*i*nW + 3*j +2] = Img2[3*i*nW + 3*j +2] ;

	  		ImgVerite[3*i*nW + 3*j] = 0 ;
	  		ImgVerite[3*i*nW + 3*j +1] = 255 ;
	  		ImgVerite[3*i*nW + 3*j +2] = 0 ;
	  	} else {
	  		ImgOut[3*i*nW + 3*j] = Img1[3*i*nW + 3*j] ;
	  		ImgOut[3*i*nW + 3*j +1] = Img1[3*i*nW + 3*j +1] ;
	  		ImgOut[3*i*nW + 3*j +2] = Img1[3*i*nW + 3*j +2] ;

	  		ImgVerite[3*i*nW + 3*j] = 255 ;
	  		ImgVerite[3*i*nW + 3*j +1] = 0 ;
	  		ImgVerite[3*i*nW + 3*j +2] = 0 ;
	  	}
	  }
	}

	// Création des images

	ecrire_image_ppm(cNomImgOut , ImgOut,  nH, nW);
	ecrire_image_ppm(cNomImgVerite , ImgVerite,  nH, nW);

  // Libération des espaces mémoires
  free(Img1);
  free(Img2);
  free(ImgOut);
  free(ImgVerite);
}
