#include "pch.h"

#define H3D_XYZ              0x0000
#define H3D_NORMAL           0x0002
#define H3D_TANGENT          0x0004
#define H3D_TEXTURE          0x0008


//***********************************************************************************************************
//***********************************************************************************************************

MeshImporter::MeshImporter()
{
  
}

MeshImporter::~MeshImporter()
{
  
}





Mesh* MeshImporterOBJ::Load(const VertexFormat& vertexFormat,const String& fileName)
{

    

    FileStream file;
    if (!file.Open(fileName.c_str(), "r"))
    {
        LogError("Cannot open %s", fileName.c_str());
        return nullptr;
    }

    Mesh* mesh = new Mesh(vertexFormat);

    if (!parseFile(file, mesh))
    {
        LogError("Loading %s mesh failed", fileName.c_str());
        mesh->Drop();
        Clear();
        file.Close();
        return nullptr;
    }


    file.Close(); 

    Clear();
    return mesh;
 }

bool MeshImporterOBJ::parseFile( FileStream &stream,Mesh *mesh)
{
      bool firstMesh = true;
      String currentMtl;
      String currentObj;
      bool gerenateUVS = false;
      bool gerenateNormals = false;

     Surface *surface = nullptr;

      String dir = stream.GetPath();
      LogInfo("Loading %s", dir.c_str());


  String Line;
   while(!stream.IsEOF())
   { 
      Line = stream.ReadLine(true);
      if (Line.empty())
      {
          continue;
      }

      if (Line.substr(0,2) == "v ")
      {

         Vec3 v;
         sscanf(Line.substr(2).c_str(), "%f %f %f", &v.x, &v.y, &v.z);
         m_positions.push_back(v);
      }else 
      if (Line.substr(0,3) == "vt ")
      {
             Vec2 v;
             sscanf(Line.substr(3).c_str(), "%f %f", &v.x, &v.y);
             m_texCoords.push_back(v);
      }else
      if (Line.substr(0,3) == "vn ")
      {
          Vec3 v;
          sscanf(Line.substr(3).c_str(), "%f %f %f", &v.x, &v.y, &v.z);
          m_normals.push_back(v);
      }else
      if (Line.substr(0,2) == "f ")
      {
          unsigned int v1, v2, v3, v4;
          unsigned int vt1, vt2, vt3, vt4;
          unsigned int vn1, vn2, vn3, vn4;

          bool hasUV = true;
          bool hasNormal = true;
          bool done = false;

 

          // v1/v2/v3/v4
          int matches = sscanf(Line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u %u/%u/%u", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3, &v4, &vt4, &vn4);
 

          if (matches == 12)
          {
            done = true;
            gerenateNormals = false;
            gerenateUVS = false;
            pushVertex(v1, vt1, vn1, hasUV,hasNormal);
            pushVertex(v2, vt2, vn2, hasUV,hasNormal);
            pushVertex(v3, vt3, vn3, hasUV,hasNormal);
            pushVertex(v1, vt1, vn1, hasUV,hasNormal);
            pushVertex(v3, vt3, vn3, hasUV,hasNormal);
            pushVertex(v4, vt4, vn4, hasUV,hasNormal);
          }
            // v1/v2/v3
          matches = sscanf(Line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
          if (matches == 9 && !done)
          {
            done = true;
            gerenateNormals = false;
            gerenateUVS = false;
             pushVertex(v1, vt1, vn1, hasUV,hasNormal);
            pushVertex(v2, vt2, vn2, hasUV,hasNormal);
            pushVertex(v3, vt3, vn3, hasUV,hasNormal);
          }
           // v1//vn1
          matches = sscanf(Line.c_str(), "f %u//%u %u//%u %u//%u", &v1, &vn1, &v2, &vn2, &v3, &vn3);
          if (matches == 6 && !done)
          {
            done = true;
            hasUV = false;
            gerenateNormals = false;
            gerenateUVS = true;
            pushVertex(v1, vt1, vn1, hasUV,hasNormal);
            pushVertex(v2, vt2, vn2, hasUV,hasNormal);
            pushVertex(v3, vt3, vn3, hasUV,hasNormal);
          }
           //v1//
            matches = sscanf(Line.c_str(), "f %u// %u// %u//", &v1, &v2, &v3);
            if (matches == 3 && !done)
            {
              done = true;
              gerenateNormals = true;
              gerenateUVS = true;
              pushVertex(v1, vt1, vn1, hasUV,hasNormal);
              pushVertex(v2, vt2, vn2, hasUV,hasNormal);
              pushVertex(v3, vt3, vn3, hasUV,hasNormal);
            }    

          
           if (!done)
          {
            LogError( "Unsupported file!");
            return false;
          }
      }else 
      if (Line.substr(0,7) == "mtllib ")
      {
          String mtlPath = Line.substr(7);
          
          String fullPath = dir + "/" + mtlPath;
         // LogInfo("Load material lib %s", fullPath.c_str());
          LoadMaterial(fullPath);

          for (u32 i = 0; i < m_materials.size(); i++)
          {
              auto mat = m_materials[i];
              if (mat.texture.empty())
              {
                  LogWarning("Material %s use colors", m_materials[i].name.c_str());
                  mesh->AddMaterial(new TextureMaterial(TextureManager::Instance().GetDefault()));
              } else
              {
                      
                     Texture2D * texture =TextureManager::Instance().Get(mat.texture.c_str());
                      if (texture)
                      {
                           mesh->AddMaterial(new TextureMaterial(texture));
                      } else
                      {
                          LogError("Cannot load texture %s", mat.texture.c_str());
                          mesh->AddMaterial(new TextureMaterial(TextureManager::Instance().GetDefault()));
                      }
                  } 
           }
          
      }else
      if (Line.substr(0,7) == "usemtl ")
      {

           if (!firstMesh) 
          {
              int matIndex = GetIndex(currentMtl) ;


        
    

            
               surface = mesh->AddSurface(matIndex);
               surface->SetName(currentObj);
 
    
      
                for (u32 i = 0; i < m_indices.size() / 3; i++)
                {
                   surface->AddFace(m_indices[i * 3], m_indices[i * 3 + 1], m_indices[i * 3 + 2]);
                }
                  for (u32 i = 0; i < m_vertices.size(); ++i)
                {
                    
                    surface->AddVertex(m_vertices[i].position, m_vertices[i].texCoords, m_vertices[i].normal);
                }
            
             
                if (gerenateUVS)        surface->TexturePlanarMapping(0.1f);
                if (gerenateNormals)    surface->CalculateSmothNormals();
                
                 surface->Upload();
                 m_vertices.clear();
                 m_indices.clear();
          }

           firstMesh = false;

           currentMtl = Line.substr(7);
          //LogInfo("Use Material %s", currentMtl.c_str());
      }else
      if ((Line.substr(0,2) == "o ") || (Line.substr(0,2) == "g "))
      {
          //LogInfo("Object %s", Line.substr(2).c_str());
          currentObj = Line.substr(2);
      } else 
      if (Line.substr(0,1) == "#")
      {

      } else
      {
 
      }





      
   };

  // LogInfo( "Creating mesh %s material %d %s  %d %d", currentObj.c_str(), 0, currentMtl.c_str(), m_indices.size(),m_vertices.size());


    surface = mesh->AddSurface(0);
    surface->SetName(currentObj);
    for (u32 i = 0; i < m_indices.size() / 3; i++)
    {
	    surface->AddFace(m_indices[i*3], m_indices[i*3+1], m_indices[i*3+2]);
	  }   

    for (u32 i = 0; i < m_vertices.size(); ++i)
    {
	    surface->AddVertex(m_vertices[i].position,m_vertices[i].texCoords,m_vertices[i].normal);
	  }   

    if (gerenateUVS)
    {

        surface->TexturePlanarMapping(0.1f);
    }
    if (gerenateNormals)
    {

        surface->CalculateSmothNormals();
    }

    surface->Upload();
    mesh->m_boundingBox.Merge(surface->GetBoundingBox());
  
   // mesh->AddMaterial(new TextureMaterial(TextureManager::Instance().GetDefault()));
  


    mesh->OrderByMaterial();
  
    return true;
}

bool MeshImporterOBJ::LoadMaterial(const String& fileName)
{
  

    FileStream stream;
    if (!stream.Open(fileName.c_str(), "r"))
    {
        LogError("Cannot open %s", fileName.c_str());
        return false;
    }


    String Line;
    bool first = true;
    MeshMaterial currentMtl = MeshMaterial();
    while(!stream.IsEOF())
    { 
        Line = stream.ReadLine(true);
        if (Line.empty())      {          continue;      }

        if (Line.substr(0,7) == "newmtl ")
        {
            if (!first)
            {
                m_materials.push_back(currentMtl);
                m_materialsMap.insert(currentMtl.name.c_str(), currentMtl);
            //    LogInfo("Create Material %s total:%d", currentMtl.name.c_str(), (int)m_materials.size()-1);
       

                currentMtl = MeshMaterial();
             }
            first = false;
            currentMtl.name = Line.substr(7);
        }else
        if (Line.substr(0,3) == "Ka ")
        {
            sscanf(Line.substr(3).c_str(), "%f %f %f", &currentMtl.ambient.x, &currentMtl.ambient.y, &currentMtl.ambient.z);
        }else
        if (Line.substr(0,3) == "Kd ")
        {
            sscanf(Line.substr(3).c_str(), "%f %f %f", &currentMtl.diffuse.x, &currentMtl.diffuse.y, &currentMtl.diffuse.z);
        }else
        if (Line.substr(0,3) == "Ks ")
        {
            sscanf(Line.substr(3).c_str(), "%f %f %f", &currentMtl.specular.x, &currentMtl.specular.y, &currentMtl.specular.z);
        }else
        if (Line.substr(0,7) == "map_Kd ")
        {
            String texPath = Line.substr(7);
            currentMtl.texture = texPath;
          //  LogInfo("Difuse texture %s", texPath.c_str());
        } else if (Line.substr(0,1) == "#")
        {
        }

       
    }

 // LogInfo("Create Material %s total:%d", currentMtl.name.c_str(),(int)m_materials.size());
    m_materials.push_back(currentMtl);
    m_materialsMap.insert(currentMtl.name.c_str(), currentMtl);



    stream.Close();
    return true;
}


int MeshImporterOBJ::GetIndex(const String& name)
{
    for (u32 i = 0; i < m_materials.size(); i++)
    {
	    if (m_materials[i].name == name)
	        return i;
    }
    LogWarning("Material %s not found", name.c_str());

    return 0;


}

void MeshImporterOBJ::pushVertex(unsigned int v, unsigned int vt, unsigned int vn, bool hasUV, bool hasNormal)
{
       v--;
      vt--;
      vn--;

      Vertex vertex; 
     vertex.position = m_positions[v];
     vertex.texCoords = hasUV ? m_texCoords[vt] : Vec2(0.0f);
     vertex.normal = hasNormal ? m_normals[vn] : Vec3(0.0f);
     vertex.position.y = vertex.position.y;

      String k ="_";
        k+=String(v);
        k+="/";
        k+=String(vt);
        k+="/";
        k+=String(vn);

 
      if (!m_vertexmap.contains(k.c_str()))
      {

          m_vertices.push_back(vertex);
          u32 value = m_vertices.size()-1;
          m_vertexmap.insert(k.c_str(), value);
      }

        u32 value =0;
        
        if (!m_vertexmap.find(k.c_str(),value))
        {
            LogError("Cannot find vertex %s", k.c_str());
        }

        m_indices.push_back(value);

}


 void MeshImporterOBJ::Clear()
 {
 
 }

//***********************************************************************************************************
//***********************************************************************************************************


    Mesh*  MeshImporterH3D::Load(const VertexFormat& vertexFormat,const String& fileName) 
    {


         String meshPath =   MeshBuilder::Instance().GetLoadPath();
         String texturePath = TextureManager::Instance().GetTexturePath();

         String meshLoad = meshPath + fileName;


        FileStream stream(meshLoad, "rb");
        if (!stream.IsOpen())
        {
            LogError("Cannot open %s", meshLoad.c_str());
            return nullptr;
        }

        Mesh *mesh = new Mesh(vertexFormat);

       char a = stream.ReadChar(); // H
       char b = stream.ReadChar(); // 3
       char c = stream.ReadChar(); // D

        if (a != 'H' || b != '3' || c != 'D')
        {
            LogError("Invalid H3D file");
            return nullptr;
        } else 
        {
        short version = stream.ReadShort();
        LogInfo( "H3D file version %d (%c%c%c) ", version,a,b,c);

        }

      
        int numMaterial = stream.ReadInt(); // num materials

     //   Log(0, "Num materials %d", numMaterial);
            // String name = stream.ReadUTFString();
            // String base = stream.ReadUTFString();
            // int numTextures = stream.ReadInt();
            // Log(0, "Material %s base %s numTextures %d", name.c_str(), base.c_str(), numTextures);
        for (int i = 0; i < numMaterial; i++)
        {
             stream.ReadInt();//flags
            String name=stream.ReadUTFString();
            String textureName=stream.ReadUTFString();
            LogInfo( "Material (%s) texture (%s)", name.c_str(), textureName.c_str());
            if (textureName=="NONE")
            {
                mesh->AddMaterial(new TextureMaterial(TextureManager::Instance().GetDefault()));
                continue;
            }

                  String fullTexturePath = System::Instance().GetFileName(textureName.c_str());
     
          


                    String texPath = texturePath + fullTexturePath;

                    if (System::Instance().FileExists(texPath.c_str()))
                    {
    
                        Texture2D *texture = TextureManager::Instance().Get(texPath.c_str());               
                        mesh->AddMaterial(new TextureMaterial( texture));
                           
                    } else 
                    {
                         LogWarning( "Texture %s dont exits ", texPath.c_str());
                         mesh->AddMaterial(new TextureMaterial(TextureManager::Instance().GetDefault()));
                    }                  

                    
        }

        u32 numSurfaces = stream.ReadInt(); // num surfaces
      //  Log(0, "Num surfaces %d", numSurfaces);
        for (u32 i=0; i<numSurfaces; i++)
        {
            String name=stream.ReadUTFString();
            unsigned int flags = stream.ReadInt();
            int material    = stream.ReadInt();
            int numVertices = stream.ReadInt();
            int numIndices = stream.ReadInt();
       

          
          


         //   Log(0, "Surface %s material %d numVertices %d numIndices %d hasNormals %d hasTexcoords %d", name.c_str(), material, numVertices, numIndices, hasNormals, hasTexcoords);
            Surface *surface = mesh->AddSurface(material);
            surface->SetName(name);
    
            for (int j = 0; j < numVertices; j++)
            {
                float x = stream.ReadFloat();
                float y = stream.ReadFloat();
                float z = stream.ReadFloat();
                float nx =0;
                float ny =0;
                float nz =0;
                float u=0;
                float v=0;

              

                if (flags & H3D_NORMAL)
                {
                    nx = stream.ReadFloat();
                    ny = stream.ReadFloat();
                    nz = stream.ReadFloat();
   
                }

                  if (flags & H3D_TEXTURE)
                {
                    u = stream.ReadFloat();
                    v = stream.ReadFloat();
                 }
          
               
                surface->AddVertex(x, y, z, u, v, nx, ny, nz);
            }
            for (int j = 0; j < numIndices; j++)
            {
                int a = stream.ReadInt();
                int b = stream.ReadInt();
                int c = stream.ReadInt();
                surface->AddFace(a, b, c);
            }
            if (!(flags & H3D_NORMAL)) 
            {
                surface->CalculateSmothNormals();
            }
            if (!(flags & H3D_TEXTURE))
            {
                surface->TexturePlanarMapping(0.1f);
            }
            surface->Upload();
        }
        mesh->OrderByMaterial();
        Clear();
       return mesh;
 }

