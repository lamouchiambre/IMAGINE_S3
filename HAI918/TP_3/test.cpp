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
    char cNomImgLue[250],cNomImgEcrit[250], chiffrement[250];
    std::string name;
    int nH, nW, nTaille, S;
    
    if (argc != 2) 
    {
        printf("Usage: ImageIn.pgm \n"); 
        exit (1) ;
    }
    sscanf (argv[1],"%s",cNomImgLue);

    OCTET *ImgIn;
   
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

    Histogramme(ImgIn, nW, nH);

    // std::string title = "test";
    // std::string ext =".pgm";
    // name = std::string(cNomImgLue);
    // std::cout <<name<<std::endl;
    // std::string nameFile = title+ext;

    //str to string
    // strcpy(cNomImgEcrit, nameFile.c_str());
    // ecrire_image_pgm(cNomImgEcrit, ImgIn, nH, nW);

    free(ImgIn);

    return 1;
}