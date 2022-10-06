#include <stdio.h>
#include "imglib.h"
#include <iostream>
#include <string>

using namespace std;

constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

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
 
  OCTET *ImgIn, *ImgMaxPool1, *ImgMaxPool2, *ImgMaxPool3, *ImgConv1, *ImgConv2, *ImgConv3,*ImgConv11, *ImgConv21, *ImgConv31 ;
  std::string sNomImgLue = std::string(cNomImgLue);
  std::string ext = ".pgm";
  std::string sNomImgOutSub = sNomImgLue+"Sub"+ext;
  std::string sNomImgLueMessage = sNomImgLue+"Message"+ext;
  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
 
  printf("hello\n");
  
  double Prewitt_H[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
  double Laplacian[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
  double Identitee[9] = {0, 0, 0, 0, 1, 0, 0, 0, 0};
  double Nettete[9] = {0, -1, 0, -1, 5, -1, 0,-1, 0};
  double Flou[9] = {0.11, 0.11, 0.11, 0.11, 0.11, 0.11, 0.11, 0.11, 0.11};
   
  printf("%f %f %f %f %f %f %f %f %f\n", Identitee[0], Identitee[1], Identitee[2], Identitee[3], Identitee[4], Identitee[5], Identitee[6], Identitee[7], Identitee[8]);
 
  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
 
  allocation_tableau(ImgMaxPool1, OCTET, 127*127);
  allocation_tableau(ImgMaxPool2, OCTET, 127*127);
  allocation_tableau(ImgMaxPool3, OCTET, 127*127);

  allocation_tableau(ImgConv1, OCTET, (nH-2)*(nW-2));
  allocation_tableau(ImgConv2, OCTET, (nH-2)*(nW-2));
  allocation_tableau(ImgConv3, OCTET, (nH-2)*(nW-2));
  
  allocation_tableau(ImgConv11, OCTET, (127-2)*(127-2));
  allocation_tableau(ImgConv21, OCTET, (127-2)*(127-2));
  allocation_tableau(ImgConv31, OCTET, (127-2)*(127-2));
  
  //First conv
  ImgConv1 = conv3x3(ImgIn, nH, nW, Prewitt_H);
  ImgConv2 = conv3x3(ImgIn, nH, nW, Flou);
  ImgConv3 = conv3x3(ImgIn, nH, nW, Laplacian);


  //First MaxPool
  ImgMaxPool1 = maxPooling2x2(ImgConv1, nH-2, nW-2);
  ImgMaxPool2 = maxPooling2x2(ImgConv1, nH-2, nW-2);
  ImgMaxPool3 = maxPooling2x2(ImgConv1, nH-2, nW-2);

  //Second conv
  ImgConv11 = conv3x3(ImgMaxPool1, 125, 125, Prewitt_H);
  ImgConv21 = conv3x3(ImgMaxPool1, 125, 125, Flou);
  ImgConv31 = conv3x3(ImgMaxPool1, 125, 125, Laplacian);

  vector flatten_img = flatten(ImgIn, nTaille);
  vector<vector<double>> vect_all;
  vector<vector<double>> w1;
  vector<vector<double>> w2;

  //vector<double> vect1 = {1.0, 1.0, 1.0};

  vector<double> nn1 = {1.0, 2.0};
  vector<double> nn2 = {0.0, 0.0, 0.8};
  vector<double> nn3 = {1.2, 0.16};


  double n1 = 1;
  vector<double> w11 = {0.5, -1.5, 0.2};
  vector<double> w12 = {-0.5, -2.5, 0.3};

  vector<double> w21 = {0.3, 0.5};
  vector<double> w22 = {-0.5, 0.3};
  vector<double> w23 = {1.5, 0.2};


  w1.push_back(w11);
  w1.push_back(w12);
  
  w2.push_back(w21);
  w2.push_back(w22);
  w2.push_back(w23);

  cout << 1.0/6.0 <<endl;
  //cout << "g vect1 w1 : "<<g(vect1, w1) <<endl;
  cout << "g3 n1 w1 : "<<g3(n1, w11) <<endl;
  cout << "g3 n1 w1 : "<<g3(n1, w12) <<endl;
  cout << "g3 n1 w1 : "<<g3(n1, w21) <<endl;
  cout << "g3 n1 w1 : "<<g3(n1, w22) <<endl;
  cout << "g3 n1 w1 : "<<g3(n1, w23) <<endl;

  vector<double> v_nn2 = gForLayer3(nn1, w1);
  vector<double> v_nn3 = gForLayer3(v_nn2, w2);
  
 
  cout << "v_nn2 = ";
  for (auto& v : v_nn2){
     cout << v << " ";
  }
  cout << endl;

  cout << "v_nn3 = ";
  for (auto& v : v_nn3){
     cout << v << " ";
  }
  cout << endl;

  sNomImgLueMessage = sNomImgLue+"_maxpool"+ext;
  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgMaxPool1, 127, 127);

  sNomImgLueMessage = sNomImgLue+"Conv11"+ext;
  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgConv11, 125, 125);

  sNomImgLueMessage = sNomImgLue+"_Filter1"+ext;
  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgConv1, nH-2, nW-2);
    
   return 1;
}
