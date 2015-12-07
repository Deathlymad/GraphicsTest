#include <vector>
#include <glm\common.hpp>

#pragma once
class Mesh
{
	class Vertex
	{
	public:
		Vertex(glm::vec3 pos, glm::vec2 tex, glm::vec3 nor)
		{
			data[0] = pos.x; data[1] = pos.y; data[2] = pos.z;
			data[3] = tex.x; data[4] = tex.y;
			data[5] = nor.x; data[6] = nor.y; data[7] = nor.z;
		}

		void setTexCoord( glm::vec2 tex) { data[3] = tex.x; data[4] = tex.y; }

		void setNormal( glm::vec3 nor) { data[5] = nor.x; data[6] = nor.y; data[7] = nor.z; }

		glm::vec3 getNormal() { return glm::vec3(data[5], data[6], data[7]); }

		float* getData()
		{
			return data;
		}
	private:
		float data[8];
	};
public:
	Mesh ( std::string);
	Mesh(  std::vector<Vertex> &vec, std::vector < unsigned int> &i);
	Mesh();
		
	void Draw();

	~Mesh(void);
private:
	void initGL(unsigned char);

	void glDownload(  std::vector<Vertex>&, std::vector < unsigned int>&);
		
	unsigned int vao;
	unsigned int vbo; // new interleaving vvvn v = Vertex, n = normal (accessed by face as index)
	unsigned int ibo;

	unsigned int DataSize;
	unsigned int indices;
	//needed for Physics later
	std::vector<Vertex> pts;
	std::vector<unsigned int> ind;
};