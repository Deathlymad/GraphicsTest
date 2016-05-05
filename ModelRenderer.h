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
	ModelRenderer(Mesh& _mesh);
	ModelRenderer( string, string);

	void load(RessourceHandler*);
	void init(KeyMap*);
	void render(Shader*, RenderingEngine::RenderState);

	~ModelRenderer();
private:
	Texture* tex;
	Mesh* mesh;
};

