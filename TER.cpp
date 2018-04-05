#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "includes/image_ppm.h"

int getMin(int v1, int v2, int v3, int v4) {
  int min = v1 ;
  if (v2 < min)
    min = v2 ;
  if (v3 < min)
    min = v3 ;
  if (v4 < min)
    min = v4 ;
  return min ;
}

int getMax(int v1, int v2, int v3, int v4) {
  int max = v1 ;
  if (v2 > max)
    max = v2 ;
  if (v3 > max)
    max = v3 ;
  if (v4 > max)
    max = v4 ;
  return max ;
}

int getMediane(OCTET* img, int nW, int i, int j) {
	int valeurs [9] = {
		img[(i-1)*nW+j-1],
		img[i*nW+j-1],
		img[(i+1)*nW+j-1],
		img[(i-1)*nW+j],
		img[i*nW+j],
		img[(i+1)*nW+j],
		img[(i-1)*nW+j+1],
		img[i*nW+j+1],
		img[(i+1)*nW+j+1],
	};
	std::sort (valeurs, valeurs + 9);
	return valeurs[4] ;
}

OCTET* dilatation(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW) {
  for (int i = 1 ; i < nH - 1 ; i++) {
    for (int j = 1 ; j < nW - 1 ; j++) {
      ImgOut[i*nW+j] = getMin(ImgIn[i*nW+(j-1)], ImgIn[(i-1)*nW+j], ImgIn[(i+1)*nW+j], ImgIn[i*nW+(j+1)]) ;
    }
  }
  return ImgOut ;
}

OCTET* erosion(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW) {
  for (int i = 1; i < nH - 1; i++) {
    for (int j = 1; j < nW - 1; j++) {
      ImgOut[i*nW+j] = getMax(ImgIn[i*nW+(j-1)], ImgIn[(i-1)*nW+j], ImgIn[(i+1)*nW+j], ImgIn[i*nW+(j+1)]) ;
    }
  }
  return ImgOut ;
}

int main(int argc, char* argv[]) {
  char cNomImgLue[250], cNomImgMedian[250], cNomImgBruit[250], cNomImgOut[250] ;
  int nH, nW, nTaille ;

  if (argc < 5) {
    printf("Usage: ImageIn.pgm ImageMedianOut.pgm ImageBruitOut.pgm ImageOut.pgm\n"); 
    exit (1) ;
  }

  sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%s",cNomImgMedian) ;
  sscanf (argv[3],"%s",cNomImgBruit) ;
  sscanf (argv[4],"%s",cNomImgOut) ;

  OCTET *ImgIn, *ImgBruit, *ImgMedian, *ImgOut;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgBruit, OCTET, nTaille);
  allocation_tableau(ImgMedian, OCTET, nTaille);
  allocation_tableau(ImgOut, OCTET, nTaille);

  // Création d'une nouvelle image ave le filtre median -> ImgMedian
  for (int i = 1; i < nH - 1; i++) {
    for (int j = 1; j < nW - 1; j++) {
      ImgMedian[i*nW+j] = getMediane(ImgIn, nW, i, j) ;
    }
  }

  // Isolation du bruit dans ImgBruit
  for (int i = 1; i < nH - 1; i++) {
    for (int j = 1; j < nW - 1; j++) {
      ImgBruit[i*nW+j] = abs(ImgIn[i*nW+j] - ImgMedian[i*nW+j]) ;
    }
  }

  // Augmentation du bruit par expansion dynamique
  // recherche des min et max
  int bruitMin, bruitMax ;
  bruitMin = ImgBruit[nW+1];
  bruitMax = ImgBruit[nW+1];
  for (int i = 1; i < nH - 1; i++) {
		for (int j = 1; j < nW - 1; j++) {
  	  if (ImgBruit[i*nW+j] > bruitMax) {
  			bruitMax = ImgBruit[i*nW+j] ;
  	  }
  	  if (ImgBruit[i*nW+j] < bruitMin) {
  			bruitMin = ImgBruit[i*nW+j] ;
  	  }
  	}
  }
  // expansion dynamique
  for (int i = 1; i < nH - 1; i++) {
		for (int j = 1; j < nW - 1; j++) {
			ImgBruit[i*nW+j] = (ImgBruit[i*nW+j] - bruitMin) * (255/(bruitMax-bruitMin)) ;
		}
	}

	// Calcul de la valeur moyenne du bruit après expansion dynamique
	int somme, cpt ;
	double bruitMoyen ;
	somme = 0 ;
	cpt = 0 ;
	for (int i = 1; i < nH - 1; i++) {
		for (int j = 1; j < nW - 1; j++) {
			somme += ImgBruit[i*nW+j] ;
			cpt ++ ;
		}
	}
	bruitMoyen = somme / cpt ;
	
	// Calcul de la différence entre le bruit moyen de l'image
	// et le bruit moyen de chaque carré de 25 pixels dans ImgOut
	// affichage du pixel en blanc si la différence est suppérieur au seuil
	int seuil = 10 ;
	int sommePixelAdjacents = 0 ;
	double moyenneSommetsAdjacents ;
	if (bruitMoyen < 10) {
		seuil = bruitMoyen / 2 + 1 ;
	}
	for (int i = 1; i < nH - 1; i++) {
		for (int j = 1; j < nW - 1; j++) {
			// Calcul du bruit moyen des 25 pixels voisins
			sommePixelAdjacents = 0 ;
			for (int k = -2; k <= 2 ; k++) {
				for (int l = -2; l <= 2 ; l++) {
					sommePixelAdjacents += ImgBruit[(i+k)*nW+(j+l)] ;
				}
			}
			moyenneSommetsAdjacents = sommePixelAdjacents / 25 ;
			// Affichage du pixel en blanc si le bruit moyen est supérieur au seuil
			if (abs(bruitMoyen - moyenneSommetsAdjacents) > seuil) {
				ImgOut[i*nW+j] = 255 ;
			} else {
				ImgOut[i*nW+j] = 0 ;
			}
		}
	}

	// Affichage d'informations
	printf("Bruit min : %d\n", bruitMin);
	printf("Bruit max : %d\n", bruitMax);
	printf("Bruit moyen : %f\n", bruitMoyen);
	printf("Seuil : %d\n", seuil);

  // Création des images
  ecrire_image_pgm(cNomImgMedian , ImgMedian,  nH, nW);
  ecrire_image_pgm(cNomImgBruit , ImgBruit,  nH, nW);

  // Enchainement de dilatations et erosions pour mettre en évidence les zones
  // avec des différences de bruit importantes
  ImgBruit = dilatation(ImgOut, ImgBruit, nH, nW) ;
  ImgOut = dilatation(ImgBruit, ImgOut, nH, nW) ;
  ImgBruit = dilatation(ImgOut, ImgBruit, nH, nW) ;
  ImgOut = dilatation(ImgBruit, ImgOut, nH, nW) ;
  ImgBruit = dilatation(ImgOut, ImgBruit, nH, nW) ;
  ImgOut = dilatation(ImgBruit, ImgOut, nH, nW) ;
  ImgBruit = dilatation(ImgOut, ImgBruit, nH, nW) ;
  ImgOut = erosion(ImgBruit, ImgOut, nH, nW) ;
  ImgBruit = erosion(ImgOut, ImgBruit, nH, nW) ;
  ImgOut = erosion(ImgBruit, ImgOut, nH, nW) ;

  ecrire_image_pgm(cNomImgOut , ImgOut,  nH, nW);

  // Libération des espaces mémoires
  free(ImgIn);
  free(ImgMedian);
  free(ImgBruit);
  free(ImgOut);

  return 1;
}
