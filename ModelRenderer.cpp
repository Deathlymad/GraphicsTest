#include "ModelRenderer.h"
#include "Shader.h"
#include "RenderingEngine.h"


ModelRenderer::ModelRenderer(Mesh & _mesh) : EngineObject(), mesh(&_mesh), tex( new Texture("assets/textures/Test_tex2.bmp"))
{
}

ModelRenderer::ModelRenderer(string meshFile, string texFile) : EngineObject(), mesh( new Mesh(meshFile)), tex( new Texture(texFile))
{
}

ModelRenderer::ModelRenderer() : EngineObject(), mesh(nullptr), tex(nullptr)
{
}


void ModelRenderer::load(RessourceHandler * loader)
{
	if (tex)
		tex->load(loader);
	if (mesh)
		mesh->load(loader);
}

void ModelRenderer::init(KeyMap* kr)
{
	if (tex)
	tex->glDownload();
	if (mesh)
	mesh->init();
}

void ModelRenderer::render(Shader* s, RenderingEngine::RenderState)
{
	s->bind();
	if (tex)
		tex->bind();
	if (mesh)
		mesh->Draw();
}


ModelRenderer::~ModelRenderer()
{
	if(mesh)
	mesh->~Mesh();
	if (tex)
	tex->~Texture();
}
