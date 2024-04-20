#pragma once
#include "Config.hpp"
#include "Math.hpp"
#include "Std.hpp"
#include "Shader.hpp"

class Component;
class Scene;

class CORE_PUBLIC GameObject : public Ref
{
public:
    GameObject();
    ~GameObject();

    virtual void OnUpdate(u32 timeMs);
    virtual void OnRender(Shader *shader);

    template <typename T, typename... TArgs>
    T &AddComponent(TArgs &&...args)
    {
        T *cmp(new T(std::forward<TArgs>(args)...));
        cmp->owner = this;
        components.emplace_back(cmp);
        const std::type_info &type = typeid(T);
        cmp->hash = type.hash_code();
        componentTypeMap.insert(type.name(), cmp);
        cmp->Load();
        return *cmp;
    }
    template <typename T>
    bool HasComponent() const
    {
        const std::type_info &type = typeid(T);
        return componentTypeMap.contains(type.name());
    }

    template <typename T>
    T *GetComponent()
    {
        const std::type_info &type = typeid(T);
        auto it = componentTypeMap.find(type.name());
        if (it != componentTypeMap.end())
        {
            return static_cast<T *>(*it);
        }
        DEBUG_BREAK_IF(true);
        return 0x0;
    }

    template <typename T>
    bool RemoveComponent()
    {
        const std::type_info &type = typeid(T);
        auto it = componentTypeMap.find(type.name());
        if (it != componentTypeMap.end())
        {
            T *value = static_cast<T *>(*it);
            components.remove(value);
            value->Unload();
            delete value;
            it.erase();
            return true;
        }
        DEBUG_BREAK_IF(true);
        return false;
    }

private:
    Vector<Component *> components;
    HashTable<Component *> componentTypeMap;
};

class Component
{
public:
    GameObject *owner;
    Component();
    virtual ~Component() {}
    virtual void Load() {}
    virtual void Update(float deltaTime) { (void)deltaTime; }
    virtual void Render(Shader *shader) { (void)shader; }
    virtual void Unload() {}

protected:
    int priority;
    u32 hash;
    friend class GameObject;
};

class CORE_PUBLIC  MeshRenderer : public Component
{

public:
    MeshRenderer();
    ~MeshRenderer();
    void Load();
    void Update(float deltaTime);
    void Render(Shader *shader);
};

enum NodeType
{
    SCENENODE,
    MESHNODE,
    JOINTNODE
};

class CORE_PUBLIC SceneNode : Ref
{
public:
    SceneNode();
    SceneNode(const String &name);
    SceneNode(const String &name, SceneNode *parent);
    virtual ~SceneNode();
    virtual void Release();

    void SetName(const String &name);
    const String &GetName();

    void SetPosition(const Vec3 &position);
    void SetPosition(float x, float y, float z);
    const Vec3 &GetPosition();

    void SetScale(const Vec3 &scale);
    void SetScale(float x, float y, float z);
    const Vec3 &GetScale();

    void SetRotation(const Vec3 &rotation);
    void SetRotation(float x, float y, float z);
    const Vec3 &GetRotation();

    void SetID(u32 id);
    u32 GetID();

    void AddChild(SceneNode *child);
    void RemoveChild(SceneNode *child);
    SceneNode *GetChild(u32 index);
    SceneNode *GetChild(const String &name);
    u32 GetChildCount();

    void SetParent(SceneNode *parent);
    SceneNode *GetParent();

    Mat4 GetAbsoluteTransformation();
    Mat4 GetRelativeTransformation();

    bool operator==(const SceneNode &n) const;
    bool operator!=(const SceneNode &n) const;

    virtual void Update(u32 timeMs);
    virtual void Render(Shader *shader);

protected:
    friend class Scene;

    SceneNode *m_parent;
    NodeType m_type;
    u32 m_id;
    String m_name;
    Array<SceneNode *> m_children;

    Vec3 m_position;
    Vec3 m_scale;
    Vec3 m_rotation;

    Mat4 m_relTrans, m_absTrans;

    void UpdateAbsolutePosition();

private:
};

class CORE_PUBLIC  JointNode : public SceneNode
{

public:
    JointNode(const String &name, SceneNode *parent = 0);
    ~JointNode();
};

class CORE_PUBLIC  AnimationNode : public SceneNode
{

public:
    AnimationNode(AnimatedMesh* mesh,const String &name, SceneNode *parent = 0);
    ~AnimationNode();

    JointNode *GetJoint(const String &name);
    JointNode *Add(const String &name);

private:
    Array<JointNode *> m_joints;
    AnimatedMesh *m_mesh;
};

class CORE_PUBLIC  MeshNode : public SceneNode
{

public:
    MeshNode(Mesh *mesh, SceneNode *parent = 0);
    ~MeshNode();

    void SetMesh(Mesh *mesh);
    Mesh *GetMesh();

    virtual void Render(Shader *shader) override;
    void Release() override;

private:
    Mesh *m_mesh;
};