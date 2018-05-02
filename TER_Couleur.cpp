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

int getMedianeR(OCTET* img, int nW, int i, int j) {
	int valeurs [9] = {
		img[3*(i-1)*nW+3*(j-1)],
		img[3*i*nW+3*(j-1)],
		img[3*(i+1)*nW+3*(j-1)],
		img[3*(i-1)*nW+3*j],
		img[3*i*nW+3*j],
		img[3*(i+1)*nW+3*j],
		img[3*(i-1)*nW+3*(j+1)],
		img[3*i*nW+3*(j+1)],
		img[3*(i+1)*nW+3*(j+1)],
	};
	std::sort (valeurs, valeurs + 9);
	return valeurs[4] ;
}

int getMedianeG(OCTET* img, int nW, int i, int j) {
  int valeurs [9] = {
    img[3*(i-1)*nW+3*(j-1)+1],
    img[3*i*nW+3*(j-1)+1],
    img[3*(i+1)*nW+3*(j-1)+1],
    img[3*(i-1)*nW+3*j+1],
    img[3*i*nW+3*j+1],
    img[3*(i+1)*nW+3*j+1],
    img[3*(i-1)*nW+3*(j+1)+1],
    img[3*i*nW+3*(j+1)+1],
    img[3*(i+1)*nW+3*(j+1)+1],
  };
  std::sort (valeurs, valeurs + 9);
  return valeurs[4] ;
}

int getMedianeB(OCTET* img, int nW, int i, int j) {
  int valeurs [9] = {
    img[3*(i-1)*nW+3*(j-1)+2],
    img[3*i*nW+3*(j-1)+2],
    img[3*(i+1)*nW+3*(j-1)+2],
    img[3*(i-1)*nW+3*j+2],
    img[3*i*nW+3*j+2],
    img[3*(i+1)*nW+3*j+2],
    img[3*(i-1)*nW+3*(j+1)+2],
    img[3*i*nW+3*(j+1)+2],
    img[3*(i+1)*nW+3*(j+1)+2],
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
  int histoR[256], histoG[256], histoB[256], histo2R[256], histo2G[256], histo2B[256] ;
  double fR[256], fG[256], fB[256], f2R[256], f2G[256], f2B[256] ;

  if (argc < 5) {
    printf("Usage: ImageIn.ppm ImageMedianOut.ppm ImageBruitOut.ppm ImageOut.pgm\n"); 
    exit (1) ;
  }

  sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%s",cNomImgMedian) ;
  sscanf (argv[3],"%s",cNomImgBruit) ;
  sscanf (argv[4],"%s",cNomImgOut) ;

  OCTET *ImgIn, *ImgBruit, *ImgMedian, *ImgOut, *ImgOut2;

  lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, 3*nTaille);
  lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgBruit, OCTET, 3*nTaille);
  allocation_tableau(ImgMedian, OCTET, 3*nTaille);
  allocation_tableau(ImgOut, OCTET, nTaille);
  allocation_tableau(ImgOut2, OCTET, nTaille);

  // Création d'une nouvelle image avec le filtre median, moyen ou gaussien -> ImgMedian
  
  // Pour ne pas confondre la texture et le bruit, 
  // on considere que si la différence est supérieure à un certain seuil, 
  // nous avons à faire à un contour, et on garde donc le pixel original.
  int seuilContour = 5 ;
  for (int i = 1; i < nH - 1 ; i++) {
    for (int j = 1; j < nW - 1; j++) {
      if (abs(ImgIn[3*i*nW+3*j] - getMedianeR(ImgIn, nW, i, j)) > seuilContour) {
        ImgMedian[3*i*nW+3*j] = ImgIn[3*i*nW+3*j] ;
      } else {
        ImgMedian[3*i*nW+3*j] = getMedianeR(ImgIn, nW, i, j) ;
      }
      if (abs(ImgIn[3*i*nW+3*j+1] - getMedianeG(ImgIn, nW, i, j)) > seuilContour) {
        ImgMedian[3*i*nW+3*j+1] = ImgIn[3*i*nW+3*j+1] ;
      } else {
        ImgMedian[3*i*nW+3*j+1] = getMedianeG(ImgIn, nW, i, j) ;
      }
      if (abs(ImgIn[3*i*nW+3*j+2] - getMedianeB(ImgIn, nW, i, j)) > seuilContour) {
        ImgMedian[3*i*nW+3*j+2] = ImgIn[3*i*nW+3*j+2] ;
      } else {
        ImgMedian[3*i*nW+3*j+2] = getMedianeB(ImgIn, nW, i, j) ;
      }
    }
  }

  // Isolation du bruit dans ImgBruit
  for (int i = 1; i < nH - 1 ; i++) {
    for (int j = 1; j < nW - 1; j++) {
      ImgBruit[3*i*nW+3*j] = 128 + (ImgIn[3*i*nW+3*j] - ImgMedian[3*i*nW+3*j]) ;
      ImgBruit[3*i*nW+3*j+1] = 128 + (ImgIn[3*i*nW+3*j+1] - ImgMedian[3*i*nW+3*j+1]) ;
      ImgBruit[3*i*nW+3*j+2] = 128 + (ImgIn[3*i*nW+3*j+2] - ImgMedian[3*i*nW+3*j+2]) ;
    }
  }

  // Augmentation du bruit par expansion dynamique
  // recherche des min et max
  int bruitMinR, bruitMaxR, bruitMinG, bruitMaxG, bruitMinB, bruitMaxB ;
  bruitMinR = ImgBruit[3*(nW+1)];
  bruitMaxR = ImgBruit[3*(nW+1)];
  bruitMinG = ImgBruit[3*(nW+1)+1];
  bruitMaxG = ImgBruit[3*(nW+1)+1];
  bruitMinB = ImgBruit[3*(nW+1)+2];
  bruitMaxB = ImgBruit[3*(nW+1)+2];
  for (int i = 1; i < nH - 1 ; i++) {
    for (int j = 1; j < nW - 1; j++) {
  	  if (ImgBruit[3*i*nW+3*j] > bruitMaxR) {
  			bruitMaxR = ImgBruit[3*i*nW+3*j] ;
  	  }
  	  if (ImgBruit[3*i*nW+3*j] < bruitMinR) {
  			bruitMinR = ImgBruit[3*i*nW+3*j] ;
  	  }
      if (ImgBruit[3*i*nW+3*j+1] > bruitMaxG) {
        bruitMaxG = ImgBruit[3*i*nW+3*j+1] ;
      }
      if (ImgBruit[3*i*nW+3*j+1] < bruitMinG) {
        bruitMinG = ImgBruit[3*i*nW+3*j+1] ;
      }
      if (ImgBruit[3*i*nW+3*j+2] > bruitMaxB) {
        bruitMaxB = ImgBruit[3*i*nW+3*j+2] ;
      }
      if (ImgBruit[3*i*nW+3*j+2] < bruitMinB) {
        bruitMinB = ImgBruit[3*i*nW+3*j+2] ;
      }
  	}
  }
  // expansion dynamique
  for (int i = 1; i < nH - 1; i++) {
    for (int j = 1; j < nW - 1; j++) {
			ImgBruit[3*i*nW+3*j] = (ImgBruit[3*i*nW+3*j] - bruitMinR) * (255/(bruitMaxR-bruitMinR)) ;
      ImgBruit[3*i*nW+3*j+1] = (ImgBruit[3*i*nW+3*j+1] - bruitMinG) * (255/(bruitMaxG-bruitMinG)) ;
      ImgBruit[3*i*nW+3*j+2] = (ImgBruit[3*i*nW+3*j+2] - bruitMinB) * (255/(bruitMaxB-bruitMinB)) ;
		}
	}

	// Remplissage de l'histogramme global du bruit
  for (int i = 0; i < 256; ++i) {
    histoR[i] = 0 ;
    histoG[i] = 0 ;
    histoB[i] = 0 ;
  }
  for (int i=0; i < nH; i++) {
    for (int j=0; j < nW; j++) {
      histoR[ImgBruit[3*i*nW+3*j]] ++ ;
      histoG[ImgBruit[3*i*nW+3*j+1]] ++ ;
      histoB[ImgBruit[3*i*nW+3*j+2]] ++ ;
    }
  }

  // Remplissage de la densité de probabilité globale
  double sommeR, sommeG, sommeB, densiteMoyenneR, densiteMoyenneG, densiteMoyenneB ;
  sommeR = 0 ;
  sommeG = 0 ;
  sommeB = 0 ;
  for (int i = 0; i < 256; ++i) {
    fR[i] = (double) histoR[i] / nTaille ;
    sommeR += fR[i] ;
    fG[i] = (double) histoG[i] / nTaille ;
    sommeG += fG[i] ;
    fB[i] = (double) histoB[i] / nTaille ;
    sommeB += fB[i] ;

  }
  densiteMoyenneR = sommeR / 256 ;
  densiteMoyenneG = sommeG / 256 ;
  densiteMoyenneB = sommeB / 256 ;

  FILE* fichier = fopen("dpp.dat", "w+");
  for (int i = 0; i < 256; ++i) {
    fprintf(fichier, "%f %f %f\n", fR[i], fG[i], fB[i]);
  }
  fclose(fichier);

	
	// Calcul de la différence entre la densité de probabilité du bruit et 
	// celle de chaque carré de (n*2 + 1)*(n*2 + 1) pixels dans ImgOut
	// affichage du pixel en blanc si la différence est suppérieur au seuil
  int n = 15 ;
  int nTailleCarre = (n*2 + 1)*(n*2 + 1) ;
  double seuil = 0.0019 ;

  double difR[256], difG[256], difB[256] ;
  double difMoyenneR, difMoyenneG, difMoyenneB ;
  // double sommeR, sommeG, sommeB ;
  double sommeDifMoyennes ;

	for (int i = n; i < nH - n; i++) {
		for (int j = n; j < nW - n; j++) {
			// Calcul de l'histogramme du carré local
      for (int k = 0; k < 256; k++) {
        histo2R[k] = 0 ;
        histo2G[k] = 0 ;
        histo2B[k] = 0 ;
      }
			for (int k = -n; k <= n ; k++) {
				for (int l = -n ; l <= n ; l++) {
          histo2R[ImgBruit[3*(i+k)*nW+3*(j+l)]] ++ ;
          histo2G[ImgBruit[3*(i+k)*nW+3*(j+l)+1]] ++ ;
          histo2B[ImgBruit[3*(i+k)*nW+3*(j+l)+2]] ++ ;
				}
			}

      // Calcul de la densité de probabilité locale
			for (int k = 0; k < 256; k++) {
        f2R[k] = (double) histo2R[k] / nTailleCarre ;
        f2G[k] = (double) histo2G[k] / nTailleCarre ;
        f2B[k] = (double) histo2B[k] / nTailleCarre ;
      }

      // Comparaison des densités de probabilités
      sommeR = 0 ;
      sommeG = 0 ;
      sommeB = 0 ;
      for (int k = 0 ; k < 256 ; k++ ) {
        difR[k] = absDouble(fR[k] - f2R[k]);
        sommeR += difR[k] ;
        difG[k] = absDouble(fG[k] - f2G[k]);
        sommeG += difG[k] ;
        difB[k] = absDouble(fB[k] - f2B[k]);
        sommeB += difB[k] ;
      }
      difMoyenneR = (double)sommeR/256 ;
      difMoyenneG = (double)sommeG/256 ;
      difMoyenneB = (double)sommeB/256 ;

      sommeDifMoyennes = difMoyenneR + difMoyenneG + difMoyenneB ;

			// Affichage du pixel en blanc si la différence est supérieure au pourcentage du seuil
			if (sommeDifMoyennes > seuil) {
				ImgOut[i*nW+j] = 255 ;
			} else {
				ImgOut[i*nW+j] = 0 ;
			}

		}
	}

  // Création des images
  ecrire_image_ppm(cNomImgMedian , ImgMedian,  nH, nW);
  ecrire_image_ppm(cNomImgBruit , ImgBruit,  nH, nW);

  // Enchainement de dilatations et erosions pour mettre en évidence les zones
  // avec des différences de bruit importantes

  ImgOut2 = dilatation(ImgOut, ImgOut2, nH, nW) ;
  ImgOut = dilatation(ImgOut2, ImgOut, nH, nW) ;
  ImgOut2 = dilatation(ImgOut, ImgOut2, nH, nW) ;

  ImgOut = erosion(ImgOut2, ImgOut, nH, nW) ;
  ImgOut2 = erosion(ImgOut, ImgOut2, nH, nW) ;
  ImgOut = erosion(ImgOut2, ImgOut, nH, nW) ;

  ecrire_image_pgm(cNomImgOut , ImgOut,  nH, nW);

  // Libération des espaces mémoires
  free(ImgIn);
  free(ImgMedian);
  free(ImgBruit);
  free(ImgOut);

  return 0 ;
}
