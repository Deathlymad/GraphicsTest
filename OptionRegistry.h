#include <string>
#include <vector>
#include "Def.h"

NSP_STD

#pragma once
class OptionRegistry
{
	struct Option
	{
		string _name;
		void* _value;
		void* _min;
		void* _max;
	};
public:
	OptionRegistry();
	~OptionRegistry();
private:
	vector<Option> _optionList;
};