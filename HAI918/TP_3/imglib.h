#ifndef _IMGLIB_H_
#define _IMGLIB_H_

#include <stdio.h>
#include "image_ppm.h"
#include <bitset>
#include "AES.h"



//Extraction

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
bool get_bit(int num, int position)
{
	bool bit = num & (1 << position);
	return bit;
}

int set_bit(int num, int position)
{
	int mask = 1 << position;
	return num | mask;
}

void binaryM(unsigned int num, std::string m){
  std::cout << m << " ";
  for(int i = 128; i > 0; i = i/2){
    if(num & i){
      std::cout << "1 ";
    }else{
      std::cout << "0 ";
    }
  }
  std::cout << std::endl;
}

void binary(unsigned int num){
  for(int i = 128; i > 0; i = i/2){
    if(num & i){
      std::cout << "1 ";
    }else{
      std::cout << "0 ";
    }
  }
  std::cout << std::endl;
}


//Insertion de message

OCTET* Npremier(OCTET * img, OCTET * message, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nH*nW);

  int nbBlock = int((nH*nW)/(nHM*nWM*8));
  for(int i = 0; i <nH*nW ;i++){
    ImageMessage[i] = img[i];
  } 
  for(int i = 0; i <nHM*nWM ;i++){
    ImageMessage[i] = message[i];
  } 
  return ImageMessage;
}

OCTET* NpremierEctract(OCTET * img,int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nHM*nWM);
  for(int i = 0; i <nHM*nWM ;i++){
    ImageMessage[i] = img[i];
  }

  return ImageMessage;

}

OCTET* insertionSub(OCTET * img, OCTET * message, int bit, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nH*nW);

  int nbBlock = int((nH*nW)/(nHM*nWM*8));
  for(int i = 0; i <nH*nW ;i++){
    ImageMessage[i] = img[i];
  } 
  int k = 0;
  for (int i = 0; i < nHM*nWM; i++){
    for(int j = 0; j < 8; j++){
      if(get_bit(img[k*nbBlock],bit) ^ get_bit(message[i],j)){
        if(get_bit(message[i],j)){
          ImageMessage[k*nbBlock] = (ImageMessage[k*nbBlock] - ImageMessage[k*nbBlock]%2 + (1 << bit))%256;
        }else{
          ImageMessage[k*nbBlock] = (ImageMessage[k*nbBlock] - ImageMessage[k*nbBlock]%2  - (1 << bit))%256;
        }
      }

      binaryM(ImageMessage[k*nbBlock], "i :");

      k++;
    } 
  }

  return ImageMessage; 
}

OCTET* insertionSubEctract(OCTET * img, int bit, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nHM*nWM);

  int nbBlock = int((nH*nW)/(nHM*nWM*8));
  for(int i = 0; i <nHM*nWM ;i++){
    ImageMessage[i] = 0;
  }

  int i = 0;
  for (int k = 0; k < nHM*nWM; k++){
    for(int b = 0; b < 8; b++){
      if(img[i*nbBlock] & (1 << bit))
      {
        ImageMessage[k] = ImageMessage[k] ^ (1 << b);
      }
      i++;
    }
  }

  return ImageMessage;

}

OCTET* insertionRand(OCTET * img, OCTET * message, int bit, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nH*nW);

  int nbBlock = int((nH*nW)/(nHM*nWM*8));
  for(int i = 0; i <nH*nW ;i++){
    ImageMessage[i] = img[i];
  } 
  srand(23);
  int k = 0;
  for (int i = 0; i < nHM*nWM; i++){
    for(int j = 0; j < 8; j++){
      int pos = rand()%nbBlock;
      if(get_bit(img[k*nbBlock+pos],bit) ^ get_bit(message[i],j)){
        ImageMessage[k*nbBlock+pos] = ImageMessage[k*nbBlock+pos] ^ (1 << bit); 
      }
      binaryM(ImageMessage[k*nbBlock], "i :");

      k++;
    } 
  }

  return ImageMessage; 
} 

OCTET* insertionRandEctract(OCTET * img, int bit, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nHM*nWM);

  int nbBlock = int((nH*nW)/(nHM*nWM*8));
  for(int i = 0; i <nHM*nWM ;i++){
    ImageMessage[i] = 0;
  }
  srand(23);
  int i = 0;
  for (int k = 0; k < nHM*nWM; k++){
    for(int b = 0; b < 8; b++){
      int pos = rand()%nbBlock;
      if(img[i*nbBlock+pos] & (1 << bit))
      {
        ImageMessage[k] = ImageMessage[k] ^ (1 << b);
      }
      i++;
    }
  }

  return ImageMessage;

}


OCTET* insertionRandMaxi(OCTET * img, OCTET * message, int bit, int key, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nH*nW);

  int nbBlock = int((nH*nW)/(nHM*nWM*8));
  for(int i = 0; i <nH*nW ;i++){
    ImageMessage[i] = img[i];
  } 
  srand(key);
  int k = 0;
  for (int i = 0; i < nHM*nWM; i++){
    for(int j = 0; j < 8; j++){
      int pos = rand()%(nH*nW);
      if(get_bit(img[pos],bit) ^ get_bit(message[i],j)){
        ImageMessage[pos] = ImageMessage[pos] ^ (1 << bit); 
      }
      //binaryM(ImageMessage[k*nbBlock], "i :");

      k++;
    } 
  }

  return ImageMessage; 
}

OCTET* insertionRandMaxiEctract(OCTET * img, int bit,int key, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nHM*nWM);

  int nbBlock = int((nH*nW)/(nHM*nWM*8));
  for(int i = 0; i <nHM*nWM ;i++){
    ImageMessage[i] = 0;
  }
  srand(key);
  int i = 0;
  for (int k = 0; k < nHM*nWM; k++){
    for(int b = 0; b < 8; b++){
      int pos = rand()%(nH*nW);
      if(img[pos] & (1 << bit))
      {
        ImageMessage[k] = ImageMessage[k] ^ (1 << b);
      }
      i++;
    }
  }

  return ImageMessage;
}

OCTET* insertion2Bit(OCTET * img, OCTET * message, int b1, int b2, int key, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nH*nW);

  int nbBlock = int((nH*nW)/(nHM*nWM*8));
  for(int i = 0; i <nH*nW ;i++){
    ImageMessage[i] = img[i];
  } 
  srand(key);
  int k = 0;
  for (int i = 0; i < nHM*nWM; i++){
    for(int j = 0; j < 8; j+=2){
      int pos = rand()%(nH*nW);
      if(get_bit(img[pos], b1) ^ get_bit(message[i], j)){
        ImageMessage[pos] = ImageMessage[pos] ^ (1 << b1); 
      }
      if(get_bit(img[pos], b2) ^ get_bit(message[i], j + 1)){
        ImageMessage[pos] = ImageMessage[pos] ^ (1 << b2); 
      }
      //binaryM(ImageMessage[k*nbBlock], "i :");

      k++;
    } 
  }

  return ImageMessage; 
}

OCTET* insertion2BitEctract(OCTET * img, int b1, int b2 , int key, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nHM*nWM);

  int nbBlock = int((nH*nW)/(nHM*nWM*8));
  for(int i = 0; i <nHM*nWM ;i++){
    ImageMessage[i] = 0;
  }
  srand(key);
  int i = 0;
  for (int k = 0; k < nHM*nWM; k++){
    for(int b = 0; b < 8; b+=2){
      int pos = rand()%(nH*nW);
      if(img[pos] & (1 << b1))
      {
        ImageMessage[k] = ImageMessage[k] ^ (1 << b);
      }
      if(img[pos] & (1 << b2))
      {
        ImageMessage[k] = ImageMessage[k] ^ (1 << b+1);
      }
      i++;
    }
  }

  return ImageMessage;
}

OCTET* insertion3Bit(OCTET * img, OCTET * message, int b1, int b2, int b3, int key, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nH*nW);

  int nbBlock = int((nH*nW)/(nHM*nWM*8));
  for(int i = 0; i <nH*nW ;i++){
    ImageMessage[i] = img[i];
  } 
  srand(key);
  int k = 0;
  int pos;
  int nbPixel2 = nHM*nWM ;
  //- nHM*nWM%3;
  int j = 0;
  int i = 0;
  // for (int i = 0; i < nbPixel2; i++){
  while (i < nbPixel2){
    //for(int j = 0; j < 8; j+=3){

      pos = rand()%(nH*nW);
      if(get_bit(img[pos], b1) ^ get_bit(message[i], j)){
        ImageMessage[pos] = ImageMessage[pos] ^ (1 << b1); 
      }
      j++;
      if (j > 8){
        j=j%8;
        i++;
      }
      if(i > nbPixel2){
        break;
      }
      if(get_bit(img[pos], b2) ^ get_bit(message[i], j)){
        ImageMessage[pos] = ImageMessage[pos] ^ (1 << b2); 
      }
      j++;
      if (j > 8){
        j=j%8;
        i++;
      }
      if(i > nbPixel2){
        break;
      }
      if(get_bit(img[pos], b3) ^ get_bit(message[i], j)){
        ImageMessage[pos] = ImageMessage[pos] ^ (1 << b3); 
      }
      j++;
      if (j > 8){
        j=j%8;
        i++;
      }
      if(i > nbPixel2){
        break;
      }
      k++;
  }
  printf("i end : %i\n", i);

  return ImageMessage; 
}

OCTET* insertion3BitEctract(OCTET * img, int b1, int b2 ,int b3 , int key, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nHM*nWM);

  int nbBlock = int((nH*nW)/(nHM*nWM*8));
  for(int i = 0; i <nHM*nWM ;i++){
    ImageMessage[i] = 0;
  }
  srand(key);
  int nbPixel2 = nHM*nWM ;
  //- nHM*nWM%3;
  int j = 0;
  int i = 0;
  int pos;

    while (i < nbPixel2){
    //for(int j = 0; j < 8; j+=3){

      pos = rand()%(nH*nW);
      if(img[pos] & (1 << b1)){
        ImageMessage[i] = ImageMessage[i] ^ (1 << j); 
      }
      j++;
      if (j > 8){
        j=j%8;
        i++;
      }
      if(i > nbPixel2){
        break;
      }
      if(img[pos] & (1 << b2)){
        ImageMessage[i] = ImageMessage[i] ^ (1 << j); 
      }
      j++;
      if (j > 8){
        j=j%8;
        i++;
      }
      if(i > nbPixel2){
        break;
      }
      if(img[pos] & (1 << b3)){
        ImageMessage[i] = ImageMessage[i] ^ (1 << j); 
      }
      j++;
      if (j > 8){
        j=j%8;
        i++;
      }
      if(i > nbPixel2){
        break;
      }
      // k++;
  }

  return ImageMessage;
}

float ETH(OCTET* imgA, OCTET* imgB, int nTaille ){
  float sum = 0;
  for(int i = 0; i < nTaille; i++){
    for(int j = 0; j <8; j++){
      sum += int(get_bit(imgA[i], j) ^get_bit(imgB[i], j));
    }
  }

  return (sum/(nTaille*8))*1000;

}

OCTET* attackExtractionRand(OCTET* img, int bit, int nH, int nW, int nHM, int nWM ){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nHM*nWM);
  double entropie = 10;
  int key = 0;
  while (entropie > 7.9 && key < 256 ){ // 
    ImageMessage = insertionRandMaxiEctract(img, bit, key, nH, nW, nHM, nWM);
    entropie = entropy(ImageMessage, nHM, nWM);
    printf("entropy %f, key : %i \n",entropie, key);
    key++;

  }
  return ImageMessage;
}

OCTET* insertion(OCTET * img, OCTET * message, int bit, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nH*nW);

  int nbBlock = int((nH*nW)/(nHM*nWM*8));
  for(int i = 0; i <nH*nW ;i++){
    ImageMessage[i] = img[i];
  } 

  int k = 0;
  for (int i = 0; i < nHM*nWM; i++){
    for(int j = 0; j < 8; j++){
      if(get_bit(img[k*nbBlock],bit) ^ get_bit(message[i],j)){
        ImageMessage[k*nbBlock] = ImageMessage[k*nbBlock] ^ (1 << bit); 
      }
      binaryM(ImageMessage[k*nbBlock], "i :");

      k++;
    } 
  }

  return ImageMessage; 
}

OCTET* insertionEctract(OCTET * img, int bit, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nHM*nWM);

  int nbBlock = int((nH*nW)/(nHM*nWM*8));
  for(int i = 0; i <nHM*nWM ;i++){
    ImageMessage[i] = 0;
  }


  int i = 0;
  for (int k = 0; k < nHM*nWM; k++){
    for(int b = 0; b < 8; b++){
      if(img[i*nbBlock] & (1 << bit))
      {
        ImageMessage[k] = ImageMessage[k] ^ (1 << b);
      }
      i++;
    }
  }

  return ImageMessage;

}

OCTET* insertionParPixel(OCTET * img, OCTET * message, int bit, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nH*nW);
  int nbBlock = int((nH*nW)/(nHM*nWM));
  for (int i = 0; i < nH*nW; i++){
    ImageMessage[i] = img[i];
  }

  for(int i = 0; i <nHM*nWM ;i++){
    ImageMessage[i*nbBlock] = message[i];
  } 

  return ImageMessage; 
}

OCTET* insertionParPixelEctract(OCTET * img, int bit, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage2;
  allocation_tableau(ImageMessage2, OCTET, nHM*nWM);

  int nbBlock = int((nH*nW)/(nHM*nWM));
  int j = 0;
  for (int k = 0; k < nHM*nWM; k++){
    ImageMessage2[k] = img[j*nbBlock];// img[i*nbBlock];
    j++;
  }
  return ImageMessage2;
}

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

OCTET* entropyBlock16Ntaille(OCTET* img, int nTaille){
  OCTET* entropyBox;
  OCTET* entropyBoxImg;
  float* entropyBoxF;
  OCTET subArray[16];
  int tailleEntropyBox;
  float min=256;
  float max=-1;

  allocation_tableau(entropyBox, OCTET, int(nTaille/16));
  allocation_tableau(entropyBoxImg, OCTET, nTaille);

  if (nTaille%16 == 0)
  {
    tailleEntropyBox = int(nTaille/16);
  }else
  {
    tailleEntropyBox = int(nTaille/16) - 1;
  }
  
  for (int i = 0; i < tailleEntropyBox; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      subArray[j] = img[i*16 + j]; 
    } 
    float ent = entropy(subArray , 4, 4);
    min = std::min(min, ent);
    max = std::max(max, ent);
  } 

  for (int i = 0; i < tailleEntropyBox; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      subArray[j] = img[i*16 + j]; 
    } 
    if (min+max == 0)
    {
      entropyBox[i] = 0;
    }else{
      entropyBox[i] = 255 + int(((entropy(subArray , 4, 4)-min)*(0-255))/(max-min)); 
    }
  }

  int compt = 0;//compteur jusqu'à 16
  int indEnt = 0;//indice de l'entropie

  for (int i = 0; i < nTaille; i++)
  {
    if (compt == 16)
    {
      compt = 0;
      indEnt++;
    }

    if (min+max == 0)
    {
      entropyBoxImg[i] = 0;
    }else{
      entropyBoxImg[i] = entropyBox[indEnt];
    }
    compt++;
  }
  printf("nTaille modulo 16 %i\n",nTaille%16);

  printf("min : %f, max : %f \n", min, max);
  return entropyBoxImg;
}

OCTET* entropyBlock16(OCTET* img, int nTaille){
  OCTET* entropyBox;
  float* entropyBoxF;
  OCTET subArray[16];
  int tailleEntropyBox;
  float min=256;
  float max=-1;


  allocation_tableau(entropyBox, OCTET, int(nTaille/16));

  if (nTaille%16 == 0)
  {
    tailleEntropyBox = int(nTaille/16);
  }else
  {
    tailleEntropyBox = int(nTaille/16) - 1;
  }
  
  for (int i = 0; i < tailleEntropyBox; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      subArray[j] = img[i*16 + j]; 
    } 
    float ent = entropy(subArray , 4, 4);
    min = std::min(min, ent);
    max = std::max(max, ent);
  } 

  for (int i = 0; i < tailleEntropyBox; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      subArray[j] = img[i*16 + j]; 
    } 

    if (min+max == 0)
    {
      entropyBox[i] = 0;
    }else{
      entropyBox[i] = 255 + int((entropy(subArray , 4, 4)-min)*(0-255)/max-min); 
    }
  }  

  return entropyBox;
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
      imgNoise[valRand] = set_bit(img[valRand], rand()%8);
   
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

void print_result(OCTET* ImgIn, OCTET* ImgOut,OCTET* ImgOutMessage,OCTET* ImgInMessage,int nHM, int nWM, int nH, int nW){
  int nTaille = nW*nH;
  int nTailleM = nWM*nHM;
  printf("PSNR entre l'image initiale et l'image avec tatouage : %f dB\n",PSNR(ImgIn, ImgOut,nH,nW));
  printf("PSNR entre le message et le message extrait : %f dB\n",PSNR(ImgInMessage, ImgOutMessage,nHM,nWM));
  
  printf("Entropie de l'image avec tatouage : %f bits/pixel\n", entropy(ImgOut, nH, nW));
  printf("Entropie du message extrait : %f bits/pixel\n", entropy(ImgOutMessage, nHM, nWM));
  
  printf("ETH du message extrait : %f bits tout les 1000 bits\n",ETH(ImgInMessage, ImgOutMessage, nTailleM));
  printf("ETH du message avec tatouage : %f bits tout les 1000 bits\n",ETH(ImgIn, ImgOut, nTaille));
}

#endif