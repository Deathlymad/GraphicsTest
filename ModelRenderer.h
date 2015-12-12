#include "EngineObject.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"

class RenderingEngine;
class KeyMap;

#pragma once
class ModelRenderer : public EngineObject
{
public:
	ModelRenderer( std::string, std::string);

	void init(RenderingEngine* r, KeyMap* k);
	void render(Shader*);

	~ModelRenderer();
private:
	Texture tex;
	Material mat;
	Mesh mesh;
};

