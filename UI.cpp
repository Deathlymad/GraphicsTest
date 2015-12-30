#include "UI.h"
#include "UIPart.h"

Shader UI::renderer = Shader("UIRender_vs.glsl","UIRender_fs.glsl");

UI::UI()
{
}

void UI::render()
{
	if (_enabled)
	{
		//internal render
		renderer.bind();
		for (UIPart* part : _parts)
			part->render();
		//external Render
		for (UI* child : _children)
			child->render();
	}
}

void UI::update()
{
}


UI::~UI()
{
}