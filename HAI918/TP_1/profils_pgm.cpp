// histogram_pgm.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"


int main(int argc, char* argv[]){
    char cNomImgLue[250], cNomImgEcrite[250], type[250];
    int nH, nW, nTaille, S;
    int indice;
    //colonne 1; ligne 0
  
    if (argc != 5) 
    {
        printf("Usage: ImageIn.pgm histo.dat indice type \n"); 
        exit (1) ;
    }
   
    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgEcrite);
    sscanf (argv[3],"%d",&indice);
    sscanf (argv[4],"%s",type);
    

    OCTET *ImgIn, *ImgOut;
   
    //printf("here");
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    
    if (strcmp(type,"lines")){
        for (int i = 0; i < nW; i++)
        {
            printf("%d %d \n",i, ImgIn[indice * nW + i]);
        }
        
    }
    else{
        /* code */
        for (int i = 0; i < nH; i++)
        {
            printf("%d %d \n",i, ImgIn[i * nW + indice]);
        }
    }

}