#include "ModelRenderer.h"
#include "Shader.h"
#include "RenderingEngine.h"


ModelRenderer::ModelRenderer(std::string meshFile, std::string texFile) : EngineObject(), mesh(meshFile), tex(texFile), mat(2, 32)
{
	if (!tex.Loaded())
		tex.glDownload();
}

void ModelRenderer::init(RenderingEngine * r)
{
	if (!mat.isLoaded())
		r->initOnShaders([this](Shader* s) {mat.createUniforms(s); });
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
