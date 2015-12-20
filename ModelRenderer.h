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
	ModelRenderer( string, string);

	void render(Shader*);

	~ModelRenderer();
private:
	Texture tex;
	Mesh mesh;
};

