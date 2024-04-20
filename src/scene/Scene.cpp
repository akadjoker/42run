#include "pch.h"
#include "Scene.hpp"

Scene::Scene()
{
    m_root = new SceneNode();
}

Scene::~Scene()
{
    Release();
}

void Scene::Release()
{
    if (m_root)
    {
        m_root->Drop();
    }
    for (u32 i = 0; i < m_nodes.size(); i++)
    {
        m_nodes[i]->Drop();
    }
}

void Scene::AddNode(SceneNode *node)
{
   m_nodes.push_back(node);
}

void Scene::RemoveNode(SceneNode *node)
{
    for (u32 i = 0; i < m_nodes.size(); i++)
    {
        if (m_nodes[i] == node)
        {
            m_nodes.erase(i);
            break;
        }
    }
}

SceneNode *Scene::CreateSceneNode(const String &name, SceneNode *parent)
{
  
    bool add = false;
    if (!parent)
    {
        parent = m_root;
        add=true;
    }


    SceneNode *node = new SceneNode(name, parent);

    if (add)
        AddNode(node);
    else 
        parent->AddChild(node);

    return node;
}

MeshNode *Scene::CreateMeshNode(Mesh *mesh, SceneNode *parent)
{
    bool add = false;
    if (!parent)
    {
        parent = m_root;
        add=true;
    }

    MeshNode *node = new MeshNode(mesh, parent);

    if (add)
        AddNode(node);
    else 
        parent->AddChild(node);


    return node;
    
}

void Scene::Update(u32 timeMs)
{
    m_root->Update(timeMs);
    for (u32 i = 0; i < m_nodes.size(); i++)
    {
       m_nodes[i]->Update(timeMs);
    }
}

void Scene::Render(Shader *shader)
{
    for (u32 i = 0; i < m_nodes.size(); i++)
    {
        m_nodes[i]->Render(shader);
    }
}
