// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"
/*
int write_pixel(OCTET *ImgIn, *ImgOut, *mapPixel, int nH, int nW){
  nTaille = nH * nW;


  return 0;

}*/

float PSNR(OCTET *ImgA, OCTET *ImgB, int nH, int nW){
  float eqm = 0;
  for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
      eqm = ImgA[i*nW+j]*ImgB[i*nW+j];
    }

  eqm = eqm*(1/(nW*nH));
  return 10*log((255*255)/);

}

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, S;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm \n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);

   OCTET *ImgIn, *ImgOut, *mapPixel, *mapPixelD ;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);
   allocation_tableau(mapPixel, OCTET, nTaille);
   allocation_tableau(mapPixelD, OCTET, nTaille);
  //char test[250];
  //ecrire_image_pgm(test, mapPixel,  nH, nW);

  int key = rand();
  int pos = 0;
/*
   for (int i=0; i < nH; i++)
    for (int j=0; j < nW; j++)
      printf("%i",mapPixel[i*nW+j]);
*/

  //** CHiffrage
 for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
       /*
       do{
        pos = (rand()%key)%(nH * nW);
        }while(mapPixel[pos] == 1);
      */
      pos = (rand()%key)%(nH * nW);
      while(mapPixel[pos] == 1){
        pos = (pos+1)%nTaille;
      }

       ImgOut[pos] = ImgIn[i*nW+j];
       mapPixel[pos] = 1;
     }
  PSNR(ImgIn, ImgOut,nH,nW)
   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);

  //Histogramme
  int * tabHisto;
  allocation_tableau(tabHisto, int, 256);
  for(int i = 0; i < 256; i++){
        tabHisto[i] = 0;
    }

    for (int i=0; i < nH; i++)
        for (int j=0; j < nW; j++)
            tabHisto[ImgIn[i*nW+j]] += 1;
        
    for (int i = 0; i < 256; i++)
    {
      printf("%d %d \n", i, tabHisto[i]);
    }
    

  //** Dechiffrage

   free(ImgIn);
   free(ImgOut);
   return 1;
}
