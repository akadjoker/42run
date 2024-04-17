#pragma once


#include "Config.hpp"
#include "Std.hpp"
#include "Utils.hpp"


class CORE_PUBLIC Shader 
{
    public:
           Shader();
            virtual ~Shader();
        

            bool Create(const char* vShaderCode, const char* fShaderCode);
            bool Create(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode);
            bool Load(const char* vShaderCode, const char* fShaderCode);
            bool Load(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode);
            bool operator ==(const Shader&      other) const { return m_program == other.m_program; }
            bool operator !=(const Shader&      other) const { return m_program != other.m_program; }


   

        u32 GetID()    {        return m_program;    }

        void Use() const;



   

        void SetInt(const String &name, int value) ;
 
        void SetMatrix4(const String &name, const float *value) ;
        void SetMatrix3(const String &name, const float *value) ;

        void SetFloat(const String& name, float v);
        void SetFloat(const String& name, float x, float y);
        void SetFloat(const String& name, float x, float y, float z);
        void SetFloat(const String& name, float x, float y, float z,float w);



        void Release();

        


    void print();

    bool findUniform(const String &name)const;

    int  getUniform(const String &name);
    int  getAttribute(const String &name);


    int  getUniformLocation(const String &uniformName) const;
    int  getAttribLocation( const String &attribName) const;
    
    bool addUniform(const char *name);
    bool addAttribute(const char* name);

    bool ContainsUniform(const String &name) const;

    void LoadDefaults();

private:
        u32 m_program;
        int m_numAttributes;
        int m_numUniforms;
        bool m_panic;


    private:
         void checkCompileErrors(unsigned int shader, const String &type);
 
           Shader& operator =(const Shader& other) = delete;
           Shader(const Shader& other) = delete;

           HashTable<int> m_uniforms;
           HashTable<int> m_attributes;
          
};