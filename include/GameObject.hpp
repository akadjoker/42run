#pragma once
#include "Config.hpp"
#include "Math.hpp"
#include "Std.hpp"
#include "Shader.hpp"

class Component;


class  CORE_PUBLIC   GameObject : public Ref
{
    public:
    GameObject();
    ~GameObject();


    virtual void OnUpdate(u32 timeMs);
    virtual void OnRender(Shader* shader);


    template <typename T, typename... TArgs>
    T& AddComponent(TArgs&&... args)
    {
        T* cmp(new T(std::forward<TArgs>(args)...));
        cmp->owner = this;
        components.emplace_back(cmp);           
        const std::type_info& type = typeid(T);
        cmp->hash = type.hash_code();
        componentTypeMap.insert(type.name(), cmp);
        cmp->Load();
        return *cmp;
    }
        template <typename T>
        bool HasComponent() const
        {
            const std::type_info& type = typeid(T);
            return componentTypeMap.contains(type.name());
        }

        template <typename T>
        T* GetComponent() 
        {
            const std::type_info& type = typeid(T);
            auto it = componentTypeMap.find(type.name());
            if (it != componentTypeMap.end())
            {
                return static_cast<T*>(*it);
            }
            DEBUG_BREAK_IF(true);
            return 0x0;
        }

        template <typename T>   
        bool RemoveComponent()
        {
            const std::type_info& type = typeid(T);       
            auto it = componentTypeMap.find(type.name());
            if (it != componentTypeMap.end())
            {
                T* value = static_cast<T*>(*it);
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
      Vector<Component*> components;
      HashTable<Component*> componentTypeMap;

};




class Component
 {
    public:

        GameObject* owner;
        Component() ;
        virtual ~Component() {}
        virtual void Load() {}
        virtual void Update(float deltaTime) {(void)deltaTime;}
        virtual void Render(Shader *shader) {(void)shader;}
        virtual void Unload() {}
    protected:
        int priority;
        u32  hash;
        friend class GameObject;
};

class MeshRenderer : public Component
{
    
    public:
    MeshRenderer();
    ~MeshRenderer();
    void Load();
    void Update(float deltaTime);
    void Render(Shader *shader);
};