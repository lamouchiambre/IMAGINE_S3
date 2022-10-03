#ifndef _CNNLIB_H_
#define _CNNLIB_H_
#include <stdio.h>
#include <bitset>
#include "AES.h"


//fonction un neuronne
// https://blog.paperspace.com/filters-in-convolutional-neural-networks/

double Relu(double f){
    return f> 0.0 ? f : 0.0;
}

int max(OCTET* list, int len){
    int max = list[0];
    for (int i = 0; i < len; i++){
        max  = max > list[i] ? max : list[0];
    }
    return max;
}

OCTET * maxPooling(OCTET* ImgIn, int nH, int nW, int s1, int s2){
    OCTET * ImgMaxPool;
    OCTET * list_tmp;
    int len_maxPool = (nH*nW)/(s1*s2);
    allocation_tableau(ImgMaxPool, OCTET, len_maxPool);
    allocation_tableau(list_tmp, OCTET, s1*s2);


    for (int i = 0; i < len_maxPool; i++){

        for(int j = 0; j < s1*s2; j++){
            list_tmp[j] = ImgIn[i*(s1*s2) + j]; 
        }

        ImgMaxPool[i] = max(list_tmp, s1*s2);

    }

    return ImgMaxPool;
}

OCTET* filter(int type){
    OCTET Prewitt_H[9] =  [-1, 0, 1,
                        -1, 0, 1,
                        -1, 0, 1];

    OCTET Prewitt_V[9] = [1, 1, 1,
                       0, 0, 0,
                      -1, -1, -1];
    
    OCTET Sobel_H[9] =  [-1, 0, 1,
                      -2, 0, 2,
                      -1, 0, 1];

    OCTET Sobel_V[9] = [1, 2, 1,
                     0, 0, 0,
                    -1, -2, -1];

    OCTET Laplacian[9] = [-1, -1, -1,
                       -1, 8, -1,
                       -1, -1, -1];

    //Robinson Compass Masks
    OCTET north_west[9] = [-2, -1, 0,
                        -1,  0, 1,
                         0,  1, 2];

    OCTET north[9] = [-2, -1, 0,
                   -1,  0, 1,
                     0,  1, 2];

    return north;

}

OCTET* conv3x3(OCTET* ImgIn,int nH, int nW, OCTET* filter){
    //int w = s1;
    OCTET * ImgMaxPool;
    OCTET * list_tmp;
    
    //int len_maxPool = (nH*nW)/(s1*s2);
    int len_maxPool = (nH-2)*(nW-2);
    
    allocation_tableau(ImgMaxPool, OCTET, len_maxPool);
    allocation_tableau(list_tmp, OCTET, 9);


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