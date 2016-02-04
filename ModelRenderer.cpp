#include "ModelRenderer.h"
#include "Shader.h"
#include "RenderingEngine.h"


ModelRenderer::ModelRenderer(string meshFile, string texFile) : EngineObject(), mesh(meshFile), tex(texFile)
{
}

void ModelRenderer::load(RessourceLoader * loader)
{
	tex.load(loader);
	mesh.load(loader);
}

void ModelRenderer::init(KeyMap* kr)
{
	tex.glDownload();
	mesh.glDownload();
}

void ModelRenderer::render(Shader* s, bool)
{
	s->bind();
	tex.bind();
	mesh.Draw();
}


ModelRenderer::~ModelRenderer()
{
}
