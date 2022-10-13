#include <igl/opengl/glfw/Viewer.h>
#include <math.h> 

Eigen::Vector3d barycentre(Eigen::MatrixXd V ){
  Eigen::Vector3d b;
  std::cout << V.size() << std::endl;
  int size = V.size()/3;

  double x = 0; 
  double y = 0;
  double z = 0;

  for (int i = 0; i < size; i++){
    //std::cout << V(i)<<" "<< V(size+i)<<" "<< V(2*size+i)<< " " << std::endl;
    x +=V(i)/size;
    y +=V(size + i)/size;
    z +=V(2 * size + i)/size;
  }

  b(0) = x;
  b(1) = y;
  b(2) = z;

  std::cout << b(0) << " " << b(1) << " " << b(2) << std::endl;
  return b;
}

Eigen::Vector3d coordSphere(Eigen::Vector3d p, Eigen::Vector3d g){
  Eigen::Vector3d o;

  o(0) = std::sqrt(std::pow(p(0) - g(0), 2) + std::pow(p(1) - g(1), 2) + std::pow(p(2) - g(2), 2));//pi
  o(1) = atan((p(1)-g(1))/(p(0)-g(0)));//theta
  o(2) = acos((p(2)-g(2))/(std::sqrt(std::pow(p(0) - g(0), 2) + std::pow(p(1) - g(1), 2) + std::pow(p(2) - g(2), 2))));//phi
  std::cout<<"coord shere " << o(0) << " " << o(1) << " "<<o(2) << " " <<std::endl;

  return o;
}

Eigen::Vector3d coordCart(Eigen::Vector3d o, Eigen::Vector3d g){
  Eigen::Vector3d p;

  p(0) = o(0)*cos(o(1))*sin(o(2))+g(0);
  p(1) = o(0)*sin(o(1))*sin(o(2))+g(1);
  p(2) = o(0)*cos(o(1))+g(2);
  std::cout<<"coord cart " << p(0) << " " << p(1) << " "<<p(2) << " " <<std::endl;
  return p;
}

/*
Eigen::Vector2d minMax(Eigen::MatrixXd V, Eigen::Vector3d g){
  Eigen::Vector3d tmp;
  Eigen::Vector3d tmpO;
  Eigen::Vector2d mM;
  double min, max;

  tmp = {V(0), V(size), V(2*size)};
  min = coordSphere(tmp, g); 
  max = 
  int size = V.size()/3;
  for (int i = 1; i < size; i++){
    tmp = {V(i), V(size+i), V(2*size+i)};
    tmpo = coordSphere(tmp, g); 

    min = std::min(min, tmpo(1));
    max = std::max(max, tmpo(1));
  }

  mM(0) = min;
  mM(1) = max;


  return mM;
}*/

int main(int argc, char *argv[])
{
  // Inline mesh of a cube
  const Eigen::MatrixXd V= (Eigen::MatrixXd(8,3)<<
    0.0,0.0,0.0,
    0.0,0.0,1.0,
    0.0,1.0,0.0,
    0.0,1.0,1.0,
    1.0,0.0,0.0,
    1.0,0.0,1.0,
    1.0,1.0,0.0,
    1.0,1.0,1.0).finished();
  const Eigen::MatrixXi F = (Eigen::MatrixXi(12,3)<<
    1,7,5,
    1,3,7,
    1,4,3,
    1,2,4,
    3,8,7,
    3,4,8,
    5,7,8,
    5,8,6,
    1,5,6,
    1,6,2,
    2,6,8,
    2,8,4).finished().array()-1;

  printf("Hello\n");
  Eigen::Vector3d b {0.0, 1.0, 0.0 };
  Eigen::Vector3d bary = barycentre(V);
  Eigen::Vector3d sh = coordSphere(b,bary);
  coordCart(sh,bary);
  //Eigen::Vector3d c = coordCart() coordSphere(b,barycentre(V));

  // Plot the mesh
  igl::opengl::glfw::Viewer viewer;
  viewer.data().set_mesh(V, F);
  viewer.data().set_face_based(true);
  viewer.launch();
}
