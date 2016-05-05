#include "ModelRenderer.h"
#include "Shader.h"
#include "RenderingEngine.h"


ModelRenderer::ModelRenderer(Mesh & _mesh) : EngineObject(), mesh(&_mesh), tex( new Texture("assets/textures/Test_tex2.bmp"))
{
}

ModelRenderer::ModelRenderer(string meshFile, string texFile) : EngineObject(), mesh( new Mesh(meshFile)), tex( new Texture(texFile))
{
}

void ModelRenderer::load(RessourceHandler * loader)
{
	tex->load(loader);
	mesh->load(loader);
}

void ModelRenderer::init(KeyMap* kr)
{
	tex->glDownload();
	mesh->init();
}

void ModelRenderer::render(Shader* s, RenderingEngine::RenderState)
{
	s->bind();
	tex->bind();
	mesh->Draw();
}


ModelRenderer::~ModelRenderer()
{
	mesh->~Mesh();
	tex->~Texture();
}
