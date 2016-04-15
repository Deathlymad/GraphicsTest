#include "ModelRenderer.h"
#include "Shader.h"
#include "RenderingEngine.h"


ModelRenderer::ModelRenderer(string meshFile, string texFile) : EngineObject(), mesh(meshFile), tex(texFile)
{
}

void ModelRenderer::load(RessourceHandler * loader)
{
	tex.load(loader);
	mesh.load(loader);
}

void ModelRenderer::init(KeyMap* kr)
{
	tex.glDownload();
	mesh.init();
}

void ModelRenderer::render(Shader* s, RenderingEngine::RenderState)
{
	s->bind();
	tex.bind();
	mesh.Draw();
}


ModelRenderer::~ModelRenderer()
{
}
