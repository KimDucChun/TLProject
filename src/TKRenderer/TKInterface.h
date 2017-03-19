#pragma once

#include "TKCommon.h"

#define LOD_INFINITY_DISTANCE 0x7FFFFFFF

class ITKObject
{
public:
    virtual ETKObjectType GetObjectType(void) = 0;
};

class TKEngine;

class TKMeshShaderEffect;
class TKMeshRes;
class TKMeshMaterial;
class TKMeshVertexes;
class TKMeshFaces;
class TKMeshGeometry;
class TKTerrainGeom;

class TKCollisionResource;
class TKCollisionObjectRes;
struct ST_ORIGINCOLLISIONDATA;

typedef vector<TKMeshRes *> MESHRESLIST;
typedef unordered_map<string, TKMeshMaterial *> MATERIALLIST;
typedef vector<TKMeshVertexes *> VERTEXESLIST;
typedef vector<TKMeshFaces *> FACESLIST;
typedef vector<TKMeshGeometry *> GEOMLIST;
typedef unordered_map<int, GEOMLIST *> LODLIST;
typedef vector<TKTerrainGeom *> TERRAINGEOMLIST;
typedef unordered_map<int, TKTerrainGeom *> LODTERRAINGEOMLIST;

typedef vector<TKCollisionResource *> COLLISIONRESLIST;
typedef vector<TKCollisionObjectRes *> COLLISIONOBJRESLIST;
typedef vector<ST_ORIGINCOLLISIONDATA*> ORIGINCOLLISIONLIST;

///////////////////////// 메시 클래스 관련
#define template_R_FrameSide template <typename DATATYPE, class RENDERRESOURCE>
#define template_R_RenderSide template <typename DATATYPE, class RENDERRESOURCE>
#define template_R_Object template <class FRAMESIDEOBJECT, class RENDERSIDEOBJECT, class RENDERRESOURCE>

/////////////////////////