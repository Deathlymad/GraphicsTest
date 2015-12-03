#include "ModelRenderer.h"
#include "Shader.h"


ModelRenderer::ModelRenderer(std::string meshFile, std::string texFile) : EngineObject(), mesh(meshFile), tex(texFile)
{
	tex.glDownload();
}

void ModelRenderer::render(Shader* s)
{
	s->bind();
	//tex.bind();
	mesh.Draw();
}


ModelRenderer::~ModelRenderer()
{
}
