#ifndef _CNNLIB_H_
#define _CNNLIB_H_

#include <stdio.h>
#include "image_ppm.h"
#include "imag_lib.h"
#include <bitset>
#include "AES.h"


//fonction un neuronne
// https://blog.paperspace.com/filters-in-convolutional-neural-networks/

double Relu(double f){
    return f> 0.0 f : 0.0;
}

int max(OCTET* list, int len){
    max = list[0];
    for (int i = 0; i < len; i++){
        max  = max > list[i] ? max : list[0];
    }

}

OCTET * maxPooling(OCTET* ImgIn, int nH, int nW, int s1, int s2){
    OCTET * ImgMaxPool;
    OCTET * list_tmp;
    int len_maxPool = (nH*nW)/(s1*s2);
    allocation_tableau(ImgMaxPool, OCTET, len_maxPool);
    allocation_tableau(list_tmp, OCTET, s1*s2);


    for (int i = 0; i < len_maxPool; i++){

        for(int j = 0; j < s1*s2; j++){
            list_tmp[j] = ImageIn[i*(s1*s2) + j]; 
        }

        ImgMaxPool[i] = ImgMaxPoolmax(list_tmp, s1*s2);

    }

    return ImgMaxPool;
}

OCTET filter(int type){
    OCTET Prewitt_H =  [-1, 0, 1,
                        -1, 0, 1,
                        -1, 0, 1];

    OCTET Prewitt_V = [1, 1, 1,
                       0, 0, 0,
                      -1, -1, -1];
    
    OCTET Sobel_H =  [-1, 0, 1,
                      -2, 0, 2,
                      -1, 0, 1];

    OCTET Sobel_V = [1, 2, 1,
                     0, 0, 0,
                    -1, -2, -1];

    OCTET Laplacian = [-1, -1, -1,
                       -1, 8, -1,
                       -1, -1, -1];

    //Robinson Compass Masks
    OCTET north_west = [-2, -1, 0,
                        -1,  0, 1,
                         0,  1, 2];

    OCTET north = [-2, -1, 0,
                   -1,  0, 1,
                     0,  1, 2];

}

// double calculMatrice(OCTET* ImgIn,int nH, int nW, OCTET* filter, int s1, int s2, int posX, int posY){
//     double sum = 0.0;
//     for(int i = - int((s1-1)/2); i <= int((s1-1)/2); i++ ){
//         for(int j = - int((s2-1)/2); j <= int((s2-1)/2); j++ ){
//             sum += ImgIn[(posX + i)*nW + posY]
//         }
//     }

// }

// OCTET conv(OCTET* ImgIn,int nH, int nW, OCTET* filter, int s1, int s2){
//     //int w = s1;
//     OCTET * ImgMaxPool;
//     OCTET * list_tmp;
    
//     //int len_maxPool = (nH*nW)/(s1*s2);
//     int len_maxPool = (nH-(s1-1))*(nW-(s2-1));
    
//     allocation_tableau(ImgMaxPool, OCTET, len_maxPool);
//     allocation_tableau(list_tmp, OCTET, s1*s2);


//     for (int i = int((s1-1)/2); i < len_maxPool - int((s1-1)/2); i++){
//         for (int j = int((s2-1)/2); j < len_maxPool - int((s2-1)/2); j++){

//             for(int j = 0; j < s1*s2; j++){

//                 list_tmp[j] = ImageIn[i*(s1*s2) + j]; 
//             }

//             ImgMaxPool[i] = ImgMaxPoolmax(list_tmp, s1*s2);
        
//         }

//     }

//     return ImgMaxPool;

    
// }

OCTET conv3x3(OCTET* ImgIn,int nH, int nW, OCTET* filter){
    //int w = s1;
    OCTET * ImgMaxPool;
    OCTET * list_tmp;
    
    //int len_maxPool = (nH*nW)/(s1*s2);
    int len_maxPool = (nH-(s1-1))*(nW-(s2-1));
    
    allocation_tableau(ImgMaxPool, OCTET, len_maxPool);
    allocation_tableau(list_tmp, OCTET, s1*s2);


    for (int i = 1; i < len_maxPool - 1; i++){
        for (int j = 1; j < len_maxPool - 1; j++){

            int p = (
                ImgIn[(i-1)*nW+(j-1)] * filter[0] +
                ImgIn[i*nW+(j-1)] * filter[1]+
                ImgIn[(i+1)*nW+(j-1)] * filter[2]+

                ImgIn[(i-1)*nW+j] * filter[3] +
                ImgIn[i*nW+j] * filter[4]+
                ImgIn[(i+1)*nW+j] * filter[5]+

                ImgIn[(i-1)*nW+(j+1)] * filter[6] +
                ImgIn[i*nW+(j+1)] * filter[7]+
                ImgIn[(i+1)*nW+(j+1)] * filter[8]

            );

            ImgMaxPool[i] = p;
        
        }

    }

    return ImgMaxPool;

    
}

#endif