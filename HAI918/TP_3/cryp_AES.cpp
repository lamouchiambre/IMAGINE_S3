#include <stdio.h>
#include "imglib.h"
#include <iostream>
#include <string>

constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250],cNomImgDecr[250], chiffrement[250], c[250];
  int nH, nW, nTaille, S;
  
  if (argc != 4) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm chiffrement\n"); 
       exit (1) ;
     }
   
  sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%s",cNomImgEcrite);
  //sscanf (argv[3],"%s",cNomImgDecr);
  sscanf (argv[3],"%s",chiffrement);
	  
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
  std::string strs(chiffrement);
  if (strs == "CBC")
  {
      // CBC
      ImgOut = aes.EncryptCBC(ImgIn, len_bytes, key, vector_in);
      //ImgOutNoise = addNoise(ImgOut, nTaille);
      
      ImgDecr = aes.DecryptCBC(ImgOut, len_bytes, key, vector_in);
      //ImgDecrNoise = aes.DecryptCBC(ImgOutNoise, len_bytes, key, vector_in);

      //ImgSubCryp = substraction(ImgDecr, ImgDecrNoise, nTaille);
  }
  else if (strs == "CFB")
  {
    std::cout<<strs<<std::endl;
    ImgOut = aes.EncryptCFB(ImgIn, len_bytes, key, vector_in);
    ImgDecr = aes.DecryptCFB(ImgOut, len_bytes, key, vector_in);
  }
  else if(strs == "OFB")
  {
    std::cout<<strs<<std::endl;
    ImgOut = aes.EncryptOFB(ImgIn, len_bytes, key, vector_in);
    ImgDecr = aes.DecryptOFB(ImgOut, len_bytes, key, vector_in);
  }else if(strs == "CTR")
  {
    std::cout<<strs<<std::endl;
    ImgOut = aes.EncryptCTR(ImgIn, len_bytes, key);
    ImgDecr = aes.DecryptCTR(ImgOut, len_bytes, key);
  }else if(strs == "ECB")
  {
    std::cout<<strs<<std::endl;
    ImgOut = aes.EncryptECB(ImgIn, len_bytes, key);
    ImgDecr = aes.DecryptECB(ImgOut, len_bytes, key);
  }else
  {
    printf("mauvais choix\n");
    printf("O_O\n");

  } 
  
  printf("PSNR %f\n",PSNR(ImgIn, ImgOut,nH,nW));
  printf("Entropy Image claire %f\n",entropy(ImgIn, nH, nW));
  printf("Entropy Image Crypter %f\n",entropy(ImgOut, nH, nW));
  printf("### HISTOGRAMME ###\n");
  //Histogramme(ImgOut, nW, nH);
  
  ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);

  //ecrire_image_pgm(cNomImgEcrite, addNoise(ImgIn, nTaille),  nH, nW);
  //ecrire_image_pgm(cNomImgEcrite, ImgDecr,  nH, nW);

  //ecrire_image_pgm(cNomImgEcrite, ImgDecrNoise,  nH, nW);
  //ecrire_image_pgm(cNomImgDecr, ImgSubCryp,  nH, nW);

   free(ImgIn);
   free(ImgOut);
   return 1;
}
