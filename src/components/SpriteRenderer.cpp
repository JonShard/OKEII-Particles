#include "components/SpriteRenderer.hpp"

extern glm::vec2 windowSize;

namespace OK
{
    SpriteRenderer::SpriteRenderer( 
        GameObject* gameObject, 
        int id,
        Texture2D texture
    ) : Component(gameObject, id), 
        m_texture{texture}, 
        m_VAO()
    {
        prepare();
    }
    
    // https://learnopengl.com/In-Practice/2D-Game/Rendering-Sprites
    void SpriteRenderer::prepare()
    {
        // Configure VAO and VBO
        GLfloat vertices[] = {
            // Pos      // UV
            0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f
        };
        GLuint indices[] = {
            0,1,2,
            2,1,3
        };

        // Setup shader and uniforms:
        m_shader = createProgram("assets/shaders/spriteVertex.vert", "assets/shaders/spriteFragment.frag");

        glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(windowSize.x), 
        0.0f, static_cast<GLfloat>(windowSize.y), -1.0f, 1.0f);
        
	    GFX_GL_CALL(glUniform1i(m_shader.getUniformLocation("image"), 0));
        GFX_GL_CALL(glUniformMatrix4fv(m_shader.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection)));

        // Setup buffers:
        //TODO unify the interface for VBOs and IBOs, the first argument is confusing (count vs size).

        m_VAO.bind();
        GFX_ASSERT(m_VAO.valid(), "invalid VAO!");
        IndexBuffer<GLuint> ibo(sizeof(indices)/sizeof(indices[0]), indices);
        GFX_ASSERT(ibo.valid(), "invalid IBO!");
        VertexBuffer vbo(sizeof(vertices), vertices); 
        GFX_ASSERT(vbo.valid(), "invalid VAO!");

        InterleavingVertexLayout layout;
        layout.pushAttrib<GL_FLOAT, 2, 1, GL_FALSE, false, 8>("position");
        layout.pushAttrib<GL_FLOAT, 2, 1, GL_FALSE, false, 8>("uv");
        layout.applyToBuffer(vbo);
    }


    void SpriteRenderer::update(float deltaTime)
    {
        m_rotation += deltaTime*2;
        draw(); //TEMPORARILY, as the GraphicsComponent is not yet in place
    }

    void SpriteRenderer::draw()
    {
        glm::mat4 mvp = glm::mat4(1);// m_gameObject->m_trasform.modelMatrix();

        mvp = glm::translate(mvp, glm::vec3(m_offset.x, m_offset.y, 0.0f));

        //Move origin to rotate around center
        mvp = glm::translate(mvp, glm::vec3(m_pivot.x * m_size.x, m_pivot.y * m_size.y, 0.0f));
        mvp = glm::rotate(mvp, m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        mvp = glm::translate(mvp, glm::vec3(-m_pivot.x * m_size.x, -m_pivot.y * m_size.y, 0.0f));

        mvp = glm::scale(mvp, glm::vec3(m_size, 1.0f));

        m_shader.bind();
        m_texture.bind();

        m_VAO.bind();
        // Set "update every frame" uniforms:
        GFX_GL_CALL(glUniformMatrix4fv(m_shader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(mvp)));
        GFX_GL_CALL(glUniform4f(m_shader.getUniformLocation("spriteColor"), m_color.x, m_color.y, m_color.z, m_color.w));
        GFX_GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
    }

    SpriteRenderer::~SpriteRenderer()
    {
        //delete m_VBO;
    }

}
