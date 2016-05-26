#include "World.h"



World::World(Camera* ref) : player(ref), generator(NoiseGraph(2))
{

}

void World::init()
{

}

void World::update()
{

}

void World::render()
{

}

World::~World()
{
}

void World::onMoveX(float val)
{
	offset.x += val;
}

void World::onMoveY(float val)
{
	offset.y += val;

}
