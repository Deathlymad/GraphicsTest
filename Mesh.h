#include <vector>
#include <glm\common.hpp>

#pragma once
class Mesh
{
public:
	Mesh ( std::string);
	Mesh(  std::vector<glm::vec3> &vec, std::vector < unsigned int> &i);
	Mesh();
		
	void Draw();

	~Mesh(void);
private:
	void initGL(unsigned char);

	void glDownload(  std::vector<glm::vec3>&, std::vector < unsigned int>&);
		
	unsigned int vao;
	unsigned int vbo; // new interleaving vvvfn v = Vertex, f = faceID, n = normal (accessed by face as index)
	unsigned int ibo;

	unsigned int indices;
	//needed for Physics later
	std::vector<glm::vec3> pts;
	std::vector<unsigned int> ind;
};