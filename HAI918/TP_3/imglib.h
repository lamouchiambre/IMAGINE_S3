#ifndef _IMGLIB_H_
#define _IMGLIB_H_

#include <stdio.h>
#include "image_ppm.h"
#include <bitset>
#include "AES.h"

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
// OCTET* Npremier(OCTET * img, OCTET * message, int nH, int nW, int nHM, int nWM){
//   OCTET* ImageMessage;
//   allocation_tableau(ImageMessage, OCTET, nH*nW);

//   int nbBlock = int((nH*nW)/(nHM*nWM));
//   for(int i = 0; i <nH*nW ;i++){
//     ImageMessage[i] = img[i];
//   }

//   printf("%i %i %i ", nH*nW, nHM*nWM, nbBlock);

//   for (int k = 0; k < nHM*nWM; k++){
//     if(get_bit(ImageMessage[k*nbBlock],0) != get_bit(message[k],7)){
//     // if(get_bit(message[k],0)){
//       ImageMessage[k*nbBlock] = set_bit(ImageMessage[k*nbBlock], 0);
//     }
//   }
//   return ImageMessage;

// }
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


OCTET* Poid(OCTET * img, OCTET * message, int bit, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nH*nW);

  int nbBlock = int((nH*nW)/(nHM*nWM*8));
  for(int i = 0; i <nH*nW ;i++){
    ImageMessage[i] = img[i];
  } 

  int k = 0;
  for (int i = 0; i < nHM*nWM; i++){
    for(int j = 0; j < 8; j++){

      // printf("XXXXXXXXXXXXXXX\n");
      // binary(ImageMessage[k*nbBlock]);
      // binary(message[i]);
      if((img[k*nbBlock] & (1 << bit))|(message[i] & (1 << bit)))
      {
        ImageMessage[k*nbBlock] = img[k*nbBlock] | (1 << bit);
      }
      //ImageMessage[k*nbBlock] = img[k*nbBlock]|((img[k*nbBlock] & (1 << bit))|(message[i] & (1 << bit)));
      //binary(ImageMessage[k*nbBlock]);

      k++;

    } 

  }
  return ImageMessage; 

}

OCTET* PoidPixel(OCTET * img, OCTET * message, int bit, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nH*nW);
  int nbBlock = int((nH*nW)/(nHM*nWM));
  for (int i = 0; i < nH*nW; i++){
    ImageMessage[i] = img[i];
  }

  for(int i = 0; i <nHM*nWM ;i++){
    // printf("xxxxxxxxxxxxxxxxxxx\n");
    // printf("%i\n", ImageMessage[i]);
    ImageMessage[i*nbBlock] = message[i];
    // printf("%i\n", ImageMessage[i]);
  } 

  return ImageMessage; 
}

OCTET* PoidPixelEctract(OCTET * img, int bit, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage2;
  allocation_tableau(ImageMessage2, OCTET, nHM*nWM);

  int nbBlock = int((nH*nW)/(nHM*nWM));
  // for (int i = 0; i < nHM*nWM; i++)
  // {
  //   printf("%i\n", ImageMessage2[i]);
  //   //std::cout << ImageMessage2[i] << std::endl;
  // }
  // std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
  
  int j = 0;
  for (int k = 0; k < nHM*nWM; k++){
    ImageMessage2[k] = img[j*nbBlock];// img[i*nbBlock];
    j++;
  }
  // for (int i = 0; i < nHM*nWM; i++)
  // {
  //   printf("%i\n", ImageMessage2[i]);
  //   // std::cout << ImageMessage2[i] << std::endl;
  // }
  
  return ImageMessage2;
}

OCTET* PoidEctract(OCTET * img, int bit, int nH, int nW, int nHM, int nWM){
  OCTET* ImageMessage;
  allocation_tableau(ImageMessage, OCTET, nHM*nWM);

  int nbBlock = int((nH*nW)/(nHM*nWM*8));
  for(int i = 0; i <nHM*nWM ;i++){
    ImageMessage[i] = 0;
  }


  int i = 0;
  for (int k = 0; k < nHM*nWM; k++){
    for(int b = 7; b >= 0; b--){
      printf("XXXXXXXXXXXXXXX\n");
      binary(img[i*nbBlock]);
      binary(ImageMessage[k]);
      binary((1 << bit));
      binary((1 << b));
      //binary(message[i]);
      if(img[i*nbBlock] & (1 << bit))
      {
        ImageMessage[k] = ImageMessage[k] | (1 << b);
      }
      i++;
      binary(ImageMessage[k]);
    }
  }

  return ImageMessage;

}

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
      //imgNoise[valRand] = rand()%256;     
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


#endif