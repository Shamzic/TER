#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "includes/image_ppm.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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

OCTET* erosion(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW) {
  for (int i = 1 ; i < nH - 1 ; i++) {
    for (int j = 1 ; j < nW - 1 ; j++) {
      ImgOut[i*nW+j] = getMin(ImgIn[i*nW+(j-1)], ImgIn[(i-1)*nW+j], ImgIn[(i+1)*nW+j], ImgIn[i*nW+(j+1)]) ;
    }
  }
  return ImgOut ;
}

OCTET* dilatation(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW) {
  for (int i = 1; i < nH - 1; i++) {
    for (int j = 1; j < nW - 1; j++) {
      ImgOut[i*nW+j] = getMax(ImgIn[i*nW+(j-1)], ImgIn[(i-1)*nW+j], ImgIn[(i+1)*nW+j], ImgIn[i*nW+(j+1)]) ;
    }
  }
  return ImgOut ;
}

OCTET* dilatationRGB(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW) {
  for (int i = 1 ; i < nH - 1 ; i++) {
    for (int j = 1 ; j < nW - 1 ; j++) {
      //ImgOut[3*i*nW+j] =   getMin(ImgIn[3*i*nW+(j-1)],   ImgIn[3*(i-1)*nW+j],   ImgIn[3*(i+1)*nW+j],   ImgIn[3*i*nW+(j+1)]) ;
      ImgOut[3*i*nW+j+1] = getMin(ImgIn[3*i*nW+(j-1)+1], ImgIn[3*(i-1)*nW+j+1], ImgIn[3*(i+1)*nW+j+1], ImgIn[3*i*nW+(j+1)+1]) ;
      //ImgOut[3*i*nW+j+2] = getMin(ImgIn[3*i*nW+(j-1)+2], ImgIn[3*(i-1)*nW+j+2], ImgIn[3*(i+1)*nW+j+2], ImgIn[3*i*nW+(j+1)+2]) ;
    }
  }
  return ImgOut ;
}


int main(int argc, char* argv[]) {

  char cNomImgIn[250], cNomImgOut[250], cNomImgBinOut[250], cNomImgMedian[250], cNomImgBruit[250], cNomImgMini[250], cNomImgBinFinal[250], cNomImgBinTemp[250];

  int nH, nW, nTaille, nbcasesW, nbcasesH;
  double seuil;

  if (argc < 12) {
    printf("Usage: ImgIn.pgm ImgOut.ppm  nbcasesW nbcasesH seuilMoyenneHisto ImgBinOut.ppm ImgMedian.ppm ImgBruit.ppm ImgMini.ppm ImgBinFinal.ppm ImgBinTemp.ppm\n"); 
    exit (1) ;
  }

  sscanf (argv[1],"%s",cNomImgIn);
  sscanf (argv[2],"%s",cNomImgOut);
  sscanf (argv[3],"%d",&nbcasesW);
  sscanf (argv[4],"%d",&nbcasesH);
  sscanf (argv[5],"%lf",&seuil);
  sscanf (argv[6],"%s",cNomImgBinOut);
  sscanf (argv[7],"%s",cNomImgMedian);
  sscanf (argv[8],"%s",cNomImgBruit);
  sscanf (argv[9],"%s",cNomImgMini);
  sscanf (argv[10],"%s",cNomImgBinFinal);
  sscanf (argv[11],"%s",cNomImgBinTemp);

  OCTET *ImgIn, *ImgOut, *ImgBinOut, *ImgBruit, *ImgMedian, *ImgOut2, *ImgMini, *ImgBinFinal, *ImgMiniTemp;

  lire_nb_lignes_colonnes_image_ppm(cNomImgIn, &nH, &nW);
  nTaille = nH * nW;
  std::cout<<"taille "<<nH<<" * "<<nW<<std::endl;

  allocation_tableau(ImgIn, OCTET, 3*nTaille);
  lire_image_ppm(cNomImgIn, ImgIn, nH * nW);
  
  allocation_tableau(ImgOut, OCTET, 3*nTaille);
  allocation_tableau(ImgBruit, OCTET, 3*nTaille);
  allocation_tableau(ImgMedian, OCTET, 3*nTaille);
  allocation_tableau(ImgOut2, OCTET, nTaille);
  allocation_tableau(ImgBinOut, OCTET, nTaille);
  allocation_tableau(ImgMini, OCTET, nbcasesW * nbcasesH);
  allocation_tableau(ImgMiniTemp, OCTET, nbcasesW * nbcasesH);
  allocation_tableau(ImgBinFinal, OCTET, nTaille);

  int nb_cases_l = nbcasesH;
  int nb_cases_h = nbcasesW;

  int largeur_case = nW/nb_cases_l;
  int hauteur_case = nH/nb_cases_h;

  int histoR[256],histoG[256], histoB[256];
  int histoR_general[256],histoG_general[256], histoB_general[256];
  int nR, nG, nB;
  //double seuil = 0.005 ;
  double difR[256], difG[256], difB[256] ;
  double difMoyenneR, difMoyenneG, difMoyenneB ;
  double sommeDifMoyennes;
  double fR[256], fG[256], fB[256], f2R[256], f2G[256], f2B[256];

  // Création d'une nouvelle image avec le filtre median, moyen ou gaussien -> ImgMedian
  
  // Pour ne pas confondre la texture et le bruit, 
  // on considere que si la différence est supérieure à un certain seuil, 
  // nous avons à faire à un contour, et on garde donc le pixel original.
/*  int seuilContour = 5 ;
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
*/


 double moyenneR, moyenneG, moyenneB, ecartTypeR, ecartTypeG, ecartTypeB ;
  int R = 0 ;
  int G = 0 ;
  int B = 0 ;
  for (int i = 0; i < nH ; i++) {
    for (int j = 0; j < nW ; j++) {
      R += ImgIn[3*i*nW+3*j] ;
      G += ImgIn[3*i*nW+3*j+1] ;
      B += ImgIn[3*i*nW+3*j+2] ;
    }
  }
  moyenneR = R / nTaille ;
  moyenneG = G / nTaille ;
  moyenneB = B / nTaille ;
  R = 0 ;
  G = 0 ;
  B = 0 ;
  for (int i = 0; i < nH ; i++) {
    for (int j = 0; j < nW ; j++) {
      R += abs(ImgIn[3*i*nW+3*j] - moyenneR) ;
      G += abs(ImgIn[3*i*nW+3*j+1] - moyenneG) ;
      B += abs(ImgIn[3*i*nW+3*j+2] - moyenneB) ;
    }
  }

  ecartTypeR = (double) R / nTaille ;
  ecartTypeG = (double) G / nTaille ;
  ecartTypeB = (double) B / nTaille ;
  int seuilTextureR = ecartTypeR / 10 ;
  int seuilTextureG = ecartTypeG / 10 ;
  int seuilTextureB = ecartTypeB / 10 ;

  for (int i = 1; i < nH - 1 ; i++) {
    for (int j = 1; j < nW - 1; j++) {
      if (abs(ImgIn[3*i*nW+3*j] - getMedianeR(ImgIn, nW, i, j)) > seuilTextureR) {
        ImgMedian[3*i*nW+3*j] = ImgIn[3*i*nW+3*j] ;
        ImgBruit[3*i*nW+3*j] = 0 ;
      } else {
        ImgMedian[3*i*nW+3*j] = getMedianeR(ImgIn, nW, i, j) ;
        ImgBruit[3*i*nW+3*j] = 128 + (ImgIn[3*i*nW+3*j] - ImgMedian[3*i*nW+3*j]) ;
      }
      if (abs(ImgIn[3*i*nW+3*j+1] - getMedianeG(ImgIn, nW, i, j)) > seuilTextureG) {
        ImgMedian[3*i*nW+3*j+1] = ImgIn[3*i*nW+3*j+1] ;
        ImgBruit[3*i*nW+3*j+1] = 0 ;
      } else {
        ImgMedian[3*i*nW+3*j+1] = getMedianeG(ImgIn, nW, i, j) ;
        ImgBruit[3*i*nW+3*j+1] = 128 + (ImgIn[3*i*nW+3*j+1] - ImgMedian[3*i*nW+3*j+1]) ;
      }
      if (abs(ImgIn[3*i*nW+3*j+2] - getMedianeB(ImgIn, nW, i, j)) > seuilTextureB) {
        ImgMedian[3*i*nW+3*j+2] = ImgIn[3*i*nW+3*j+2] ;
        ImgBruit[3*i*nW+3*j+2] = 0 ;
      } else {
        ImgMedian[3*i*nW+3*j+2] = getMedianeB(ImgIn, nW, i, j) ;
        ImgBruit[3*i*nW+3*j+2] = 128 + (ImgIn[3*i*nW+3*j+2] - ImgMedian[3*i*nW+3*j+2]) ;
      }
    }
  }

  // Augmentation du bruit par expansion dynamique
  // recherche des min et max
  int bruitMinR, bruitMaxR, bruitMinG, bruitMaxG, bruitMinB, bruitMaxB ;
  bruitMinR = 128 ;
  bruitMaxR = 128 ;
  bruitMinG = 128 ;
  bruitMaxG = 128 ;
  bruitMinB = 128 ;
  bruitMaxB = 128 ;
  for (int i = 1; i < nH - 1 ; i++) {
    for (int j = 1; j < nW - 1; j++) {
      if (ImgBruit[3*i*nW+3*j] > bruitMaxR) {
        bruitMaxR = ImgBruit[3*i*nW+3*j] ;
      }
      if (ImgBruit[3*i*nW+3*j] < bruitMinR && ImgBruit[3*i*nW+3*j] > 0) {
        bruitMinR = ImgBruit[3*i*nW+3*j] ;
      }
      if (ImgBruit[3*i*nW+3*j+1] > bruitMaxG) {
        bruitMaxG = ImgBruit[3*i*nW+3*j+1] ;
      }
      if (ImgBruit[3*i*nW+3*j+1] < bruitMinG && ImgBruit[3*i*nW+3*j+1] > 0) {
        bruitMinG = ImgBruit[3*i*nW+3*j+1] ;
      }
      if (ImgBruit[3*i*nW+3*j+2] > bruitMaxB) {
        bruitMaxB = ImgBruit[3*i*nW+3*j+2] ;
      }
      if (ImgBruit[3*i*nW+3*j+2] < bruitMinB && ImgBruit[3*i*nW+3*j+2] > 0) {
        bruitMinB = ImgBruit[3*i*nW+3*j+2] ;
      }
    }
  }
/*  bruitMinR = ImgBruit[3*(nW+1)];
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
  }*/
  // expansion dynamique
  for (int i = 1; i < nH - 1; i++) {
    for (int j = 1; j < nW - 1; j++) {
      if(ImgBruit[3*i*nW+3*j]!=0)
        ImgBruit[3*i*nW+3*j] = (ImgBruit[3*i*nW+3*j] - bruitMinR) * (255/(bruitMaxR-bruitMinR)) ;

      if(ImgBruit[3*i*nW+3*j+1]!=0)
        ImgBruit[3*i*nW+3*j+1] = (ImgBruit[3*i*nW+3*j+1] - bruitMinG) * (255/(bruitMaxG-bruitMinG)) ;

      if(ImgBruit[3*i*nW+3*j+2]!=0)
        ImgBruit[3*i*nW+3*j+2] = (ImgBruit[3*i*nW+3*j+2] - bruitMinB) * (255/(bruitMaxB-bruitMinB)) ;
    }
  }


  // Init tableau histogramme case (posl, posh)
  for(int i=0;i<256;i++)
  {
    histoR_general[i]=0;
    histoG_general[i]=0;
    histoB_general[i]=0;
  }

  for(int i = 0 ; i <nH; i++) 
  {
    for(int j = 0 ; j < nW; j++) 
    {    
        nR = ImgBruit[3*i*nW+3*j];
        nG = ImgBruit[3*i*nW+3*j+1];
        nB = ImgBruit[3*i*nW+3*j+2];
        histoR_general[nR]++;
        histoG_general[nG]++;
        histoB_general[nB]++;
    }
  }

// Remplissage de la densité de probabilité globale
  double sommeR, sommeG, sommeB;//, densiteMoyenneR, densiteMoyenneG, densiteMoyenneB ;
  sommeR = 0 ;
  sommeG = 0 ;
  sommeB = 0 ;
  for (int i = 0; i < 256; ++i) {
    fR[i] = (double) histoR_general[i] / nTaille ;
    sommeR += fR[i] ;
    fG[i] = (double) histoG_general[i] / nTaille ;
    sommeG += fG[i] ;
    fB[i] = (double) histoB_general[i] / nTaille ;
    sommeB += fB[i] ;

  }

  std::string nameHisto= "histos/Histo_GENERAL";
  nameHisto.append(".dat");
  std::ofstream fichier(nameHisto, std::ios::out | std::ios::trunc); 
  if(fichier)
  {
    for (int i = 0; i<256; i++) 
    {
      fichier<<i<<" "<<histoR_general[i]<<" "<<histoG_general[i]<<" "<<histoB_general[i]<<"\n" ;
    }
    fichier.close();
  }
  else
    std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;

/*  std::vector< std::vector<int*> >* HistosR = new std::vector< std::vector<int*> >();
  std::vector< std::vector<int*> >* HistosG = new std::vector< std::vector<int*> >();
  std::vector< std::vector<int*> >* HistosB = new std::vector< std::vector<int*> >();*/

  //std::cout<<"largeur et hauteur :  "<<largeur_case<<" , "<<hauteur_case <<std::endl;
  int posl = 0;
  int posh = 0;

  double minS = 1000, maxS  = -1000;

  for(int hn = 0; hn < nb_cases_h; hn++)
  {
    for(int ln = 0; ln < nb_cases_l; ln++)
    {          
      //std::cout<<"case "<<posl<<" , "<<posh <<std::endl;
      for(int i = posl ; i < (posl+hauteur_case); i++) 
      {
        for(int j = posh ; j < (posh+largeur_case); j++) 
        {      

          ImgOut[3*i*nW+3*j]=ImgBruit[3*i*nW+3*j];
          ImgOut[3*i*nW+3*j+1]=ImgBruit[3*i*nW+3*j+1];
          ImgOut[3*i*nW+3*j+2]=ImgBruit[3*i*nW+3*j+2];

          // Séparateurs blancs pour visualiser la division 
          // (a mettre en commentaire pour que le pgrm fonctionne correctement)
          if((i == posl) || (j == posh) || (i == posl+hauteur_case-1) || (j == posh+largeur_case-1))
          {
            ImgOut[3*i*nW+3*j]=255;
            ImgOut[3*i*nW+3*j+1]=255;
            ImgOut[3*i*nW+3*j+2]=255;
          }
        }


        // Remplissage de l'histogramme du patch en cours
        for(int i=0;i<256;i++)
        {
          histoR[i]=0;
          histoG[i]=0;
          histoB[i]=0;
        }
        for(int i = posl ; i < (posl+hauteur_case); i++) 
        {
          for(int j = posh ; j < (posh+largeur_case); j++) 
          {  
            nR = ImgBruit[3*i*nW+3*j];
            nG = ImgBruit[3*i*nW+3*j+1];
            nB = ImgBruit[3*i*nW+3*j+2];
            histoR[nR]++;
            histoG[nG]++;
            histoB[nB]++;
          }
        }

        int nTailleCarre = hauteur_case * largeur_case;
        // Calcul de la densité de probabilité locale
        for (int k = 0; k < 256; k++) {
          f2R[k] = (double) histoR[k] / nTailleCarre ;
          f2G[k] = (double) histoG[k] / nTailleCarre ;
          f2B[k] = (double) histoB[k] / nTailleCarre ;
        }

        // Comparaison des densités de probabilités
        sommeR = 0 ;
        sommeG = 0 ;
        sommeB = 0 ;
        for (int k = 0 ; k < 256 ; k++ ) {
          difR[k] = std::abs(fR[k] - f2R[k]);
          sommeR += difR[k] ;
          difG[k] = std::abs(fG[k] - f2G[k]);
          sommeG += difG[k] ;
          difB[k] = std::abs(fB[k] - f2B[k]);
          sommeB += difB[k] ;
        }
        difMoyenneR = (double)sommeR/256 ;
        difMoyenneG = (double)sommeG/256 ;
        difMoyenneB = (double)sommeB/256 ;

        sommeDifMoyennes = difMoyenneR + difMoyenneG + difMoyenneB ;


        if(sommeDifMoyennes<minS)
          minS = sommeDifMoyennes;
        if(sommeDifMoyennes>maxS)
          maxS=sommeDifMoyennes;

        if (sommeDifMoyennes < seuil) 
         // mettre zone en blanc dans img binaire de sortie
        {
          for(int i = posl ; i < (posl+hauteur_case); i++) 
          {
            for(int j = posh ; j < (posh+largeur_case); j++) 
            {      
              ImgBinOut[i*nW+j]= 0;
            }
          }
        }
        else
        {
          for(int i = posl ; i < (posl+hauteur_case); i++) 
          {
            for(int j = posh ; j < (posh+largeur_case); j++) 
            {      
              ImgBinOut[i*nW+j]= 255;
            }
          }
        }




/*      HistosR->at(ln).at(hn) = histoR;
        HistosG->at(ln).at(hn) = histoG;
        HistosB->at(ln).at(hn) = histoB;*/
        std::string nameHistot = "histos/Histo";
        nameHistot.append(std::to_string(hn));
        nameHistot.append("_");
        nameHistot.append(std::to_string(ln));
        nameHistot.append(".dat");

      // + (std::string)hn+"_"+(std::string)ln+".dat";
/*      std::string nameHistoR = "histoG"+ (std::string)hn+"_"+(std::string)ln+".dat";
        std::string nameHistoR = "HistoB"+ (std::string)hn+"_"+(std::string)ln+".dat";*/

      std::ofstream fichier(nameHistot, std::ios::out | std::ios::trunc); 

      if(fichier)
      {
        for (int i = 0; i<256; i++) 
        {
          fichier<<i<<" "<<histoR[i]<<" "<<histoG[i]<<" "<<histoB[i]<<"\n" ;
        }
        fichier.close();
      }
      else
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
      }    


      posl+=hauteur_case; 
      if(posl>(nH-hauteur_case))
        posl=0;   
    }
    posh+=largeur_case;
    if(posh>(nW-largeur_case))
        posh=0;  
  }



  posl = 0;
  posh = 0;

  std::cout<<"max : "<<maxS<<"\n";
  std::cout<<"min : "<<minS<<"\n";


  double alpha = (-255.0*(double)(minS))/255.0;
  double beta = 255.0/(double)(maxS-minS);
  for(int hn = 0; hn < nb_cases_h; hn++)
  {
    for(int ln = 0; ln < nb_cases_l; ln++)
    { 
        // Remplissage de l'histogramme du patch en cours
        for(int i=0;i<256;i++)
        {
          histoR[i]=0;
          histoG[i]=0;
          histoB[i]=0;
        }
        for(int i = posl ; i < (posl+hauteur_case); i++) 
        {
          for(int j = posh ; j < (posh+largeur_case); j++) 
          {  
            nR = ImgBruit[3*i*nW+3*j];
            nG = ImgBruit[3*i*nW+3*j+1];
            nB = ImgBruit[3*i*nW+3*j+2];
            histoR[nR]++;
            histoG[nG]++;
            histoB[nB]++;
          }
        }

        int nTailleCarre = hauteur_case * largeur_case;
        // Calcul de la densité de probabilité locale
        for (int k = 0; k < 256; k++) {
          f2R[k] = (double) histoR[k] / nTailleCarre ;
          f2G[k] = (double) histoG[k] / nTailleCarre ;
          f2B[k] = (double) histoB[k] / nTailleCarre ;
        }
        sommeR = 0 ;
        sommeG = 0 ;
        sommeB = 0 ;
        for (int k = 0 ; k < 256 ; k++ ) {
          difR[k] = std::abs(fR[k] - f2R[k]);
          sommeR += difR[k] ;
          difG[k] = std::abs(fG[k] - f2G[k]);
          sommeG += difG[k] ;
          difB[k] = std::abs(fB[k] - f2B[k]);
          sommeB += difB[k] ;
        }
        difMoyenneR = (double)sommeR/256 ;
        difMoyenneG = (double)sommeG/256 ;
        difMoyenneB = (double)sommeB/256 ;

        sommeDifMoyennes = difMoyenneR + difMoyenneG + difMoyenneB ;


        for(int i = posl ; i < (posl+hauteur_case); i++) 
        {
          for(int j = posh ; j < (posh+largeur_case); j++) 
          {  
            if(sommeDifMoyennes>seuil /*+ 20*seuil/100)*/)
              ImgBinOut[i*nW+j] = 255;
/*            else if(sommeDifMoyennes<(seuil - 20*seuil/100))
              ImgBinOut[i*nW+j] = 0;*/
            else
            ImgBinOut[i*nW+j] = alpha + beta*sommeDifMoyennes;


          }
        }

      posl+=hauteur_case; 
      if(posl>(nH-hauteur_case))
        posl=0;   
    }
    posh+=largeur_case;
    if(posh>(nW-largeur_case))
        posh=0;  
  }


  for(int i = 0; i< nbcasesH; i++)
  {
    for(int j= 0 ; j < nbcasesW; j++)
    {
      ImgMini[i*nbcasesW+j]= ImgBinOut[i*hauteur_case*nW+j*largeur_case];
    }
  }
/*  ImgMiniTemp = erosion(ImgMini, ImgMiniTemp, nbcasesH, nbcasesW) ;
  ImgMini = dilatation(ImgMiniTemp, ImgMini, nbcasesH, nbcasesW) ;*/

  ecrire_image_pgm(cNomImgBinTemp, ImgBinOut,  nH, nW);

  ImgMiniTemp = erosion(ImgMini, ImgMiniTemp, nbcasesH, nbcasesW) ;
  ecrire_image_pgm(cNomImgMini, ImgMiniTemp,  nbcasesH, nbcasesW);
  
  posl = 0;
  posh = 0;

  for(int hn = 0; hn < nb_cases_h; hn++)
  {
    for(int ln = 0; ln < nb_cases_l; ln++)
    {
      for(int i = posl ; i < (posl+hauteur_case); i++) 
      {
        for(int j = posh ; j < (posh+largeur_case); j++) 
        {
          ImgBinOut[i*nW+j]=ImgMiniTemp[hn+ln*nbcasesW];
        }
      }
     posl+=hauteur_case; 
      if(posl>(nH-hauteur_case))
        posl=0;   
    }
    posh+=largeur_case;
    if(posh>(nW-largeur_case))
        posh=0;  
  }

  ecrire_image_ppm(cNomImgMedian , ImgMedian,  nH, nW);
  ecrire_image_ppm(cNomImgBruit, ImgBruit,  nH, nW);

  ecrire_image_ppm(cNomImgOut , ImgOut,  nH, nW);

  ecrire_image_pgm(cNomImgBinOut , ImgBinOut,  nH, nW);

  free(ImgIn);
  free(ImgMedian);
  free(ImgBruit);
  free(ImgBinOut);

  return 0;
}
