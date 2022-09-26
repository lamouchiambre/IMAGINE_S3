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
  char cNomImgLue[250], cNomImgEcrite[250],cNomImgDecr[250], chiffrement[250], c[250], cNomImgKey[250];
  int nH, nW, nTaille, S;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn.pgm chiffrement\n"); 
       exit (1) ;
     }
   
  sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%s",chiffrement);

  OCTET *ImgKey, *ImgIn, *ImgInNoise, *ImgOut, *ImgOutNoise,*ImgDecr, *ImgDecrNoise, *ImgSubCryp,*ImgSubDecryp, *ImgEntropy, *ImgEntropyNtaille;
   
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
  allocation_tableau(ImgSubDecryp, OCTET, nTaille);
  allocation_tableau(ImgEntropy, OCTET, int(nTaille/16));
  allocation_tableau(ImgEntropyNtaille, OCTET, nTaille);
  allocation_tableau(ImgKey, OCTET, nTaille);
  
   
  std::string strs(chiffrement);
  std::string sNomImgLue = std::string(cNomImgLue);
  std::string ext = ".pgm";
  std::string sNomImgLueNoise = sNomImgLue+strs+"Noise"+ext;
  std::string sNomImgLueEntropy = sNomImgLue+strs+"Entropy"+ext;
  std::string sNomImgLueEntropyNtaille = sNomImgLue+strs+"Entropy"+"Ntaille"+ext;
  std::string sNomImgCrypNoise = sNomImgLue+strs+"Cryp"+"Noise"+ext;
  std::string sNomImgCryp = sNomImgLue+strs+"Cryp"+ext;
  std::string sNomImgDecryp = sNomImgLue+strs+"Decryp"+ext;
  std::string sNomImgDecrypNoise = sNomImgLue+strs+"Decryp"+"Noise"+ext;
  std::string sNomImgDecrypSub = sNomImgLue+strs+"Decryp"+"Sub"+"DecrypNoise"+ext;
  std::string sNomImgCrypSub = sNomImgLue+strs+"Cryp"+"Sub"+"CrypNoise"+ext;
  std::string sNomImgKey = "img_in/15.pgm";
  strcpy(cNomImgKey, sNomImgKey.c_str());
  lire_image_pgm(cNomImgKey, ImgKey, nH * nW);

  srand(1);
  OCTET key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
  OCTET vector_in[] = { 0x00, 0x00, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

  unsigned int len_bytes = nTaille * sizeof(OCTET);
  AES aes(AESKeyLength::AES_128);

  if (strs == "CBC")
  {   
    ImgInNoise = addNoise(ImgIn, nTaille);
    ImgOut = aes.EncryptCBC(ImgIn, len_bytes, key, vector_in);
    ImgOutNoise = addNoise(ImgOut, nTaille);
    ImgDecr = aes.DecryptCBC(ImgOut, len_bytes, key, vector_in);
    ImgDecrNoise = aes.DecryptCBC(ImgOutNoise, len_bytes, key, vector_in);

    ImgSubCryp = substraction(ImgOut, ImgOutNoise, nTaille);
    ImgSubDecryp = substraction(ImgDecr, ImgDecrNoise, nTaille);
    
    ImgEntropy = entropyBlock16(ImgSubDecryp, nTaille); 
    ImgEntropyNtaille = entropyBlock16Ntaille(ImgSubDecryp, nTaille); 
  }
  else if (strs == "CFB")
  {
    std::cout<<strs<<std::endl;
    ImgInNoise = addNoise(ImgIn, nTaille);
    ImgOut = aes.EncryptCFB(ImgIn, len_bytes, key, vector_in);
    ImgOutNoise = addNoise(ImgOut, nTaille);
    ImgDecr = aes.DecryptCFB(ImgOut, len_bytes, key, vector_in);
    ImgDecrNoise = aes.DecryptCFB(ImgOutNoise, len_bytes, key, vector_in);
    ImgSubCryp = substraction(ImgOut, ImgOutNoise, nTaille);
    ImgSubDecryp = substraction(ImgDecr, ImgDecrNoise, nTaille);
    
    ImgEntropy = entropyBlock16(ImgSubDecryp, nTaille); 
    ImgEntropyNtaille = entropyBlock16Ntaille(ImgSubDecryp, nTaille);  
  }
  else if(strs == "OFB")
  {
    std::cout<<strs<<std::endl;
    ImgInNoise = addNoise(ImgIn, nTaille);
    ImgOut = aes.EncryptOFB(ImgIn, len_bytes, key, vector_in);
    ImgOutNoise = addNoise(ImgOut, nTaille);
    ImgDecr = aes.DecryptOFB(ImgOut, len_bytes, key, vector_in);
    ImgDecrNoise = aes.DecryptOFB(ImgOutNoise, len_bytes, key, vector_in);
    ImgSubCryp = substraction(ImgOut, ImgOutNoise, nTaille);
    ImgSubDecryp = substraction(ImgDecr, ImgDecrNoise, nTaille);
    
    ImgEntropy = entropyBlock16(ImgSubDecryp, nTaille); 
    ImgEntropyNtaille = entropyBlock16Ntaille(ImgSubDecryp, nTaille); 

  }else if(strs == "CTR")
  {
    std::cout<<strs<<std::endl;
    ImgInNoise = addNoise(ImgIn, nTaille);
    ImgOut = aes.EncryptCTR(ImgIn, len_bytes, key);
    ImgOutNoise = addNoise(ImgOut, nTaille);
    ImgDecr = aes.DecryptCTR(ImgOut, len_bytes, key);
    ImgDecrNoise = aes.DecryptCTR(ImgOutNoise, len_bytes, key);
    ImgSubCryp = substraction(ImgOut, ImgOutNoise, nTaille);
    ImgSubDecryp = substraction(ImgDecr, ImgDecrNoise, nTaille);
    
    ImgEntropy = entropyBlock16(ImgSubDecryp, nTaille); 
    ImgEntropyNtaille = entropyBlock16Ntaille(ImgSubDecryp, nTaille); 
  }else if(strs == "ECB")
  {
    std::cout<<strs<<std::endl;
    ImgInNoise = addNoise(ImgIn, nTaille);
    ImgOut = aes.EncryptECB(ImgIn, len_bytes, key);
    ImgOutNoise = addNoise(ImgOut, nTaille);
    ImgDecr = aes.DecryptECB(ImgOut, len_bytes, key);
    ImgDecrNoise = aes.DecryptECB(ImgOutNoise, len_bytes, key);
    ImgSubCryp = substraction(ImgOut, ImgOutNoise, nTaille);
    ImgSubDecryp = substraction(ImgDecr, ImgDecrNoise, nTaille);
    
    ImgEntropy = entropyBlock16(ImgSubDecryp, nTaille); 
    ImgEntropyNtaille = entropyBlock16Ntaille(ImgSubDecryp, nTaille); 
  }else if (strs == "XOR")
  {
    std::cout<<strs<<std::endl;
    ImgInNoise = addNoise(ImgIn, nTaille);

    aes.XorBlocks(ImgIn, ImgKey, ImgOut, nTaille);//cryptage 
    aes.XorBlocks(ImgInNoise, ImgKey, ImgOutNoise, nTaille);//cryptage noise

    ImgOutNoise = addNoise(ImgOut, nTaille);
    
    aes.XorBlocks(ImgKey, ImgOut, ImgDecr, nTaille);
    aes.XorBlocks(ImgKey, ImgOutNoise, ImgDecrNoise, nTaille);


    ImgSubCryp = substraction(ImgOut, ImgOutNoise, nTaille);
    ImgSubDecryp = substraction(ImgDecr, ImgDecrNoise, nTaille);

    ImgEntropy = entropyBlock16(ImgSubDecryp, nTaille); 
    ImgEntropyNtaille = entropyBlock16Ntaille(ImgSubDecryp, nTaille); 
  }
  else{
    printf("mauvais choix\n");
    printf("O_O\n");

  } 
  
  printf("PSNR ImgIn/ImgOut %f\n",PSNR(ImgIn, ImgOut,nH,nW));
  printf("PSNR ImgIn/ImgDecryp avec bruit %f\n",PSNR(ImgIn, ImgDecrNoise,nH,nW));
  printf("PSNR Bruit initial/ bruit propagé %f\n",PSNR(ImgSubCryp, ImgSubDecryp,nH,nW));

  printf("Entropy Image claire %f\n",entropy(ImgIn, nH, nW));
  printf("Entropy Image Crypter %f\n",entropy(ImgOut, nH, nW));
  printf("Entropy Image Crypter avec bruit %f\n",entropy(ImgOutNoise, nH, nW));
  printf("Entropy Image DéCrypter %f\n",entropy(ImgDecr, nH, nW));
  printf("Entropy Image DéCrypter avec bruit %f\n",entropy(ImgDecrNoise, nH, nW));
  printf("Entropy Image Bruit Sub Cryp %f\n",entropy(ImgSubCryp, nH, nW));
  printf("Entropy Image Bruit Sub Decryp %f\n",entropy(ImgSubDecryp, nH, nW));

  //printf("### HISTOGRAMME ###\n");
  //Histogramme(ImgOut, nW, nH);
  
  //ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);

  strcpy(cNomImgEcrite, sNomImgLueEntropy.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgEntropy, int(nH/16), int(nW/16));

  strcpy(cNomImgEcrite, sNomImgLueNoise.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgInNoise, nH, nW);
  
  strcpy(cNomImgEcrite, sNomImgLueNoise.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgKey, nH, nW);

  strcpy(cNomImgEcrite, sNomImgCryp.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);

  strcpy(cNomImgEcrite, sNomImgCrypNoise.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgOutNoise, nH, nW);

  strcpy(cNomImgEcrite, sNomImgDecryp.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgDecr, nH, nW);

  strcpy(cNomImgEcrite, sNomImgDecrypNoise.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgDecrNoise, nH, nW);

  strcpy(cNomImgEcrite, sNomImgDecrypSub.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgSubDecryp, nH, nW);

  strcpy(cNomImgEcrite, sNomImgCrypSub.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgSubCryp, nH, nW);

  strcpy(cNomImgEcrite, sNomImgLueEntropyNtaille.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgEntropyNtaille, nH, nW);

   free(ImgIn);
   free(ImgOut);
   return 1;
}
