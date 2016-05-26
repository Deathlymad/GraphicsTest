#pragma once
#include <vector>

#include "Terrain.h"


class World
{
public:
	World(Camera*);

	void init();

	void update();
	void render();

	~World();
private:
	void setPos(vec3 pos);
	void onMoveX(float val);
	void onMoveY(float val);

	NoiseGraph generator;

	vector<size_t> gc;
	vector<Terrain> allocator;
	vector<vector<Terrain*>> _map;
	vec2 offset;
	Camera* player;
};