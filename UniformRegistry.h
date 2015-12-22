#include <vector>

#pragma once
template<int size>
class UniformRegistry
{
public:
	UniformRegistry<size>() {}

	void addMemPos(float * data)
	{
		MemPos.push_back(data);
	}

	void update(float* data)
	{
		for (float* pos : MemPos)
				for (size_t i = 0; i < size; i++)
					pos[i] = data[i];
	}

	~UniformRegistry<size>() {}
private:
	vector<float*> MemPos;
};

