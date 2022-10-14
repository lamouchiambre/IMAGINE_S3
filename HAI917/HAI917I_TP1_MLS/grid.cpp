// -------------------------------------------
// gMini : a minimal OpenGL/GLUT application
// for 3D graphics.
// Copyright (C) 2006-2008 Tamy Boubekeur
// All rights reserved.
// -------------------------------------------

// -------------------------------------------
// Disclaimer: this code is dirty in the
// meaning that there is no attention paid to
// proper class attribute access, memory
// management or optimisation of any kind. It
// is designed for quick-and-dirty testing
// purpose.
// -------------------------------------------

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <GL/glut.h>
#include <float.h>
#include "src/Vec3.h"
#include "src/Camera.h"

enum DisplayMode
{
    WIRE = 0,
    SOLID = 1,
    LIGHTED_WIRE = 2,
    LIGHTED = 3
};

struct Triangle
{
    inline Triangle()
    {
        v[0] = v[1] = v[2] = 0;
    }
    inline Triangle(const Triangle &t)
    {
        v[0] = t.v[0];
        v[1] = t.v[1];
        v[2] = t.v[2];
    }
    inline Triangle(unsigned int v0, unsigned int v1, unsigned int v2)
    {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
    }
    unsigned int &operator[](unsigned int iv) { return v[iv]; }
    unsigned int operator[](unsigned int iv) const { return v[iv]; }
    inline virtual ~Triangle() {}
    inline Triangle &operator=(const Triangle &t)
    {
        v[0] = t.v[0];
        v[1] = t.v[1];
        v[2] = t.v[2];
        return (*this);
    }
    // membres indices des sommets du triangle:
    unsigned int v[3];
};

int weight_type;
int resolution = 128;
struct Mesh
{
    std::vector<Vec3> vertices;         //array of mesh vertices positions
    std::vector<Vec3> normals;          //array of vertices normals useful for the display
    std::vector<Triangle> triangles;    //array of mesh triangles
    std::vector<Vec3> triangle_normals; //triangle normals to display face normals

    //Compute face normals for the display
    void computeTrianglesNormals()
    {

        triangle_normals.clear();
        triangle_normals.resize(triangles.size(), Vec3(0., 0., 0.));
        //TODO: implémenter le calcul des normales par face
        //Attention commencer la fonction par triangle_normals.clear();
        //Iterer sur les triangles
        Vec3 e_10, e_20, cross, n;
        for (int i = 0; i < (int)triangles.size(); i++)
        {
            e_10 = vertices[triangles[i][1]] - vertices[triangles[i][0]];
            e_20 = vertices[triangles[i][2]] - vertices[triangles[i][0]];
            e_10.normalize();
            e_20.normalize();
            triangle_normals[i] = Vec3::cross(e_10, e_20);
        }
    }

    //Compute vertices normals as the average of its incident faces normals
    void computeVerticesNormals()
    {
        //Attention commencer la fonction par normals.clear();
        normals.clear();
        int normales_size = vertices.size(), triangles_size = triangles.size();
        normals.resize(normales_size, Vec3(0., 0., 0.));
        //Initializer le vecteur normals taille vertices.size() avec Vec3(0., 0., 0.)

        //Iterer sur les triangles
        //0 et 1
        if (weight_type != 2)
        {
            for (int triangle = 0; triangle < triangles_size; triangle++)
            {
                for (int sommet = 0; sommet < 3; sommet++)
                {
                    if (weight_type == 0)
                        normals[triangles[triangle][sommet]] += triangle_normals[triangle];
                    else if (weight_type == 1)
                        normals[triangles[triangle][sommet]] += (float)(triangle_normals[triangle].length() * 0.5) * triangle_normals[triangle];
                }
            }
        }
        else
        {
            Vec3 e_10, e_20;
            for (int triangle = 0; triangle < triangles_size; triangle++)
            {
                //que pour sommet 0?
                e_10 = vertices[triangles[triangle][1]] - vertices[triangles[triangle][0]];
                e_20 = vertices[triangles[triangle][2]] - vertices[triangles[triangle][0]];
                e_10.normalize();
                e_20.normalize();
                normals[triangles[triangle][0]] = (float)acos(Vec3::dot(e_10, e_20)) * triangle_normals[triangle];
            }
        }
        for (int sommet = 0; sommet < normales_size; sommet++)
        {
            //normals[sommet] /= (float)triangles_par_sommet[sommet];
            normals[sommet].normalize();
        }
    }

    void computeNormals()
    {
        computeTrianglesNormals();
        computeVerticesNormals();
    }

    std::pair<Vec3, Vec3> make_bounding_box()
    {
        //Cube C englobant le maillage défini avec deux points opposés
        //inferieur gauche avant à superieur droit arrière
        std::pair<Vec3, Vec3> C;
        float minX = vertices[0][0];
        float minY = vertices[0][1];
        float minZ = vertices[0][2];

        float maxX = vertices[1][0];
        float maxY = vertices[1][1];
        float maxZ = vertices[1][2];

        for (size_t i = 0; i < vertices.size(); i++)
        {
            if (vertices[i][0] < minX)
                minX = vertices[i][0];
            if (vertices[i][1] < minY)
                minY = vertices[i][1];
            if (vertices[i][2] < minZ)
                minZ = vertices[i][2];

            if (vertices[i][0] > maxX)
                maxX = vertices[i][0];
            if (vertices[i][1] > maxY)
                maxY = vertices[i][1];
            if (vertices[i][2] > maxZ)
                maxZ = vertices[i][2];
        }
        //facteur d'élargissement du cube afin d'éviter les erreur d'imprécision
        float enlarge_factor = 0.05;
        C.first = Vec3(minX - enlarge_factor * std::abs(minX), minY - enlarge_factor * std::abs(minY), minZ - enlarge_factor * std::abs(minZ));
        C.second = Vec3(maxX + enlarge_factor * std::abs(maxX), maxY + enlarge_factor * std::abs(maxY), maxZ + enlarge_factor * std::abs(maxZ));

        return C;
    }

    std::vector<std::pair<Vec3, Vec3>> make_grid(std::pair<Vec3, Vec3> bounding_box, unsigned int resolution)
    {
        std::vector<std::pair<Vec3, Vec3>> G(resolution * resolution * resolution);
        Vec3 bboxX = (Vec3(bounding_box.second[0] - bounding_box.first[0], 0., 0.)) / resolution;
        Vec3 bboxY = (Vec3(0., bounding_box.second[1] - bounding_box.first[1], 0.)) / resolution;
        Vec3 bboxZ = (Vec3(0., 0., bounding_box.second[2] - bounding_box.first[2])) / resolution;
        Vec3 bboxDiag = bboxX + bboxY + bboxZ;

        for (int dz = 0; dz < resolution; dz++)
        {
            for (int dy = 0; dy < resolution; dy++)
            {
                for (int dx = 0; dx < resolution; dx++)
                {
                    int i = resolution * resolution * dz + resolution * dy + dx;
                    // std::cout << i << std::endl;
                    G[i].first = bounding_box.first + dx * bboxX + dy * bboxY + dz * bboxZ;
                    G[i].second = G[i].first + bboxDiag;
                }
            }
        }

        return G;
    }

    bool isInsideCube(Vec3 point, std::pair<Vec3, Vec3> diag, int cote)
    {
        float x = point[0];
        float y = point[1];
        float z = point[2];

        Vec3 c1 = diag.first;
        Vec3 c2 = diag.second;
        // if (c1[1] > c2[1])
        // {
        //     std::cout << c1 << " -- " << c2 << std::endl;
        // }
        if ((x > c1[0] && x < c2[0]) && (y > c1[1] && y < c2[1]) && (z > c1[2] && z < c2[2]))
        {
            return true;
        }
        return false;
    }
    int findCellIndex(Vec3 point, std::vector<std::pair<Vec3, Vec3>> grid, int resolution)
    {
        for (int i = 0; i < grid.size(); i++)
        {
            if (isInsideCube(point, grid[i], resolution))
            {
                return i;
            }
        }

        return -1;
    }
    void simplify(unsigned int resolution)
    {
        std::pair<Vec3, Vec3> C = make_bounding_box();
        std::vector<std::pair<Vec3, Vec3>> G = make_grid(C, resolution);

        std::vector<Vec3> representants_pos;
        representants_pos.clear();
        representants_pos.resize(resolution * resolution * resolution);
        fill(representants_pos.begin(), representants_pos.end(), Vec3(0., 0., 0.));

        std::vector<Vec3> representants_normals;
        representants_normals.clear();
        representants_normals.resize(resolution * resolution * resolution);
        fill(representants_normals.begin(), representants_normals.end(), Vec3(0., 0., 0.));

        std::vector<int> nbe_sommets_par_cellules;
        nbe_sommets_par_cellules.clear();
        nbe_sommets_par_cellules.resize(resolution * resolution * resolution);
        fill(nbe_sommets_par_cellules.begin(), nbe_sommets_par_cellules.end(), 0);

        for (int i = 0; i < vertices.size(); i++)
        {
            // std::cout << "le sommet " << i << " appartient à la cellule " << findCellIndex(vertices[i], G) << " sur " << resolution * resolution * resolution - 1 << std::endl;
            int index = findCellIndex(vertices[i], G, resolution);
            representants_pos[index] += vertices[i];
            representants_normals[index] += normals[i];
            nbe_sommets_par_cellules[index]++;
        }
        for (int i = 0; i < triangles.size(); i++)
        {
            int index1 = findCellIndex(vertices[triangles[i][0]], G, resolution);
            int index2 = findCellIndex(vertices[triangles[i][1]], G, resolution);
            int index3 = findCellIndex(vertices[triangles[i][2]], G, resolution);
            if ((index1 != index2) && (index2 != index3) && (index3 != index1))
            {
                triangles[i][0] = index1;
                triangles[i][1] = index2;
                triangles[i][2] = index3;
            }
            else
            {
                triangles.erase(triangles.begin() + i);
                i--;
            }
        }
        int n = 0;
        for (int i = 0; i < representants_pos.size(); i++)
        {
            if (nbe_sommets_par_cellules[i] > 0)
            {
                representants_pos[i] /= nbe_sommets_par_cellules[i];
                // representants_normals[i] /= nbe_sommets_par_cellules[i];
                representants_normals[i].normalize();
                // n += nbe_sommets_par_cellules[i];
            }
        }
        // std::cout << "somme des nbe de sommets par cellules = " << n << std::endl;
        // std::cout << "nbe vertices = " << vertices.size() << std::endl;
        vertices.clear();
        vertices.resize(representants_pos.size());
        vertices = representants_pos;
        normals.clear();
        normals.resize(representants_normals.size());
        normals = representants_normals;

        // computeNormals();
    }
};
//Transformation made of a rotation and translation
struct Transformation
{
    Mat3 rotation;
    Vec3 translation;
};

//Basis ( origin, i, j ,k )
struct Basis
{
    inline Basis(Vec3 const &i_origin, Vec3 const &i_i, Vec3 const &i_j, Vec3 const &i_k)
    {
        origin = i_origin;
        i = i_i;
        j = i_j;
        k = i_k;
    }

    inline Basis()
    {
        origin = Vec3(0., 0., 0.);
        i = Vec3(1., 0., 0.);
        j = Vec3(0., 1., 0.);
        k = Vec3(0., 0., 1.);
    }
    Vec3 operator[](unsigned int ib)
    {
        if (ib == 0)
            return i;
        if (ib == 1)
            return j;
        return k;
    }

    Vec3 origin;
    Vec3 i;
    Vec3 j;
    Vec3 k;
};

//Fonction à completer
void collect_one_ring(std::vector<Vec3> const &i_vertices,
                      std::vector<Triangle> const &i_triangles,
                      std::vector<std::vector<unsigned int>> &o_one_ring)
{ //one-ring of each vertex, i.e. a list of vertices with which it shares an edge

    // std::cout << "collect_one_ring::begin" << std::endl;
    //Initialiser le vecetur de o_one_ring de la taille du vecteur vertices
    o_one_ring.clear();
    o_one_ring.resize(i_vertices.size());
    int triangles_size = i_triangles.size();
    // std::cout << triangles_size << std::endl;
    //Parcourir les triangles et ajouter les voisins dans le 1-voisinage
    for (int curr_tr = 0; curr_tr < triangles_size - 1; curr_tr++)
    {
        for (int curr_sommet = 0; curr_sommet < 3; curr_sommet++)
        {
            for (int voisin = 0; voisin < 3; voisin++)
            {
                //         //Attention verifier que l'indice n'est pas deja present
                bool deja_present = std::find(o_one_ring[i_triangles[curr_tr][curr_sommet]].begin(), o_one_ring[i_triangles[curr_tr][curr_sommet]].end(), i_triangles[curr_tr][voisin]) != o_one_ring[i_triangles[curr_tr][curr_sommet]].end();

                if (i_triangles[curr_tr][curr_sommet] != i_triangles[curr_tr][voisin] && !deja_present)
                {
                    o_one_ring[i_triangles[curr_tr][curr_sommet]].push_back(i_triangles[curr_tr][voisin]);
                }
            }
        }
    }
}

//Fonction à completer
void compute_vertex_valences(const std::vector<Vec3> &i_vertices,
                             const std::vector<Triangle> &i_triangles,
                             std::vector<unsigned int> &o_valences)
{
    //Utiliser la fonction collect_one_ring pour récuperer le 1-voisinage
    o_valences.clear();
    o_valences.resize(i_vertices.size());
    std::vector<std::vector<unsigned int>> o_one_ring;
    collect_one_ring(i_vertices, i_triangles, o_one_ring);
    int valences_size = (int)i_vertices.size();
    for (int curr_sommet = 0; curr_sommet < o_one_ring.size(); curr_sommet++)
    {
        o_valences[curr_sommet] = o_one_ring[curr_sommet].size();
        //std::cout << o_valences[curr_sommet] << std::endl;
    }
}

//Input mesh loaded at the launch of the application
Mesh mesh;
std::vector<float> mesh_valence_field; //normalized valence of each vertex

Basis basis;

bool display_normals;
bool display_smooth_normals;
bool display_mesh;
bool display_basis;
bool display_grid;
DisplayMode displayMode;

// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint window;
static unsigned int SCREENWIDTH = 1600;
static unsigned int SCREENHEIGHT = 900;
static Camera camera;
static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static int lastX = 0, lastY = 0, lastZoom = 0;
static bool fullScreen = false;

// ------------------------------------
// File I/O
// ------------------------------------
bool saveOFF(const std::string &filename,
             std::vector<Vec3> const &i_vertices,
             std::vector<Vec3> const &i_normals,
             std::vector<Triangle> const &i_triangles,
             std::vector<Vec3> const &i_triangle_normals,
             bool save_normals = true)
{
    std::ofstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open())
    {
        std::cout << filename << " cannot be opened" << std::endl;
        return false;
    }

    myfile << "OFF" << std::endl;

    unsigned int n_vertices = i_vertices.size(), n_triangles = i_triangles.size();
    myfile << n_vertices << " " << n_triangles << " 0" << std::endl;

    for (unsigned int v = 0; v < n_vertices; ++v)
    {
        myfile << i_vertices[v][0] << " " << i_vertices[v][1] << " " << i_vertices[v][2] << " ";
        if (save_normals)
            myfile << i_normals[v][0] << " " << i_normals[v][1] << " " << i_normals[v][2] << std::endl;
        else
            myfile << std::endl;
    }
    for (unsigned int f = 0; f < n_triangles; ++f)
    {
        myfile << 3 << " " << i_triangles[f][0] << " " << i_triangles[f][1] << " " << i_triangles[f][2] << " ";
        if (save_normals)
            myfile << i_triangle_normals[f][0] << " " << i_triangle_normals[f][1] << " " << i_triangle_normals[f][2];
        myfile << std::endl;
    }
    myfile.close();
    return true;
}

void openOFF(std::string const &filename,
             std::vector<Vec3> &o_vertices,
             std::vector<Vec3> &o_normals,
             std::vector<Triangle> &o_triangles,
             std::vector<Vec3> &o_triangle_normals,
             bool load_normals = true)
{
    std::ifstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open())
    {
        std::cout << filename << " cannot be opened" << std::endl;
        return;
    }

    std::string magic_s;

    myfile >> magic_s;

    if (magic_s != "OFF")
    {
        std::cout << magic_s << " != OFF :   We handle ONLY *.off files." << std::endl;
        myfile.close();
        exit(1);
    }

    int n_vertices, n_faces, dummy_int;
    myfile >> n_vertices >> n_faces >> dummy_int;

    o_vertices.clear();
    o_normals.clear();

    for (int v = 0; v < n_vertices; ++v)
    {
        float x, y, z;

        myfile >> x >> y >> z;
        o_vertices.push_back(Vec3(x, y, z));

        if (load_normals)
        {
            myfile >> x >> y >> z;
            o_normals.push_back(Vec3(x, y, z));
        }
    }

    o_triangles.clear();
    o_triangle_normals.clear();
    for (int f = 0; f < n_faces; ++f)
    {
        int n_vertices_on_face;
        myfile >> n_vertices_on_face;

        if (n_vertices_on_face == 3)
        {
            unsigned int _v1, _v2, _v3;
            myfile >> _v1 >> _v2 >> _v3;

            o_triangles.push_back(Triangle(_v1, _v2, _v3));

            if (load_normals)
            {
                float x, y, z;
                myfile >> x >> y >> z;
                o_triangle_normals.push_back(Vec3(x, y, z));
            }
        }
        else if (n_vertices_on_face == 4)
        {
            unsigned int _v1, _v2, _v3, _v4;
            myfile >> _v1 >> _v2 >> _v3 >> _v4;

            o_triangles.push_back(Triangle(_v1, _v2, _v3));
            o_triangles.push_back(Triangle(_v1, _v3, _v4));
            if (load_normals)
            {
                float x, y, z;
                myfile >> x >> y >> z;
                o_triangle_normals.push_back(Vec3(x, y, z));
            }
        }
        else
        {
            std::cout << "We handle ONLY *.off files with 3 or 4 vertices per face" << std::endl;
            myfile.close();
            exit(1);
        }
    }
}

// ------------------------------------
// Application initialization
// ------------------------------------
void initLight()
{
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f, -16.0f, -50.0f};
    GLfloat color1[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 0.5f};

    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, color1);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
}

void init()
{
    camera.resize(SCREENWIDTH, SCREENHEIGHT);
    initLight();
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_COLOR_MATERIAL);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    display_normals = false;
    display_mesh = true;
    display_smooth_normals = true;
    display_grid = false;
    displayMode = LIGHTED;
    display_basis = false;
}

// ------------------------------------
// Rendering.
// ------------------------------------

void drawVector(Vec3 const &i_from, Vec3 const &i_to)
{

    glBegin(GL_LINES);
    glVertex3f(i_from[0], i_from[1], i_from[2]);
    glVertex3f(i_to[0], i_to[1], i_to[2]);
    glEnd();
}

void drawCube(Vec3 const &i_from, Vec3 const &i_to)
{
    float dx = std::abs(i_to[0] - i_from[0]);
    float dy = std::abs(i_to[1] - i_from[1]);
    float dz = std::abs(i_to[2] - i_from[2]);
    drawVector(i_from, i_from + Vec3(dx, 0., 0.));
    drawVector(i_from + Vec3(dx, 0., 0.), i_from + Vec3(dx, 0., 0.) + Vec3(0., dy, 0.));
    drawVector(i_from + Vec3(dx, 0., 0.), i_from + Vec3(dx, 0., 0.) + Vec3(0., 0., dz));

    drawVector(i_from, i_from + Vec3(0., dy, 0.));
    drawVector(i_from + Vec3(0., dy, 0.), i_from + Vec3(0., dy, 0.) + Vec3(dx, 0., 0.));
    drawVector(i_from + Vec3(0., dy, 0.), i_from + Vec3(0., dy, 0.) + Vec3(0., 0., dz));

    drawVector(i_from, i_from + Vec3(0., 0., dz));
    drawVector(i_from + Vec3(0., 0., dz), i_from + Vec3(0., 0., dz) + Vec3(0., dy, 0.));
    drawVector(i_from + Vec3(0., 0., dz), i_from + Vec3(0., 0., dz) + Vec3(dx, 0., 0.));

    drawVector(i_to, i_to - Vec3(dx, 0., 0.));
    drawVector(i_to, i_to - Vec3(0., dy, 0.));
    drawVector(i_to, i_to - Vec3(0., 0., dz));

    glEnd();
}

void drawAxis(Vec3 const &i_origin, Vec3 const &i_direction)
{

    glLineWidth(4); // for example...
    drawVector(i_origin, i_origin + i_direction);
}

void drawReferenceFrame(Vec3 const &origin, Vec3 const &i, Vec3 const &j, Vec3 const &k)
{

    glDisable(GL_LIGHTING);
    glColor3f(0.8, 0.2, 0.2);
    drawAxis(origin, i);
    glColor3f(0.2, 0.8, 0.2);
    drawAxis(origin, j);
    glColor3f(0.2, 0.2, 0.8);
    drawAxis(origin, k);
    glEnable(GL_LIGHTING);
}

void drawReferenceFrame(Basis &i_basis)
{
    drawReferenceFrame(i_basis.origin, i_basis.i, i_basis.j, i_basis.k);
}

typedef struct
{
    float r; // ∈ [0, 1]
    float g; // ∈ [0, 1]
    float b; // ∈ [0, 1]
} RGB;

RGB scalarToRGB(float scalar_value) //Scalar_value ∈ [0, 1]
{
    RGB rgb;
    float H = scalar_value * 360., S = 1., V = 0.85,
          P, Q, T,
          fract;

    (H == 360.) ? (H = 0.) : (H /= 60.);
    fract = H - floor(H);

    P = V * (1. - S);
    Q = V * (1. - S * fract);
    T = V * (1. - S * (1. - fract));

    if (0. <= H && H < 1.)
        rgb = (RGB){.r = V, .g = T, .b = P};
    else if (1. <= H && H < 2.)
        rgb = (RGB){.r = Q, .g = V, .b = P};
    else if (2. <= H && H < 3.)
        rgb = (RGB){.r = P, .g = V, .b = T};
    else if (3. <= H && H < 4.)
        rgb = (RGB){.r = P, .g = Q, .b = V};
    else if (4. <= H && H < 5.)
        rgb = (RGB){.r = T, .g = P, .b = V};
    else if (5. <= H && H < 6.)
        rgb = (RGB){.r = V, .g = P, .b = Q};
    else
        rgb = (RGB){.r = 0., .g = 0., .b = 0.};

    return rgb;
}

void drawSmoothTriangleMesh(Mesh const &i_mesh, bool draw_field = false)
{
    glBegin(GL_TRIANGLES);
    for (unsigned int tIt = 0; tIt < i_mesh.triangles.size(); ++tIt)
    {

        for (unsigned int i = 0; i < 3; i++)
        {
            const Vec3 &p = i_mesh.vertices[i_mesh.triangles[tIt][i]]; //Vertex position
            const Vec3 &n = i_mesh.normals[i_mesh.triangles[tIt][i]];  //Vertex normal

            if (draw_field && mesh_valence_field.size() > 0)
            {
                RGB color = scalarToRGB(mesh_valence_field[i_mesh.triangles[tIt][i]]);
                glColor3f(color.r, color.g, color.b);
            }
            glNormal3f(n[0], n[1], n[2]);
            glVertex3f(p[0], p[1], p[2]);
        }
    }
    glEnd();
}

void drawTriangleMesh(Mesh const &i_mesh, bool draw_field = false)
{
    glBegin(GL_TRIANGLES);
    for (unsigned int tIt = 0; tIt < i_mesh.triangles.size(); ++tIt)
    {
        const Vec3 &n = i_mesh.triangle_normals[tIt]; //Triangle normal
        for (unsigned int i = 0; i < 3; i++)
        {
            const Vec3 &p = i_mesh.vertices[i_mesh.triangles[tIt][i]]; //Vertex position

            if (draw_field)
            {
                RGB color = scalarToRGB(mesh_valence_field[i_mesh.triangles[tIt][i]]);
                glColor3f(color.r, color.g, color.b);
            }
            glNormal3f(n[0], n[1], n[2]);
            glVertex3f(p[0], p[1], p[2]);
        }
    }
    glEnd();
}

void drawMesh(Mesh const &i_mesh, bool draw_field = false)
{
    if (display_smooth_normals)
        drawSmoothTriangleMesh(i_mesh, draw_field); //Smooth display with vertices normals
    else
        drawTriangleMesh(i_mesh, draw_field); //Display with face normals
}

void drawVectorField(std::vector<Vec3> const &i_positions, std::vector<Vec3> const &i_directions)
{
    glLineWidth(1.);
    for (unsigned int pIt = 0; pIt < i_directions.size(); ++pIt)
    {
        Vec3 to = i_positions[pIt] + 0.02 * i_directions[pIt];
        drawVector(i_positions[pIt], to);
    }
}

void drawNormals(Mesh const &i_mesh)
{

    if (display_smooth_normals)
    {
        drawVectorField(i_mesh.vertices, i_mesh.normals);
    }
    else
    {
        std::vector<Vec3> triangle_baricenters;
        for (const Triangle &triangle : i_mesh.triangles)
        {
            Vec3 triangle_baricenter(0., 0., 0.);
            for (unsigned int i = 0; i < 3; i++)
                triangle_baricenter += i_mesh.vertices[triangle[i]];
            triangle_baricenter /= 3.;
            triangle_baricenters.push_back(triangle_baricenter);
        }

        drawVectorField(triangle_baricenters, i_mesh.triangle_normals);
    }
}

//Draw fonction
void draw()
{

    if (displayMode == LIGHTED || displayMode == LIGHTED_WIRE)
    {

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_LIGHTING);
    }
    else if (displayMode == WIRE)
    {

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_LIGHTING);
    }
    else if (displayMode == SOLID)
    {
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glColor3f(0.8, 1, 0.8);
    drawMesh(mesh, true);

    if (displayMode == SOLID || displayMode == LIGHTED_WIRE)
    {
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1.0f);
        glPolygonOffset(-2.0, 1.0);

        glColor3f(0., 0., 0.);
        drawMesh(mesh, false);

        glDisable(GL_POLYGON_OFFSET_LINE);
        glEnable(GL_LIGHTING);
    }

    glDisable(GL_LIGHTING);
    if (display_normals)
    {
        glColor3f(1., 0., 0.);
        drawNormals(mesh);
    }
    if (display_grid)
    {
        glColor3f(1., 0, 0.);
        glLineWidth(2);
        std::pair<Vec3, Vec3> C = mesh.make_bounding_box();
        drawCube(C.first, C.second);
        glColor3f(0.8, 0, 0.);
        glLineWidth(0.25);

        std::vector<std::pair<Vec3, Vec3>> G = mesh.make_grid(C, resolution);
        for (size_t i = 0; i < G.size(); i++)
        {
            drawCube(G[i].first, G[i].second);
        }
    }
    if (display_basis)
    {
        drawReferenceFrame(basis);
    }
    glEnable(GL_LIGHTING);
}

void changeDisplayMode()
{
    if (displayMode == LIGHTED)
        displayMode = LIGHTED_WIRE;
    else if (displayMode == LIGHTED_WIRE)
        displayMode = SOLID;
    else if (displayMode == SOLID)
        displayMode = WIRE;
    else
        displayMode = LIGHTED;
}

void display()
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply();
    draw();
    glFlush();
    glutSwapBuffers();
}

void idle()
{
    glutPostRedisplay();
}

// ------------------------------------
// User inputs
// ------------------------------------
void update_valences()
{
    std::vector<unsigned int> valences;
    // TODO : Question 1 le calcul des valence pour chaques sommets (vertices) remplir le vector valences
    //          Le nombre de sommets voisin au sommet donné ( partage une arête )
    //          TODO : collect_one_ring() [ Permet de collecter le 1-voisinage ]
    compute_vertex_valences(mesh.vertices, mesh.triangles, valences);
    // A faire : normaliser les valences pour avoir une valeur flotante entre 0. et 1. dans mesh_valence_field
    int valences_size = (int)valences.size();
    mesh_valence_field.clear();
    mesh_valence_field.resize(valences_size);
    // max
    int max = 0;
    for (int i = 0; i < valences_size; i++)
    {
        if (valences[i] > max)
            max = valences[i];
    }
    // std::cout << "max: " << max << std::endl;
    //normalisation
    for (int valence; valence < valences_size; valence++)
    {
        mesh_valence_field[valence] = (((float)valences[valence]) / (float)max);
    }
}

//Keyboard event
void key(unsigned char keyPressed, int x, int y)
{
    switch (keyPressed)
    {
    case 'f':
        if (fullScreen == true)
        {
            glutReshapeWindow(SCREENWIDTH, SCREENHEIGHT);
            fullScreen = false;
        }
        else
        {
            glutFullScreen();
            fullScreen = true;
        }
        break;
    case 'g': //Toggle uniform grid
        display_grid = !display_grid;
        break;
    case 'w': //Change le mode d'affichage
        changeDisplayMode();
        break;

    case 'b': //Toggle basis display
        display_basis = !display_basis;
        break;

    case 'n': //Press n key to display normals
        display_normals = !display_normals;
        break;

    case '1': //Toggle loaded mesh display
        display_mesh = !display_mesh;
        break;
    case 's': //Simplify operation
    {
        resolution /= 2;
        if (resolution == 2)
        {
            resolution = 4;
        }
        std::cout << "résolution = " << resolution << std::endl;
        mesh.simplify(resolution);
        mesh.computeNormals();
        update_valences();

        break;
    }
    case 'a': //Adaptative Simplify operation
    {

        break;
    }
    case 'S': //Switches between face normals and vertices normals
        display_smooth_normals = !display_smooth_normals;
        break;

    case '+': //Changes resolution
        resolution++;
        std::cout << "résolution = " << resolution << std::endl;
        mesh.simplify(resolution);
        mesh.computeNormals();
        update_valences();
        break;
    case '-': //Changes resolution
        resolution--;
        std::cout << "résolution = " << resolution << std::endl;
        mesh.simplify(resolution);
        mesh.computeNormals();
        update_valences();
        break;

    default:
        break;
    }
    idle();
}

//Mouse events
void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_UP)
    {
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    }
    else
    {
        if (button == GLUT_LEFT_BUTTON)
        {
            camera.beginRotate(x, y);
            mouseMovePressed = false;
            mouseRotatePressed = true;
            mouseZoomPressed = false;
        }
        else if (button == GLUT_RIGHT_BUTTON)
        {
            lastX = x;
            lastY = y;
            mouseMovePressed = true;
            mouseRotatePressed = false;
            mouseZoomPressed = false;
        }
        else if (button == GLUT_MIDDLE_BUTTON)
        {
            if (mouseZoomPressed == false)
            {
                lastZoom = y;
                mouseMovePressed = false;
                mouseRotatePressed = false;
                mouseZoomPressed = true;
            }
        }
    }

    idle();
}

//Mouse motion, update camera
void motion(int x, int y)
{
    if (mouseRotatePressed == true)
    {
        camera.rotate(x, y);
    }
    else if (mouseMovePressed == true)
    {
        camera.move((x - lastX) / static_cast<float>(SCREENWIDTH), (lastY - y) / static_cast<float>(SCREENHEIGHT), 0.0);
        lastX = x;
        lastY = y;
    }
    else if (mouseZoomPressed == true)
    {
        camera.zoom(float(y - lastZoom) / SCREENHEIGHT);
        lastZoom = y;
    }
}

void reshape(int w, int h)
{
    camera.resize(w, h);
}

// ------------------------------------
// Start of graphical application
// ------------------------------------
int main(int argc, char **argv)
{
    if (argc > 2)
    {
        exit(EXIT_FAILURE);
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);
    window = glutCreateWindow("TP HAI702I");

    init();
    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutReshapeFunc(reshape);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    key('?', 0, 0);

    //Mesh loaded with precomputed normals
    openOFF("data/elephant_n.off", mesh.vertices, mesh.normals, mesh.triangles, mesh.triangle_normals);

    //Completer les fonction de calcul de normals
    mesh.computeNormals();

    basis = Basis();

    // A faire : completer la fonction compute_vertex_valences pour calculer les valences
    //***********************************************//
    std::vector<unsigned int> valences;
    // TODO : Question 1 le calcul des valence pour chaques sommets (vertices) remplir le vector valences
    //          Le nombre de sommets voisin au sommet donné ( partage une arête )
    //          TODO : collect_one_ring() [ Permet de collecter le 1-voisinage ]
    compute_vertex_valences(mesh.vertices, mesh.triangles, valences);
    // A faire : normaliser les valences pour avoir une valeur flotante entre 0. et 1. dans mesh_valence_field
    int valences_size = (int)valences.size();
    mesh_valence_field.clear();
    mesh_valence_field.resize(valences_size);
    // max

    int max = 0;
    for (int i = 0; i < valences_size; i++)
    {
        if (valences[i] > max)
            max = valences[i];
    }
    // std::cout << "max: " << max << std::endl;
    //normalisation
    for (int valence; valence < valences_size; valence++)
    {
        mesh_valence_field[valence] = (((float)valences[valence]) / (float)max);
    }
    //***********************************************//
    // Utile pour la question 2 permettant d'afficher une couleur dépendant de la valence des sommets
    //mesh_valence_field.clear();

    glutMainLoop();
    return EXIT_SUCCESS;
}
