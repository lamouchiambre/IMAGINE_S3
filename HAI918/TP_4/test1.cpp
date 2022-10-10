#include <stdio.h>
#include "imglib.h"
#include <iostream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

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
 
  OCTET *ImgIn, *ImgMaxPool1, *ImgMaxPool2, *ImgMaxPool3, *ImgMaxPool4, *ImgMaxPool5, *ImgConv1, *ImgConv2, *ImgConv3, *ImgConv4, *ImgConv5;
  OCTET *ImgConv11, *ImgConv21, *ImgConv31, *ImgConv41, *ImgConv51 ;
  OCTET *ImgConv12, *ImgConv22, *ImgConv32, *ImgConv42, *ImgConv52 ;
  OCTET *ImgConv13, *ImgConv23, *ImgConv33, *ImgConv43, *ImgConv53 ;
  OCTET *ImgConv14, *ImgConv24, *ImgConv34, *ImgConv44, *ImgConv54 ;
  OCTET *ImgConv15, *ImgConv25, *ImgConv35, *ImgConv45, *ImgConv55 ;

  OCTET *ImgMaxPool11, *ImgMaxPool21, *ImgMaxPool31, *ImgMaxPool41, *ImgMaxPool51 ;
  OCTET *ImgMaxPool12, *ImgMaxPool22, *ImgMaxPool32, *ImgMaxPool42, *ImgMaxPool52 ;
  OCTET *ImgMaxPool13, *ImgMaxPool23, *ImgMaxPool33, *ImgMaxPool43, *ImgMaxPool53 ;
  OCTET *ImgMaxPool14, *ImgMaxPool24, *ImgMaxPool34, *ImgMaxPool44, *ImgMaxPool54 ;
  OCTET *ImgMaxPool15, *ImgMaxPool25, *ImgMaxPool35, *ImgMaxPool45, *ImgMaxPool55 ;
  OCTET *ImgFlat, * ImgFlatMean;

  std::string sNomImgLue = std::string(cNomImgLue);
  std::string ext = ".pgm";
  std::string sNomImgOutSub = sNomImgLue+"Sub"+ext;
  std::string sNomImgLueMessage = sNomImgLue+"Message"+ext;
  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
 
  printf("hello\n");
  
  double Prewitt_H[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};//contour
  double Prewitt_V[9] = {1, 1, 1, 0, 0, 0, -1, -1, 1};//contour
  double Laplacian[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};//
  double Identitee[9] = {0, 0, 0, 0, 1, 0, 0, 0, 0};
  double Nettete[9] = {0, -1, 0, -1, 5, -1, 0,-1, 0};
  double Flou[9] = {0.11, 0.11, 0.11, 0.11, 0.11, 0.11, 0.11, 0.11, 0.11};//passe bas
   
  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  
  allocation_tableau(ImgFlat, OCTET, (315)*(315));

  allocation_tableau(ImgConv1, OCTET, (nH-2)*(nW-2));
  allocation_tableau(ImgConv2, OCTET, (nH-2)*(nW-2));
  allocation_tableau(ImgConv3, OCTET, (nH-2)*(nW-2));
  allocation_tableau(ImgConv4, OCTET, (nH-2)*(nW-2));
  allocation_tableau(ImgConv5, OCTET, (nH-2)*(nW-2));
  
  allocation_tableau(ImgMaxPool1, OCTET, 127*127);
  allocation_tableau(ImgMaxPool2, OCTET, 127*127);
  allocation_tableau(ImgMaxPool3, OCTET, 127*127);
  allocation_tableau(ImgMaxPool4, OCTET, 127*127);
  allocation_tableau(ImgMaxPool5, OCTET, 127*127);

  allocation_tableau(ImgConv11, OCTET, (125)*(125));
  allocation_tableau(ImgConv21, OCTET, (125)*(125));


  //maxpooling 2
  allocation_tableau(ImgMaxPool11, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool21, OCTET, (62)*(62));

  // std::string path = "../../../archive/Shoe_vs_Sandal_vs_Boot_Dataset/Boot";
  std::string path = "img_in/";
  int etape = 0;

  // for (const auto & entry : fs::directory_iterator(path)){
  //   std::cout << entry.path() << std::endl;
  //   strcpy(cNomImgLue, entry.path().c_str());
  //   sNomImgLue = std::string(cNomImgLue);

  //   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    
  //   printf("First conv\n");
  //   ImgConv1 = conv3x3(ImgIn, nH, nW, Prewitt_H);

  //   printf("First MaxPool\n");
  //   ImgMaxPool1 = maxPooling2x2(ImgConv1, nH-2, nW-2);

    
  //   printf("Second conv\n");
  //   ImgConv11 = conv3x3(ImgMaxPool1, 127, 127, Prewitt_H);

  //   printf("Second MaxPool\n");
  //   ImgMaxPool11 = maxPooling2x2(ImgConv11, 125, 125);


  //   sNomImgLueMessage = sNomImgLue+"_MaxPool11"+ext;
  //   cout << sNomImgLueMessage << endl ;
  //   strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  //   ecrire_image_pgm(cNomImgEcrite, ImgMaxPool11, 63, 63);


  //   sNomImgLueMessage = sNomImgLue+"_Conv11"+ext;
  //   strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  //   ecrire_image_pgm(cNomImgEcrite, ImgConv11, 125, 125);

  //   sNomImgLueMessage = sNomImgLue+"_MaxPool1"+ext;
  //   strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  //   ecrire_image_pgm(cNomImgEcrite, ImgMaxPool1, 127, 127);

  //   sNomImgLueMessage = sNomImgLue+"_Conv1"+ext;
  //   strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  //   ecrire_image_pgm(cNomImgEcrite, ImgConv1, nH-2, nW-2);
  // }


  // // First conv
  // printf("First conv\n");
  // ImgConv1 = conv3x3(ImgIn, nH, nW, Prewitt_H);
  // ImgConv2 = conv3x3(ImgIn, nH, nW, Prewitt_V);
  // ImgConv3 = conv3x3(ImgIn, nH, nW, Flou);
  // ImgConv4 = conv3x3(ImgIn, nH, nW, Nettete);
  // ImgConv5 = conv3x3(ImgIn, nH, nW, Laplacian);


  // // First MaxPool
  // printf("First MaxPool\n");

  // ImgMaxPool1 = maxPooling2x2(ImgConv1, nH-2, nW-2);

  // // Second conv
  // printf("Second conv\n");

  // ImgConv11 = conv3x3(ImgMaxPool1, 127, 127, Prewitt_H);
  // ImgConv21 = conv3x3(ImgMaxPool1, 127, 127, Prewitt_V);

  // // 2 second maxpooling
  // ImgMaxPool1 = maxPooling2x2(ImgConv1, nH-2, nW-2); 
  // printf("Second MaxPool\n");

  
  // ImgMaxPool11 = maxPooling2x2(ImgConv11, 125, 125);
  // ImgMaxPool21 = maxPooling2x2(ImgConv21, 124, 124);

  vector<vector<double>> vect_all;
  vector<vector<double>> w1;
  vector<vector<double>> w2;

  //vector<double> vect1 = {1.0, 1.0, 1.0};
  vector<double> prediction = {1, 0};
  vector<double> nn1 = {1.0, 2.0};
  vector<double> nn2 = {0.0, 0.0, 0.8};

  vector<double> e_nn1 = {0.0, 0.0};
  vector<double> e_nn2 = {0.0, 0.0, 0.0};
  vector<double> e_nn3 = { 0.0, 0.0};
  vector<double> e_n = { 0.0, 0.0};



  vector<double> nn3 = {1.2, 0.16};
  vector<double> w21 = {0.3, -0.5, 1.5};
  vector<double> w22 = {0.5, -0.3, 0.2};

  // w1.push_back(w11);
  // w1.push_back(w12);
  // w1.push_back(w13);


  w2.push_back(w21);
  w2.push_back(w22);

    vector flatten_img = flatten(ImgIn, nTaille);



  // cout << "flatten Image = ";
  // for (auto& v : flatten_img){
  //    cout << v << " ";
  // }
  // cout << endl;

  for (int i = 0; i < 3; i++){
    vector<double> tmp;
    for(int j = 0; j < flatten_img.size(); j++ ){
      ImgFlat[j] = flatten_img[j];
      tmp.push_back(1.0/flatten_img.size());
    }
    w1.push_back(tmp);
  }

 for (const auto & entry : fs::directory_iterator(path)){
    strcpy(cNomImgLue, entry.path().c_str());
    sNomImgLue = std::string(cNomImgLue);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    flatten_img = flatten(ImgIn, nTaille);

  double n1 = 1;
  // vector<double> w11 = {0.5, -0.5};
  // vector<double> w12 = {-1.5, -2.5};
  // vector<double> w13 = {0.2, 0.3};



  vector<double> v_nn2 = gForLayer4(nn1, w1);
  vector<double> v_nn3 = gForLayer4(v_nn2, w2);
 
  // cout << "v_nn2 = ";
  // for (auto& v : v_nn2){
  //    cout << v << " ";
  // }
  // cout << endl;
  printf("etape = %i\n", etape);
  etape++;

  cout << "sortie = ";
  for (auto& v : v_nn3){
     cout << v << " ";
  }
  cout << endl;
  int i = 0;

  for (auto& v : v_nn3){
     e_nn3[i] = ei_sortie(v, prediction[i], v);
     i++;
  }
  cout << "erreur de sortie = ";

  for (auto& v : e_nn3){
     cout << v << " ";
  }
  cout << endl;

  i = 0;
  vector<double> tmp; 
  for (auto& v : v_nn2){
    tmp = {w21[i], w22[i]};
    e_nn2[i] = ei(v, tmp, e_nn3);
    i++;
  }

  // cout << "e_nn2 = ";
  // for (auto& v : e_nn2){
  //    cout << v << " ";
  // }
  // cout << endl;

  i = 0;
  vector<double> tmp2; 
  for (auto& v : nn1){
   
    tmp = {w1[0][i], w1[1][i], w1[2][i]};
    e_nn1[i] = ei(v, tmp2, e_nn2);
    i++;
  }

  // cout << "e_nn1 = ";
  // for (auto& v : e_nn1){
  //    cout << v << " ";
  // }
  // cout << endl;
  i = 0;
  for (auto& v : w2)
  {
    for (int j = 0; j < v.size(); j++)
    {
      /* code */
      v[j] = correction_poids(v[j], e_nn3[i], v_nn2[j] ,1);
    }
    
    i++;
  }
  
  // cout << "w2 = ";
  // for (auto& v : w2){
  //   for (auto& t : v)
  //     cout << t << " ";
  // cout << endl;
  // }
  // cout << endl;
  
  // correction w1
    i = 0;
  for (auto& v : w1)
  {
    for (int j = 0; j < v.size(); j++)
    {
      /* code */
      v[j] = correction_poids(v[j], e_nn2[i], nn1[j] ,0.1);
    }
    
    i++;
  }
 }

  // cout << "w1 = ";
  // for (auto& v : w1){
  //   for (auto& t : v)
  //     cout << t << " ";
  // cout << endl;
  // }
  // cout << endl;


  // sNomImgLueMessage = sNomImgLue+"_Fatten"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgFlat, 315, 315);

  // sNomImgLueMessage = sNomImgLue+"_MaxPool11"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgMaxPool11, 63, 63);


  // sNomImgLueMessage = sNomImgLue+"_MaxPool21"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgMaxPool21, 62, 62);

  // sNomImgLueMessage = sNomImgLue+"_Conv11"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgConv11, 125, 125);

  // sNomImgLueMessage = sNomImgLue+"_Conv21"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgConv21, 125, 125);

  // sNomImgLueMessage = sNomImgLue+"_MaxPool1"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgMaxPool1, 127, 127);

  // sNomImgLueMessage = sNomImgLue+"_Conv1"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgConv1, nH-2, nW-2);

  
   return 1;
}
