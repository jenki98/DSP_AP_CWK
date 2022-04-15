#include "Common.h"
#include "Wall.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))


CWall::CWall()
{}

CWall::~CWall()
{}


// Create the Wall, including its geometry, texture mapping, normal, and colour
void CWall::Create(string directory, string filename, float textureRepeat)
{

	// Load the texture
	m_texture.Load(directory + filename, true);
	m_directory = directory;
	m_filename = filename;

	// Set parameters for texturing using sampler object
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// Use VAO to store state associated with vertices
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Create a VBO
	m_vbo.Create();
	m_vbo.Bind();

	// Vertex positions
	glm::vec3 WallVertices[24] =
	{

		//front of cube
		glm::vec3(-50.0f, -50.0f, 20.0f),
		glm::vec3(-50.0f, 50.0f, 20.0f),
		glm::vec3(-50.0f, -50.0f, 0.0f),
		glm::vec3(-50.0f, 50.0f, 0.0f),
		//left of cube
		glm::vec3(-50.0f, -50.0f, 0.0f),
		glm::vec3(-50.0f, 50.0f, 0.0f),
		glm::vec3(50.0f, -50.0f, 0.0f),
		glm::vec3(50.0f, 50.0f, 0.0f),
		//back of cube
		glm::vec3(50.0f, -50.0f, 0.0f),
		glm::vec3(50.0f, 50.0f, 0.0f),
		glm::vec3(50.0f, -50.0f, 20.0f),
		glm::vec3(50.0f, 50.0f, 20.0f),
		//right of cube
		glm::vec3(50.0f, -50.0f, 20.0f),
		glm::vec3(50.0f, 50.0f, 20.0f),
		glm::vec3(-50.0f, -50.0f, 20.0f),
		glm::vec3(-50.0f, 50.0f, 20.0f),
		//bottom of cube
		glm::vec3(-50.0f, -50.0f, 20.0f),
		glm::vec3(-50.0f, -50.0f, 0.0f),
		glm::vec3(50.0f, -50.0f, 20.0f),
		glm::vec3(50.0f, -50.0f, 0.0f),
		//top of cube
		glm::vec3(50.0f,  50.0f, 20.0f),
		glm::vec3(50.0f,  50.0f, 0.0f),
		glm::vec3(-50.0f, 50.0f, 20.0f),
		glm::vec3(-50.0f, 50.0f, 0.0f),
		

	};

	
	// Texture coordinates
	glm::vec2 WallTexCoords[24] =
	{
		//good
		//front of cube
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		//left of cube
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		//backof cube
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		//right of cube
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		//bottom of cube
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		//top of cube
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
	};

	// Wall normal
	glm::vec3 WallNormals[24] = {

		//front of cube
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		//left of cube
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		//back of cube
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		//right of cube
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		//bottom of cube
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		//top of cube
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
	};



	// Put the vertex attributes in the VBO
	for (unsigned int i = 0; i < 24; i++) {
		m_vbo.AddData(&WallVertices[i], sizeof(glm::vec3));
		m_vbo.AddData(&WallTexCoords[i], sizeof(glm::vec2));
		m_vbo.AddData(&WallNormals[i], sizeof(glm::vec3));
	}


	// Upload the VBO to the GPU
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);

	
	GLsizei istride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, istride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, istride, (void*)(sizeof(glm::vec3)));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, istride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}

// Render the Wall as a triangle strip
void CWall::Render()
{
	glBindVertexArray(m_vao);
	m_texture.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);


}

// Release resources
void CWall::Release()
{
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}
