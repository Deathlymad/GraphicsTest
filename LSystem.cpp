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

LSystem::LSystem(LSystem & other) : _sentence(other._sentence), _maxDepth(other._maxDepth), alphabet(other.alphabet), _rules(other._rules)
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
		return _sentence;
	else
		return "";
}

LSystem & LSystem::operator=(LSystem & other)
{
	_maxDepth = other._maxDepth;
	_sentence = other._sentence;
	_rules = other._rules;
	alphabet = other.alphabet;
	return *this;
}

LSystem::~LSystem()
{
}

string LSystem::applyRules(string& str, unsigned depth)
{
	string newSentence = "";
	for (const char& c : str)
		newSentence.append(_rules[alphabet.find(c, 0)].getRes()); // add exceptions
	if (depth == _maxDepth)
		return newSentence;
	else
		return applyRules(newSentence, depth+1);
}

void LSystem::generate()
{
	//generate Alphabet from Rules
	for (Rule& rule : _rules)
		alphabet.push_back(rule.getTrigger());

	_sentence = applyRules(_sentence, 1);
}
