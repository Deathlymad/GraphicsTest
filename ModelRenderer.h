#include "EngineObject.h"
#include "Texture.h"
#include "Mesh.h"

#pragma once
class ModelRenderer : public EngineObject
{
public:
	ModelRenderer( std::string, std::string);

	void render(Shader*);

	~ModelRenderer();
private:
	Texture tex;
	Mesh mesh;
};

