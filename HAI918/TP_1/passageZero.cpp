#include <stdio.h>
#include <cmath>
#include <vector>
#include "image_ppm.h"

using namespace std;
#define PI 3.14159265

bool estMax(int i, double* tab){

    return tab[i] == 1;
}
bool estMax(int i, OCTET* tab){

    return tab[i] == 255;
}

bool passageAZero(int i1, int i2){
    return (i1 >= 128 && i2 <= 128) || (i2 >= 128 && i1 <= 128);
}

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, S;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm \n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);

   OCTET *ImgIn, *ImgOut, *ImgOut2;
   OCTET *ImgPhase, *ImgBlack;
   double *maxLoc;

   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;

   allocation_tableau(ImgPhase, OCTET, nTaille);
   allocation_tableau(ImgBlack, OCTET, nTaille);
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);
   allocation_tableau(ImgOut2, OCTET, nTaille);
   allocation_tableau(maxLoc, double, nTaille);

 for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
       maxLoc[i*nW+j] = 0;
     }

 for (int i=0; i < nH-1; i++)
   for (int j=0; j < nW-1; j++)
     {
        int gj = ImgIn[i*nW+j+1] - ImgIn[i*nW+j];
        int gi = ImgIn[(i+1)*nW+j] - ImgIn[i*nW+j];

        double angle = atan2(gj, gi);
        ImgPhase[i*nW+j] = angle;

        ImgOut[i*nW+j] = max(sqrt(gi*gi), sqrt(gj*gj));
        //printf("img = %i\n",ImgOut[i*nW+j]);

    }

ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);


 for (int i=1; i < nH-1; i++)
   for (int j=1; j < nW-1; j++)
    {
        int m1, m2;
        int m = ImgOut[i*nW + j];
        double angle = ImgPhase[i*nW+j];
        if ((-PI/4) < angle && angle < (PI/4))
        {
            m1 = ImgOut[(i-1)*nW + j];
            m2 = ImgOut[(i+1)*nW + j];
        }else
        {
            if (((PI/2) > angle && angle > (PI/4)) || (-(PI/2) < angle && angle < -(PI/4)))
            {
                m1 = ImgOut[(i)*nW + j - 1];
                m2 = ImgOut[(i)*nW + j + 1];
            }
        }
        if (passageAZero((int) m1, (int) m2))
        {
            //maximumLocal.push_back(m);
            maxLoc[i*nW+j] = 1;
            ImgBlack[i*nW+j] = 255;
        }
    }
ecrire_image_pgm("img/black_zero.pgm", ImgBlack,  nH, nW);


    vector<double> maximumLocal;

 for (int i=1; i < nH-1; i++){
   for (int j=1; j < nW-1; j++){
       if (ImgOut[i*nW + j] > 20)
       {
           ImgOut2[i*nW+j] = 255;
           maximumLocal.push_back(i*nW+j);
       }else
       {
           if (ImgOut[i*nW + j] > 7)
           {
           }else
           {
               ImgOut2[i*nW+j] = 0;
           }
       }
   }
 }

    while (maximumLocal.size() != 0)
    {   
        //printf("while\n");
        vector<double> newMaximumLocal;
        for (int i=1; i < nH-1; i++){
            for (int j=1; j < nW-1; j++){
                if (ImgOut[i*nW + j] >= 7 && ImgOut[i*nW + j] < 20)
                {
                    if(!estMax(i*nW+j, ImgOut2)){
                        if (estMax(i*nW + j+1, ImgOut2)||estMax(i*nW + j-1, ImgOut2)||estMax((i-1)*nW + j, ImgOut2)||estMax((i+1)*nW + j, ImgOut2)||estMax((i+1)*nW + j+1, ImgOut2)||estMax((i+1)*nW + j-1, ImgOut2)||estMax((i-1)*nW + j-1, ImgOut2)||estMax((i-1)*nW + j+1, ImgOut2))
                        {
                            ImgOut2[i*nW+j] = 255;
                            //printf("push\n");
                            newMaximumLocal.push_back(i*nW+j);
                        }
                    }
                }
            }
        }
        maximumLocal = newMaximumLocal;
    }
    

   
//   ecrire_image_pgm("img/phase.pgm", ImgPhase,  nH, nW);
    ecrire_image_pgm("img/seuil_hist_zero.pgm", ImgOut2,  nH, nW);


   free(ImgIn);
   return 1;
}