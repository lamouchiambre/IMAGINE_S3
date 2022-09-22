#ifndef _IMGLIB_H_
#define _IMGLIB_H_

#include <stdio.h>
#include "image_ppm.h"
#include "AES.h"

int set_bit(int num, int position)
{
	int mask = 1 << position;
	return num | mask;
}

void binary(unsigned int num){
  for(int i = 256; i > 0; i = i/2){
    if(num & i){
      std::cout << "1 ";
    }else{
      std::cout << "0 ";
    }
  }
  std::cout << std::endl;
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
      /* code */
      subArray[j] = img[i*16 + j]; 
    } 
    float ent = entropy(subArray , 4, 4);
    min = std::min(min, ent);
    max = std::max(max, ent);
    
    //entropyBoxF[i] = entropy(subArray , 4, 4); 

  } 

  for (int i = 0; i < tailleEntropyBox; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      /* code */
      subArray[j] = img[i*16 + j]; 
    } 

    // entropyBox[i] = int(entropy(subArray , 4, 4))*10; 
    printf("%i \n", int(entropy(subArray , 4, 4)));
    // entropyBox[i] = 255 - 10*int(entropy(subArray , 4, 4));
    if (min+max == 0)
    {
      entropyBox[i] = 0;
    }else{
      entropyBox[i] = 255 + int((int(entropy(subArray , 4, 4)-min)*(0-255))/int(max-min)); 
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