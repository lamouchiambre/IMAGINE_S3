// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"

int* Histogramme(OCTET *ImgIn, int nW, int nH){
  int * tabHisto;
  allocation_tableau(tabHisto, int, 256);
  for(int i = 0; i < 256; i++){
    tabHisto[i] = 0;
  }

  for (int i=0; i < nH; i++)
    for (int j=0; j < nW; j++)
      tabHisto[ImgIn[i*nW+j]] += 1;
  return tabHisto;
}

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

float ENT(float *Hist){
  float sum = 0.0;
  for(int i=0; i < 256; i++){
    //printf("log = %f\n",double(sum) + double(Hist[i]*log2(Hist[i])));
    sum += double((Hist[i]*log2(Hist[i])));
  }
  // printf("%f\n", -sum);
  return -sum;
}
float ENT_2(OCTET *ImgIn, int nH, int nW){
  int *Hist = Histogramme(ImgIn, nW, nH);
  float sum = 0.0;
  for(int i=0; i < 256; i++){
    //printf("log = %f\n",double(sum) + double(Hist[i]*log2(Hist[i])));
    sum += double((Hist[i]*log2(Hist[i])));
  }
  // printf("%f\n", -sum);
  for (int i = 0; i < 256; i++){
    //printf("%d, %d \n", i, Hist[i]);
  }
  return -sum;
}
int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250],cNomImgDecr[250];
  int nH, nW, nTaille, S;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm \n"); 
       exit (1) ;
     }
   
  sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%s",cNomImgEcrite);
  sscanf ("img_out/01_melange_decript.pgm","%s",cNomImgDecr);

   OCTET *ImgIn, *ImgOut,*ImgDecr, *mapPixel, *mapPixelD ;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
 
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);
   allocation_tableau(ImgDecr, OCTET, nTaille);
   allocation_tableau(mapPixel, OCTET, nTaille);
   allocation_tableau(mapPixelD, OCTET, nTaille);

  for(int i =0; i< nTaille; i++){
    mapPixel[i] = 0;
    mapPixelD[i] = 0;

  }

  srand(1);

  int pos = 0;

  //** Chiffrage
  for (int i=0; i < nH; i++)
    for (int j=0; j < nW; j++)
    {
      pos = rand()%nTaille;
      //printf("%i\n", mapPixel[pos]);
      while(mapPixel[pos] == 1){
        pos = (pos+1)%nTaille; 
      }
       ImgOut[pos] = ImgIn[i*nW+j];
       mapPixel[pos] = 1;
     }
  //printf("comp %i, RAND_MAX %i \n",comp, RAND_MAX);
  printf("PSNR %f\n",PSNR(ImgIn, ImgOut,nH,nW));
  printf("Entropy Image Décrypter %f\n",entropy(ImgIn, nH, nW));
  printf("Entropy Image Crypter %f\n",entropy(ImgOut, nH, nW));

  
  

  //** Dechiffrage
  srand(1);
  pos = 0;
  for (int i=0; i < nH; i++)
    for (int j=0; j < nW; j++)
    {
      pos = rand()%nTaille;

      
      while(mapPixelD[pos] == 1){
        pos = (pos+1)%nTaille; 
      }
      
      ImgDecr[i*nW+j] = ImgOut[pos];
      mapPixelD[pos] = 1;
    }
  ecrire_image_pgm(cNomImgDecr, ImgDecr,  nH, nW);


  //Histogramme
  //int * tabHisto;
  /*
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

  */
  /*
  tabHisto = Histogramme(ImgOut, nW, nH);
  for (int i = 0; i < 256; i++){
    printf("%d, %d \n", i, tabHisto[i]);
  }
  */
  ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);

   //free(ImgIn);
   //free(ImgOut);
   return 1;
}
