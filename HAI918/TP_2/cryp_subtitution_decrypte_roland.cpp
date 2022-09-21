// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"
#include <math.h>
#include <typeinfo>

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

float entropy(unsigned char* img, int nH, int nW){
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
  char cNomImgLue[250], cNomImgEcrite[250], test[250], number[250], extension[250];
  int nH, nW, nTaille, S;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm\n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);
   sscanf ("hello.pgm","%s",test);
  char test_2[250]="test";
  sprintf(extension, "%s", ".pgm");

  char *cat;
  asprintf(&cat, "%s%s%s", test_2, number, extension);
  puts(cat);

   OCTET *ImgIn,*ImageDechiffre;
   int *key ;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, unsigned char, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);;
   allocation_tableau(ImageDechiffre, unsigned char, nTaille);
   
  for(int k=0; k < 256; k++){
    srand(k);
    unsigned char newkey = (unsigned char)rand()%256;
    ImageDechiffre[0] = (ImgIn[0] - newkey)%256;
    for (int i=0; i < nTaille; i++){
      newkey = rand()%256;
      ImageDechiffre[i] = (ImgIn[i] - ImageDechiffre[i-1]-newkey)%256;
      }
    
    if(entropy(ImageDechiffre,nH, nW ) < 8){
      printf("entropy %f ", entropy(ImageDechiffre, nH,nW));
      printf("La clée est %i\n", k);
      ecrire_image_pgm(cNomImgEcrite, ImageDechiffre,  nH, nW);
    }
  }

   free(ImgIn);
   free(ImageDechiffre);
   return 1;
}
