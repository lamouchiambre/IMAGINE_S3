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
  OCTET *ImgFlat, *ImgFlatMean;

  std::string sNomImgLue = std::string(cNomImgLue);
  std::string ext = ".pgm";
  std::string sNomImgOutSub = sNomImgLue+"Sub"+ext;
  std::string sNomImgLueMessage = sNomImgLue+"Message"+ext;
  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  double Prewitt_H[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};//contours
  double Prewitt_V[9] = {1, 1, 1, 0, 0, 0, -1, -1, -1};//contours
  double Laplacian[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};//
  double Identitee[9] = {0, 0, 0, 0, 1, 0, 0, 0, 0};
  double Nettete[9] = {0, -1, 0, -1, 5, -1, 0,-1, 0};
  // double Flou[9] = {0.111, 0.111, 0.111, 0.111, 0.111, 0.111, 0.111, 0.111, 0.111};//passe bas
  double Flou[9] = {1.0/16.0, 2.0/16.0, 1.0/16.0, 2.0/16.0, 4.0/16.0, 2.0/16.0, 1.0/16.0, 2.0/16.0, 1.0/16.0};//passe bas

   
  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
 
  allocation_tableau(ImgFlat, OCTET, (315)*(315));
  allocation_tableau(ImgFlatMean, OCTET, (315)*(315));

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
  allocation_tableau(ImgConv31, OCTET, (125)*(125));
  allocation_tableau(ImgConv41, OCTET, (125)*(125));
  allocation_tableau(ImgConv51, OCTET, (125)*(125));

  allocation_tableau(ImgConv12, OCTET, (125)*(125));
  allocation_tableau(ImgConv22, OCTET, (125)*(125));
  allocation_tableau(ImgConv32, OCTET, (125)*(125));
  allocation_tableau(ImgConv42, OCTET, (125)*(125));
  allocation_tableau(ImgConv52, OCTET, (125)*(125));

  allocation_tableau(ImgConv13, OCTET, (125)*(125));
  allocation_tableau(ImgConv23, OCTET, (125)*(125));
  allocation_tableau(ImgConv33, OCTET, (125)*(125));
  allocation_tableau(ImgConv43, OCTET, (125)*(125));
  allocation_tableau(ImgConv53, OCTET, (125)*(125));

  allocation_tableau(ImgConv14, OCTET, (125)*(125));
  allocation_tableau(ImgConv24, OCTET, (125)*(125));
  allocation_tableau(ImgConv34, OCTET, (125)*(125));
  allocation_tableau(ImgConv44, OCTET, (125)*(125));
  allocation_tableau(ImgConv54, OCTET, (125)*(125));

  allocation_tableau(ImgConv15, OCTET, (125)*(125));
  allocation_tableau(ImgConv25, OCTET, (125)*(125));
  allocation_tableau(ImgConv35, OCTET, (125)*(125));
  allocation_tableau(ImgConv45, OCTET, (125)*(125));
  allocation_tableau(ImgConv55, OCTET, (125)*(125));

  //maxpooling 2
  allocation_tableau(ImgMaxPool11, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool21, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool31, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool41, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool51, OCTET, (63)*(63));

  allocation_tableau(ImgMaxPool12, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool22, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool32, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool42, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool52, OCTET, (63)*(63));

  allocation_tableau(ImgMaxPool12, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool22, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool32, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool42, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool52, OCTET, (63)*(63));

  allocation_tableau(ImgMaxPool13, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool23, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool33, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool43, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool53, OCTET, (63)*(63));

  allocation_tableau(ImgMaxPool14, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool24, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool34, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool44, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool54, OCTET, (63)*(63));

  allocation_tableau(ImgMaxPool15, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool25, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool35, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool45, OCTET, (63)*(63));
  allocation_tableau(ImgMaxPool55, OCTET, (63)*(63));

// std::string path = "../../../archive/Shoe_vs_Sandal_vs_Boot_Dataset/Sandal";
std::string path = "img_in2/";

vector<OCTET*> list_img_flatten;
vector<vector<OCTET>> list_img_flattenV;


for (const auto & entry : fs::directory_iterator(path)){
    // std::cout << entry.path() << std::endl;
    strcpy(cNomImgLue, entry.path().c_str());
    sNomImgLue = std::string(cNomImgLue);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  
  //First conv
  // printf("First conv\n");
  ImgConv1 = conv3x3(ImgIn, nH, nW, Prewitt_H);
  ImgConv2 = conv3x3(ImgIn, nH, nW, Prewitt_V);
  ImgConv3 = conv3x3(ImgIn, nH, nW, Flou);
  ImgConv4 = conv3x3(ImgIn, nH, nW, Nettete);
  ImgConv5 = conv3x3(ImgIn, nH, nW, Laplacian);
 
  //First MaxPool
  // printf("First MaxPool\n");

  ImgMaxPool1 = maxPooling2x2(ImgConv1, nH-2, nW-2);
  ImgMaxPool2 = maxPooling2x2(ImgConv2, nH-2, nW-2);
  ImgMaxPool3 = maxPooling2x2(ImgConv3, nH-2, nW-2);
  ImgMaxPool4 = maxPooling2x2(ImgConv4, nH-2, nW-2);
  ImgMaxPool5 = maxPooling2x2(ImgConv5, nH-2, nW-2);


  //Second conv
  // printf("Second conv\n");

  ImgConv11 = conv3x3(ImgMaxPool1, 127, 127, Prewitt_H);
  ImgConv21 = conv3x3(ImgMaxPool2, 127, 127, Prewitt_H);
  ImgConv31 = conv3x3(ImgMaxPool3, 127, 127, Prewitt_H);
  ImgConv41 = conv3x3(ImgMaxPool4, 127, 127, Prewitt_H);
  ImgConv51 = conv3x3(ImgMaxPool5, 127, 127, Prewitt_H);

  ImgConv12 = conv3x3(ImgMaxPool1, 127, 127, Prewitt_V);
  ImgConv22 = conv3x3(ImgMaxPool2, 127, 127, Prewitt_V);
  ImgConv32 = conv3x3(ImgMaxPool3, 127, 127, Prewitt_V);
  ImgConv42 = conv3x3(ImgMaxPool4, 127, 127, Prewitt_V);
  ImgConv52 = conv3x3(ImgMaxPool5, 127, 127, Prewitt_V);

  ImgConv13 = conv3x3(ImgMaxPool1, 127, 127, Flou);
  ImgConv23 = conv3x3(ImgMaxPool2, 127, 127, Flou);
  ImgConv33 = conv3x3(ImgMaxPool3, 127, 127, Flou);
  ImgConv43 = conv3x3(ImgMaxPool4, 127, 127, Flou);
  ImgConv53 = conv3x3(ImgMaxPool5, 127, 127, Flou);

  ImgConv14 = conv3x3(ImgMaxPool1, 127, 127, Laplacian);
  ImgConv24 = conv3x3(ImgMaxPool2, 127, 127, Laplacian);
  ImgConv34 = conv3x3(ImgMaxPool3, 127, 127, Laplacian);
  ImgConv44 = conv3x3(ImgMaxPool4, 127, 127, Laplacian);
  ImgConv54 = conv3x3(ImgMaxPool5, 127, 127, Laplacian);

  ImgConv15 = conv3x3(ImgMaxPool1, 127, 127, Nettete);
  ImgConv25 = conv3x3(ImgMaxPool2, 127, 127, Nettete);
  ImgConv35 = conv3x3(ImgMaxPool3, 127, 127, Nettete);
  ImgConv45 = conv3x3(ImgMaxPool4, 127, 127, Nettete);
  ImgConv55 = conv3x3(ImgMaxPool5, 127, 127, Nettete);

  //2 second maxpooling
  //ImgMaxPool1 = maxPooling2x2(ImgConv1, nH-2, nW-2); 
  // printf("Second MaxPool\n");

  ImgMaxPool11 = maxPooling2x2(ImgConv11, 125, 125);
  ImgMaxPool21 = maxPooling2x2(ImgConv21, 125, 125);
  ImgMaxPool31 = maxPooling2x2(ImgConv31, 125, 125);
  ImgMaxPool41 = maxPooling2x2(ImgConv41, 125, 125);
  ImgMaxPool51 = maxPooling2x2(ImgConv51, 125, 125);

  ImgMaxPool12 = maxPooling2x2(ImgConv12, 125, 125);
  ImgMaxPool22 = maxPooling2x2(ImgConv22, 125, 125);
  ImgMaxPool32 = maxPooling2x2(ImgConv32, 125, 125);
  ImgMaxPool42 = maxPooling2x2(ImgConv42, 125, 125);
  ImgMaxPool52 = maxPooling2x2(ImgConv52, 125, 125);

  ImgMaxPool13 = maxPooling2x2(ImgConv13, 125, 125);
  ImgMaxPool23 = maxPooling2x2(ImgConv23, 125, 125);
  ImgMaxPool33 = maxPooling2x2(ImgConv33, 125, 125);
  ImgMaxPool43 = maxPooling2x2(ImgConv43, 125, 125);
  ImgMaxPool53 = maxPooling2x2(ImgConv53, 125, 125);

  ImgMaxPool14 = maxPooling2x2(ImgConv14, 125, 125);
  ImgMaxPool24 = maxPooling2x2(ImgConv24, 125, 125);
  ImgMaxPool34 = maxPooling2x2(ImgConv34, 125, 125);
  ImgMaxPool44 = maxPooling2x2(ImgConv44, 125, 125);
  ImgMaxPool54 = maxPooling2x2(ImgConv54, 125, 125);

  ImgMaxPool15 = maxPooling2x2(ImgConv15, 125, 125);
  ImgMaxPool25 = maxPooling2x2(ImgConv25, 125, 125);
  ImgMaxPool35 = maxPooling2x2(ImgConv35, 125, 125);
  ImgMaxPool45 = maxPooling2x2(ImgConv45, 125, 125);
  ImgMaxPool55 = maxPooling2x2(ImgConv55, 125, 125);

  // vector<double> flat_img(tab_to_vect(ImgMaxPool11, 63*63));
  vector<OCTET> flat_img(tab_to_vectO(ImgMaxPool11, 63*63));

  // vector<double> flat_img0(tab_to_vect(ImgMaxPool11, 63*63));

// flat_img.end();
  vector<double> tmp(tab_to_vect(ImgMaxPool21, 63*63));
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool31, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool41, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool51, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());


  tmp  = tab_to_vect(ImgMaxPool12, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool22, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool32, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool42, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool52, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());
 

  tmp  = tab_to_vect(ImgMaxPool13, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool23, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool33, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool43, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool53, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());


  tmp  = tab_to_vect(ImgMaxPool14, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool24, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool34, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool44, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool54, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());


  tmp  = tab_to_vect(ImgMaxPool15, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool25, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool35, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool45, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  tmp  = tab_to_vect(ImgMaxPool55, 63*63);
  flat_img.insert( flat_img.end(), tmp.begin(), tmp.end());

  // cout << flat_img.size() << endl;
  vector flatten_img(flat_img);

  vector<vector<double>> vect_all;
  vector<vector<double>> w1; // poids de la couche flatten_img à la premiere couche de 5 neurones ligne = 5, colone = N
  vector<vector<double>> w2; // couche de 5 neurones 
  vector<vector<double>> w_out; // poid de la couche 2 de 5 neurone a la sortie 2 neurones ligne 2 colone 5

  for (int i = 0; i < 5; i++){
    vector<double> tmp;
    for(int j = 0; j < flatten_img.size(); j++ ){
      ImgFlat[j] = flatten_img[j];
      tmp.push_back(1.0/flat_img.size());
    }
    w1.push_back(tmp);
  }

  for (int i = 0; i < 2; i++){
    vector<double> tmp;
    for(int j = 0; j < 5; j++ ){
      tmp.push_back(1.0/5);
    }
    w_out.push_back(tmp);
  } 


  // cout << "size v_nn2 " << flatten_img.size() << "size w1 " <<  w1.size() << endl;
  // vector<double> v_nn2 = gForLayer4(flatten_img, w1);
  // cout << "size v_nn2 " << v_nn2.size() << " size w_out " <<  w_out.size() << endl;
  // vector<double> v_nn3 = gForLayer4(v_nn2, w_out);


  sNomImgLueMessage = sNomImgLue+"_Fatten"+ext;
  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgFlat, 315, 315);

  OCTET * tab_tmp;
  allocation_tableau(tab_tmp, OCTET, (315)*(315));

  tab_tmp = ImgFlat;

  // copy(vect1.begin(), vect1.end(), back_inserter(vect2)); 

  list_img_flattenV.push_back(create_copy(flatten_img));
  

  sNomImgLueMessage = sNomImgLue+"_MaxPool1"+ext;
  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgMaxPool1, 127, 127);



  sNomImgLueMessage = sNomImgLue+"_MaxPool3"+ext;
  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgMaxPool3, 127, 127);

  sNomImgLueMessage = sNomImgLue+"_MaxPool31"+ext;
  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgMaxPool31, 63, 63);

  sNomImgLueMessage = sNomImgLue+"_MaxPool32"+ext;
  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgMaxPool32, 63, 63);

  sNomImgLueMessage = sNomImgLue+"_MaxPool21"+ext;
  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgMaxPool21, 63, 63);

  sNomImgLueMessage = sNomImgLue+"_Conv11"+ext;
  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgConv11, 125, 125);

  sNomImgLueMessage = sNomImgLue+"_Conv21"+ext;
  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgConv21, 125, 125);


  sNomImgLueMessage = sNomImgLue+"_Conv1"+ext;
  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgConv1, nH-2, nW-2);

  sNomImgLueMessage = sNomImgLue+"_Conv2"+ext;
  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgConv2, nH-2, nW-2);

  sNomImgLueMessage = sNomImgLue+"_Conv3"+ext;
  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgConv3, nH-2, nW-2);

  sNomImgLueMessage = sNomImgLue+"_Conv4"+ext;
  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgConv4, nH-2, nW-2);

  sNomImgLueMessage = sNomImgLue+"_Conv5"+ext;
  strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  ecrire_image_pgm(cNomImgEcrite, ImgConv5, nH-2, nW-2);

}
  
  // cout << "flatten Image = "; 
  // int indice = 0;
  // for (auto& v : list_img_flattenV){
  //    cout << v << " ";
  //     sNomImgLueMessage = "Boot_Fat"+to_string(indice)+ext;
  //     strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  //     ImgFlatMean = mean_imgV(list_img_flattenV, 315, 315);
  //     ecrire_image_pgm(cNomImgEcrite, v, 315, 315);
  //     indice ++;

  // }
  // cout << endl;

 
  
  // sNomImgLueMessage = "Sandal_FattenAll"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ImgFlatMean = mean_imgV(list_img_flattenV, 315, 315);
  // ecrire_image_pgm(cNomImgEcrite, ImgFlatMean, 315, 315);


// printf("size %li \n",list_img_flatten.size());



  // cout << "flatten Image = ";
  // for (auto& v : flatten_img){
  //    cout << v << " ";
  // }
  // cout << endl;

  // vector flatten_img = flatten(ImgIn, nTaille);
  // vector<vector<double>> vect_all;
  // vector<vector<double>> w1;
  // vector<vector<double>> w2;

  // //vector<double> vect1 = {1.0, 1.0, 1.0};
  // vector<double> prediction = {1, 0};
  // vector<double> nn1 = {1.0, 2.0};
  // vector<double> nn2 = {0.0, 0.0, 0.8};
  // vector<double> nn3 = {1.2, 0.16};


  // double n1 = 1;
  // vector<double> w11 = {0.5, -0.5};
  // vector<double> w12 = {-1.5, -2.5};
  // vector<double> w13 = {0.2, 0.3};

  // vector<double> w21 = {0.3, -0.5, 1.5};
  // vector<double> w22 = {0.5, -0.3, 0.2};

  // w1.push_back(w11);
  // w1.push_back(w12);
  // w1.push_back(w13);


  // w2.push_back(w21);
  // w2.push_back(w22);

  // vector<double> v_nn2 = gForLayer4(nn1, w1);
  // vector<double> v_nn3 = gForLayer4(v_nn2, w2);
 
  // cout << "v_nn2 = ";
  // for (auto& v : v_nn2){
  //    cout << v << " ";
  // }
  // cout << endl;

  // cout << "v_nn3 = ";
  // for (auto& v : v_nn3){
  //    cout << v << " ";
  // }
  // cout << endl;


 
  // sNomImgLueMessage = sNomImgLue+"_Fatten"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgFlat, 315, 315);

  // sNomImgLueMessage = sNomImgLue+"_MaxPool1"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgMaxPool1, 127, 127);



  // sNomImgLueMessage = sNomImgLue+"_MaxPool3"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgMaxPool3, 127, 127);

  // sNomImgLueMessage = sNomImgLue+"_MaxPool31"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgMaxPool31, 63, 63);

  // sNomImgLueMessage = sNomImgLue+"_MaxPool32"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgMaxPool32, 63, 63);

  // sNomImgLueMessage = sNomImgLue+"_MaxPool21"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgMaxPool21, 63, 63);

  // sNomImgLueMessage = sNomImgLue+"_Conv11"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgConv11, 125, 125);

  // sNomImgLueMessage = sNomImgLue+"_Conv21"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgConv21, 125, 125);

  // sNomImgLueMessage = sNomImgLue+"_Conv1"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgConv1, nH-2, nW-2);

  // sNomImgLueMessage = sNomImgLue+"_Conv2"+ext;
  // strcpy(cNomImgEcrite, sNomImgLueMessage.c_str());
  // ecrire_image_pgm(cNomImgEcrite, ImgConv2, nH-2, nW-2);
  
   return 1;
}
