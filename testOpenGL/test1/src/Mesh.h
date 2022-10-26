#ifndef MESH_H
#define MESH_H


#include <vector>
#include <string>
#include "Vec3.h"
#include "Skeleton.h"

#include <cmath>

#include <GL/glut.h>


// -------------------------------------------
// Basic Mesh class
// -------------------------------------------

struct MeshVertex {
    inline MeshVertex () {
        w.clear();
    }
    inline MeshVertex (const Vec3 & _p, const Vec3 & _n) : p (_p), n (_n) {
        w.clear();
    }
    inline MeshVertex (const MeshVertex & vertex) : p (vertex.p), n (vertex.n) , w(vertex.w) {
    }
    inline virtual ~MeshVertex () {}
    inline MeshVertex & operator = (const MeshVertex & vertex) {
        p = vertex.p;
        n = vertex.n;
        w = vertex.w;
        return (*this);
    }
    // membres :
    Vec3 p; // une position
    Vec3 n; // une normale
    std::vector< double > w; // skinning weights
};

struct MeshTriangle {
    inline MeshTriangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline MeshTriangle (const MeshTriangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
    }
    inline MeshTriangle (unsigned int v0, unsigned int v1, unsigned int v2) {
        v[0] = v0;   v[1] = v1;   v[2] = v2;
    }
    inline virtual ~MeshTriangle () {}
    inline MeshTriangle & operator = (const MeshTriangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
        return (*this);
    }
    // membres :
    unsigned int v[3];
};


    float projectionP(Vec3 C, Vec3 A, Vec3 u){
        float norme = ((C.x() - A.x())*u.x() + (C.y() - A.y())*u.y() + (C.z() - A.z())*u.z())/ sqrt(u.x()*u.x() + u.y()*u.y() + u.z()*u.z());
        return norme;
    }

class Mesh {
public:
    std::vector<MeshVertex> V;
    std::vector<MeshTriangle> T;

    void loadOFF (const std::string & filename);
    void recomputeNormals ();

void computeSkinningWeights( Skeleton const & skeleton ) {
        //---------------------------------------------------//
        //---------------------------------------------------//
        // code to change :

        // Indications:
        // you should compute weights for each vertex w.r.t. the skeleton bones
        // so each vertex will have B weights (B = number of bones)
        // these weights shoud be stored in vertex.w:
        int n = 25;
        for( unsigned int i = 0 ; i < V.size() ; i++ ) {
            MeshVertex & vertex = V[ i ];
            float s_w = 0.0;
            //printf("##########################################\n");
            for(int j = 0; j < skeleton.bones.size();j++){
                
                Vec3 P0 = skeleton.articulations[skeleton.bones[j].joints[0]].p;
                Vec3 P1 = skeleton.articulations[skeleton.bones[j].joints[1]].p;
                /*printf("P0 %f %f %f\n",P0.x(), P0.y(), P0.z());
                printf("P1 %f %f %f\n",P1.x(), P1.y(), P1.z());
                */
                Vec3 vecAB = P1 - P0;
                Vec3 vecAC = V[i].p - P0;
                
                Vec3 vecBA = P0 - P1;
                Vec3 vecBC = V[i].p - P1;

                //calcule de la projection
                Vec3 uAB = vecAB;
                Vec3 uBA = vecBA;
                uAB.normalize();
                uBA.normalize();
                float size_p0C = projectionP(V[i].p, P0, uAB);
                float size_p1C = projectionP(V[i].p, P1, uBA);
                /*printf("proj %f \n", size_p0C);*/

                Vec3 VH0 = P0 + Vec3(uAB[0]*size_p0C, uAB.y()*size_p0C, uAB.z()*size_p0C);
                
                Vec3 VH1 = P1 + Vec3(uBA.x()*size_p1C, uBA.y()*size_p1C, uBA.z()*size_p1C);
                float d = 0;

                if (size_p0C < size_p1C)
                {
                    if(uAB.dot(vecAB ,vecAC) > 0){
                        d = 1;
                    }else{
                        d = 1 + abs(size_p0C);
                    }
                }else
                {
                    if (uBA.dot(vecBA, vecBC) > 0)
                    {
                        d = 1;
                    }else
                    {
                        d = 1 + abs(size_p1C);
                    }
                }
                s_w += pow(1/d, n);
                V[i].w.push_back(pow(1/d, n));

                //printf("w%i %f\n", i, V[i].w[j]);

            }
            for (int k = 0; k < V[i].w.size(); k++)
            {
                V[i].w[k] /= s_w;
            }
            
        }

/*
    void computeSkinningWeights( Skeleton const & skeleton ) {
        //---------------------------------------------------//
        //---------------------------------------------------//
        // code to change :

        // Indications:
        // you should compute weights for each vertex w.r.t. the skeleton bones
        // so each vertex will have B weights (B = number of bones)
        // these weights shoud be stored in vertex.w:

        for( unsigned int i = 0 ; i < V.size() ; ++i ) {
            MeshVertex & vertex = V[ i ]; // ensemble des vertext
            int n = 20;
            std::vector< Bone > bones = skeleton.bones;
            float sum_w = 0.0;
            for(int j = 0; j < skeleton.bones.size() ; j++){
                Vec3 a1 = skeleton.articulations[skeleton.bones[j].joints[0]].p;
                Vec3 a2 = skeleton.articulations[skeleton.bones[j].joints[1]].p;
                float d = 0;
                
                if(Vec3::dot(a2 - a1, a2 - vertex.p)>=0){
                    if(Vec3::dot(a1 - a2, a1 - vertex.p)>=0){
                        Vec3 a1a2 = a2-a1/(a2-a1).length();
                        float dist_a2p = (Vec3::dot(a1a2, vertex.p - a1)*a1a2).length();
                        d = std::min(abs(dist_a2p), abs(a1a2.length() - dist_a2p));
                        //d = std::min((a2 - vertex.p).length(), (a1 - vertex.p).length());
                        //d=0.1;

                        
                    }else{
                        //prendre |a2p|
                        d = (a2 - vertex.p).length(); 
                        //d = (a1 - vertex.p).length(); 

                    }
                }else{
                    //prendre |a1p|
                    d = (a1 - vertex.p).length();
                    //d = (a2 - vertex.p).length();
                }
                sum_w += pow(1/d, 2); 
                vertex.w.push_back(pow(1/d, 2));
                
            }
            for(int j = 0; j < vertex.w.size(); j++){
                vertex.w[j] /= sum_w;
                //printf("%f\n", vertex.w[j]); 

            }

            
        }

        //---------------------------------------------------//
        //---------------------------------------------------//
        //---------------------------------------------------//
    }
*/
    void draw() const {
        glEnable(GL_LIGHTING);
        glBegin (GL_TRIANGLES);
        int W = 2;
        for (unsigned int i = 0; i < T.size (); i++)
            for (unsigned int j = 0; j < 3; j++) {
                const MeshVertex & v = V[T[i].v[j]];
                glNormal3f (v.n[0], v.n[1], v.n[2]);
                glVertex3f (v.p[0], v.p[1], v.p[2]);
                glColor3f(1*v.w[W],1*v.w[W],1-1*v.w[W]);
            }
        glEnd ();
    }

    void drawTransformedMesh( SkeletonTransformation & transfo ) const {
        std::vector< Vec3 > newPositions( V.size() );

        //---------------------------------------------------//
        //---------------------------------------------------//
        // code to change :
        for( unsigned int i = 0 ; i < V.size() ; ++i ) {
            Vec3 p = V[i].p;
            Vec3 new_p(0,0,0);

            // Indications:
            // you should use the skinning weights to blend the transformations of the vertex position by the bones.

            for(int j = 0; j < transfo.boneTransformations.size() ; j++){
                new_p += V[i].w[j]*(transfo.boneTransformations[j].worldSpaceRotation*p +transfo.boneTransformations[j].worldSpaceTranslation);
            }

            newPositions[ i ] = new_p;
        }
        //---------------------------------------------------//
        //---------------------------------------------------//
        //---------------------------------------------------//

        glEnable(GL_LIGHTING);
        glBegin (GL_TRIANGLES);
        for (unsigned int i = 0; i < T.size (); i++)
            for (unsigned int j = 0; j < 3; j++) {
                const MeshVertex & v = V[T[i].v[j]];
                Vec3 p = newPositions[ T[i].v[j] ];
                glNormal3f (v.n[0], v.n[1], v.n[2]);
                glVertex3f (p[0], p[1], p[2]);
            }
        glEnd ();
    }
};



#endif
