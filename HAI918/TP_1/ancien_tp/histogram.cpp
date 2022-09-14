// histogram_pgm.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"


int main(int argc, char* argv[]){
    char cNomImgLue[250], cNomImgEcrite[250], type[250];
    int nH, nW, nTaille, S;
    int indice;
    //colonne 1; ligne 0
  
    if (argc != 2) 
    {
        printf("Usage: ImageIn.pgm \n"); 
        exit (1) ;
    }
   
    sscanf (argv[1],"%s",cNomImgLue) ;
    //sscanf (argv[2],"%s",cNomImgEcrite);
    //sscanf (argv[3],"%d",&indice);
    //sscanf (argv[4],"%s",type);
    

    OCTET *ImgIn;
    int * tabHisto;
   
    //printf("here");
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    allocation_tableau(tabHisto, int, 256);

    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    
    for(int i = 0; i < 256; i++){
        tabHisto[i] = 0;
    }

    for (int i=0; i < nH; i++)
        for (int j=0; j < nW; j++)
            tabHisto[ImgIn[i*nW+j]] += 1;
        
    for (int i = 0; i < 256; i++)
    {
        printf("%d %d \n", i, tabHisto[i]);
    }
    
    // if (type == "lines"){
    //     for (int i = 0; i < nW; i++)
    //     {
    //         printf("%d",ImgIn[indice * nW + i]);
    //     }
        
    // }
    // else{
    //     /* code */
    //     for (int i = 0; i < nH; i++)
    //     {
    //         printf("%d %d \n",i, ImgIn[i * nW + indice]);
    //     }
    // }
    

    
    
    



}