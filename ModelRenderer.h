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

	virtual void load(RessourceHandler*);
	virtual void init(KeyMap*);
	virtual void update() { EngineObject::update(); }
	virtual void render(Shader*, RenderingEngine::RenderState);

	~ModelRenderer();

protected:
	ModelRenderer();
private:
	Texture* tex;
	Mesh* mesh;
};

