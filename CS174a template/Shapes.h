#pragma once

#ifndef EMSCRIPTEN
#include <Windows.h>
#define GLEW_STATIC
#include "..\GL\glew.h"
#endif
#define GL_GLEXT_PROTOTYPES
#include "..\GL\freeglut.h"


#ifdef __APPLE__
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#endif

struct ShapeData
{
    GLuint vao;
    int numVertices;
};

void generateCube(GLuint program, ShapeData* cubeData);
void generateSphere(GLuint program, ShapeData* sphereData);
void generateCone(GLuint program, ShapeData* coneData);
void generateCylinder(GLuint program, ShapeData* cylData);
void generateTop(GLuint program, ShapeData* topData);

#include "mat.h"
#include "vec.h"
using namespace Angel;
#include <cmath>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;
typedef Angel::vec3 point3;
typedef Angel::vec2 point2;

void setVertexAttrib(GLuint program, 
                     GLfloat* points,    GLsizeiptr psize, 
                     GLfloat* normals,   GLsizeiptr nsize,
                     GLfloat* texcoords, GLsizeiptr tsize)
{
    GLuint buffer[2];
    glGenBuffers( 2, buffer );

    glBindBuffer( GL_ARRAY_BUFFER, buffer[0] );
    glBufferData( GL_ARRAY_BUFFER, psize, points, GL_STATIC_DRAW );
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    glBindBuffer( GL_ARRAY_BUFFER, buffer[1] );
    glBufferData( GL_ARRAY_BUFFER, nsize, normals, GL_STATIC_DRAW );
    GLuint vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    if (texcoords)
    {
        GLuint tbuffer;
        glGenBuffers( 1, &tbuffer );
        glBindBuffer( GL_ARRAY_BUFFER, tbuffer );
        glBufferData( GL_ARRAY_BUFFER, tsize, texcoords, GL_STATIC_DRAW );
        GLuint vTexCoords = glGetAttribLocation( program, "TexCoordIn" );
        glEnableVertexAttribArray( vTexCoords );
        glVertexAttribPointer( vTexCoords, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    }
    
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray(0);
}

//----------------------------------------------------------------------------
// DOGE TOP

const int numTopVertices = 72; //(12 faces)(2 triangles/face)(3 vertices/triangle)

point4 topPoints[numTopVertices];
point3 topNormals[numTopVertices];
point2 topUV[numTopVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 tvertices[10] = {
	point4(-0.5,  0.25, -0.5 , 1.0),
	point4(-0.5,  0.25,  0.5 , 1.0),
	point4( 0.5,  0.25, -0.5 , 1.0),
	point4( 0.5,  0.25,  0.5 , 1.0),
	point4(-0.5, -0.25, -0.5,  1.0),
	point4(-0.5, -0.25,  0.5 , 1.0),
	point4( 0.5, -0.25, -0.5,  1.0),
	point4( 0.5, -0.25,  0.5,  1.0),
	point4( 0.0,  0.75,   0.0,  1.0),
	point4( 0.0, -0.75,   0.0,  1.0)
	
};

// quad generates two triangles for each face and assigns normals and texture coordinates
//    to the vertices
int tIndex = 0;
void quadSides(int a, int b, int c, int d, const point3& normal)
{
	topPoints[tIndex] = tvertices[a]; topNormals[tIndex] = cross(tvertices[a]-tvertices[b],tvertices[a]-tvertices[c]);
	topUV[tIndex] = point2(0.0f, 1.0f); tIndex++;
	topPoints[tIndex] = tvertices[b]; topNormals[tIndex] = cross(tvertices[a] - tvertices[b], tvertices[a] - tvertices[c]);
	topUV[tIndex] = point2(0.0f, 0.0f); tIndex++;
	topPoints[tIndex] = tvertices[c]; topNormals[tIndex] = cross(tvertices[a] - tvertices[b], tvertices[a] - tvertices[c]);
	topUV[tIndex] = point2(1.0f, 0.0f); tIndex++;
	topPoints[tIndex] = tvertices[a]; topNormals[tIndex] = cross(tvertices[a] - tvertices[c], tvertices[a] - tvertices[d]);
	topUV[tIndex] = point2(0.0f, 1.0f); tIndex++;
	topPoints[tIndex] = tvertices[c]; topNormals[tIndex] = cross(tvertices[a] - tvertices[c], tvertices[a] - tvertices[d]);
	topUV[tIndex] = point2(1.0f, 0.0f); tIndex++;
	topPoints[tIndex] = tvertices[d]; topNormals[tIndex] = cross(tvertices[a] - tvertices[c], tvertices[a] - tvertices[d]);
	topUV[tIndex] = point2(1.0f, 1.0f); tIndex++;
}


// generate 12 triangles: 36 vertices, 36 normals, 36 texture coordinates
void colorTopSides()
{
	quadSides(5, 1, 0, 4, point3(-1.0f, 0.0f, 0.0f));
	quadSides(4, 0, 2, 6, point3(0.0f, 0.0f, -1.0f));	
	quadSides(6, 2, 3, 7, point3(1.0f, 0.0f, 0.0f));
	quadSides(7, 3, 1, 5, point3(0.0f, 0.0f, 1.0f));
}

void colorTopEnds()
{
	topPoints[tIndex] = tvertices[8]; topNormals[tIndex] = -cross(tvertices[8] - tvertices[1], tvertices[8] - tvertices[0]);
	topUV[tIndex] = point2(0.0f, 1.0f); tIndex++;
	topPoints[tIndex] = tvertices[0]; topNormals[tIndex] = -cross(tvertices[8] - tvertices[1], tvertices[8] - tvertices[0]);
	topUV[tIndex] = point2(0.0f, 0.0f); tIndex++;
	topPoints[tIndex] = tvertices[1]; topNormals[tIndex] = -cross(tvertices[8] - tvertices[1], tvertices[8] - tvertices[0]);
	topUV[tIndex] = point2(1.0f, 0.0f); tIndex++;

	topPoints[tIndex] = tvertices[8]; topNormals[tIndex] = -cross(tvertices[0] - tvertices[8], tvertices[2] - tvertices[8]);
	topUV[tIndex] = point2(0.0f, 1.0f); tIndex++;
	topPoints[tIndex] = tvertices[0]; topNormals[tIndex] = -cross(tvertices[0] - tvertices[8], tvertices[2] - tvertices[8]);
	topUV[tIndex] = point2(0.0f, 0.0f); tIndex++;
	topPoints[tIndex] = tvertices[2]; topNormals[tIndex] = -cross(tvertices[0] - tvertices[8], tvertices[2] - tvertices[8]);
	topUV[tIndex] = point2(1.0f, 0.0f); tIndex++;

	topPoints[tIndex] = tvertices[8]; topNormals[tIndex] = -cross(tvertices[2] - tvertices[8], tvertices[3] - tvertices[8]);
	topUV[tIndex] = point2(0.0f, 1.0f); tIndex++;
	topPoints[tIndex] = tvertices[2]; topNormals[tIndex] = -cross(tvertices[2] - tvertices[8], tvertices[3] - tvertices[8]);
	topUV[tIndex] = point2(0.0f, 0.0f); tIndex++;
	topPoints[tIndex] = tvertices[3]; topNormals[tIndex] = -cross(tvertices[2] - tvertices[8], tvertices[3] - tvertices[8]);
	topUV[tIndex] = point2(1.0f, 0.0f); tIndex++;

	topPoints[tIndex] = tvertices[8]; topNormals[tIndex] = -cross(tvertices[3] - tvertices[8], tvertices[1] - tvertices[8]);
	topUV[tIndex] = point2(0.0f, 1.0f); tIndex++;
	topPoints[tIndex] = tvertices[3]; topNormals[tIndex] = -cross(tvertices[3] - tvertices[8], tvertices[1] - tvertices[8]);
	topUV[tIndex] = point2(0.0f, 0.0f); tIndex++;
	topPoints[tIndex] = tvertices[1]; topNormals[tIndex] = -cross(tvertices[3] - tvertices[8], tvertices[1] - tvertices[8]);
	topUV[tIndex] = point2(1.0f, 0.0f); tIndex++;

	topPoints[tIndex] = tvertices[9]; topNormals[tIndex] = -cross(tvertices[4] - tvertices[9], tvertices[5] - tvertices[9]);
	topUV[tIndex] = point2(0.0f, 1.0f); tIndex++;
	topPoints[tIndex] = tvertices[4]; topNormals[tIndex] = -cross(tvertices[4] - tvertices[9], tvertices[5] - tvertices[9]);
	topUV[tIndex] = point2(0.0f, 0.0f); tIndex++;
	topPoints[tIndex] = tvertices[5]; topNormals[tIndex] = -cross(tvertices[4] - tvertices[9], tvertices[5] - tvertices[9]);
	topUV[tIndex] = point2(1.0f, 0.0f); tIndex++;

	topPoints[tIndex] = tvertices[9]; topNormals[tIndex] = -cross(tvertices[6] - tvertices[9], tvertices[4] - tvertices[9]);
	topUV[tIndex] = point2(0.0f, 1.0f); tIndex++;
	topPoints[tIndex] = tvertices[4]; topNormals[tIndex] = -cross(tvertices[6] - tvertices[9], tvertices[4] - tvertices[9]);
	topUV[tIndex] = point2(0.0f, 0.0f); tIndex++;
	topPoints[tIndex] = tvertices[6]; topNormals[tIndex] = -cross(tvertices[6] - tvertices[9], tvertices[4] - tvertices[9]);
	topUV[tIndex] = point2(1.0f, 0.0f); tIndex++;

	topPoints[tIndex] = tvertices[9]; topNormals[tIndex] = -cross(tvertices[7] - tvertices[9], tvertices[6] - tvertices[9]);
	topUV[tIndex] = point2(0.0f, 1.0f); tIndex++;
	topPoints[tIndex] = tvertices[6]; topNormals[tIndex] = -cross(tvertices[7] - tvertices[9], tvertices[6] - tvertices[9]);
	topUV[tIndex] = point2(0.0f, 0.0f); tIndex++;
	topPoints[tIndex] = tvertices[7]; topNormals[tIndex] = -cross(tvertices[7] - tvertices[9], tvertices[6] - tvertices[9]);
	topUV[tIndex] = point2(1.0f, 0.0f); tIndex++;

	topPoints[tIndex] = tvertices[9]; topNormals[tIndex] = -cross(tvertices[5] - tvertices[9], tvertices[7] - tvertices[9]);
	topUV[tIndex] = point2(0.0f, 1.0f); tIndex++;
	topPoints[tIndex] = tvertices[5]; topNormals[tIndex] = -cross(tvertices[5] - tvertices[9], tvertices[7] - tvertices[9]);
	topUV[tIndex] = point2(0.0f, 0.0f); tIndex++;
	topPoints[tIndex] = tvertices[7]; topNormals[tIndex] = -cross(tvertices[5] - tvertices[9], tvertices[7] - tvertices[9]);
	topUV[tIndex] = point2(1.0f, 0.0f); tIndex++;

}
// initialization
void generateTop(GLuint program, ShapeData* topData)
{
	colorTopSides();
	colorTopEnds();
	topData->numVertices = numTopVertices;

	// Create a vertex array object
	glGenVertexArrays(1, &topData->vao);
	glBindVertexArray(topData->vao);

	// Set vertex attributes
	setVertexAttrib(program,
		(float*)topPoints, sizeof(topPoints),
		(float*)topNormals, sizeof(topNormals),
		(float*)topUV, sizeof(topUV));
}

//----------------------------------------------------------------------------
// Cube

const int numCubeVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 cubePoints [numCubeVertices];
point3 cubeNormals[numCubeVertices];
point2 cubeUV     [numCubeVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
    point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
};

// quad generates two triangles for each face and assigns normals and texture coordinates
//    to the vertices
int Index = 0;
void quad( int a, int b, int c, int d, const point3& normal )
{
    cubePoints[Index] = vertices[a]; cubeNormals[Index] = normal; 
    cubeUV[Index] = point2(0.0f, 1.0f); Index++;
    cubePoints[Index] = vertices[b]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(0.0f, 0.0f); Index++;
    cubePoints[Index] = vertices[c]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(1.0f, 0.0f); Index++;
    cubePoints[Index] = vertices[a]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(0.0f, 1.0f); Index++;
    cubePoints[Index] = vertices[c]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(1.0f, 0.0f); Index++;
    cubePoints[Index] = vertices[d]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(1.0f, 1.0f); Index++;
}

// generate 12 triangles: 36 vertices, 36 normals, 36 texture coordinates
void colorcube()
{
    quad( 1, 0, 3, 2, point3( 0.0f,  0.0f,  1.0f) );
    quad( 2, 3, 7, 6, point3( 1.0f,  0.0f,  0.0f) );
    quad( 3, 0, 4, 7, point3( 0.0f, -1.0f,  0.0f) );
    quad( 6, 5, 1, 2, point3( 0.0f,  1.0f,  0.0f) );
    quad( 4, 5, 6, 7, point3( 0.0f,  0.0f, -1.0f) );
    quad( 5, 4, 0, 1, point3(-1.0f,  0.0f,  0.0f) );
}

// initialization
void generateCube(GLuint program, ShapeData* cubeData)
{
    colorcube();
    cubeData->numVertices = numCubeVertices;

    // Create a vertex array object
    glGenVertexArrays( 1, &cubeData->vao );
    glBindVertexArray( cubeData->vao );

    // Set vertex attributes
    setVertexAttrib(program, 
        (float*)cubePoints,  sizeof(cubePoints), 
        (float*)cubeNormals, sizeof(cubeNormals),
        (float*)cubeUV,      sizeof(cubeUV));
}


//----------------------------------------------------------------------------
// Sphere approximation by recursive subdivision of a tetrahedron

const int N = 5;  // number of subdivisions
const int numSphereVertices = 16*256*3;  // number of resulting points

point4 spherePoints [numSphereVertices];
point3 sphereNormals[numSphereVertices];
point2 sphereUVs[numSphereVertices];

// four equally spaced points on the unit circle

point4 v[4] = {
    vec4(0.0, 0.0, 1.0, 1.0), 
    vec4(0.0, 0.942809, -0.333333, 1.0),
    vec4(-0.816497, -0.471405, -0.333333, 1.0),
    vec4(0.816497, -0.471405, -0.333333, 1.0)
};

static int k = 0;

// move a point to unit circle

point4 unit(const point4 &p)
{
    point4 c;
    double d=0.0;
    for(int i=0; i<3; i++) d+=p[i]*p[i];
    d=sqrt(d);
    if(d > 0.0) for(int i=0; i<3; i++) c[i] = p[i]/d;
    c[3] = 1.0;
    return c;
}

void triangle(point4 a, point4 b, point4 c)
{
    spherePoints[k] = a;
    k++;
    spherePoints[k] = b;
    k++;
    spherePoints[k] = c;
    k++;
}

void divide_triangle(point4 a, point4 b, point4 c, int n)
{
    point4 v1, v2, v3;
    if(n>0)
    {
        v1 = unit(a + b);
        v2 = unit(a + c);
        v3 = unit(b + c);   
        divide_triangle(a , v2, v1, n-1);
        divide_triangle(c , v3, v2, n-1);
        divide_triangle(b , v1, v3, n-1);
        divide_triangle(v1, v2, v3, n-1);
    }
    else triangle(a, b, c);
}

void tetrahedron(int n)
{
    divide_triangle(v[0], v[1], v[2], n);
    divide_triangle(v[3], v[2], v[1], n);
    divide_triangle(v[0], v[3], v[1], n);
    divide_triangle(v[0], v[3], v[2], n);
}

// initialization

void generateSphere(GLuint program, ShapeData* sphereData)
{
    tetrahedron(N);

    sphereData->numVertices = numSphereVertices;

    // Normals
    for (int i = 0; i < numSphereVertices; i++)
    {
        sphereNormals[i] = point3(spherePoints[i].x, spherePoints[i].y, spherePoints[i].z);
    }
    
    // TexCoords
    double u, v;
    for (int i = 0; i < numSphereVertices; i++)
    {
        v = 0.5 - asin(spherePoints[i].y)/M_PI ; //0~1
        u = 0.5*(atan2(spherePoints[i].z,spherePoints[i].x)/M_PI + 1); //0~1
        sphereUVs[i] = point2(u,v);
    }

    // Create a vertex array object
    glGenVertexArrays( 1, &sphereData->vao );
    glBindVertexArray( sphereData->vao );

    // Set vertex attributes
    setVertexAttrib(program,
        (float*)spherePoints,  sizeof(spherePoints),
        (float*)sphereNormals, sizeof(sphereNormals),
        (float*)sphereUVs, sizeof(sphereUVs));
}

//----------------------------------------------------------------------------
// Cone

const int numConeDivisions = 32;
const int numConeVertices = numConeDivisions * 6;

point4 conePoints [numConeVertices];
point3 coneNormals[numConeVertices];

point2 circlePoints[numConeDivisions];

void makeCircle(point2* dest, int numDivisions)
{
    for (int i = 0; i < numDivisions; i++)
    {
        float a = i * 2.0f * M_PI / numDivisions;
        dest[i] = point2(cosf(a), sinf(a));
    }
}

void makeConeWall(point4* destp, point3* destn, int numDivisions, float z1, float z2, int& Index, int dir)
{
    for (int i = 0; i < numDivisions; i++)
    {
        point3 p1(circlePoints[i].x, circlePoints[i].y, z1);
        point3 p2(0.0f, 0.0f, z2);
        point3 p3(circlePoints[(i+1)%numDivisions].x, circlePoints[(i+1)%numDivisions].y, z1);
        if (dir == -1)
        {
            point3 temp = p1;
            p1 = p3;
            p3 = temp;
        }
        point3 n = cross(p1-p2, p3-p2);
        destp[Index] = p1; destn[Index] = n; Index++;
        destp[Index] = p2; destn[Index] = n; Index++;
        destp[Index] = p3; destn[Index] = n; Index++;
    }    
}

void generateCone(GLuint program, ShapeData* coneData)
{
    makeCircle(circlePoints, numConeDivisions);
    int Index = 0;
    makeConeWall(conePoints, coneNormals, numConeDivisions, 1.0f, 1.0f, Index, 1);
    makeConeWall(conePoints, coneNormals, numConeDivisions, 1.0f, -1.0f, Index, -1);
    
    coneData->numVertices = numConeVertices;

    // Create a vertex array object
    glGenVertexArrays( 1, &coneData->vao );
    glBindVertexArray( coneData->vao );
    
    // Set vertex attributes
    setVertexAttrib(program,
                    (float*)conePoints,  sizeof(conePoints),
                    (float*)coneNormals, sizeof(coneNormals),
                    0, 0);
}

//----------------------------------------------------------------------------
// Cylinder

const int numCylDivisions = 32;
const int numCylVertices = numCylDivisions * 12;

point4 cylPoints [numCylVertices];
point3 cylNormals[numCylVertices];
point2 cylUVs[numCylVertices];

void generateCylinder(GLuint program, ShapeData* cylData)
{
    makeCircle(circlePoints, numCylDivisions);
    int Index = 0;
    makeConeWall(cylPoints, cylNormals, numCylDivisions, 1.0f, 1.0f, Index, 1);
    makeConeWall(cylPoints, cylNormals, numCylDivisions, -1.0f, -1.0f, Index, -1);
    
    for (int i = 0; i < numCylDivisions; i++)
    {
        int i2 = (i+1)%numCylDivisions;
        point3 p1(circlePoints[i2].x, circlePoints[i2].y, -1.0f);
        point3 p2(circlePoints[i2].x, circlePoints[i2].y, 1.0f);
        point3 p3(circlePoints[i].x,  circlePoints[i].y,  1.0f);
        //point3 n = cross(p3-p2, p1-p2);
		cylPoints[Index] = p1; cylNormals[Index] = point3(p1.x, p1.y, 0.0f); Index++;
        cylPoints[Index] = p2; cylNormals[Index] = point3(p2.x, p2.y, 0.0f); Index++;
        cylPoints[Index] = p3; cylNormals[Index] = point3(p3.x, p3.y, 0.0f); Index++;
        p1 = point3(circlePoints[i2].x, circlePoints[i2].y, -1.0f);
        p2 = point3(circlePoints[i].x,  circlePoints[i].y,  1.0f);
        p3 = point3(circlePoints[i].x,  circlePoints[i].y,  -1.0f);
        //n = cross(p3-p2, p1-p2);
        cylPoints[Index] = p1; cylNormals[Index] = point3(p1.x, p1.y, 0.0f); Index++;
        cylPoints[Index] = p2; cylNormals[Index] = point3(p2.x, p2.y, 0.0f); Index++;
        cylPoints[Index] = p3; cylNormals[Index] = point3(p3.x, p3.y, 0.0f); Index++;
    }
    
	double u, v;
	for (int i = 0; i < numCylVertices; i++)
	{
		v = 0.5 - asin(cylPoints[i].y) / M_PI; //0~1
		u = 0.5*(atan2(cylPoints[i].z, cylPoints[i].x) / M_PI + 1); //0~1
		cylUVs[i] = point2(u, v);
	}

    cylData->numVertices = numCylVertices;
    
    // Create a vertex array object
    glGenVertexArrays( 1, &cylData->vao );
    glBindVertexArray( cylData->vao );
    
    // Set vertex attributes
	setVertexAttrib(program,
		(float*)cylPoints, sizeof(cylPoints),
		(float*)cylNormals, sizeof(cylNormals),
		(float*)cylUVs, sizeof(cylUVs));
}