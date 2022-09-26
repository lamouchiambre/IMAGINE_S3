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
  char cNomImgLue[250],cNomImageMessage[250], cNomImgEcrite[250],cNomImgDecr[250], type[250], c[250], cNomImgKey[250];
  int nH, nW, nTaille, S;
  int nHM, nWM, nTailleM, SM;

  if (argc != 4) 
     {
       printf("Usage: ImageIn.pgm ImageMessage.pgm type\n"); 
       exit (1) ;
     }
  
  sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%s",cNomImageMessage);
  sscanf (argv[3],"%s",type);

  OCTET *ImgKey, *ImgIn,*ImgInMessage, *ImgOutMessage, *ImgOut, *ImgOutNoise,*ImgDecr, *ImgDecrNoise, *ImgSubCryp,*ImgSubDecryp, *ImgEntropy, *ImgEntropyNtaille;
   
  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
  
  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

  allocation_tableau(ImgInMessage, OCTET, nTaille);

  lire_nb_lignes_colonnes_image_pgm(cNomImageMessage, &nHM, &nWM);
  nTailleM = nHM * nWM;
  lire_image_pgm(cNomImageMessage, ImgInMessage, nHM * nWM);

  allocation_tableau(ImgOut, OCTET, nTaille);
  allocation_tableau(ImgOutMessage, OCTET, nTailleM);
  
  std::string strs(type);
  std::string sNomImgLue = std::string(cNomImgLue);
  std::string sImageMessage = std::string(cNomImageMessage);
 
  std::string ext = ".pgm";
  std::string sNomImgLueMessage = sNomImgLue+"Message"+ext;
  std::string sNomImgOutMessage = sImageMessage+"Decryp"+ext;
 
 
  srand(1);
  OCTET key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
  OCTET vector_in[] = { 0x00, 0x00, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
 
  unsigned int len_bytes = nTaille * sizeof(OCTET);
  AES aes(AESKeyLength::AES_128);


  if (strs == "Npremier")
  {   
    // int zero = 0; 
    // binary(zero);
    // int zero2 = set_bit(zero, 1);    
    // binary(zero);
    // binary(zero2);
    // std::cout << get_bit(zero2, 1) << " " << get_bit(zero, 1)<<std::endl;
    
    ImgOut = Npremier(ImgIn, ImgInMessage, nH, nW, nHM, nWM);
    ImgOutMessage = NpremierEctract(ImgOut, nH, nW, nHM, nWM); 

  }else if(strs == "Poid"){
    ImgOut = Poid(ImgIn, ImgInMessage,7, nH, nW, nHM, nWM);
    ImgOutMessage = PoidEctract(ImgOut,7, nH, nW, nHM, nWM); 
  }
  else{
    printf("mauvais choix\n");
    printf("O_O\n");

  }
  printf("PSNR %f\n",PSNR(ImgIn, ImgOut,nH,nW));
  printf("PSNR %f\n",PSNR(ImgIn, ImgIn,nH,nW));

  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);

  strcpy(cNomImgEcrite, sNomImgOutMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgOutMessage, nHM, nWM);



   free(ImgIn);
   free(ImgOut);
   return 1;
}
