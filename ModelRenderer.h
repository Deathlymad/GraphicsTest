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
	ModelRenderer() {}
	ModelRenderer( std::string, std::string, RenderingEngine*);

	void init( KeyMap* k);
	void render(Shader*);

	~ModelRenderer();
private:
	Texture tex;
	Material mat;
	Mesh mesh;
};

