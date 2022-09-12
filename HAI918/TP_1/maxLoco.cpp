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
       //if ( ImgIn[i*nW+j] < S) ImgOut[i*nW+j]=0; else ImgOut[i*nW+j]=255;
        int gj = ImgIn[i*nW+j+1] - ImgIn[i*nW+j];
        int gi = ImgIn[(i+1)*nW+j] - ImgIn[i*nW+j];
        
        //int gj = ImgBlack[i*nW+j+1] - ImgBlack[i*nW+j];
        //int gi = ImgBlack[(i+1)*nW+j] - ImgBlack[i*nW+j];

        double angle = atan2(gi,gj);//*180 / PI;
        ImgPhase[i*nW+j] = angle;
        //printf("%i\n", ImgPhase[i*nW+j]);
        ImgOut[i*nW+j] = sqrt(gi*gi + gj*gj);

     }
ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);

 for (int i=1; i < nH-1; i++)
   for (int j=1; j < nW-1; j++)
    {
        int m1, m2;
        int m = ImgOut[i*nW + j];
        double angle = ImgPhase[i*nW+j]; //atan(gj/gi);
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
        if (m1 < m && m2 < m)
        {
            //maximumLocal.push_back(m);
            maxLoc[i*nW+j] = 1;
            ImgBlack[i*nW+j] = 255;
        }
    }
    vector<double> maximumLocal;

 for (int i=1; i < nH-1; i++){
   for (int j=1; j < nW-1; j++){
       if (ImgOut[i*nW + j] > 30)
       {
           ImgOut2[i*nW+j] = 255;
           maximumLocal.push_back(i*nW+j);
       }else
       {
           if (ImgOut[i*nW + j] >= 10)
           {
            //  ImgOut2[i*nW + j] = ImgOut[i*nW + j];
            //    if (estMax(i*nW + j+1, maxLoc)||estMax(i*nW + j-1, maxLoc)||estMax((i-1)*nW + j, maxLoc)||estMax((i+1)*nW + j, maxLoc)||estMax((i+1)*nW + j+1, maxLoc)||estMax((i+1)*nW + j-1, maxLoc)||estMax((i-1)*nW + j-1, maxLoc)||estMax((i-1)*nW + j+1, maxLoc))
            //    {
            //        ImgOut2[i*nW+j] = 255;
            //    }else
            //    {
            //        ImgOut2[i*nW+j] = 0;
            //    }
           }else
           {
               ImgOut2[i*nW+j] = 0;
           }
       }
   }
 }

    while (maximumLocal.size() != 0)
    {
        vector<double> newMaximumLocal;
        for (int i=1; i < nH-1; i++){
            for (int j=1; j < nW-1; j++){
                if (ImgOut[i*nW + j] >= 10 && ImgOut[i*nW + j] < 30)
                {
                    if(!estMax(i*nW+j, ImgOut2)){
                        if (
                            estMax(i*nW + j+1, ImgOut2)||
                            estMax(i*nW + j-1, ImgOut2)||
                            estMax((i-1)*nW + j, ImgOut2)||
                            estMax((i+1)*nW + j, ImgOut2)||
                            estMax((i+1)*nW + j+1, ImgOut2)||
                            estMax((i+1)*nW + j-1, ImgOut2)||
                            estMax((i-1)*nW + j-1, ImgOut2)||
                            estMax((i-1)*nW + j+1, ImgOut2)
                        /*||
                        estMax(i*nW + j+1, maxLoc)||estMax(i*nW + j-1, maxLoc)||estMax((i-1)*nW + j, maxLoc)||estMax((i+1)*nW + j, maxLoc)||estMax((i+1)*nW + j+1, maxLoc)||estMax((i+1)*nW + j-1, maxLoc)||estMax((i-1)*nW + j-1, maxLoc)||estMax((i-1)*nW + j+1, maxLoc)
                        */
                       )
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

   ecrire_image_pgm("img2/black.pgm", ImgBlack,  nH, nW);
//   ecrire_image_pgm("img/phase.pgm", ImgPhase,  nH, nW);
    ecrire_image_pgm("img2/seuil_hist.pgm", ImgOut2,  nH, nW);

   free(ImgIn);
   return 1;
}