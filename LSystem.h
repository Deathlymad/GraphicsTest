#include <string>
#include <future>
#include <vector>

#include "Def.h"

NSP_STD
NSP_CHR

#pragma once
class LSystem
{
public:
	class Rule
	{
	public:
		Rule(char trigger, char result) : _trigger(trigger), _result() { _result.push_back(result); }
		Rule( char trigger, string result) : _trigger(trigger), _result(result) {}

		char getTrigger()
		{
			return _trigger;
		}

		string getRes()
		{
			return _result;
		}
	private:
		char _trigger;
		string _result;
	};

	LSystem(char axiom);
	LSystem(string axiom);
	LSystem(char axiom, unsigned maxDepth);
	LSystem(string axiom, unsigned maxDepth);
	LSystem(LSystem& other);

	void start();

	bool isFinished() { return _result.wait_for(seconds(0)) == std::future_status::ready; }

	void addRule(Rule&);

	string getResult();

	LSystem& operator=(LSystem& other);

	~LSystem();
private:
	future<void> _result;

	string applyRules(string&, unsigned);
	void generate();

	//recursion Data
	unsigned _maxDepth;

	//LSystem Data
	string _sentence;
	vector<Rule> _rules;
	string alphabet; //generated set of all rules
};

