#include "ModelRenderer.h"
#include "Shader.h"


ModelRenderer::ModelRenderer(std::string meshFile, std::string texFile) : EngineObject(), mesh(meshFile), tex(texFile)
{
}

void ModelRenderer::render(Shader* s)
{
	if (!tex.Loaded())
		tex.glDownload();
	s->bind();
	tex.bind();
	mesh.Draw();
}


ModelRenderer::~ModelRenderer()
{
}
