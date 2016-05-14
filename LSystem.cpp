#include "LSystem.h"

#include <chrono>

NSP_CHR

LSystem::LSystem(char axiom) : _sentence(), _maxDepth(10)
{
	_sentence.push_back(axiom);
}
LSystem::LSystem(std::string axiom) : _sentence(axiom), _maxDepth(10)
{

}
LSystem::LSystem(char axiom, unsigned maxDepth) : _sentence(), _maxDepth(maxDepth)
{
	_sentence.push_back(axiom);
}
LSystem::LSystem(std::string axiom, unsigned maxDepth) : _sentence(axiom), _maxDepth(maxDepth)
{

}

void LSystem::start()
{
	_result = async(launch::async | launch::deferred, [this] {return generate(); });
}

void LSystem::addRule(Rule& rule)
{
	_rules.push_back(rule);
}


string LSystem::getResult()
{
	if (_result.wait_for(seconds(0)) == std::future_status::ready)
		return _result.get();
	else
		return "";
}

LSystem::~LSystem()
{
}

string& LSystem::applyRules(string& str, unsigned depth)
{
	if (depth == _maxDepth)
		return str;
	else
	{
		string newSentence = "";
		for (const char& c : str)
			newSentence.append(_rules[alphabet.find(c, 0)].getRes()); // add exceptions
		return applyRules(newSentence, depth++);
	}
}

string LSystem::generate()
{
	//generate Alphabet from Rules
	for (Rule& rule : _rules)
		alphabet.push_back(rule.getTrigger());

	_sentence = applyRules(_sentence, 1);
}
