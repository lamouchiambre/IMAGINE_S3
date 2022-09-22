// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "imglib.h"
#include <iostream>
#include <string>

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
    //sscanf (argv[2],"%s",cNomImgEcrite);

    OCTET *ImgIn, *ImgOut;
   
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
  
    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    //allocation_tableau(ImgOut, OCTET, nTaille);
	
    Histogramme(ImgIn, nW, nH);


   //ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}
