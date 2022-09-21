// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"


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
/*
double PSNR(OCTET *ImgA, OCTET *ImgB, int nH, int nW){
  double eqm = 0;
  for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
      eqm += pow(ImgA[i*nW+j]-ImgB[i*nW+j],2);
    }
  
  eqm /= (nW*nH);
  return 10*log10(pow(255,2)/eqm);

}
*/
float entropy(OCTET* img, int nH, int nW){
    int count = 0;
    int* hist = new int[256];
    for (int i=0;i<256;i++){
        hist[i]=0;
    }
    for (int i=0; i<nH*nW; i++){
        hist[img[i]]++;
        count++;
    }

    float res = 0.0;
    for (int i=0; i<256; i++){
        if (hist[i]){
            res+=((float)hist[i]/(float)count)*log2((float)hist[i]/(float)count);
        }
    }
    return -res;
}

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250], cNomImgEcrite_2[250];
  int nH, nW, nTaille, S;
  
  if (argc != 4) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm ImageDechiffre.pgm \n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);
   sscanf (argv[3],"%s",cNomImgEcrite_2);

   OCTET *ImgIn, *ImgOut, *ImageDechiffre;
   //int *key ;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);
   //allocation_tableau(key, int, nTaille);
   allocation_tableau(ImageDechiffre, OCTET, nTaille);
   
  //char test[250];
  //ecrire_image_pgm(test, mapPixel,  nH, nW);
  //int seed = 
  srand(23);

  //** Chiffrage
  ImgOut[0] = (rand()%256 + ImgIn[0])%256;
  for (int i=0; i < nH; i++)
    for (int j=0; j < nW; j++)
     {
      if(i+j != 0){
        ImgOut[i*nW+j] = (ImgOut[i*nW+j-1] + ImgIn[i*nW+j]+rand()%256)%256;
      }
     }

    //printf("comp %i, RAND_MAX %i \n",comp, RAND_MAX);
  printf("PSNR %f\n",PSNR(ImgIn, ImgOut,nH,nW));
  printf("Entropy Image Décrypter %f\n",entropy(ImgIn, nH, nW));
  printf("Entropy Image Crypter %f\n",entropy(ImgOut, nH, nW));
    //Histogramme
  int * tabHisto;
  allocation_tableau(tabHisto, int, 256);
  for(int i = 0; i < 256; i++){
        tabHisto[i] = 0;
    }

    for (int i=0; i < nH; i++)
        for (int j=0; j < nW; j++)
            tabHisto[ImgOut[i*nW+j]] += 1;
        
    for (int i = 0; i < 256; i++)
    {
      printf("%d %d \n", i, tabHisto[i]);
    }
  srand(23);
  //** Dechiffrage
  ImageDechiffre[0] = (ImgOut[0] - rand()%256)%256;
  for (int i=0; i < nH; i++)
    for (int j=0; j < nW; j++)
     {
      if(i+j != 0){
        ImageDechiffre[i*nW+j] = (ImgOut[i*nW+j] - ImgOut[i*nW+j-1]-rand()%256)%256;
      }
     }

  ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
  ecrire_image_pgm(cNomImgEcrite_2, ImageDechiffre,  nH, nW);


    

  //** Dechiffrage

   free(ImgIn);
   free(ImgOut);
   return 1;
}
