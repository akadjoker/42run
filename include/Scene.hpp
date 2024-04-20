#pragma once
#include "Config.hpp"
#include "Math.hpp"
#include "Std.hpp"
#include "GameObject.hpp"

class Scene : public Ref
{
    
public:
    Scene();
    ~Scene();


    void Release();

    void AddNode(SceneNode *node);
    void RemoveNode(SceneNode *node);

    SceneNode* CreateSceneNode(const String& name, SceneNode* parent = 0);
    MeshNode *CreateMeshNode(Mesh *mesh, SceneNode *parent = 0);

    SceneNode *GetRoot() { return m_root; }

    void Update(u32 timeMs);
    void Render(Shader* shader);

private:
    SceneNode *m_root;

    Array<SceneNode*> m_nodes;
    Array<SceneNode*> m_toDelete;
};