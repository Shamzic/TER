// c++ TER.cpp -o TER
// ./TER imgIn.pgm imgMedian.pgm imgBruit.pgm imgOut.pgm

#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "image_ppm.h"

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

int getMoyenne(OCTET* img, int nW, int i, int j) {
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

int getGauss(OCTET* img, int nW, int i, int j) {
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

double absDouble(double n) {
  if (n >= 0) {
    return n ;
  } else {
    return -n ;
  }
}

int main(int argc, char* argv[]) {
  char cNomImgLue[250], cNomImgMedian[250], cNomImgBruit[250], cNomImgOut[250] ;
  int nH, nW, nTaille ;
  int histo[256], histo2[256] ;
  double f[256], f2[256] ;

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

  // Création d'une nouvelle image avec le filtre median, moyen ou gaussien -> ImgMedian
  for (int i = 1; i < nH - 1; i++) {
    for (int j = 1; j < nW - 1; j++) {
      ImgMedian[i*nW+j] = getMediane(ImgIn, nW, i, j) ;
      // ImgMedian[i*nW+j] = getMoyenne(ImgIn, nW, i, j) ;
      // ImgMedian[i*nW+j] = getGauss(ImgIn, nW, i, j) ;
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


	// Remplissage de l'histogramme global du bruit
  for (int i = 0; i < 256; ++i) {
    histo[i] = 0 ;
  }
  for (int i=0; i < nH; i++) {
    for (int j=0; j < nW; j++) {
      histo[ImgBruit[i*nW+j]] ++ ;
    }
  }

  // Remplissage de la densité de probabilité globale
  double somme = 0 ;
  for (int i = 0; i < 256; ++i) {
    f[i] = (double) histo[i] / nTaille ;
    somme += f[i] ;
  }
	
	// Calcul de la différence entre la densité de probabilité du bruit et 
	// celle de chaque carré de (n*2 + 1)*(n*2 + 1) pixels dans ImgOut
	// affichage du pixel en blanc si la différence est suppérieur au seuil
  int n = 15 ;
  int nTailleCarre = (n*2 + 1)*(n*2 + 1) ;
  double seuil = 0.005  ;
  double dif[256] ;
  double difMoyenne ;

	for (int i = n; i < nH - n; i++) {
		for (int j = n; j < nW - n; j++) {
			// Calcul de l'histogramme du carré local
      for (int k = 0; k < 256; k++) {
        histo2[k] = 0 ;
      }
			for (int k = -n; k <= n ; k++) {
				for (int l = -n ; l <= n ; l++) {
          histo2[ImgBruit[(i+k)*nW+(j+l)]] ++ ;
				}
			}

      // Calcul de la densité de probabilité locale
			for (int k = 0; k < 256; k++) {
        f2[k] = (double) histo2[k] / nTailleCarre ;
      }

      // Comparaison des densités de probabilités
      somme = 0 ;
      for (int k = 0 ; k < 256 ; k++ ) {
        dif[k] = absDouble(f[k] - f2[k]);
        somme += dif[k] ;
      }
      difMoyenne = (double)somme/256 ;

			// Affichage du pixel en blanc si la différence est supérieure au pourcentage du seuil
			if (difMoyenne > seuil) {
				ImgOut[i*nW+j] = 255 ;
			} else {
				ImgOut[i*nW+j] = 0 ;
			}

		}
	}


  // Création des images
  ecrire_image_pgm(cNomImgMedian , ImgMedian,  nH, nW);
  ecrire_image_pgm(cNomImgBruit , ImgBruit,  nH, nW);

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
  free(ImgMedian);
  free(ImgBruit);
  free(ImgOut);

  return 1;
}
