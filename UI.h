#include "EngineObject.h"
#include "Shader.h"

class UIPart;

#pragma once
class UI : EngineObject
{
public:
	UI();

	bool isActive() { return _enabled; }

	void render();
	void update();

	~UI();
private:

	UI* _parent;
	vector<UI*> _children;
	vector<UIPart*> _parts;
	bool _enabled;

	static Shader renderer;
};

