#include <stdio.h>
#include "imglib.h"
#include <iostream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

using namespace std;

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
 
  OCTET *ImgIn, *ImgBootMean, *ImgSandalAll, *ImgSandal, *ImgBoot, *ImgBootAll;


  std::string sNomImgLue = std::string(cNomImgLue);
  std::string ext = ".pgm";
  std::string sNomImgOutSub = sNomImgLue+"Sub"+ext;
  std::string sNomImgLueMessage = sNomImgLue+"Message"+ext;
  
  std::string sNomImgSandalAll = "Sandal_FattenAll.pgm";
  std::string sNomImgSandal = "img_in2/Sandal_(1).pgm_Fatten.pgm";
  std::string sNomImgBootAll = "Boot_FattenAll.pgm";
  std::string sNomImgBoot = "img_in2/boot_(1).pgm_Fatten.pgm";

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
  
  // lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
  allocation_tableau(ImgIn, OCTET, nTaille);
  allocation_tableau(ImgBoot, OCTET, nTaille);
  allocation_tableau(ImgBootAll, OCTET, nTaille);
  allocation_tableau(ImgBootMean, OCTET, nTaille);
  allocation_tableau(ImgSandal, OCTET, nTaille);
  allocation_tableau(ImgSandalAll, OCTET, nTaille);


  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);




  allocation_tableau(ImgBootMean, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgBootMean, nH * nW);

  sNomImgLue = sNomImgBootAll;
  strcpy(cNomImgLue, sNomImgBootAll.c_str());
  lire_image_pgm(cNomImgLue, ImgBootAll, nH * nW);

  sNomImgLue = sNomImgSandalAll;
  strcpy(cNomImgLue, sNomImgSandalAll.c_str());
  lire_image_pgm(cNomImgLue, ImgSandalAll, nH * nW);

  sNomImgLue = sNomImgBoot;
  strcpy(cNomImgLue, sNomImgLue.c_str());
  lire_image_pgm(cNomImgLue, ImgBoot, nH * nW);

  sNomImgLue = sNomImgSandal;
  strcpy(cNomImgLue, sNomImgLue.c_str());
  lire_image_pgm(cNomImgLue, ImgSandal, nH * nW);

  // std::string path = "../../../archive/Shoe_vs_Sandal_vs_Boot_Dataset/Boot";
  std::string path = "img_in2/";

  printf("PSNR entre l'image initiale Boot et l'image moyenne Sandal : %f dB\n",PSNR(ImgBoot, ImgSandalAll,nH,nW));
  printf("PSNR entre l'image initiale Boot et l'image moyenne Boot  : %f dB\n",PSNR(ImgBoot, ImgBootAll,nH,nW));
  printf("PSNR entre l'image initiale Sandal et l'image moyenne Sandal : %f dB\n",PSNR(ImgSandal, ImgSandalAll,nH,nW));
  printf("PSNR entre l'image initiale Sandal et l'image moyenne Boot : %f dB\n",PSNR(ImgSandal, ImgBootAll,nH,nW));


  // printf("PSNR entre le message et le message extrait : %f dB\n",PSNR(ImgInMessage, ImgOutMessage,nHM,nWM));
  
  printf("Entropie de l'image Sandal : %f bits/pixel\n", entropy(ImgSandal, nH, nW));
  printf("Entropie de l'image SandalAll : %f bits/pixel\n", entropy(ImgSandal, nH, nW));

  printf("Entropie de l'image Boot : %f bits/pixel\n", entropy(ImgBoot, nH, nW));
  printf("Entropie de l'image BootAll : %f bits/pixel\n", entropy(ImgBootAll, nH, nW));

  // printf("Entropie du message extrait : %f bits/pixel\n", entropy(ImgOutMessage, nHM, nWM));
 
  
   return 1;
}
