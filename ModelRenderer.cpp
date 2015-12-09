#include "ModelRenderer.h"
#include "Shader.h"


ModelRenderer::ModelRenderer(std::string meshFile, std::string texFile) : EngineObject(), mesh(meshFile), tex(texFile), mat(2, 32)
{
}

void ModelRenderer::render(Shader* s)
{
	if (!tex.Loaded()) //these only work on the first shader...
		tex.glDownload();
	if (!mat.isLoaded())
		mat.createUniforms(s);
	s->bind();
	tex.bind();
	mesh.Draw();
}


ModelRenderer::~ModelRenderer()
{
}
