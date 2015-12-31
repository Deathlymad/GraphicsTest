#include "UI.h"
#include "UIPart.h"

Shader* UI::renderer = nullptr;

UI::UI()
{
	_parts.push_back(new UIPart(this, vec2(-1.0f, -1.0f), vec2(1.0f, 1.0f))); //add Background
	_enabled = true;
}

void UI::render()
{
	if (!renderer)
		renderer = new Shader("UIRender_vs.glsl", "UIRender_fs.glsl");
	if (_enabled)
	{
		//internal render
		renderer->bind();
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