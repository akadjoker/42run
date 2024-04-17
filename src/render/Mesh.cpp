#include "pch.h"



 extern double GetTime();









//***********************************************************************************************************


VertexFormat::VertexFormat(const Element* elements, unsigned int elementCount)
    : _vertexSize(0)
{
    for (unsigned int i = 0; i < elementCount; ++i)
    {
        Element element;
        memcpy(&element, &elements[i], sizeof(Element));
        _elements.push_back(element);
        _vertexSize += element.size * sizeof(float);
    }
}

VertexFormat::~VertexFormat(){}

const VertexFormat::Element& VertexFormat::getElement(unsigned int index) const{return _elements[index];}

unsigned int VertexFormat::getElementCount() const{    return (unsigned int)_elements.size();}
unsigned int VertexFormat::getVertexSize() const{    return _vertexSize;}

bool VertexFormat::operator == (const VertexFormat& f) const
{
    if (_elements.size() != f._elements.size())
        return false;
    for (size_t i = 0, count = _elements.size(); i < count; ++i)
    {
        if (_elements[i] != f._elements[i])
            return false;
    }

    return true;
}
bool VertexFormat::operator != (const VertexFormat& f) const{    return !(*this == f);}
VertexFormat::Element::Element() :    usage(POSITION), size(0){}
VertexFormat::Element::Element(Usage usage, unsigned int size) :    usage(usage), size(size){}
bool VertexFormat::Element::operator == (const VertexFormat::Element& e) const{    return (size == e.size && usage == e.usage);}
bool VertexFormat::Element::operator != (const VertexFormat::Element& e) const{    return !(*this == e);}

//***********************************************************************************************************
 void Material::Bind(Shader *shader) 
 {
   
    (void)shader;
 }
TextureMaterial::TextureMaterial(Texture2D * texture): Material("TextureMaterial")
{
    if (texture)
    {
            m_texture = texture;
    } 
    layer=0;

}

void TextureMaterial::Bind( Shader *shader) 
{
    (void)shader;
    if (m_texture)
    {
       m_texture->Use(layer);
    }
}
void TextureMaterial::SetTexture(Texture2D * texture)
{
    m_texture = texture;

}



MeshBuffer::MeshBuffer( bool dynamic)
{
    m_vbo = 0;
    m_ibo = 0;
    m_vao = 0;
    m_dynamic = dynamic;
    m_useIndices = false;
   

}

MeshBuffer::~MeshBuffer()
{
   
    Release();
}

void MeshBuffer::CreateVertexBuffer(const VertexFormat& vertexFormat, unsigned int vertexCount)
{
    m_vertexFormat = vertexFormat; 
    m_numVertices = vertexCount;
    glGenBuffers(1, &this->m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexFormat.getVertexSize() * vertexCount, NULL, m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) ;

    if (!m_vao)
    {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);
    } else 
    {
        glBindVertexArray(m_vao);
    }
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    unsigned int offset = 0;
    for (unsigned int i = 0; i < vertexFormat.getElementCount(); ++i)
    {
        const VertexFormat::Element& element = vertexFormat.getElement(i);
     
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, (GLint)element.size, GL_FLOAT, GL_FALSE,(GLsizei)vertexFormat.getVertexSize(), (void*)(offset * sizeof(float)));

     
        offset += element.size;
    }

    glBindVertexArray(0);
}

void MeshBuffer::CreateIndexBuffer(unsigned int indexCount)
{
        if (!m_vao)
        {
            glGenVertexArrays(1, &m_vao);
            glBindVertexArray(m_vao);
        } else 
        {
            glBindVertexArray(m_vao);
        }
        m_useIndices = true;
        m_numIndices = indexCount;
        glGenBuffers(1, &this->m_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), NULL, m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) ;
        glBindVertexArray(0);
}

 void MeshBuffer::SetIndexData(const void* indexData)
 {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ibo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_numIndices * sizeof(unsigned int), indexData);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
 }

void MeshBuffer::SetVertexData(const void *vertexData)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_numVertices * m_vertexFormat.getVertexSize(), vertexData);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



void MeshBuffer::Render(int mode,int count)
{
   

    glBindVertexArray(m_vao);
    if (m_useIndices) 
    {
  
        glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
    }
    else 
    {
       
        glDrawArrays(mode, 0, count);
    }
}

void MeshBuffer::Release()
{

    if (m_vao != 0) 
    {
        glDeleteVertexArrays(1, &m_vao);
    }

    if (m_useIndices && m_ibo != 0) 
    {
        glDeleteBuffers(1, &m_ibo);
    }

    if (m_vbo != 0) 
    {
        glDeleteBuffers(1, &m_vbo);
    }

   m_vbo = 0;
   m_ibo = 0;


}

void *MeshBuffer::Lock()
{
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    
}

bool MeshBuffer::Unlock()
{
    
    bool ret = glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return ret;
}

//***********************************************************************************************************

Mesh::Mesh(const VertexFormat& vertexFormat,bool dynamic):Ref(),m_vertexFormat(vertexFormat), m_dynamic(dynamic)
{
    setDebugName("Mesh");   
}

Mesh::~Mesh()
{
   
    Release();
}

Surface *Mesh::AddSurface(u32 material)
{
    Surface *buffer = new Surface(m_vertexFormat,material, this->m_dynamic);
    m_surfaces.push_back(buffer);
    return buffer;
}

Surface *Mesh::GetSurface(u32 index)
{
    DEBUG_BREAK_IF(index >= m_surfaces.size());
    return m_surfaces[index];
}

int Mesh::AddMaterial(Material *material)
{
   
    m_materials.push_back(material);
    return (int)m_materials.size() - 1;
}


Material *Mesh::GetMaterial(u32 index)
{
    if (GetMaterialCount() == 0) 
    {
        return nullptr;
    }
    DEBUG_BREAK_IF(index >= m_materials.size());
    return m_materials[index];
}

void Mesh::SetMaterial(u32 index, Material *material)
{
    DEBUG_BREAK_IF(index >= m_materials.size());
    if (m_materials[index] != nullptr) 
    {
        delete m_materials[index];
    }
    m_materials[index] = material;
}

void Mesh::CalculateBoundingBox()
{
    m_boundingBox.Clear();
    for (u32 i = 0; i < m_surfaces.size(); ++i) 
    {
        m_boundingBox.Merge(m_surfaces[i]->GetBoundingBox());
    }
}

void Mesh::Release()
{

    for (u32 i = 0; i < m_surfaces.size(); i++) 
    {
     //   printf("Release Surface %d\n",i);
        delete m_surfaces[i];
    }


    for (u32 i = 0; i < m_materials.size(); i++) 
    {
       //  printf("Release material %d %s\n",i, m_materials[i]->GetName().c_str());
         delete m_materials[i];
    }


    m_surfaces.clear();
    m_materials.clear();

   
}

void Mesh::Transform(const Mat4 &transform)
{
    for (u32 i = 0; i < m_surfaces.size(); ++i) 
    {
        m_surfaces[i]->Transform(transform);
    }
    CalculateBoundingBox();
}
void Mesh::CalculateNormals()
{

    for (u32 i = 0; i < m_surfaces.size(); ++i) 
    {
        m_surfaces[i]->CalculateNormals();
    }
}

void Mesh::CalculateSmothNormals(bool angleWeighted)
{

    for (u32 i = 0; i < m_surfaces.size(); ++i) 
    {
        m_surfaces[i]->CalculateSmothNormals(angleWeighted);
    }
}

static s32 CompareSurfaces(Surface *a, Surface *b)
{
    return a->GetMaterial() - b->GetMaterial();
}

void Mesh::OrderByMaterial()
{
    m_surfaces.sort(CompareSurfaces);
}


void Mesh::Render(Shader *shader,bool ignoreMaterial)
{
    
    for ( u32 i = 0; i < m_surfaces.size(); ++i) 
    {
        if (!ignoreMaterial) 
        {
            Material *material = GetMaterial(m_surfaces[i]->GetMaterial());
            if (material!=nullptr) 
            {
               material->Bind(shader);
            }
        }
        m_surfaces[i]->Render();
    }

   
}
    

void Mesh::Render(Shader *shader,u32 mode,bool ignoreMaterial)
{
  
    for ( u32 i = 0; i < m_surfaces.size(); ++i) 
    {
       if (!ignoreMaterial) 
        {
            Material *material = GetMaterial(m_surfaces[i]->GetMaterial());
            if (material!=nullptr) 
            {
               material->Bind(shader);
            }
        }
        m_surfaces[i]->Render(mode);
    }
  

}

void Mesh::Render(Shader *shader,u32 mode, u32 count,bool ignoreMaterial)
{
   
    
    for ( u32 i = 0; i < m_surfaces.size(); ++i) 
    {
        if (!ignoreMaterial) 
        {
            Material *material = GetMaterial(m_surfaces[i]->GetMaterial());
            if (material!=nullptr) 
            {
               material->Bind(shader);
            }
        }
        m_surfaces[i]->Render(mode,count);
    }
  
  
}

Surface::Surface(const VertexFormat &vertexFormat, u32 material, bool dynamic)
{
    m_vertexFormat = vertexFormat;
    m_material = material;
    m_dynamic = dynamic;
    flags = VBO_POSITION | VBO_INDICES;
    m_stride = vertexFormat.getVertexSize();
    isDirty = true;
    IBO=0;
    VAO=0;
    m_name = "Surface";

    Init();
   
    
}

Surface::~Surface()
{
    Release();
}

void Surface::Transform(const Mat4 &transform)
{

    bool hasNormals = normals.size() == positions.size();
    for (u32 i = 0; i < positions.size(); ++i) 
    {
        positions[i] = Mat4::Transform(transform,positions[i]);
     
        if (hasNormals) 
        {
            normals[i] = Mat4::TranformNormal(transform,normals[i]);
        }
        
    }
    flags |= VBO_POSITION;
    if (hasNormals) 
    {
        flags |= VBO_NORMAL;
    }
    CalculateBoundingBox();

    isDirty = true;
}

void Surface::Init()
{

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    isDirty = true;


     for (u32 j = 0; j <m_vertexFormat.getElementCount(); ++j)
    {
            const VertexFormat::Element& e = m_vertexFormat.getElement(j);           
            if (e.usage == VertexFormat::POSITION) 
            {
                flags |= VBO_POSITION;
                VertexBuffer * buffer = new VertexBuffer();
                glGenBuffers(1, &buffer->id);
                glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
                glEnableVertexAttribArray(j);
                glVertexAttribPointer(j, 3, GL_FLOAT, GL_FALSE, (GLint)sizeof(Vec3), 0);

                buffer->size  = e.size;
                buffer->usage = e.usage;
                buffer->name = "POSITION";
            //    Log(1,"POSITION");
                
                AddBuffer(buffer);
            }
            else
            if (e.usage == VertexFormat::TEXCOORD0) 
            {
                flags |= VBO_TEXCOORD0;

                VertexBuffer * buffer = new VertexBuffer();
                glGenBuffers(1, &buffer->id);
                glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
                glEnableVertexAttribArray(j);
                glVertexAttribPointer(j, 2, GL_FLOAT, GL_FALSE, (GLint)sizeof(Vec2), 0);
                buffer->size  = e.size;
                buffer->usage = e.usage;
                buffer->name = "TEXCOORD0";
              //  Log(1,"TEXCOORD0");
                AddBuffer(buffer);

                

            }
            else if (e.usage == VertexFormat::NORMAL) 
            {
                flags |= VBO_NORMAL;
                VertexBuffer * buffer = new VertexBuffer();
                glGenBuffers(1, &buffer->id);
                glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
                glEnableVertexAttribArray(j);
                glVertexAttribPointer(j, 3, GL_FLOAT, GL_FALSE, (GLint)sizeof(Vec3), 0);
                buffer->size  = e.size;
                buffer->usage = e.usage;
                buffer->name = "NORMAL";
            //   Log(1,"NORMAL");
                AddBuffer(buffer);
            } else
                
            if (e.usage == VertexFormat::TANGENT) 
            {
                flags |= VBO_TANGENT;
               
                VertexBuffer * buffer = new VertexBuffer();
                glGenBuffers(1, &buffer->id);
                glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
    
                glEnableVertexAttribArray(j);
                glVertexAttribPointer(j, 3, GL_FLOAT, GL_FALSE, (GLint)sizeof(Vec3), 0);

                buffer->size  = e.size;
                buffer->usage = e.usage;
                buffer->name = "TANGENT";
                
                AddBuffer(buffer);
            } else          
            if (e.usage == VertexFormat::COLOR)
            {
                flags |= VBO_COLOR;
                VertexBuffer * buffer = new VertexBuffer();
                glGenBuffers(1, &buffer->id);
                glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
           
                glEnableVertexAttribArray(j);
               // glVertexAttribPointer(j, (GLint)e.size, GL_FLOAT, GL_FALSE, (GLint)e.size * sizeof(float), (void*)(0));
                glVertexAttribPointer(j, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
                buffer->size  = e.size;
                buffer->usage = e.usage;
                buffer->name = "COLOR";

           
                AddBuffer(buffer);
            } 
        }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Surface::FlipFaces()
{
    const u32 idxcnt = (u32)indices.size();
    if (idxcnt == 0) return;
    
     unsigned int *idx = indices.pointer();

    for (u32 i = 0; i < idxcnt; i += 3) 
    {
       const unsigned int tmp = idx[i+1];
        idx[i+1] = idx[i + 2];
        idx[i + 2] = tmp;
    }
    isDirty = true;
    flags |= VBO_INDICES;

}

void Surface::TexturePlanarMapping(float resolution)
{

     const u32 idxcnt = (u32)indices.size();
     const u32 texcnt = (u32)texCoords.size();


    if (idxcnt == 0) return;
    if (texcnt < positions.size()) 
    {
        for (size_t i = 0; i < positions.size(); ++i) 
        {
            texCoords.push_back(Vec2(0.0f, 0.0f));
        }
    }
    


    for (u32 i = 0; i < idxcnt; i += 3) 
    {
        Plane3D plane(positions[indices[i]], positions[indices[i + 1]], positions[indices[i + 2]]);
        Vec3 normal = plane.normal;
        normal.x = fabs(normal.x);
        normal.y = fabs(normal.y);
        normal.z = fabs(normal.z);

        if (normal.x > normal.y && normal.x > normal.z) 
        {
            for (u32 j = 0; j < 3; ++j) 
            {
                texCoords[indices[i + j]].x = positions[indices[i + j]].y * resolution;
                texCoords[indices[i + j]].y = positions[indices[i + j]].z * resolution;
            }
        } else 
        if (normal.y > normal.x && normal.y > normal.z) 
        {
            for (u32 j = 0; j < 3; ++j) 
            {
                texCoords[indices[i + j]].x = positions[indices[i + j]].x * resolution;
                texCoords[indices[i + j]].y = positions[indices[i + j]].z * resolution;
            }
        } else 
        {
            for (u32 j = 0; j < 3; ++j) 
            {
                texCoords[indices[i + j]].x = positions[indices[i + j]].x * resolution;
                texCoords[indices[i + j]].y = positions[indices[i + j]].y * resolution;
            }
        }
           
    }



    flags |= VBO_TEXCOORD0;
    isDirty = true;
}
void Surface::TexturePlanarMapping(float resolutionS, float resolutionT, u8 axis, const Vec3& offset)
{
     const u32 idxcnt = (u32)indices.size();
     const u32 texcnt = (u32)texCoords.size();
     if (idxcnt == 0 || texcnt==0) return;


    for (u32 i = 0; i < idxcnt; i += 3)
    {

        if (axis==0)
        {
                for (u32 j = 0; j < 3; ++j) 
                {
                    texCoords[indices[i + j]].x =0.5f +(positions[indices[i + j]].z + offset.z) * resolutionS;
                    texCoords[indices[i + j]].y =0.5f -(positions[indices[i + j]].y + offset.y) * resolutionT;
                

                }
        } else if (axis==1)
        {
                for (u32 j = 0; j < 3; ++j) 
                {
                    texCoords[indices[i + j]].x =0.5f +(positions[indices[i + j]].x + offset.x) * resolutionS;
                    texCoords[indices[i + j]].y =1.0f -(positions[indices[i + j]].z + offset.z) * resolutionT;
                }
        } else if (axis==2)
        {
                for (u32 j = 0; j < 3; ++j) 
                {
                    texCoords[indices[i + j]].x =0.5f +(positions[indices[i + j]].x + offset.x) * resolutionS;
                    texCoords[indices[i + j]].y =0.5f -(positions[indices[i + j]].y + offset.y) * resolutionT;
                }
        }
    
    }



    flags |= VBO_TEXCOORD0;
    isDirty = true;
}

void Surface::Upload()
{

    for (u32 i = 0; i < buffers.size(); ++i) 
    {
        VertexBuffer *buffer = buffers[i];

      //  Log(1,"Upload usage %d id %d size %d name %s",buffer->usage, buffer->id ,buffer->size, buffer->name.c_str());
        if (buffer->usage == VertexFormat::POSITION) 
        {
            if (flags & VBO_POSITION) 
            {
                glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
                glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(Vec3), positions.const_pointer(), m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
             //  Log(1,"Upload POSITION  ");
            }
        }
        else if (buffer->usage == VertexFormat::TEXCOORD0) 
        {
            if (flags & VBO_TEXCOORD0) 
            {
                if (texCoords.size() != positions.size()) 
                {
                    for (size_t i = 0; i < positions.size(); ++i) 
                    {
                        texCoords.push_back(Vec2(0.0f, 0.0f));
                    }
                }
             //   Log(1,"Upload TEXCOORD0  ");
                glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
                glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(Vec2), texCoords.const_pointer(), m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
            }

        }
        else if (buffer->usage == VertexFormat::NORMAL) 
        {
            if (flags & VBO_NORMAL) 
            {

                if (normals.size() != positions.size()) 
                {
                    for (size_t i = 0; i < positions.size(); ++i) 
                    {
                        normals.push_back(Vec3(0.0f, 0.0f, 0.0f));
                    }
                }

            //    Log(1,"Upload NORMAL  ");
                glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
                glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vec3), normals.const_pointer(), m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
            }
        }
        else if (buffer->usage == VertexFormat::TANGENT) 
        {
            // if (flags & VBO_TANGENT) 
            // {
            //     if (tangents.size() != positions.size()) 
            //     {
            //         for (size_t i = 0; i < positions.size(); ++i) 
            //         {
            //             tangents.push_back(Vec3(0.0f, 0.0f, 0.0f));
            //         }
            //     }

            //     glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
            //     glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(Vec3), &tangents[0], m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
            // }

            // glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
            // glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(Vec3), &tangents[0], m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        }
        else if (buffer->usage == VertexFormat::COLOR) 
        {

            if (colors.size() != positions.size()) 
            {
                for (size_t i = 0; i < positions.size(); ++i) 
                {
                    colors.push_back(255);colors.push_back(255);colors.push_back(255);colors.push_back(255);                
                }
            }

            glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
            glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(unsigned char), colors.const_pointer(), m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        }
    }

    if (flags & VBO_INDICES) 
    {
       // Log(1,"Upload INDICES  ");
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.const_pointer(), m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    flags = 0;
    isDirty = false;

}

void Surface::Release()
{

    if (VAO != 0) 
    {
        glDeleteVertexArrays(1, &VAO);
    }
    if (IBO != 0) 
    {
        glDeleteBuffers(1, &IBO);
    }
    for (u32 i = 0; i < buffers.size(); ++i) 
    {
        VertexBuffer *buffer = buffers[i];
    
        glDeleteBuffers(1, &buffer->id);
        buffer->id = 0;
        delete buffer;
    }

    buffers.clear();

}

void Surface::Render(u32 mode,u32 count)
{
    if (isDirty) 
    {
        Upload();
    }

  

    glBindVertexArray(VAO);
    glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Surface::Render(u32 mode)
{
   
    this->Render(mode, this->GetIndexCount());
}
void Surface::Render()
{
    this->Render(GL_TRIANGLES, this->GetIndexCount());
}

const void* Surface::GetVertices() const
{
	return positions.const_pointer();
}
void* Surface::GetVertices()
{
	return positions.pointer();
}

void* Surface::GetIndices()
{
    	return indices.pointer();

}
const void* Surface::GetIndices() const
{
		return indices.const_pointer();
}


int Surface::AddVertex(const Vec3 &position)
{
    positions.push_back(position);
    flags |= VBO_POSITION;
    texCoords.push_back(Vec2(1.0f));
    normals.push_back(Vec3(1.0f));

    m_boundingBox.AddPoint(position);
    isDirty = true;
    return (int)positions.size() - 1;
}

int Surface::AddVertex(float x, float y, float z)
{
    Vec3 position(x, y, z);
    positions.push_back(position);
    texCoords.push_back(Vec2(1.0f));
    normals.push_back(Vec3(1.0f));
    flags |= VBO_POSITION;
    isDirty = true;
    m_boundingBox.AddPoint(position);
    return (int)positions.size() - 1;
}

int Surface::AddVertex(const Vec3 &position,const Vec2 &texCoord)
{
    positions.push_back(position);
    texCoords.push_back(texCoord);
    m_boundingBox.AddPoint(position);
    flags |= VBO_POSITION | VBO_TEXCOORD0;
     isDirty = true;
    return (int)positions.size() - 1;
}

int Surface::AddVertex(float x, float y, float z, float u, float v)
{
    Vec3 position(x, y, z);
    positions.push_back(position);
    m_boundingBox.AddPoint(position);
    texCoords.push_back(Vec2(u, v));
    flags |= VBO_POSITION | VBO_TEXCOORD0;
     isDirty = true;
    return (int)positions.size() - 1;
}

int Surface::AddVertex(const Vec3 &position,const Vec2 &texCoord,const Vec3 &normal)
{

    positions.push_back(position);
    m_boundingBox.AddPoint(position);
    texCoords.push_back(texCoord);
    normals.push_back(normal);
    flags |= VBO_POSITION | VBO_TEXCOORD0 | VBO_NORMAL;
     isDirty = true;
    return (int)positions.size() - 1;
}

int Surface::AddVertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
{
     Vec3 position(x, y, z);
    positions.push_back(position);
    m_boundingBox.AddPoint(position);

    texCoords.push_back(Vec2(u, v));
    normals.push_back(Vec3(nx, ny, nz));
    flags |= VBO_POSITION | VBO_TEXCOORD0 | VBO_NORMAL;
     isDirty = true;
    return (int)positions.size() - 1;
}



void Surface::VertexPosition(u32 index, const Vec3 &position)
{
    m_boundingBox.AddPoint(position);
    positions[index] = position;
    flags |= VBO_POSITION;
     isDirty = true;
}

void Surface::VertexPosition(u32 index, float x, float y, float z)
{
    positions[index] = Vec3(x, y, z);
     m_boundingBox.AddPoint(positions[index]);
    flags |= VBO_POSITION;
     isDirty = true;
}

void Surface::VertexNormal(u32 index, const Vec3 &normal)
{
    normals[index] = normal;
    flags |= VBO_NORMAL;
     isDirty = true;
}

void Surface::VertexNormal(u32 index, float x, float y, float z)
{
    normals[index] = Vec3(x, y, z);
    flags |= VBO_NORMAL;
     isDirty = true;
}

void Surface::VertexTexCoord(u32 index, const Vec2 &texCoord)
{
    texCoords[index] = texCoord;
    flags |= VBO_TEXCOORD0;
     isDirty = true;
}

void Surface::VertexTexCoord(u32 index, float u, float v)
{
    texCoords[index] = Vec2(u, v);
    flags |= VBO_TEXCOORD0;
     isDirty = true;
}

int Surface::AddFace(u32 v0, u32 v1, u32 v2)
{
    indices.push_back(v0);
    indices.push_back(v1);
    indices.push_back(v2);
    flags |= VBO_INDICES;
     isDirty = true;
    return (int)indices.size() - 3;
}



void Surface::CalculateNormals()
{

    if (indices.size() > 0) 
    {

        if (normals.size() != positions.size()) 
        {
            for (size_t i = 0; i < positions.size(); ++i) 
            {
                normals.push_back(Vec3(0.0f, 0.0f, 0.0f));
            }
        }
        for (u32 i = 0; i < GetIndexCount(); i += 3)
        {
            Plane3D plane = Plane3D(positions[indices[i]], positions[indices[i + 1]], positions[indices[i + 2]]);
          
            Vec3 normal = plane.normal;
            normals[indices[i]] = normal;
            normals[indices[i + 1]] = normal;
            normals[indices[i + 2]] = normal;

        }


         flags |= VBO_NORMAL;
         isDirty = true;
    }

       
}

void Surface::Clear()
{
    positions.clear();
    normals.clear();
    texCoords.clear();
    indices.clear();
    colors.clear();
    m_boundingBox.Clear();
}

void Surface::CalculateSmothNormals(bool angleWeighted)
{

    if (normals.size() != positions.size()) 
    {
        for (size_t i = 0; i < positions.size(); ++i) 
        {
            normals.push_back(Vec3(0.0f, 0.0f, 0.0f));
        }
    }

    if (indices.size() > 0) 
    {
        if (normals.size() != positions.size()) 
        {
            for (size_t i = 0; i < positions.size(); ++i) 
            {
                normals.push_back(Vec3(0.0f, 0.0f, 0.0f));
            }
        }
        for (size_t i = 0; i < indices.size(); i += 3) 
        {
            Vec3 v0 = positions[indices[i + 0]];
            Vec3 v1 = positions[indices[i + 1]];
            Vec3 v2 = positions[indices[i + 2]];

            Vec3 normal = Vec3::Normalize(Vec3::Cross(v1 - v0, v2 - v0));
         
            Vec3 weight(1.f, 1.f, 1.f);
            if (angleWeighted)
            {
                weight = Vec3::GetAngleWeights(v0, v1, v2);
            }



            normals[indices[i + 0]] += weight.x*normal.x;
            normals[indices[i + 1]] += weight.y*normal.y;
            normals[indices[i + 2]] += weight.z*normal.z;
        }

        for (size_t i = 0; i < normals.size(); ++i) 
        {
            normals[i] = Vec3::Normalize(normals[i]);
        }
          flags |= VBO_NORMAL;
           isDirty = true;
    }
  
}

void Surface::CalculateBoundingBox()
{
    m_boundingBox.Clear();
    for (size_t i = 0; i < positions.size(); ++i) 
    {
        m_boundingBox.AddPoint(positions[i]);
    }
}

 void Surface::VertexUpload(MeshBuffer *buffer ,const VertexFormat& vertexFormat)
 {
     Array<float> vertexData;

   
     u32 count = vertexFormat.getElementCount();
     

    for (u32 i = 0; i <(u32) this->GetVertexCount(); ++i)
    {
        vertexData.push_back(this->positions[i].x);
        vertexData.push_back(this->positions[i].y);
        vertexData.push_back(this->positions[i].z);
   
        for (u32 j = 0; j < count; ++j)
        {
            const VertexFormat::Element& e = vertexFormat.getElement(j);           
           
            if (e.usage == VertexFormat::TEXCOORD0) 
            {
                vertexData.push_back(this->texCoords[i].x);
                vertexData.push_back(this->texCoords[i].y);
            }
            else if (e.usage == VertexFormat::NORMAL) 
            {
                vertexData.push_back(this->normals[i].x);
                vertexData.push_back(this->normals[i].y);
                vertexData.push_back(this->normals[i].z);
            } else
            if (e.usage == VertexFormat::COLOR)
            {
                if (e.size==3)
                {
                        vertexData.push_back(1.0f);
                        vertexData.push_back(1.0f);
                        vertexData.push_back(1.0f);
                } else
                {
                    vertexData.push_back(1.0f);
                    vertexData.push_back(1.0f);
                    vertexData.push_back(1.0f);
                    vertexData.push_back(1.0f);
                }
            } 
        }
    }
    buffer->SetVertexData(vertexData.const_pointer());

 }

//***********************************************************************************************************