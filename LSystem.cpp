#include "LSystem.h"

#include <chrono>

NSP_CHR

LSystem::LSystem(char axiom) : _sentence(), _maxDepth(10), _done(false)
{
	_sentence.push_back(axiom);
}
LSystem::LSystem(std::string& axiom) : _sentence(axiom), _maxDepth(10), _done(false)
{

}
LSystem::LSystem(char axiom, unsigned maxDepth) : _sentence(), _maxDepth(maxDepth), _done(false)
{
	_sentence.push_back(axiom);
}
LSystem::LSystem(std::string& axiom, unsigned maxDepth) : _sentence(axiom), _maxDepth(maxDepth), _done(false)
{

}

LSystem::LSystem(LSystem & other) : _sentence(other._sentence), _maxDepth(other._maxDepth), alphabet(other.alphabet), _rules(other._rules)
{
}

int LSystem::run()
{
	generate();
	_done = true;
	return 0;
}

void LSystem::addRule(Rule& rule)
{
	_rules.push_back(rule);
}


string LSystem::getResult()
{
	if (_done)
		return _sentence;
	else
		return "";
}

LSystem& LSystem::operator=(LSystem & other)
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
		newSentence.append(_rules[alphabet.find(c, 0)].getRes()); //not Thread safe
	if (depth == _maxDepth)
		return newSentence;
	else
		return applyRules(newSentence, depth+1);
}

void LSystem::generate()
{
	//generate Alphabet from Rules
	if (alphabet.empty())
		for (Rule& rule : _rules)
			alphabet.push_back(rule.getTrigger());

	_sentence = applyRules(_sentence, 1);
}
