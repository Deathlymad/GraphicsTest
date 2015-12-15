#include "ModelRenderer.h"
#include "Shader.h"
#include "RenderingEngine.h"


ModelRenderer::ModelRenderer(std::string meshFile, std::string texFile, RenderingEngine* r) : EngineObject(), mesh(meshFile), tex(texFile), mat(2, 1)
{
	if (!tex.Loaded())
		tex.glDownload();
	if (!mat.isLoaded())
		r->initOnShaders([this](Shader* s) {mat.createUniforms(s); });
}

void ModelRenderer::init( KeyMap * k)
{
}

void ModelRenderer::render(Shader* s)
{
	s->bind();
	tex.bind();
	mesh.Draw();
}


ModelRenderer::~ModelRenderer()
{
}
