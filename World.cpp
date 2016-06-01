#include "World.h"



World::World(Camera* ref, unsigned x, unsigned z) : EngineObject(), player(ref), generator(NoiseGraph(2)),
chunkX(x), chunkZ(z), allocator(16, Terrain(generator, 0, 0, 0, 0)), initialized(false), ground("assets/textures/Test_tex2.bmp")
{

}

void World::load(RessourceHandler * loader)
{
	ground.load(loader);
}

void World::init(KeyMap* map)
{
	if (initialized)
		return;
	setPos(vec2(0, 0));
	initialized = true;
	ground.glDownload();
}

void World::update()
{
}

void World::render(Shader* s, RenderingEngine::RenderState state)
{
	setPos(toWorldPos(player->getPos())); //make asynchronous (init OGL at later Point in )
	s->bind();
	ground.bind();
	for (unsigned i = 0; i < 16; i++)
	{
		allocator[i].Draw();
	}
}

World::~World()
{
}

unsigned World::getMemPosForTerrain(int x, int z, bool invX, bool invZ)
{
	int bitX, bitZ;
	bitX = x % 4;
	bitZ = z % 4;

	if (invX)
		bitX = 4 - bitX;
	if (invZ)
		bitZ = 4 - bitZ;

	return (bitX | (bitZ << 2)) % 16;
}

void World::TerrainForPos(vec2 p, float xO, float zO)
{
	int   modX = p.x - int(p.x) % 50
		, modZ = p.y - int(p.y) % 50;
	int   xOff = (xO - 2) * chunkX
		, zOff = (zO - 2) * chunkZ;
	unsigned pos = getMemPosForTerrain(abs(modX + xOff) / chunkX, abs(modZ + zOff) / chunkZ, modX + xOff < 0, modZ + zOff < 0);

	if (!(allocator[pos] == vec2(modX + xOff, modZ + zOff))) //checks if cache is correct
	{
		allocator.erase(allocator.begin() + pos);//removes object
		allocator.insert( allocator.begin() + pos, Terrain(generator, modX + xOff, modZ + zOff, chunkX, chunkZ));//inserts new object
		allocator[pos].init(); //initializes the new Object
	}
}

void World::setPos(vec2 pos)
{
	for (unsigned char x = 0; x < 4; x++)
		for (unsigned char z = 0; z < 4; z++)
			TerrainForPos(pos, x, z);
}
