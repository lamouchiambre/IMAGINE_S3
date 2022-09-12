// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"
#include <math.h>

float PSNR(OCTET *ImgA, OCTET *ImgB, int nH, int nW){
  float eqm = 0;
  for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
      eqm = ImgA[i*nW+j]*ImgB[i*nW+j];
    }

  eqm = eqm*(1/(nW*nH));
  return 10*log((255*255));

}

float ENT(float *Hist){
  float sum = 0.0;
  for(int i=0; i < 256; i++){
    //printf("log = %f\n",double(sum) + double(Hist[i]*log2(Hist[i])));
    sum += double((Hist[i]*log2(Hist[i])));
  }
  printf("%f\n", -sum);
  return -sum;
}

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, S;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm\n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);

   OCTET *ImgIn,*ImageDechiffre;
   int *key ;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);;
   allocation_tableau(ImageDechiffre, OCTET, nTaille);
   
  for(int k=0; k < 256; k++){
    srand(k);
    ImageDechiffre[0] = (ImgIn[0] - rand()%256)%256;
    for (int i=0; i < nH; i++)
      for (int j=0; j < nW; j++)
      {
        if(i+j != 0){
          ImageDechiffre[i*nW+j] = (ImgIn[i*nW+j] - ImgIn[i*nW+j-1]-rand()%256)%256;
          //printf("ImageDechiffre[i*nW+j] = %i\n", ImageDechiffre[i*nW+j]);
        }
      }
    
    float * tabHisto;
    allocation_tableau(tabHisto, float, 256);

    for(int i = 0; i < 256; i++){
      tabHisto[i] = 0;
    }

    for (int i=0; i < nH; i++)
      for (int j=0; j < nW; j++)
        tabHisto[ImgIn[i*nW+j]] += 1;
    
    for(int i = 0; i < 256; i++){
      tabHisto[i] = tabHisto[i]/nTaille;
    }
    if(ENT(tabHisto) < 7.9){
      printf("La clée est %i\n", k);
      ecrire_image_pgm(cNomImgEcrite, ImageDechiffre,  nH, nW);
    }
  }

  //Histogramme
  int * tabHisto;
  allocation_tableau(tabHisto, int, 256);
  for(int i = 0; i < 256; i++){
        tabHisto[i] = 0;
    }

    for (int i=0; i < nH; i++)
        for (int j=0; j < nW; j++)
            tabHisto[ImgIn[i*nW+j]] += 1;
    
  //ENT(tabHisto);
  //** Dechiffrage

   free(ImgIn);
   free(ImageDechiffre);
   return 1;
}
