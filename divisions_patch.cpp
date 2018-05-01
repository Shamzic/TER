#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "includes/image_ppm.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {

  char cNomImgIn[250], cNomImgOut[250], cNomImgBinOut[250] ;

  int nH, nW, nTaille, nbcasesW, nbcasesH, seuil;

  if (argc < 7) {
    printf("Usage: ImgIn.pgm ImgOut.ppm  nbcasesW nbcasesH seuilMoyenneHisto ImgBinOut.ppm\n"); 
    exit (1) ;
  }

  sscanf (argv[1],"%s",cNomImgIn);
  sscanf (argv[2],"%s",cNomImgOut);
  sscanf (argv[3],"%d",&nbcasesW);
  sscanf (argv[4],"%d",&nbcasesH);
  sscanf (argv[5],"%d",&seuil);
  sscanf (argv[6],"%s",cNomImgBinOut);

  OCTET *ImgIn, *ImgOut, *ImgBinOut;

  lire_nb_lignes_colonnes_image_ppm(cNomImgIn, &nH, &nW);
  nTaille = nH * nW;
  std::cout<<"taille "<<nH<<" * "<<nW<<std::endl;

  allocation_tableau(ImgIn, OCTET, 3*nTaille);
  lire_image_ppm(cNomImgIn, ImgIn, nH * nW);
  
  allocation_tableau(ImgOut, OCTET, 3*nTaille);
  allocation_tableau(ImgBinOut, OCTET, 3*nTaille);

  int nb_cases_l = nbcasesH;
  int nb_cases_h = nbcasesW;

  int largeur_case = nW/nb_cases_l;
  int hauteur_case = nH/nb_cases_h;

  int histoR[256],histoG[256], histoB[256];
  int histoR_general[256],histoG_general[256], histoB_general[256];
  int nR, nG, nB;
  float moyenneR_histoG=0, moyenneG_histoG=0, moyenneB_histoG=0;//, moyenneglob_general=0;
  float moyenneR_t=0, moyenneG_t=0, moyenneB_t=0;//, moyenneglob_t=0;
  float sommeDifR_histoG=0, sommeDifG_histoG=0, sommeDifB_histoG=0;//, sommeDifglob_general=0;
  float sommeDifR_t=0, sommeDifG_t=0, sommeDifB_t=0, sommeDifglob_t=0;

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
        nR = ImgIn[3*i*nW+3*j];
        nG = ImgIn[3*i*nW+3*j+1];
        nB = ImgIn[3*i*nW+3*j+2];
        histoR_general[nR]++;
        histoG_general[nG]++;
        histoB_general[nB]++;
    }
  }

  for(int i=0;i<256;i++)
  {
    moyenneR_histoG += histoR_general[i];
    moyenneG_histoG += histoG_general[i];
    moyenneB_histoG += histoB_general[i];
  }
  moyenneR_histoG/=255;
  moyenneG_histoG/=255;
  moyenneB_histoG/=255;

    for(int i=0;i<256;i++)
  {
    sommeDifR_histoG += histoR_general[i];
    sommeDifG_histoG += histoG_general[i];
    sommeDifB_histoG += histoB_general[i];
  }
  sommeDifR_histoG/=255;
  sommeDifG_histoG/=255;
  sommeDifB_histoG/=255;



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

  std::cout<<"largeur et hauteur :  "<<largeur_case<<" , "<<hauteur_case <<std::endl;
  int posl = 0;
  int posh = 0;

  for(int hn = 0; hn < nb_cases_h; hn++)
  {
    for(int ln = 0; ln < nb_cases_l; ln++)
    {          
      std::cout<<"case "<<posl<<" , "<<posh <<std::endl;
      for(int i = posl ; i < (posl+hauteur_case); i++) 
      {
        for(int j = posh ; j < (posh+largeur_case); j++) 
        {      

          ImgOut[3*i*nW+3*j]=ImgIn[3*i*nW+3*j];
          ImgOut[3*i*nW+3*j+1]=ImgIn[3*i*nW+3*j+1];
          ImgOut[3*i*nW+3*j+2]=ImgIn[3*i*nW+3*j+2];

          // Séparateurs blancs pour visualiser la division 
          // (a mettre en commentaire pour que le pgrm fonctionne correctement)
          if((i == posl) || (j == posh) || (i == posl+hauteur_case-1) || (j == posh+largeur_case-1))
          {
            ImgOut[3*i*nW+3*j]=255;
            ImgOut[3*i*nW+3*j+1]=255;
            ImgOut[3*i*nW+3*j+2]=255;
          }
        }


        // Init tableau histogramme case (posl, posh)
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
            nR = ImgIn[3*i*nW+3*j];
            nG = ImgIn[3*i*nW+3*j+1];
            nB = ImgIn[3*i*nW+3*j+2];
            histoR[nR]++;
            histoG[nG]++;
            histoB[nB]++;
          }
        }


        for(int i=0;i<256;i++)
        {
          moyenneR_t += histoR[i];
          moyenneG_t += histoG[i];
          moyenneB_t += histoB[i];
        }
        moyenneR_t/=255;
        moyenneG_t/=255;
        moyenneB_t/=255;

        //moyenneglob_t = (moyenneR_t+moyenneG_t+moyenneB_t)/3;

        sommeDifR_t=0;
        sommeDifG_t=0;
        sommeDifB_t=0;

        for(int i=0;i<256;i++)
        {
          sommeDifR_t += std::abs(histoR_general[i] - histoR[i]);
          sommeDifG_t += std::abs(histoG_general[i] - histoG[i]);
          sommeDifB_t += std::abs(histoB_general[i] - histoB[i]);
        }
        sommeDifR_t/=255;
        sommeDifG_t/=255;
        sommeDifB_t/=255;

         sommeDifglob_t = (sommeDifR_t+sommeDifG_t+sommeDifB_t)/3;

        //if(std::abs(moyenneglob_general-moyenneglob_t)<seuil)
         if(sommeDifglob_t<seuil)
         // mettre zone en blanc dans img binaire de sortie
        {
          for(int i = posl ; i < (posl+hauteur_case); i++) 
          {
            for(int j = posh ; j < (posh+largeur_case); j++) 
            {      

              ImgBinOut[3*i*nW+3*j]= 0;
              ImgBinOut[3*i*nW+3*j+1]= 255;
              ImgBinOut[3*i*nW+3*j+2]= 0;
            }
          }
        }
        else
        {
          for(int i = posl ; i < (posl+hauteur_case); i++) 
          {
            for(int j = posh ; j < (posh+largeur_case); j++) 
            {      

              ImgBinOut[3*i*nW+3*j]= 0;
              ImgBinOut[3*i*nW+3*j+1]= 0;
              ImgBinOut[3*i*nW+3*j+2]= 0;
            }
          }
        }




/*        HistosR->at(ln).at(hn) = histoR;
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

        // Remplissage histogramme
      }    
      std::cout<<"sommeDifglob_t : "<<sommeDifglob_t<<std::endl;

      posl+=hauteur_case; 
      if(posl>(nH-hauteur_case))
        posl=0;   
    }
    posh+=largeur_case;
    if(posh>(nW-largeur_case))
        posh=0;  
  }

  ecrire_image_ppm(cNomImgOut , ImgOut,  nH, nW);
  ecrire_image_ppm(cNomImgBinOut , ImgBinOut,  nH, nW);

  free(ImgIn);

  return 0;
}
