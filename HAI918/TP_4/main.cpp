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
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, S;


  if (argc != 2) 
     {
       printf("Usage: ImageIn.pgm \n"); 
       exit (1) ;
     } 

  sscanf (argv[1],"%s",cNomImgLue) ;

 
  OCTET *ImgIn,*ImgMaxPool;

  allocation_tableau(ImgMaxPool, OCTET, 128*128);

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
 
  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

  ImgMaxPool = maxPooling(ImgIn, nH, nW, 2, 2);

  std::string sNomImgLue = std::string(cNomImgLue);

  std::string ext = ".pgm";
  
  std::string sNomImgOutSub = sNomImgLue+"Sub"+ext;
  std::string sNomImgLueMessage = sNomImgLue+"Message"+ext;

   
  sNomImgLueMessage = sNomImgLue+"_maxpool"+ext;
  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgMaxPool, 128, 128);
    
   return 1;
}
