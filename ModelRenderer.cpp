#include "ModelRenderer.h"
#include "Shader.h"
#include "RenderingEngine.h"


ModelRenderer::ModelRenderer(string meshFile, string texFile) : EngineObject(), mesh(meshFile), tex(texFile)
{
	if (!tex.Loaded())
		tex.glDownload();
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
