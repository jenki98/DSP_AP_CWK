#pragma once

#include "Texture.h"
#include "VertexBufferObject.h"

// Class for generating a xz Wall of a given size
class CWall
{
public:
	CWall();
	~CWall();
	void Create(string sDirectory, string sFilename, float fTextureRepeat);
	void Render();
	void Release();
	glm::vec3 position;
	glm::vec3 up;
private:
	UINT m_vao;
	CVertexBufferObject m_vbo;
	CTexture m_texture;
	string m_directory;
	string m_filename;
	int m_numTriangles;

};