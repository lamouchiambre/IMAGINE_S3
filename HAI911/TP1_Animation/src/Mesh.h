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

        for( unsigned int i = 0 ; i < V.size() ; ++i ) {
            MeshVertex & vertex = V[ i ]; // ensemble des vertext
            int n = 2;
            std::vector< Bone > bones = skeleton.bones;
            for(int j = 0; j < skeleton.bones.size() ; j++){
                Vec3 a1 = skeleton.articulations[skeleton.bones[j].joints[0]].p;
                Vec3 a2 = skeleton.articulations[skeleton.bones[j].joints[1]].p;
                float d = 0;
                
                if(Vec3::dot(a2 - a1, a2 - vertex.p)>=0){
                    if(Vec3::dot(a1 - a2, a1 - vertex.p)>=0){
                        Vec3 a1a2 = a2-a1/(a2-a1).length();
                        float dist_a2p = (Vec3::dot(a1a2, vertex.p - a1)*a1a2).length();
                        d = std::min(abs(dist_a2p), abs(a1a2.length() - dist_a2p));
                        
                    }else{
                        //prendre |a2p|
                        d = (a2 - vertex.p).length(); 
                    }
                }else{
                    //prendre |a1p|
                    d = (a1 - vertex.p).length();
                }
                vertex.w.push_back(pow(1/d, 2));
                //faire la normalisation des poids
                //printf("%i %i", skeleton.bones[i].joints[0], skeleton.bones[i].joints[0]);
                
            }

            
        }

        //---------------------------------------------------//
        //---------------------------------------------------//
        //---------------------------------------------------//
    }

    void draw() const {
        glEnable(GL_LIGHTING);
        glBegin (GL_TRIANGLES);
        for (unsigned int i = 0; i < T.size (); i++)
            for (unsigned int j = 0; j < 3; j++) {
                const MeshVertex & v = V[T[i].v[j]];
                glNormal3f (v.n[0], v.n[1], v.n[2]);
                glVertex3f (v.p[0], v.p[1], v.p[2]);
            }
        glEnd ();
    }

    void drawTransformedMesh( SkeletonTransformation const & transfo ) const {
        std::vector< Vec3 > newPositions( V.size() );

        //---------------------------------------------------//
        //---------------------------------------------------//
        // code to change :
        for( unsigned int i = 0 ; i < V.size() ; ++i ) {
            Vec3 p = V[i].p;

            // Indications:
            // you should use the skinning weights to blend the transformations of the vertex position by the bones.

            newPositions[ i ] = p;
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
                glVertex3f (v.p[0], v.p[1], v.p[2]);
            }
        glEnd ();
    }
};



#endif
