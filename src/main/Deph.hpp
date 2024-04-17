#pragma once



struct StencilBuffer
{
        unsigned int framebuffer;
        unsigned int textureColorbuffer;
        unsigned int rbo;
        unsigned int width;
        unsigned int height;

        bool Init(unsigned int width, unsigned int height)
        {
            this->width = width;
            this->height = height;
            glGenFramebuffers(1, &framebuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

              // create a color attachment texture
            glGenTextures(1, &textureColorbuffer);
            glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

              // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                LogWarning("Framebuffer is not complete!");
                return false;
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            return true;
        }

        void Begin()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
           // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
          //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        }

        void End()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void BindTexture()
        {
            glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        }

        void Release()
        {
            glDeleteFramebuffers(1, &framebuffer);
            glDeleteTextures(1, &textureColorbuffer);
            glDeleteRenderbuffers(1, &rbo);
        }

    
};
class TextureCascade
{

    private:
    u32 *m_textures;
    u32 m_width;
    u32 m_height;
    u32 m_count;
    public:
    void Init(u32 count,u32 width, u32 height)
    {
        m_width = width;
        m_height = height;
        m_count = count;
        m_textures = new u32[count];
        glGenTextures(count, m_textures);

        for (int i = 0; i < count; i++)
        {
            glBindTexture(GL_TEXTURE_2D, m_textures[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }
    }

    void Bind(u32 start)
    {
        for (int i = 0; i < m_count; i++)
        {
            glActiveTexture(GL_TEXTURE0 + start + i);
            glBindTexture(GL_TEXTURE_2D, m_textures[i]);
        }

    
    }

     void Set( u32 id)
    {
            DEBUG_BREAK_IF(id >= m_count);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_textures[id]);
       
    }

    void Release()
    {
       for (int i = 0; i < m_count; i++)
        {
            glDeleteTextures(1, &m_textures[i]);
        }
        delete[] m_textures;
    }
    //operator

    operator u32*() const {return m_textures;}

};

struct CascadeShadow
{
         unsigned int depthMapFBO;


        unsigned int width;
        unsigned int height;
        u32 m_count;

        TextureCascade textures;

        bool Init(u32 count,unsigned int width, unsigned int height)
        {
            this->width = width;
            this->height = height;
            m_count = count;
            textures.Init(count, width, height);
           
           
            glGenFramebuffers(1, &depthMapFBO);
            // create depth texture
        
         

            // attach depth texture as FBO's depth buffer
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[0], 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);


            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
            {
                LogError("Could not create FrameBuffer");
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                return false;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            return true;
        }

        void Begin()
        {
          
             glViewport(0, 0, width, height);
             glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
              
         
        }

         void Set(u32 index)
        {
          
            
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[index], 0);
            glClear(GL_DEPTH_BUFFER_BIT);
        
            
         
        }

        u32 count() const {return m_count;}

        void End()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void BindTextures(u32 start)
        {
            textures.Bind(start);       
        }
          void BindTexture(u32 id)
        {
            textures.Set(id);       
        }

        void Release()
        {
            glDeleteFramebuffers(1, &depthMapFBO);
            textures.Release();
           
        }

};

struct  DepthBuffer
{
        unsigned int depthMapFBO;
        unsigned int depthMap;

        unsigned int width;
        unsigned int height;

        bool Init(unsigned int width, unsigned int height)
        {
            this->width = width;
            this->height = height;
           
           
            glGenFramebuffers(1, &depthMapFBO);
            // create depth texture
        
            glGenTextures(1, &depthMap);
            glBindTexture(GL_TEXTURE_2D, depthMap);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  

            // attach depth texture as FBO's depth buffer
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);


            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
            {
                LogError("Could not create FrameBuffer");
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                return false;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            return true;
        }

        void Begin()
        {
          
             glViewport(0, 0, width, height);
             glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
             glClear(GL_DEPTH_BUFFER_BIT);
         
        }

        void End()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void BindTexture()
        {
            glBindTexture(GL_TEXTURE_2D, depthMap);
        }

        void Release()
        {
            glDeleteFramebuffers(1, &depthMapFBO);
            glDeleteTextures(1, &depthMap);
           
        }

};



class QuadRender
{

    private:
    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    Shader      shader;

    public:
    void Init()
    {


   {

        const char *vertexShaderSource = GLSL(
                layout (location = 0) in vec3 aPos;
                    layout (location = 1) in vec2 aTexCoords;

                    out vec2 TexCoords;

                    void main()
                    {
                        TexCoords = aTexCoords;
                        gl_Position = vec4(aPos, 1.0);
                    }
        );

                const char *fragmentShaderSource = GLSL(
                    out vec4 FragColor;

                    in vec2 TexCoords;

                    uniform sampler2D depthMap;
                    uniform float near_plane;
                    uniform float far_plane;

                    // required when using a perspective projection matrix
                    float LinearizeDepth(float depth)
                    {
                        float z = depth * 2.0 - 1.0; // Back to NDC 
                        return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
                    }

                    void main()
                    {             
                        float depthValue = texture(depthMap, TexCoords).r;
                        // FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
                        FragColor = vec4(vec3(depthValue), 1.0); // orthographic
                    }
                    );

        shader.Create(vertexShaderSource, fragmentShaderSource);
        shader.LoadDefaults();
        shader.SetInt("depthMap", 0);

        }

        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }

    void Release()
    {
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
    }
    void Render(float x, float y, float size)
    {

          float quadVertices[] = 
        {
            // positions        // texture Coords
            x-size,  y+size, 0.0f, 0.0f, 1.0f,
            x-size,  y-size, 0.0f, 0.0f, 0.0f,
             x+size,  y+size, 0.0f, 1.0f, 1.0f,
             x+size, y-size, 0.0f, 1.0f, 0.0f,
        };
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);



        shader.Use();



        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
};