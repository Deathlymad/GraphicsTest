#include "ModelRenderer.h"
#include "Shader.h"
#include "RenderingEngine.h"


ModelRenderer::ModelRenderer(string meshFile, string texFile) : EngineObject(), mesh(meshFile), tex(Image(texFile))
{
	tex.glDownload();
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
