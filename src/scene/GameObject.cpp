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

  //******************************************************************************
  // Node
  //******************************************************************************

  static u32 nodeId = 0;

  SceneNode::SceneNode():Ref()
  {
    setDebugName("SceneNode");
    nodeId++;
    m_name = "SceneNode";
    m_type = SCENENODE;
    m_parent = NULL;
    m_id = nodeId;
    m_position.set(0,0,0);
    m_scale.set(1,1,1);
    m_rotation.set(0,0,0);

    m_relTrans = Mat4::Scale( m_scale.x, m_scale.y, m_scale.z );
    m_relTrans.rotate( degToRad( m_rotation.x ), degToRad( m_rotation.y ), degToRad( m_rotation.z ) );
    m_relTrans.translate( m_position.x, m_position.y, m_position.z );

  }

SceneNode::SceneNode(const String& name):SceneNode()
{
    
    m_name = name;

}

SceneNode::SceneNode(const String &name, SceneNode *parent):SceneNode()
{
 
    m_name = name;

    m_parent = parent;

}

SceneNode::~SceneNode()
{
  Release();  
}

void SceneNode::Release()
{

    for (u32 i = 0; i < m_children.size(); i++)
    {
        
        m_children[i]->Drop();
    }

}

void SceneNode::SetName(const String &name)
{
    m_name = name;
}

const String &SceneNode::GetName()
{
    return m_name;
}

void SceneNode::SetPosition(const Vec3 &position)
{
    m_position = position;
}

void SceneNode::SetPosition(float x, float y, float z)
{

    m_position.set(x, y, z);
}

const Vec3 &SceneNode::GetPosition()
{
    return m_position;
}

void SceneNode::SetScale(const Vec3 &scale)
{
    m_scale = scale;
}

void SceneNode::SetScale(float x, float y, float z)
{

    m_scale.set(x, y, z);
}

const Vec3 &SceneNode::GetScale()
{
    return m_scale;
}

void SceneNode::SetRotation(const Vec3 &rotation)
{
    m_rotation = rotation;
}

const Vec3 &SceneNode::GetRotation()
{
    return m_rotation;
}

void SceneNode::SetRotation(float x, float y, float z)
{

    m_rotation.set(x, y, z);
}



void SceneNode::Update(u32 timeMs)
{

    for (u32 i = 0; i < m_children.size(); i++)
    {
        m_children[i]->Update(timeMs);
    }
}

void SceneNode::Render(Shader *shader)
{
    UpdateAbsolutePosition();
    for (u32 i = 0; i < m_children.size(); i++)
    {
        m_children[i]->Render(shader);
    }
}




void SceneNode::AddChild(SceneNode *child)
{
    child->m_parent = this;
    m_children.push_back(child);
}

void SceneNode::RemoveChild(SceneNode *child)
{
    for (u32 i = 0; i < m_children.size(); i++)
    {
        if (m_children[i] == child)
        {
            m_children.erase(i);
            break;
        }
    }
}

SceneNode *SceneNode::GetChild(u32 index)
{
    DEBUG_BREAK_IF(index >= m_children.size());

    if (index < m_children.size())
    {
        return m_children[index];
    }
    return NULL;
}

SceneNode *SceneNode::GetChild(const String &name)
{

    for (u32 i = 0; i < m_children.size(); i++)
    {
        if (m_children[i]->m_name == name)
        {
            return m_children[i];
        }
    }
    return NULL;
}

u32 SceneNode::GetChildCount()
{
    return m_children.size();
}

void SceneNode::SetParent(SceneNode *parent)
{
    m_parent = parent;
}

SceneNode *SceneNode::GetParent()
{
    return m_parent;
}

bool SceneNode::operator == (const SceneNode& n) const
{
   return  (
        (m_id == n.m_id) &&
        (m_name == n.m_name) &&
        (m_type == n.m_type) 
        );

}

bool SceneNode::operator != (const SceneNode& n) const
{
   return  (
        (m_id != n.m_id) ||
        (m_name != n.m_name) ||
        (m_type != n.m_type) 
        );
}



Mat4 SceneNode::GetAbsoluteTransformation()
{
     if (m_parent)
    {
      Mat4::fastMult43(m_absTrans, m_parent->m_absTrans, m_relTrans );
    }
    else
    {
      m_absTrans = GetRelativeTransformation();
    }    
    return m_absTrans;
}

Mat4 SceneNode::GetRelativeTransformation()
{
  return m_relTrans;
}

void SceneNode::UpdateAbsolutePosition()
{
    m_relTrans = Mat4::Scale( m_scale.x, m_scale.y, m_scale.z );
    m_relTrans.rotate( degToRad( m_rotation.x ), degToRad( m_rotation.y ), degToRad( m_rotation.z ) );
    m_relTrans.translate( m_position.x, m_position.y, m_position.z );
    
}


//******************************************************************************
// JOINTNODE
//******************************************************************************


JointNode::JointNode(const String &name, SceneNode *parent):SceneNode(name,parent)
{

}

JointNode::~JointNode()
{
}


//******************************************************************************
// ANIMATIONNODE
//******************************************************************************

AnimationNode::AnimationNode(AnimatedMesh *mesh, const String &name, SceneNode *parent):SceneNode(name,parent),m_mesh(mesh)
{
    m_mesh->Grab();
}

AnimationNode::~AnimationNode()
{
    LogInfo( "DESTROYED: AnimationNode");
    m_mesh->Drop();
    for (u32 i = 0; i < m_joints.size(); i++)
    {
        m_joints[i]->Release();
    }

}

JointNode *AnimationNode::GetJoint(const String &name)
{

    for (u32 i = 0; i < m_joints.size(); i++)
    {
        if (m_joints[i]->GetName() == name)
        {
            return m_joints[i];
        }
    }
    return NULL;
}

JointNode *AnimationNode::Add(const String &name)
{
    JointNode *node = new JointNode(name, this);
    m_joints.push_back(node);
    return node;
}

//******************************************************************************
// MeshNode
//******************************************************************************



MeshNode::MeshNode(Mesh *mesh, SceneNode *parent):SceneNode("MeshNode",parent),m_mesh(mesh)
{
    m_mesh->Grab();
}

MeshNode::~MeshNode()
{
    LogInfo( "DESTROYED: MeshNode");
    Release();
}

void MeshNode::SetMesh(Mesh *mesh)
{
    if (m_mesh)
    {
        m_mesh->Drop();
    }
    m_mesh = mesh;
    m_mesh->Grab();
}

Mesh *MeshNode::GetMesh()
{
    return m_mesh;
}

void MeshNode::Render(Shader *shader)
{
    SceneNode::Render(shader);
    
    const Mat4 &model = GetAbsoluteTransformation();
    shader->SetMatrix4("model", model.x);
    if (m_mesh)
    {
        m_mesh->Render(shader);
    }
}

void MeshNode::Release()
{
    SceneNode::Release();
    if (m_mesh)
    {
        m_mesh->Drop();
    }
}
