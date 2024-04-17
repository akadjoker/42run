#include "pch.h"
#include "GameObject.hpp"

GameObject::GameObject():Ref()
{
 components.reserve(16);
} 


GameObject::~GameObject()
{
    LogInfo( "DESTROYED: GameObject");

    for (u32 i = 0; i < components.size(); i++)
    {
        components[i]->Unload();
        delete components[i];
    }
    componentTypeMap.clear();
}
void GameObject::OnUpdate(u32 timeMs)
{

    for (u32 i = 0; i < components.size(); i++)
    {
        components[i]->Update(timeMs);
    }
}

void GameObject::OnRender(Shader *shader)
{
  
    for (u32 i = 0; i < components.size(); i++)
    {
        components[i]->Render(shader);
    }
}
  


  //******************************************************************************
  // Component
  //******************************************************************************
  Component::Component()
  {
    owner = NULL;
    priority = 0;
  }


  MeshRenderer::MeshRenderer():Component()

  {
      LogInfo( "CREATED: MeshRenderer");

  }  

  MeshRenderer::~MeshRenderer()
  {
    LogInfo( "DESTROYED: MeshRenderer");
  }
  void MeshRenderer::Render(Shader *shader)
  {
    (void)  shader; 
    LogInfo( "RENDER: MeshRenderer");
  }


  void MeshRenderer::Update(float deltaTime)
  {
    (void)  deltaTime;
  }


  void MeshRenderer::Load()
  {
    LogInfo( "LOAD: MeshRenderer");
  }


 