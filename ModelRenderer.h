#include "EngineObject.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"

#pragma once
class ModelRenderer : public EngineObject
{
public:
	ModelRenderer( std::string, std::string);

	void render(Shader*);

	~ModelRenderer();
private:
	Texture tex;
	Material mat;
	Mesh mesh;
};

