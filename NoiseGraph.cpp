#include "NoiseGraph.h"
#include "LSystem.h"

#include <stack>

const LSystem::Rule NoiseGraph::GenRules[] = {
	LSystem::Rule('A',"BDH"),
	LSystem::Rule('B',"CEI"),
	LSystem::Rule('C',"DFJ"),
	LSystem::Rule('D',"EGK"),
	LSystem::Rule('E',"FHA"),
	LSystem::Rule('F',"GIB"),
	LSystem::Rule('G',"HJC"),
	LSystem::Rule('H',"IKD"),
	LSystem::Rule('I',"JAE"),
	LSystem::Rule('J',"KBF"),
	LSystem::Rule('K',"ACG")
	};


NoiseGraph::NoiseGraph(unsigned dimension) : systems(), _dimension(dimension)
{
	unsigned systemAmt = dimension * 2	//two samples per dimension
						+ dimension		// merge multiplier for samples
						+ 1				//dimension sample Merge
						;
	for (unsigned i = 0; i < systemAmt; i++)
	{
		systems.push_back(LSystem('A' + rand() % 10, 10)); // generate Seeds
	}

	for (LSystem& system : systems)
	{
		for (const LSystem::Rule& r : GenRules)
			system.addRule(const_cast<LSystem::Rule&>(r));
		system.start();
	}
	sampler = async([this] {return sample(); });
}

NoiseGraph::~NoiseGraph()
{
}

float NoiseGraph::get( float x, float y)
{
	if (graph.empty())
		return 0;
	float xWeight = getInterpolated(graph[0], min(x+y, 99.0f));
	vector<float> mult;
	vector<float> invMult;
	for (unsigned i = 1; i < 2 + 1; i++)
	{
		mult.push_back(getInterpolated(graph[i], i % 2 ? x : y));
		invMult.push_back(1 - mult[mult.size() - 1]);
	}
	vector<float> mixedX, mixedY;
	mix(mult, invMult, mixedX, mixedY);

	vector<float> xGraphs;
	for (unsigned i = 2 + 1; i < 2 * 2 + 1; i++)
	{
		mult.push_back(getInterpolated(graph[i], x));
	}
	vector<float> yGraphs;
	for (unsigned i = 2 * 2 + 1; i < 3 * 2 + 1; i++)
	{
		mult.push_back(getInterpolated(graph[i], y));
	}

	return getWeightedSum( getWeightedSum(xGraphs, mixedX), getWeightedSum(yGraphs, mixedY), xWeight, 1 - xWeight);
}

void NoiseGraph::sample()
{
	stack<float> pushStack;

	for (LSystem& system : systems)
	{
		if (system.isFinished())
		{
			string algo = system.getResult();
			graph.push_back(vector<float>());
			vector<float>& operating = graph[graph.size() - 1];
			operating.push_back(rand() % 20 - 10);
			for (char& c : algo)
			{
				float val = 0.0f;
				float sum;
				switch (c)
				{
				case 'A':
					operating.push_back(operating[operating.size() - 1] + 2);
					break;
				case 'B':
					operating.push_back(operating[operating.size() - 1] - 2);
					break;
				case 'C':
					pushStack.push(operating[operating.size() - 1]);
					operating.push_back(1);
					break;
				case 'D':
					if (!pushStack.empty())
					{
						operating.push_back(pushStack.top());
						pushStack.pop();
					} else
						operating.push_back(operating[operating.size() - 1]);
					break;
				case 'E':
					operating.push_back(operating[operating.size() - 1] * 2);
					break;
				case 'F':
					operating.push_back(operating[operating.size() - 1] / 2);
					break;
				case 'G':
					operating.push_back(operating[operating.size() - 1] * 0.5);
					break;
				case 'H':
					operating.push_back(powf(operating[operating.size() - 1], 2));
					break;
				case 'I':
					val = operating[operating.size() - 1];
					operating.push_back(val * (int(val) % 2));
					break;
				case 'J':
					val = operating[operating.size() - 1];
					sum = 0;
					while (val > 0) {
						sum = sum + int(val) % 10;
						val = val / 10;
					}
					operating.push_back(sum);
					break;
				case 'K':
					operating.push_back(operating[operating.size() - 1] * 4);
					break;
				default:
					operating.push_back(operating[operating.size() - 1]);
					break;
				}
			}
		}
		else
		{
			this_thread::sleep_for(milliseconds(50));
		}
	}
}

float NoiseGraph::getWeightedSum(vector<float>& vals, vector<float>& weights)
{
	float val = 0.0f;
	if (vals.size() != weights.size())
		return 1.0f;
	for (unsigned i = 0; i < vals.size(); i++)
		val += vals[i] * weights[i];
	return val;
}

float NoiseGraph::getWeightedSum(float v1, float v2, float w1, float w2)
{
	return v1 * w1 + v2 * w2;
}

float NoiseGraph::getInterpolated(vector<float>& arr, float index)
{
	float ind = abs(index);
	unsigned minVal = unsigned (floorf(ind));
	unsigned maxVal = minVal + 1;
	float minWeight = -(ind - maxVal);
	float maxWeight = ind - minVal;
	float res = arr[minVal] * minWeight + arr[maxVal] * maxWeight;
	if (res > 10)
		return 10;
	if (res < -10)
		return 10;
	return res;
}

void NoiseGraph::mix(vector<float>& sort1, vector<float>& sort2, vector<float>& mixed1, vector<float>& mixed2)
{
	unsigned i = 0, j = 0;
	bool arr = true;
	while (sort1.size() > i && sort2.size() > j)
	{
		vector<float>& operating = arr ? mixed1 : mixed2;
		if (sort1.size() > i)
		{
			operating.push_back(sort1[i]);
			i++;
		}
		if (sort2.size() > j)
		{
			operating.push_back(sort2[j]);
			j++;
		}
		arr != arr;
	}
}
