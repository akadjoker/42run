
#include "pch.h"
#include "Deph.hpp"
#include "Camera.hpp"


struct Cascade
{
    float splitDepth;
    Mat4 viewProjMatrix;

};

  
 int screenWidth = 1024 ;
 int screenHeight = 768 ;

Camera camera(Vec3(-20.0f, 10.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
float mouseDeltaX = 0.0f;
float mouseDeltaY = 0.0f;
bool firstMouse = true;
bool IsMouseDown = false;

CascadeShadow depthBuffer;
  
const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
const int SHADOW_MAP_CASCADE_COUNT = 4;
Cascade cascades[SHADOW_MAP_CASCADE_COUNT];
#define MAX_TEXT_BUFFER_LENGTH              512 

 const char *TextFormat(const char *text, ...)
{

    #define MAX_TEXTFORMAT_BUFFERS 4        // Maximum number of static buffers for text formatting
    static char buffers[MAX_TEXTFORMAT_BUFFERS][MAX_TEXT_BUFFER_LENGTH] = { 0 };
    static int  index = 0;
    char *currentBuffer = buffers[index];
    memset(currentBuffer, 0, MAX_TEXT_BUFFER_LENGTH);   // Clear buffer before using
    va_list args;
    va_start(args, text);
    vsprintf(currentBuffer, text, args);
    va_end(args);
    index += 1;     // Move to next buffer for next function call
    if (index >= MAX_TEXTFORMAT_BUFFERS) index = 0;

    return currentBuffer;
}


int main()  
{   

//     GameObject obj;

//     obj.AddComponent<MeshRenderer>();
//     obj.AddComponent<MeshRenderer>();
//     obj.AddComponent<MeshRenderer>();
    

//     if (obj.HasComponent<MeshRenderer>())
//     {
//             LogInfo("Has Component");
//     } else 
//     {
//         LogInfo("No Component");    
//     }

//      MeshRenderer *meshRenderer = obj.GetComponent<MeshRenderer>();
//      meshRenderer->Load();

//      obj.RemoveComponent<MeshRenderer>();

//       if (obj.HasComponent<MeshRenderer>())
//     {
//             LogInfo("Has Component");
//     } else 
//     {
//         LogInfo("No Component");    
//     }


//      meshRenderer = obj.GetComponent<MeshRenderer>();


//      HashTable<int> hashTable;

// // Adicione alguns elementos à tabela de hash
// hashTable.insert("key1", 10);
// hashTable.insert("key2", 20); 
// hashTable.insert("key3", 30);

// for (auto it = hashTable.begin(); it != hashTable.end(); ++it) 
// {

//         LogInfo("Key: %s, ", it.key()); 
// }       

// auto it = hashTable.find("key2");
// if (it != hashTable.end())
// {
   
   
//     it.erase();
//    LogInfo("Element removed successfully!");
// } else 
// {
//    LogInfo("Element not found in the hash table.");
// }

// for (auto it = hashTable.begin(); it != hashTable.end(); ++it) 
// {

//         LogInfo("Key: %s, ", it.key()); 
// }

// return 0;

    Modulos::Init();

   

    Device      *device = Device::InstancePtr();


    device->Create(screenWidth, screenHeight, "Hello World");


     

     QuadRender  quadRender;



    RenderBatch batch;
    
    Shader      shader;

    Shader      meshShader;


    Font        font;


    device->Create(screenWidth, screenHeight, "Hello World");

    batch.Init(1,10124);


    font.LoadDefaultFont();
    font.SetBatch(&batch);


{
      const char* vShader = GLSL(
        layout(location = 0) in vec3 position;
        layout(location = 1) in vec2 texCoord;
        layout(location = 2) in vec4 color;



        uniform mat4 mvp;

        out vec2 TexCoord;
        out vec4 vertexColor;
        void main()
        {
            gl_Position = mvp * vec4(position, 1.0);
            TexCoord = texCoord;
            vertexColor = color;
        }
        );


        const char* fShader = GLSL(
            in vec2 TexCoord;
        out vec4 color;
        in vec4 vertexColor;
        uniform sampler2D texture0;
        void main()
        {
            color = texture(texture0, TexCoord) * vertexColor;
        }
        );




       
         Logger::Instance().Info("Creating Batch Shader");
         shader.Create(vShader, fShader);
         shader.LoadDefaults();
}
       



   
{
     const char* vShader = GLSL(

    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoords;




  
    out vec2 TexCoords;



    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;


    void main()
    {
      
        TexCoords = aTexCoords;
      
 

        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }

     );


  const char* fShader = GLSL(

        out vec4 FragColor; 
      
 
        in    vec2 TexCoords;


        uniform sampler2D diffuseTexture;



         



       


        void main()
        {           
              vec4 textureColor = texture(diffuseTexture, TexCoords);
           
              FragColor =     textureColor;

        }    );
 
 
       Logger::Instance().Info("Creating 3D Shader");
      if (!meshShader.Create(vShader, fShader))
      {
          SDL_LogInfo(1, " Cant create: %s", "MeshShader");
          return false;
      }
      meshShader.LoadDefaults();
      meshShader.SetInt("diffuseTexture", 0);


      


}



    


      VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2),

         
    };     

MeshImporterH3D importer;


 


 


    TextureManager::Instance().SetTexturePath("assets/");

    Pixmap colors;
    
    // Pixmap height;
    // colors.Load("assets/terrain-texture.jpg");
    // height.Load("assets/terrain-heightmap.bmp");
    

    Mesh* cube =MeshBuilder::Instance().CreatePlane(VertexFormat(elements, 2), 5, 5, 50, 50);

  //  Mesh * cube = MeshBuilder::Instance().CreateCube(VertexFormat(elements, 2), 1,1,1);
       
    cube->SetMaterial(0, new TextureMaterial(TextureManager::Instance().Load("dirt.png")));
    
    // Mesh* cube =MeshBuilder::Instance().CreateTerrain(VertexFormat(elements, 3),height,colors, FloatSize(0.1f,0.1f),0.0f,IntSize(256,256));
    //MeshBuilder::Instance().CreateCube(VertexFormat(elements, 3), 1,1,1);

  ///   Mesh* cube = MeshBuilder::Instance().CreatePlane(VertexFormat(elements, 2), 10, 10, 10, 10);

   // cube->SetMaterial(0, new TextureMaterial(TextureManager::Instance().Load("container.png")));

 


     
    Driver::Instance().SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    Vec3 lightPosition = Vec3(6.5f, 4.0f,  7.5f);

    Uint32 prevTime = SDL_GetTicks();
    const Uint32 updateTimeStep = 5000;
    bool fisrtUpdate = true;
    bool isMoving = false;

    while (device->Run())
    {
        float deltaTime =  device->GetFrameTime() * 0.5f;

        if (Keyboard::Down(KEY_SPACE))
        {
            
        lightPosition.x = sin((float)device->GetTime()) * 10.0f;
        lightPosition.z = cos((float)device->GetTime()) * 10.0f;
        lightPosition.y=4.0f;

        printf("Light position: %f,  %f\n", lightPosition.x,  lightPosition.z);

        }


        isMoving = false;
         if (Keyboard::Down(KEY_W))
         {
             camera.ProcessKeyboard(FORWARD, deltaTime);
             isMoving = true;
         } 
         if (Keyboard::Down(KEY_S))
         {
             camera.ProcessKeyboard(BACKWARD, deltaTime);
             isMoving = true;
         }
         if (Keyboard::Down(KEY_A))
         {
			 camera.ProcessKeyboard(LEFT, deltaTime);
             isMoving = true;
		 }
    
         if (Keyboard::Down(KEY_D))
         {
             camera.ProcessKeyboard(RIGHT, deltaTime);
             isMoving = true;
         }


                 int xposIn, yposIn;
                 u32 IsMouseDown = SDL_GetMouseState(&xposIn, &yposIn);


                  if ( IsMouseDown & SDL_BUTTON(SDL_BUTTON_LEFT) )
                  {

                         
                       
                          float xpos = static_cast<float>(xposIn);
                          float ypos = static_cast<float>(yposIn);

                          if (firstMouse)
                          {
                              lastX = xpos;
                              lastY = ypos;
                              firstMouse = false;
                          }

                          float xoffset = xpos - lastX;
                          float yoffset = lastY - ypos; 

                          lastX = xpos;
                          lastY = ypos;

                       camera.ProcessMouseMovement(xoffset, yoffset);


                  }
                  else
                  {
                      firstMouse = true;
                  }


        screenWidth = Driver::Instance().GetWidth();
        screenHeight = Driver::Instance().GetHeight();

      

        Driver::Instance().SetViewport(0, 0, screenWidth, screenHeight);
        Mat4 model = Mat4::Identity();
        Mat4 proj = camera.GetProjectionMatrix((float)screenWidth / (float)screenHeight) ;
        Mat4 view = camera.GetViewMatrix();
        Vec3 cameraPosition = camera.position;


        
 
 
   



        Driver::Instance().SetBlend(true);
        Driver::Instance().SetBlendMode(BlendMode::BLEND);
        Driver::Instance().SetDepthTest(true);
        Driver::Instance().SetDepthClamp(true);

        
        
        
        
        Driver::Instance().SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        Driver::Instance().Clear();
    



         meshShader.Use();


  
      

  

    //    meshShader.SetFloat("lightPos", lightPosition.x, lightPosition.y, lightPosition.z);
    //    meshShader.SetFloat("viewPos", cameraPosition.x, cameraPosition.y, cameraPosition.z);
        meshShader.SetMatrix4("model", &model.c[0][0]);
        meshShader.SetMatrix4("view", &view.c[0][0]);
        meshShader.SetMatrix4("projection", &proj.c[0][0]);
        meshShader.SetInt("diffuseTexture", 0);
    



        


        model = Mat4::Identity();
        meshShader.SetMatrix4("model", &model.c[0][0]);
        cube->Render(&meshShader); 
       


 
//batch
   
        shader.Use();
        Mat4 mvp;

        Mat4 transfom=Mat4::Identity() * proj * view ; 
        shader.SetMatrix4("mvp", &transfom.c[0][0]);


        batch.Grid(10,10);
        batch.Render();



        mvp = Mat4::Ortho(0.0f, screenWidth, screenHeight,0.0f, -5.0f, 5.0f);
        shader.Use();
        shader.SetMatrix4("mvp", &mvp.c[0][0]);


   
//2d
        Driver::Instance().SetBlend(true);
        Driver::Instance().SetBlendMode(BlendMode::BLEND);
        Driver::Instance().SetDepthTest(false);


        batch.SetColor(255, 255, 255, 255);
        font.Print(10, 10, "Hello World");
        batch.Render();






        device->Flip();
    }


    cube->Drop();


    font.Release();
    batch.Release();
    shader.Release();
   /// device.Close();
  

    Modulos::Release();
    
 

    return 0;
}
