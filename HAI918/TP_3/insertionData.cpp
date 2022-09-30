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
  OCTET * Img256;  
  OCTET * Img16, * Img16Out;
  OCTET * Img2, * Img2Out;

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
  allocation_tableau(Img256, OCTET, nTaille);


  allocation_tableau(Img16, OCTET, 16*16);
  allocation_tableau(Img16Out, OCTET, 16*16);

  allocation_tableau(Img2, OCTET, 2*2);
  allocation_tableau(Img2Out, OCTET, 2*2);

  std::string strs(type);
  std::string sNomImgLue = std::string(cNomImgLue);
  std::string sImageMessage = std::string(cNomImageMessage);
 
  std::string ext = ".pgm";
  
  std::string sNomImgOutSub = sNomImgLue+"Sub"+ext;
  std::string sNomImgLueMessage = sNomImgLue+"Message"+ext;
  std::string sNomImgOutMessage = sImageMessage+"Decryp"+ext;


  srand(1);
  OCTET key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
  OCTET vector_in[] = { 0x00, 0x00, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };


  unsigned int len_bytes = nTaille * sizeof(OCTET);
  AES aes(AESKeyLength::AES_128);
  int b = 0;
  for (int i = 0; i < nTaille; i++)
  {
    Img256[i] = b;
    b = (b +1)%256 ;
  }

  if (strs == "Npremier") 
  {  
    ImgOut = Npremier(ImgIn, ImgInMessage, nH, nW, nHM, nWM);
    ImgOutMessage = NpremierEctract(ImgOut, nH, nW, nHM, nWM); 
  }else if(strs == "inser"){
    ImgOut = insertion(ImgIn, ImgInMessage,0, nH, nW, nHM, nWM);
    ImgOutMessage = insertionEctract(ImgOut,0, nH, nW, nHM, nWM); 
  }
  else if(strs == "inserPixel"){
    ImgOut = insertionParPixel(ImgIn, ImgInMessage, 7, nH, nW, nHM, nWM);
    ImgOutMessage = insertionParPixelEctract(ImgOut, 7, nH, nW, nHM, nWM); 
  } 
  else if(strs == "inserSub"){
    ImgOut = insertionSub(ImgIn, ImgInMessage, 7, nH, nW, nHM, nWM);
    ImgOutMessage = insertionSubEctract(ImgOut, 7, nH, nW, nHM, nWM); 
  }   
  else if(strs == "inserRand"){
    ImgOut = insertionRand(ImgIn, ImgInMessage, 7, nH, nW, nHM, nWM);
    ImgOutMessage = insertionRandEctract(ImgOut, 7, nH, nW, nHM, nWM); 
  }
  else if(strs == "inserRandMaxi"){
    ImgOut = insertionRandMaxi(ImgIn, ImgInMessage, 7, 23, nH, nW, nHM, nWM);
    ImgOutMessage = insertionRandMaxiEctract(ImgOut, 7, 23, nH, nW, nHM, nWM);
  }
  else if(strs == "attack"){
    ImgOut = insertionRandMaxi(ImgIn, ImgInMessage, 0, 23, nH, nW, nHM, nWM);
    ImgOutMessage = attackExtractionRand(ImgOut, 0, nH, nW, nHM, nWM ); 
  } 
  else if(strs == "2LSB"){
    ImgOut = insertion2Bit(ImgIn, ImgInMessage, 7, 5, 23, nH, nW, nHM, nWM);
    ImgOutMessage = insertion2BitEctract(ImgOut, 7, 5, 23, nH, nW, nHM, nWM ); 
  }
  else if(strs == "3LSB"){ 
    ImgOut = insertion3Bit(ImgIn, ImgInMessage, 7, 5, 6, 23, nH, nW, nHM, nWM);
    ImgOutMessage = insertion3BitEctract(ImgOut, 7, 5, 6, 23, nH, nW, nHM, nWM ); 
  }
  else if(strs == "all"){ 
    // ImgOut = Npremier(ImgIn, ImgInMessage, nH, nW, nHM, nWM);
    // ImgOutMessage = NpremierEctract(ImgOut, nH, nW, nHM, nWM); 


    
    printf("\n");
    printf("\n");
    printf("########### RAND ###########\n");

    for (int i = 0; i < 8; i++)
    {
      /* code */
      printf("\n");
      printf("Insertion 1 bit du message au bit %i de l'image \n", i);
      ImgOut = insertionRandMaxi(ImgIn, ImgInMessage, i, 23, nH, nW, nHM, nWM);
      ImgOutMessage = insertionRandMaxiEctract(ImgOut, i, 23, nH, nW, nHM, nWM);
      print_result(ImgIn, ImgOut, ImgOutMessage, ImgInMessage,nHM, nWM, nH, nW);
      
      sNomImgLueMessage = sNomImgLue+"Rand"+std::to_string(i)+"_"+ext;
      strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
      ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);

      sNomImgOutSub = sNomImgLue+"Sub"+"Rand"+std::to_string(i)+ext;
      strcpy(cNomImgEcrite, sNomImgOutSub.c_str());
      ecrire_image_pgm(cNomImgEcrite, substraction(ImgOut, ImgIn, nTaille), nH, nW);

      sNomImgOutMessage = sImageMessage+"Decryp"+"Sub"+"Rand"+std::to_string(i)+ext;
      strcpy(cNomImgEcrite, sNomImgOutMessage.c_str());
      ecrire_image_pgm(cNomImgEcrite, ImgOutMessage, nHM, nWM);
    }
    

    for (int i = 0; i < 8; i++)
    {
      for (int j = i; j < 8; j++)
      {
        printf("\n");
        printf("Insertion 2 bits du message aux bits %i et %i de l'image \n", i,j);
        ImgOut = insertion2Bit(ImgIn, ImgInMessage, i, j, 23, nH, nW, nHM, nWM);
        ImgOutMessage = insertion2BitEctract(ImgOut, i, j, 23, nH, nW, nHM, nWM ); 
        print_result(ImgIn, ImgOut, ImgOutMessage, ImgInMessage,nHM, nWM, nH, nW);
        
        sNomImgLueMessage = sNomImgLue+"2bit"+std::to_string(i)+"_"+std::to_string(j)+ext;
        strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
        ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);

        sNomImgOutSub = sNomImgLue+"Sub"+"2bit"+std::to_string(i)+"_"+std::to_string(j)+ext;
        strcpy(cNomImgEcrite, sNomImgOutSub.c_str());
        ecrire_image_pgm(cNomImgEcrite, substraction(ImgOut, ImgIn, nTaille), nH, nW);

        sNomImgOutMessage = sImageMessage+"Decryp"+"Sub"+"2bit"+std::to_string(i)+"_"+std::to_string(j)+ext;
        strcpy(cNomImgEcrite, sNomImgOutMessage.c_str());
        ecrire_image_pgm(cNomImgEcrite, ImgOutMessage, nHM, nWM);
      }
    }
    printf("\n");
    printf("\n");
    printf("########### 3 BIT ###########\n");
    for (int i = 0; i < 8; i++)
    {
      for (int j = i; j < 8; j++)
      {
        for (int k = j; k < 8; k++)
        {
          printf("\n");
          printf("Insertion 3 bits du message aux bits %i, %i et %i de l'image \n", i,j, k);
          ImgOut = insertion3Bit(ImgIn, ImgInMessage, i, j, k, 23, nH, nW, nHM, nWM);
          ImgOutMessage = insertion3BitEctract(ImgOut, i, j, k, 23, nH, nW, nHM, nWM); 
          print_result(ImgIn, ImgOut, ImgOutMessage, ImgInMessage,nHM, nWM, nH, nW);
          
          sNomImgLueMessage = sNomImgLue+"3bit"+std::to_string(i)+"_"+std::to_string(j)+"_"+std::to_string(k)+ext;
          strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
          ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);

          sNomImgOutSub = sNomImgLue+"Sub"+"3bit"+std::to_string(i)+"_"+std::to_string(j)+"_"+std::to_string(k)+ext;
          strcpy(cNomImgEcrite, sNomImgOutSub.c_str());
          ecrire_image_pgm(cNomImgEcrite, substraction(ImgOut, ImgIn, nTaille), nH, nW);

          sNomImgOutMessage = sImageMessage+"Decryp"+"Sub"+"3bit"+std::to_string(i)+"_"+std::to_string(j)+"_"+std::to_string(k)+ext;
          strcpy(cNomImgEcrite, sNomImgOutMessage.c_str());
          ecrire_image_pgm(cNomImgEcrite, ImgOutMessage, nHM, nWM);
        }
      }
    }
  }
  // insertion3Bit(OCTET * img, OCTET * message, int b1, int b2, int b3, int key, int nH, int nW, int nHM, int nWM)
  else if (strs == "test")
  {
  }
  else if (strs == "test2")
  {
  }
  else{
    printf("mauvais choix\n");
    printf("O_O\n");
  }

  // printf("PSNR image/image avec tatouage %f\n",PSNR(ImgIn, ImgOut,nH,nW));
  // printf("PSNR message/message decrype %f\n",PSNR(ImgInMessage, ImgOutMessage,nH,nW));
  // printf("ETH message décrype %f bit tout les 1000 bit\n",ETH(ImgInMessage, ImgOutMessage,nTailleM));
  // printf("ETH message avec tatouage %f bit tout les 1000 bit\n",ETH(ImgInMessage, ImgInMessage,nTaille));


  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);

  // strcpy(cNomImgEcrite, sNomImgOutMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgOutMessage, nHM, nWM);

  // strcpy(cNomImgEcrite, sNomImgOutSub.c_str());
  // ecrire_image_pgm(cNomImgEcrite,substraction(ImgOut, ImgIn, nTaille), nH, nW);

   free(ImgIn);
   free(ImgOut);
   return 1;
}
