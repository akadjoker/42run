#pragma once

#include "Config.hpp"
#include "Math.hpp"
#include "Std.hpp"
#include "OpenGL.h"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Shader.hpp"






class  MeshImporter
{
public:

    MeshImporter();
    virtual ~MeshImporter();




        struct MeshMaterial 
        {
        String name;
        String texture;

         Vec3 ambient;
         Vec3 diffuse;
         Vec3 specular;
        MeshMaterial()
            {
                name = "";
                texture = "";
                ambient = Vec3(1.0f);
                diffuse = Vec3(1.0f);
                specular = Vec3(1.0f);
  
            }
           MeshMaterial(const MeshMaterial& other)
        {
            name = other.name;
            texture = other.texture;
            ambient = other.ambient;
            diffuse = other.diffuse;
            specular = other.specular;

        }
        MeshMaterial& operator=(const MeshMaterial& other) 
        {
            if (this == &other)
                return *this;
            name = other.name;

            texture = other.texture;
            ambient = other.ambient;
            diffuse = other.diffuse;
            specular = other.specular;
            return *this;
        }
        
        };





    virtual Mesh* Load(const VertexFormat& vertexFormat,void* data, u64 size) { (void)vertexFormat;(void) data; (void)size; return nullptr; };
    virtual Mesh* Load(const VertexFormat& vertexFormat,const String& fileName) { (void)vertexFormat;(void)fileName; return nullptr; };
    virtual void Clear() {};



protected:

 


    Array<MeshMaterial> m_materials;
    HashTable<MeshMaterial> m_materialsMap;


};

class  MeshImporterOBJ : public MeshImporter
{
public:
    MeshImporterOBJ() : MeshImporter()
     {
        

      

     };

    Mesh* Load(const VertexFormat& vertexFormat,const String& fileName) override;
    void Clear() override;




private:


    Array<Vec3> m_positions;
    Array<Vec2> m_texCoords;
    Array<Vec3> m_normals;

    Array<Vertex> m_vertices;
    Array<unsigned int> m_indices;




    HashTable<u32> m_vertexmap;




    
        bool parseFile( FileStream &stream,Mesh *mesh);
        bool LoadMaterial(const String& fileName);

      
        void pushVertex(unsigned int v, unsigned int vt, unsigned int vn, bool hasUV, bool hasNormal);
        
        int GetIndex(const String& name);

  
};

class  MeshImporterH3D : public MeshImporter
{
    private:
        
public:
    MeshImporterH3D() : MeshImporter() {};
    Mesh* Load(const VertexFormat& vertexFormat,const String& fileName) override;


};