#include "pch.h"


MeshBuilder* MeshBuilder::instance = 0x0; 

MeshBuilder::MeshBuilder()
{
    if (instance != 0x0) 
    {
        LogError("[MESHBUILDER] already created");
        return;
    }

    instance = this;


    m_texturePath = "assets/textures/";
    m_loadPath = "assets/meshes/";


    
}

MeshBuilder* MeshBuilder::InstancePtr()
{
    return instance;
}

MeshBuilder& MeshBuilder::Instance()
{
    DEBUG_BREAK_IF(!instance);
    return *instance;
}

void MeshBuilder::SetLoadPath(const String &path)
{
    m_loadPath = path;
}
String MeshBuilder::GetLoadPath() const
{
    return m_loadPath;
}

MeshBuilder::~MeshBuilder()
{
    for (u32 i = 0; i < m_meshes.size(); ++i)
    {
        m_meshes[i]->Drop();
    }

    instance = nullptr;
    LogInfo("[MESHBUILDER] destroyed");
}


Mesh* MeshBuilder::CreateCube(const VertexFormat& vertexFormat,float width, float height, float length)
{
    Mesh *mesh = new Mesh(vertexFormat);
    m_meshes.push_back(mesh);

   
     width /= 2;
	 height /= 2;
	 length /= 2;

    
    Surface *surf = mesh->AddSurface(0);
    surf->AddVertex(-width,-height,-length);
	surf->AddVertex(-width, height,-length);
	surf->AddVertex(width, height,-length);
	surf->AddVertex(width,-height,-length);

	surf->AddVertex(-width,-height, length);
	surf->AddVertex(-width, height, length);
	surf->AddVertex(width, height, length);
	surf->AddVertex(width,-height, length);

	surf->AddVertex(-width,-height, length);
	surf->AddVertex(-width, height, length);
	surf->AddVertex(width, height, length);
	surf->AddVertex(width,-height, length);

	surf->AddVertex(-width,-height,-length);
	surf->AddVertex(-width, height,-length);
	surf->AddVertex(width, height,-length);
	surf->AddVertex(width,-height,-length);

	surf->AddVertex(-width,-height, length);
	surf->AddVertex(-width, height, length);
	surf->AddVertex(width, height, length);
	surf->AddVertex(width,-height, length);

	surf->AddVertex(-width,-height,-length);
	surf->AddVertex(-width, height,-length);
	surf->AddVertex(width, height,-length);
	surf->AddVertex(width,-height,-length);

	surf->VertexNormal(0,0.0,0.0,-1.0);
	surf->VertexNormal(1,0.0,0.0,-1.0);
	surf->VertexNormal(2,0.0,0.0,-1.0);
	surf->VertexNormal(3,0.0,0.0,-1.0);

	surf->VertexNormal(4,0.0,0.0,1.0);
	surf->VertexNormal(5,0.0,0.0,1.0);
	surf->VertexNormal(6,0.0,0.0,1.0);
	surf->VertexNormal(7,0.0,0.0,1.0);

	surf->VertexNormal(8,0.0,-1.0,0.0);
	surf->VertexNormal(9,0.0,1.0,0.0);
	surf->VertexNormal(10,0.0,1.0,0.0);
	surf->VertexNormal(11,0.0,-1.0,0.0);

	surf->VertexNormal(12,0.0,-1.0,0.0);
	surf->VertexNormal(13,0.0,1.0,0.0);
	surf->VertexNormal(14,0.0,1.0,0.0);
	surf->VertexNormal(15,0.0,-1.0,0.0);

	surf->VertexNormal(16,-1.0,0.0,0.0);
	surf->VertexNormal(17,-1.0,0.0,0.0);
	surf->VertexNormal(18,1.0,0.0,0.0);
	surf->VertexNormal(19,1.0,0.0,0.0);

	surf->VertexNormal(20,-1.0,0.0,0.0);
	surf->VertexNormal(21,-1.0,0.0,0.0);
	surf->VertexNormal(22,1.0,0.0,0.0);
	surf->VertexNormal(23,1.0,0.0,0.0);

	surf->VertexTexCoord(0,0.0,1.0);
	surf->VertexTexCoord(1,0.0,0.0);
	surf->VertexTexCoord(2,1.0,0.0);
	surf->VertexTexCoord(3,1.0,1.0);

	surf->VertexTexCoord(4,1.0,1.0);
	surf->VertexTexCoord(5,1.0,0.0);
	surf->VertexTexCoord(6,0.0,0.0);
	surf->VertexTexCoord(7,0.0,1.0);

	surf->VertexTexCoord(8,0.0,1.0);
	surf->VertexTexCoord(9,0.0,0.0);
	surf->VertexTexCoord(10,1.0,0.0);
	surf->VertexTexCoord(11,1.0,1.0);

	surf->VertexTexCoord(12,0.0,0.0);
	surf->VertexTexCoord(13,0.0,1.0);
	surf->VertexTexCoord(14,1.0,1.0);
	surf->VertexTexCoord(15,1.0,0.0);

	surf->VertexTexCoord(16,0.0,1.0);
	surf->VertexTexCoord(17,0.0,0.0);
	surf->VertexTexCoord(18,1.0,0.0);
	surf->VertexTexCoord(19,1.0,1.0);

	surf->VertexTexCoord(20,1.0,1.0);
	surf->VertexTexCoord(21,1.0,0.0);
	surf->VertexTexCoord(22,0.0,0.0);
	surf->VertexTexCoord(23,0.0,1.0);

	surf->VertexTexCoord(0,0.0,1.0);
	surf->VertexTexCoord(1,0.0,0.0);
	surf->VertexTexCoord(2,1.0,0.0);
	surf->VertexTexCoord(3,1.0,1.0);

	surf->VertexTexCoord(4,1.0,1.0);
	surf->VertexTexCoord(5,1.0,0.0);
	surf->VertexTexCoord(6,0.0,0.0);
	surf->VertexTexCoord(7,0.0,1.0);

	surf->VertexTexCoord(8,0.0,1.0);
	surf->VertexTexCoord(9,0.0,0.0);
	surf->VertexTexCoord(10,1.0,0.0);
	surf->VertexTexCoord(11,1.0,1.0);

	surf->VertexTexCoord(12,0.0,0.0);
	surf->VertexTexCoord(13,0.0,1.0);
	surf->VertexTexCoord(14,1.0,1.0);
	surf->VertexTexCoord(15,1.0,0.0);

	surf->VertexTexCoord(16,0.0,1.0);
	surf->VertexTexCoord(17,0.0,0.0);
	surf->VertexTexCoord(18,1.0,0.0);
	surf->VertexTexCoord(19,1.0,1.0);

	surf->VertexTexCoord(20,1.0,1.0);
	surf->VertexTexCoord(21,1.0,0.0);
	surf->VertexTexCoord(22,0.0,0.0);
	surf->VertexTexCoord(23,0.0,1.0);

	surf->AddFace(0,1,2); // front
	surf->AddFace(0,2,3);
	surf->AddFace(6,5,4); // back
	surf->AddFace(7,6,4);
	surf->AddFace(6+8,5+8,1+8); // top
	surf->AddFace(2+8,6+8,1+8);
	surf->AddFace(0+8,4+8,7+8); // bottom
	surf->AddFace(0+8,7+8,3+8);
	surf->AddFace(6+16,2+16,3+16); // right
	surf->AddFace(7+16,6+16,3+16);
	surf->AddFace(0+16,1+16,5+16); // left
	surf->AddFace(0+16,5+16,4+16);

    mesh->m_boundingBox.Merge(surf->GetBoundingBox());
    surf->Upload();
    mesh->AddMaterial(new TextureMaterial(TextureManager::Instance().GetDefault()));



    

  


    return mesh;
}

Mesh * MeshBuilder::CreatePlane(const VertexFormat& vertexFormat,int stacks, int slices, int tilesX, int tilesY)
{

     Mesh *mesh = new Mesh(vertexFormat);
     m_meshes.push_back(mesh);
    
    Surface *surface = mesh->AddSurface(0);
  
   Vec3 center(-9999999.0f, 0.0f, -9999999.0f);


  
  for (int i = 0; i <= stacks; ++i) 
  {
        float y = static_cast<float>(i) / static_cast<float>(stacks) * tilesY;
        for (int j = 0; j <= slices; ++j) 
        {
            float x = static_cast<float>(j) / static_cast<float>(slices) * tilesX;
            
                if (x>center.x) center.x = x;
                if (y>center.z) center.z = y;
                if (x<center.x) center.x = x;
                if (y<center.z) center.z = y;

            surface->AddVertex(x, 0.0f, y, x ,y, 0.0f, 1.0f, 0.0f);
            
        }
    }

    for (u32 i =0; i < surface->GetVertexCount(); ++i)
    {
      Vec3 &v = surface->positions[i];
       v.x -= center.x * 0.5f;
       v.z -= center.z * 0.5f;
    } 
  

    for (int i = 0; i < stacks; ++i) 
    {
        for (int j = 0; j < slices; ++j) 
        {
            u16 index = (slices + 1) * i + j;
            surface->AddFace(index, index + slices + 1, index + slices + 2);
            surface->AddFace(index, index + slices + 2, index + 1);
        }
    }
  
  
    if (surface->normals.size() == surface->positions.size())
    {

        for (u32 i = 0; i < surface->GetIndexCount(); i += 3)
        {
            Plane3D plane = Plane3D(surface->positions[surface->indices[i]], surface->positions[surface->indices[i + 1]], surface->positions[surface->indices[i + 2]]);

           Vec3 normal = plane.normal;
            surface->normals[surface->indices[i]] = normal;
            surface->normals[surface->indices[i + 1]] = normal;
            surface->normals[surface->indices[i + 2]] = normal;

        }
    }
    mesh->m_boundingBox.Merge(surface->GetBoundingBox());
    surface->Upload();
    mesh->AddMaterial(new TextureMaterial(TextureManager::Instance().GetDefault()));
    return mesh;
}

Mesh *MeshBuilder::CreateCylinder(const VertexFormat& vertexFormat,int stacks, int slices)
{

  
     Mesh *mesh = new Mesh(vertexFormat);
     m_meshes.push_back(mesh);
    
    Surface *surface = mesh->AddSurface(0);

   
    const float pi = 3.14159265359f;
    const float stackHeight = 1.0f / static_cast<float>(stacks);
    const float sliceAngle = 2.0f * pi / static_cast<float>(slices);



    // vértices para os lados do cilindro
    for (int i = 0; i <= stacks; ++i) {
        float y = -0.5f + static_cast<float>(i) * stackHeight;
        for (int j = 0; j <= slices; ++j) {
            float x = cos(static_cast<float>(j) * sliceAngle);
            float z = sin(static_cast<float>(j) * sliceAngle);

             surface->AddVertex(x, y, z, static_cast<float>(j) / slices, static_cast<float>(i) / stacks);
        }
    }

    for (int i = 0; i < stacks; ++i) 
    {
        for (int j = 0; j < slices; ++j) 
        {
            int index = (slices + 1) * i + j;
   
                surface->AddFace(index, index + slices + 1, index + slices + 2);
                surface->AddFace(index, index + slices + 2, index + 1);
        }
    }

    

    int topSurfaceStartIndex= surface->GetVertexCount();
    surface->AddVertex(0.0f, 0.5f, 0.0f, 0.5f, 0.5f); // Vértice do topo

    for (int i = 0; i <= slices; ++i) 
    {
        float x = cos(static_cast<float>(i) * sliceAngle);
        float z = sin(static_cast<float>(i) * sliceAngle);
        surface->AddVertex(x, 0.5f, z, 0.5f * (x + 1.0f), 0.5f * (z + 1.0f));
    }

    for (int i = 0; i < slices; ++i) 
    {
        surface->AddFace(topSurfaceStartIndex, topSurfaceStartIndex + (i + 1) % slices + 1, topSurfaceStartIndex + i + 1);
    }


      
       


        int bottomSurfaceStartIndex = surface->GetVertexCount();

        surface->AddVertex(0.0f, -0.5f, 0.0f, 0.5f, 0.5f); // Vértice da base

        for (int i = 0; i <= slices; ++i) {
            float x = cos(static_cast<float>(i) * sliceAngle);
            float z = sin(static_cast<float>(i) * sliceAngle);

            surface->AddVertex(x, -0.5f, z, 0.5f * (x + 1.0f), 0.5f * (z + 1.0f));
        }

        for (int i = 0; i < slices; ++i) 
        {
           surface->AddFace(bottomSurfaceStartIndex, bottomSurfaceStartIndex + i + 1, bottomSurfaceStartIndex + (i + 1) % slices + 1);
        }


        surface->CalculateSmothNormals();
        mesh->m_boundingBox.Merge(surface->GetBoundingBox());
    surface->Upload();
    mesh->AddMaterial(new TextureMaterial(TextureManager::Instance().GetDefault()));


    return mesh;
}

Mesh *MeshBuilder::CreateSphere(const VertexFormat& vertexFormat,int stacks, int slices)
{
     
     Mesh *mesh = new Mesh(vertexFormat);
     m_meshes.push_back(mesh);
    
    Surface *surface = mesh->AddSurface(0);
          const float pi = 3.14159265359f;

    for (int i = 0; i <= stacks; ++i) 
    {
        float phi = static_cast<float>(i) * pi / static_cast<float>(stacks); // Ângulo phi
        for (int j = 0; j <= slices; ++j) 
        {
            float theta = static_cast<float>(j) * 2.0f * pi / static_cast<float>(slices); // Ângulo theta

            
            float x = sin(phi) * cos(theta);
            float y = cos(phi);
            float z = sin(phi) * sin(theta);

            float u = static_cast<float>(j) / static_cast<float>(slices);
            float v = static_cast<float>(i) / static_cast<float>(stacks);

            
             surface->AddVertex(x, y, z, u,v); 
        }
    }

    
    for (int i = 0; i < stacks; ++i) 
    {
        for (int j = 0; j < slices; ++j) 
        {
            int index = (slices + 1) * i + j;
             
              

             surface->AddFace(index, index + 1, index + slices + 2);
            surface->AddFace(index, index + slices + 2, index + slices + 1);


        }
    }
    
    surface->CalculateSmothNormals();
        mesh->m_boundingBox.Merge(surface->GetBoundingBox());
    surface->Upload();
    mesh->AddMaterial(new TextureMaterial(TextureManager::Instance().GetDefault()));

    return mesh;
}



Mesh *MeshBuilder::CreateCone(const VertexFormat& vertexFormat,int stacks, int slices)
{

  
     Mesh *mesh = new Mesh(vertexFormat);
     m_meshes.push_back(mesh);
    
    Surface *surface = mesh->AddSurface(0);
    const float pi = 3.14159265359f;

    const float stackHeight = 1.0f / static_cast<float>(stacks) ;
    const float sliceAngle = 2.0f * pi / static_cast<float>(slices);
    
  
   


    for (int i = 0; i <= stacks; ++i) 
    {
        float y = -0.5f + static_cast<float>(i) * stackHeight;
        float radius = 0.5f - y; // Reduzir o raio à medida que subimos o cone
        for (int j = 0; j <= slices; ++j) 
        {
            float x = radius * cos(static_cast<float>(j) * sliceAngle);
            float z = radius * sin(static_cast<float>(j) * sliceAngle);


            surface->AddVertex(x, y, z, static_cast<float>(j) / slices, static_cast<float>(i) / stacks);
        }
    }
    

        
        for (int i = 0; i < stacks; ++i) 
        {
            for (int j = 0; j < slices; ++j) 
            {
                int index = (slices + 1) * i + j;
                 surface->AddFace(index,
                index + slices + 1,
                index + slices + 2);

                surface->AddFace(index,
                index + slices + 2,
                index + 1);
            }
        }

    //floor 


    


    float yBottom = -0.5f;
    
    int baseStartIndex = surface->GetVertexCount();

     surface->AddVertex(0.0f, yBottom, 0.0f, 0.5f, 0.5f); // Vértice central da base

    for (int i = 0; i <= slices; ++i) 
    {
        float x = cos(static_cast<float>(i) * sliceAngle);
        float z = sin(static_cast<float>(i) * sliceAngle);
          surface->AddVertex(x, yBottom, z, 0.5f * (x + 1.0f), 0.5f * (z + 1.0f));
    }




    for (int i = 0; i < slices; ++i) 
    {
            surface->AddFace(baseStartIndex,
            baseStartIndex + i + 1,
            baseStartIndex + (i + 1) % slices + 1);
    }
     
    surface->CalculateSmothNormals();
            mesh->m_boundingBox.Merge(surface->GetBoundingBox());
    surface->Upload();
    mesh->AddMaterial(new TextureMaterial(TextureManager::Instance().GetDefault()));
 
    return mesh;
}



Mesh *MeshBuilder::CreateTorus(const VertexFormat& vertexFormat,int stacks, int slices,float innerRadius, float outerRadius)
{

   
     Mesh *mesh = new Mesh(vertexFormat);
     m_meshes.push_back(mesh);
    
    Surface *surface = mesh->AddSurface(0);

    const float pi = 3.14159265359f;
    const float stackAngle = 2.0f * pi / static_cast<float>(stacks);
    const float sliceAngle = 2.0f * pi / static_cast<float>(slices);

    for (int i = 0; i <= stacks; ++i) 
    {
        float u = static_cast<float>(i) * stackAngle;

        for (int j = 0; j <= slices; ++j) 
        {
            float v = static_cast<float>(j) * sliceAngle;

            float x = (outerRadius + innerRadius * cos(v)) * cos(u);
            float y = (outerRadius + innerRadius * cos(v)) * sin(u);
            float z = innerRadius * sin(v);

            float textureU = static_cast<float>(i) / stacks;
            float textureV = static_cast<float>(j) / slices;

            surface->AddVertex(x, y, z, textureU, textureV);
        }
    }

    for (int i = 0; i < stacks; ++i) 
    {
        for (int j = 0; j < slices; ++j) 
        {
            int index = (slices + 1) * i + j;

            surface->AddFace(index,
            index + slices + 1,
            index + slices + 2);

            surface->AddFace(index,
            index + slices + 2,
            index + 1);
        }
   
    }
     
    surface->CalculateSmothNormals(true);
    mesh->m_boundingBox.Merge(surface->GetBoundingBox());
    surface->Upload();

    mesh->AddMaterial(new TextureMaterial(TextureManager::Instance().GetDefault()));
  
    return mesh;
}

Mesh* MeshBuilder::CreateHillsPlane(const VertexFormat& vertexFormat,
    const FloatSize& tileSize,
    const IntSize& tc,
    float hillHeight,
    const FloatSize& ch,
    const FloatSize& textureRepeatCount)
{
    Mesh* mesh = new Mesh(vertexFormat);
    m_meshes.push_back(mesh);

    Surface* surface = mesh->AddSurface(0);

    IntSize          tileCount = tc;
    FloatSize       countHills = ch;




    if (countHills.width < 0.01f)
        countHills.width = 1.f;
    if (countHills.height < 0.01f)
        countHills.height = 1.f;

    // center
    const Vec2 center((tileSize.width * tileCount.width) * 0.5f, (tileSize.height * tileCount.height) * 0.5f);


    const FloatSize tx(
        textureRepeatCount.width / tileCount.width,
        textureRepeatCount.height / tileCount.height);

    ++tileCount.height;
    ++tileCount.width;

    u32 x;

    f32 sx = 0.f, tsx = 0.f;
    for (x = 0; x <(u32) tileCount.width; ++x)
    {
        f32 sy = 0.f, tsy = 0.f;
        for (u32 y = 0; y <(u32) tileCount.height; ++y)
        {
            float vx = sx - center.x;
            float vz = sy - center.y;
            float vy = hillHeight * sinf(vx * countHills.width * 2.0f * 3.14159f / center.x) *
                cosf(vz * countHills.height * 2.0f * 3.14159f / center.y);

            //   hillHeight * (sin((x / ch.x) * countHills.x * 2.0f * 3.14159f) * cos((z / ch.y) * countHills.y * 2.0f * 3.14159f));
            surface->AddVertex(vx, vy, vz, tsx, 1.0f - tsy, 0.0f, 1.0f, 0.0f);
            sy += tileSize.height;
            tsy += tx.height;
        }
        sx += tileSize.width;
        tsx += tx.width;
    }

    for (x = 0; x < (u32)tileCount.width - 1; ++x)
    {
        for (u32 y = 0; y < (u32)tileCount.height - 1; ++y)
        {
            const s32 index = x * tileCount.height + y;
            surface->AddFace(index, index + 1, index + tileCount.height);
            surface->AddFace(index + 1, index + 1 + tileCount.height, index + tileCount.height);
        }
    }

    if (surface->normals.size() == surface->positions.size())
    {
    
        for (u32 i = 0; i < surface->GetIndexCount(); i += 3)
        {
            Plane3D plane = Plane3D(surface->positions[surface->indices[i]], surface->positions[surface->indices[i + 1]], surface->positions[surface->indices[i + 2]]);
          
            Vec3 normal = plane.normal;
            surface->normals[surface->indices[i]] = normal;
            surface->normals[surface->indices[i + 1]] = normal;
            surface->normals[surface->indices[i + 2]] = normal;

        }
    }

  
  
    return mesh;
}


Mesh *MeshBuilder::CreateTerrain(const VertexFormat& vertexFormat, Pixmap &heightmap,
 const Pixmap &colors,const FloatSize& stretchSize, f32 maxHeight, const IntSize &maxVtxBlockSize)

{
   Mesh *mesh = new Mesh(vertexFormat);
   m_meshes.push_back(mesh);

   	const s32 borderSkip =1;
    
   

	const double tm =static_cast<double>( SDL_GetTicks()/1000.0 );
    const double    startTime = (double)tm * 100;
	const IntSize hMapSize= IntSize(heightmap.width, heightmap.height);
	const IntSize tMapSize= IntSize(colors.width, colors.height);

	const Vec2 thRel(static_cast<f32>(tMapSize.width) / hMapSize.width, static_cast<f32>(tMapSize.height) / hMapSize.height);

    LogInfo( "CreateTerrain: %d x %d", tMapSize.width, tMapSize.height);
    LogInfo( "CreateTerrain: %d x %d", hMapSize.width, hMapSize.height);
    LogInfo( "CreateTerrain: %f x %f", thRel.x,thRel.y);
    
    

   // return CreateCube(vertexFormat, 1, 1, 1);


//	maxHeight /= 255.0f; // height step per color value

	u32 processedX = 0;
    u32 processedY = 0;

    int index =0;
    while (processedY<hMapSize.height)
	{
		while(processedX<hMapSize.width)
		{
			IntSize blockSize = maxVtxBlockSize;
	
			if (processedX + blockSize.width > hMapSize.width)				
                blockSize.width = hMapSize.width - processedX;
			
            if (processedY + blockSize.height > hMapSize.height)				
                blockSize.height = hMapSize.height - processedY;

		    Surface * surface = mesh->AddSurface(index);
            index++;

			int y;
			
            Vec2 pos(0.f, processedY*stretchSize.height);

			const Vec2 bs(1.f/blockSize.width, 1.f/blockSize.height);
			
            Vec2 tc(0.f, 0.5f*bs.y);
			
            for (y=0; y<blockSize.height; ++y)
			{
				pos.x=processedX*stretchSize.width;
				tc.x=0.5f*bs.x;
				for (int x=0; x<blockSize.width; ++x)
				{
                    Color color = heightmap.GetPixelColor(x+processedX, y+processedY);
					const f32 height = (color.r/255.0f) * maxHeight;

                    surface->AddVertex(pos.x, height, pos.y, tc.x,tc.y, 0.0f, 1.0f, 0.0f);

					pos.x += stretchSize.width;
					tc.x += bs.x;
				}
				pos.y += stretchSize.height;
				tc.y += bs.y;
			}
          // mesh->m_boundingBox.Add(surface->m_boundingBox);

	
			s32 c1 = 0;
			for (y=0; y< blockSize.height-1; ++y)
			{
				for (u32 x=0; x<(u32)blockSize.width-1; ++x)
				{
					const s32 c = c1 + x;

                    surface->AddFace(c,	c + blockSize.width,c + 1);

					surface->AddFace(c + 1,c + blockSize.width,c + 1 + blockSize.width);
				}
				c1 += blockSize.width;
			}

			// recalculate normals
		  	surface->CalculateNormals();

        //  for (u32 i = 0; i < surface->GetIndexCount(); i += 3)
        // {
        //     Plane3D plane = Plane3D(surface->positions[surface->indices[i]], surface->positions[surface->indices[i + 1]], surface->positions[surface->indices[i + 2]]);
          
        //     Vec3 normal = plane.normal;
        //     surface->normals[surface->indices[i]] = normal;
        //     surface->normals[surface->indices[i + 1]] = normal;
        //     surface->normals[surface->indices[i + 2]] = normal;

        // }

			if (surface->GetVertexCount() > 0)
            {
          
				c8 textureName[64];
       
                IntRect clip = IntRect(
                    (int)floor(processedX*thRel.x), 
                    (int)floor(processedY*thRel.y),
                    (int)floor(blockSize.width*thRel.x), 
                    (int)floor(blockSize.height*thRel.y));

                    sprintf(textureName, "terrain_%f_%u",startTime, mesh->GetSurfaceCount());
                    
                    Pixmap paint(colors,clip);
            


                    Texture2D *texture =  TextureManager::Instance().Create(paint);

                    mesh->AddMaterial(new TextureMaterial( texture));

                    TextureManager::Instance().Add(texture, textureName);

               


		
				

         
				 LogInfo ("Generated terrain texture (%dx%d): %s",
				  		clip.width,				 		clip.height,				 		textureName);


		
			}
            surface->Upload();

        //    if (index>5) break;

	
			// keep on processing
			processedX += maxVtxBlockSize.width - borderSkip;
		}

		// keep on processing
		processedX = 0;
		processedY += maxVtxBlockSize.height - borderSkip;
	}

    const double tend = static_cast<double>( SDL_GetTicks()/1000.0 );
    double timeTotal = (tend-tm);

    LogInfo("Terrain  (%d) created in %f ms",mesh->GetSurfaceCount(),timeTotal);
 

    return mesh;
}

