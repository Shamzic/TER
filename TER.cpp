// c++ TER.cpp -o TER
// ./TER imgIn.pgm imgMedian.pgm imgBruit.pgm imgOut.pgm

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

int getMediane(OCTET* img, int nW, int i, int j, int sizeMat) {
  std::vector<int> val;
  for (int k = -sizeMat; k<sizeMat; k++) {
    for (int l = -sizeMat; l<sizeMat; l++) {
      val.push_back(img[(i+k)*nW+j+l]);
    }
  }
  std::sort (val.begin(), val.end());
  return val.at(val.size()/2);
  // int valeurs [9] = {
  //  img[(i-1)*nW+j-1],
  //  img[i*nW+j-1],
  //  img[(i+1)*nW+j-1],
  //  img[(i-1)*nW+j],
  //  img[i*nW+j],
  //  img[(i+1)*nW+j],
  //  img[(i-1)*nW+j+1],
  //  img[i*nW+j+1],
  //  img[(i+1)*nW+j+1],
  // };
  // std::sort (valeurs, valeurs + 9);
  // return valeurs[4] ;
}

int getMoyenne(OCTET* img, int nW, int i, int j, int sizeMat) {

  int somme = 0 ;
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
  for (int i = 0 ; i < 9 ; i++) {
    somme += valeurs[i] ;
  }
  return somme / 9 ;
}

int getGauss(OCTET* img, int nW, int i, int j, int sizeMat) {
  int somme = 0 ;
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
  somme = valeurs[0] + 2 * valeurs[1] + valeurs[2] 
          + 2 * valeurs[3] + 4 * valeurs[4] + 2 * valeurs[5] 
          + valeurs[6] + 2 * valeurs[7] + valeurs[8] ;
  return somme / 16 ;
}

int passeHaut(OCTET* img, int nW, int i, int j, int sizeMat) {
  int somme = 0 ;
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
  somme = -valeurs[1] -valeurs[3] + 5*valeurs[4] -valeurs[5] -valeurs[7] ;
  return somme ;
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
  int nH, nW, nTaille, sizeMat ;

  if (argc < 6) {
    printf("Usage: ImageIn.pgm ImageMedianOut.pgm ImageBruitOut.pgm ImageOut.pgm sizeMat (1 => 3*3, 2 = 5*5) \n"); 
    exit (1) ;
  }

  sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%s",cNomImgMedian) ;
  sscanf (argv[3],"%s",cNomImgBruit) ;
  sscanf (argv[4],"%s",cNomImgOut);
  sscanf (argv[5],"%d", &sizeMat);

  OCTET *ImgIn, *ImgInAug, *ImgBruit, *ImgMedian, *ImgOut;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, nTaille);
  allocation_tableau(ImgInAug, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgBruit, OCTET, nTaille);
  allocation_tableau(ImgMedian, OCTET, nTaille);
  allocation_tableau(ImgOut, OCTET, nTaille);


  //   int rayon = 150;
  // for (int i = nH/2; i < nH/2 + rayon; i++) {
  //   for (int j = nW/2; j < nW/2 + rayon; j++) {
  //     ImgIn[i*nW+j] = passeHaut(ImgIn, nW, i, j, sizeMat) ;
  //   }
  // }

  // Appliacation d'un filtre passe-haut pour augmenter le bruit de l'image d'entrée
  for (int i = sizeMat; i < nH - sizeMat; i++) {
    for (int j = sizeMat; j < nW - sizeMat; j++) {
      ImgInAug[i*nW+j] = passeHaut(ImgIn, nW, i, j, sizeMat) ;
    }
  }
  // Augmentation du bruit dans une certaine zone rayon*rayon

  // int rayon = 150;
  // for (int i = nH/2; i < nH/2 + rayon; i++) {
  //   for (int j = nW/2; j < nW/2 + rayon; j++) {
  //     ImgInAug[i*nW+j] = passeHaut(ImgInAug, nW, i, j, sizeMat) ;
  //   }
  // }
  //   for (int i = nH/2; i < nH/2 + rayon; i++) {
  //   for (int j = nW/2; j < nW/2 + rayon; j++) {
  //     ImgInAug[i*nW+j] = passeHaut(ImgInAug, nW, i, j, sizeMat) ;
  //   }
  // }

  // Création d'une nouvelle image avec le filtre median, moyen ou gaussien -> ImgMedian
  for (int i = sizeMat; i < nH - sizeMat; i++) {
    for (int j = sizeMat; j < nW - sizeMat; j++) {
      ImgMedian[i*nW+j] = getMediane(ImgIn, nW, i, j, sizeMat) ;
      // ImgMedian[i*nW+j] = getMoyenne(ImgIn, nW, i, j) ;
      // ImgMedian[i*nW+j] = getGauss(ImgIn, nW, i, j) ;
    }
  }

  // Isolation du bruit dans ImgBruit
  for (int i = sizeMat; i < nH - sizeMat; i++) {
    for (int j = sizeMat; j < nW - sizeMat; j++) {
      ImgBruit[i*nW+j] = abs(ImgInAug[i*nW+j] - ImgMedian[i*nW+j]) ;
    }
  }


/***********************************************/







  // Augmentation du bruit par expansion dynamique
  // recherche des min et max
  int bruitMin, bruitMax ;
  bruitMin = ImgBruit[nW+1];
  bruitMax = ImgBruit[nW+1];
  for (int i = sizeMat; i < nH - sizeMat; i++) {
		for (int j = sizeMat; j < nW - sizeMat; j++) {
  	  if (ImgBruit[i*nW+j] > bruitMax) {
  			bruitMax = ImgBruit[i*nW+j] ;
  	  }
  	  if (ImgBruit[i*nW+j] < bruitMin) {
  			bruitMin = ImgBruit[i*nW+j] ;
  	  }
  	}
  }
  // expansion dynamique
  for (int i = sizeMat; i < nH - sizeMat; i++) {
		for (int j = sizeMat; j < nW - sizeMat; j++) {
			ImgBruit[i*nW+j] = (ImgBruit[i*nW+j] - bruitMin) * (255/(bruitMax-bruitMin)) ;
		}
	}

	// Calcul de la valeur moyenne du bruit après expansion dynamique
	int somme, cpt ;
	double bruitMoyen ;
	somme = 0 ;
	cpt = 0 ;
	for (int i = sizeMat; i < nH - sizeMat; i++) {
		for (int j = sizeMat; j < nW - sizeMat; j++) {
			somme += ImgBruit[i*nW+j] ;
			cpt ++ ;
		}
	}
	bruitMoyen = somme / cpt ;
	
	// Calcul de la différence entre le bruit moyen de l'image
	// et le bruit moyen de chaque carré de (n*2 + 1)*(n*2 + 1) pixels dans ImgOut
	// affichage du pixel en blanc si la différence est suppérieur au seuil
	int seuil = bruitMoyen;
	int sommePixelAdjacents = 0 ;
  int n = 11 ;
	double moyenneSommetsAdjacents ;
	if (bruitMoyen < 10) {
		seuil = bruitMoyen / 2 + 1 ;
	}
	for (int i = sizeMat; i < nH - sizeMat; i++) {
		for (int j = sizeMat; j < nW - sizeMat; j++) {
			// Calcul du bruit moyen des n*n pixels voisins
			sommePixelAdjacents = 0 ;
			for (int k = -n; k <= n ; k++) {
				for (int l = -n ; l <= n ; l++) {
					sommePixelAdjacents += ImgBruit[(i+k)*nW+(j+l)] ;
				}
			}
			moyenneSommetsAdjacents = sommePixelAdjacents / ((n*2 + 1)*(n*2 + 1)) ;
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
  ecrire_image_pgm((char*)"./images/FILTRE_PASSE_HAUT.pgm" , ImgInAug,  nH, nW);

  // Enchainement de dilatations et erosions pour mettre en évidence les zones
  // avec des différences de bruit importantes


  ImgBruit = dilatation(ImgOut, ImgBruit, nH, nW) ;
  ImgOut = dilatation(ImgBruit, ImgOut, nH, nW) ;
  ImgBruit = dilatation(ImgOut, ImgBruit, nH, nW) ;

  ImgOut = erosion(ImgBruit, ImgOut, nH, nW) ;
  ImgBruit = erosion(ImgOut, ImgBruit, nH, nW) ;
  ImgOut = erosion(ImgBruit, ImgOut, nH, nW) ;

  ecrire_image_pgm(cNomImgOut , ImgOut,  nH, nW);

  // Libération des espaces mémoires
  free(ImgIn);
  free(ImgInAug);
  free(ImgMedian);
  free(ImgBruit);
  free(ImgOut);

  return 1;
}
