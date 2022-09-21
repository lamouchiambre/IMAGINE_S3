// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"
#include "AES.h"

int* Histogramme(OCTET *ImgIn, int nW, int nH){
  int * tabHisto;
  allocation_tableau(tabHisto, int, 256);
  for(int i = 0; i < 256; i++){
    tabHisto[i] = 0;
  }

  for (int i=0; i < nH; i++)
    for (int j=0; j < nW; j++)
      tabHisto[ImgIn[i*nW+j]] += 1;
  
  for(int i = 0; i < 256; i++){
    printf("%i %i \n", i, tabHisto[i]);
  }
  
  return tabHisto;
}

OCTET* addNoise(OCTET* img, int nTaille){
  srand(0);
  OCTET* imgNoise = new unsigned char[nTaille];
  int nbBlock = int(nTaille/16);
  int * indBlockNoise = new int[int(nbBlock)];
  int ibn = 0;
  for (int i = 0; i<nTaille; i++){
    imgNoise[i] = img[i];
  }
  //create list of random indice of noised block
  int j = 0;
  for(int i = 0; i < nbBlock; i+=5){
    indBlockNoise[j] = i + rand()%5;
    j++;

  }
  int valRand = 0;
  for (int i = 0; i < nbBlock; i++){
    if(indBlockNoise[ibn]==i){
      valRand = i*16 + rand()%16;
      imgNoise[valRand] = rand()%256;     
      ibn++;
      }
    }

  return imgNoise;

}

OCTET* substraction(OCTET* imgA, OCTET* imgB, int nTaille){
  OCTET* imgSub = new unsigned char[nTaille];
  for (int i = 0; i< nTaille; i++){
    imgSub[i] = abs(imgA[i]-imgB[i]);
  }
  return imgSub;
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

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250],cNomImgDecr[250];
  int nH, nW, nTaille, S;
  
  if (argc != 4) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm ImageDecrypt.pgm \n"); 
       exit (1) ;
     }
   
  sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%s",cNomImgEcrite);
  sscanf (argv[3],"%s",cNomImgDecr);
	  
  //sscanf ("img_out/01_melange_decript.pgm","%s",cNomImgDecr);

   OCTET *ImgIn, *ImgInNoise, *ImgOut, *ImgOutNoise,*ImgDecr, *ImgDecrNoise, *ImgSubCryp;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
 
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);
   allocation_tableau(ImgDecr, OCTET, nTaille);
   allocation_tableau(ImgInNoise, OCTET, nTaille);
   allocation_tableau(ImgOutNoise, OCTET, nTaille);
   allocation_tableau(ImgDecrNoise, OCTET, nTaille);
   allocation_tableau(ImgSubCryp, OCTET, nTaille);


  srand(1);
  OCTET key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
  OCTET vector_in[] = { 0x00, 0x00, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

  unsigned int len_bytes = nTaille * sizeof(OCTET);
  AES aes(AESKeyLength::AES_128);
  //sub test

  // CBC
  ImgInNoise = addNoise(ImgIn, nTaille);
  ImgOut = aes.EncryptCTR(ImgIn, len_bytes, key);
  ImgOutNoise = aes.EncryptCTR(ImgInNoise, len_bytes, key);
  
  // ImgDecr = aes.DecryptCBC(ImgOut, len_bytes, key, vector_in);
  // ImgDecrNois = aes.DecryptCBC(ImgOut, len_bytes, key, vector_in);


  ImgSubCryp = substraction(ImgOutNoise, ImgOut, nTaille);

  // CFB
  // ImgOut = aes.EncryptCFB(ImgIn, len_bytes, key, vector_in);
  // ImgDecr = aes.DecryptCFB(ImgOut, len_bytes, key, vector_in);

  // OFB
  //ImgOut = aes.EncryptOFB(ImgIn, len_bytes, key, vector_in);
  //ImgDecr = aes.DecryptOFB(ImgOut, len_bytes, key, vector_in);

    // CTR
  // ImgOut = aes.EncryptCTR(ImgIn, len_bytes, key);
  // ImgDecr = aes.DecryptCTR(ImgOut, len_bytes, key);

  // ECB 
  // ImgOut = aes.EncryptECB(ImgIn, len_bytes, key);
  // ImgDecr = aes.DecryptECB(ImgOut, len_bytes, key);
  

  
   //printf("%i\n", ImgDecr[0]);
  
  
  printf("PSNR %f\n",PSNR(ImgIn, ImgOut,nH,nW));
  printf("Entropy Image claire %f\n",entropy(ImgIn, nH, nW));
  printf("Entropy Image Crypter %f\n",entropy(ImgOut, nH, nW));
  printf("### HISTOGRAMME ###\n");
  //Histogramme(ImgOut, nW, nH);

  //ecrire_image_pgm(cNomImgEcrite, addNoise(ImgIn, nTaille),  nH, nW);
  ecrire_image_pgm(cNomImgEcrite, ImgOutNoise,  nH, nW);
  // ecrire_image_pgm(cNomImgDecr, ImgDecr,  nH, nW);
  ecrire_image_pgm(cNomImgDecr, ImgSubCryp,  nH, nW);

   free(ImgIn);
   free(ImgOut);
   return 1;
}
