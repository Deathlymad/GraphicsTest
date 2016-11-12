#include <array>
#include <vector>
#include <queue>
#include <future>

#include "Def.h"
NSP_STD
NSP_CHR

#pragma once
class ThreadManager //needs taskEvaluator, distributed Processing
{
public:
	class CallableObject {
	public:
		virtual int run() = 0;
	};
private:
	typedef vector<CallableObject*> UpdateRegistry;
	class Threads;

	class TaskDistributor { //needs lock
		friend class Threads;
	public:
		TaskDistributor();

		void registerCallable(CallableObject*);
		void addTask(function<int()>);

		void notify();

		size_t getTasks() { return _tasks.size(); }

		~TaskDistributor();
	private:
		function<int()> getTask();

		mutex _lock;
		bool _notified;
		condition_variable _notifier;
		UpdateRegistry _registry;
		queue<function<int()>> _tasks;
	};

	class Threads {
	public:
		Threads();
		explicit Threads(ThreadManager::TaskDistributor* distributor);

		void start();

		~Threads();
	private:
		void run();

		const static unsigned _concurrency;
		bool _running;
		std::vector<thread> _threads;
		TaskDistributor* _distributor;

		time_point<system_clock> lastTick;
	};

public:
	ThreadManager();

	void registerCallable(CallableObject* obj);
	void addTask(function<int()>);

	~ThreadManager();
private:
	Threads _threadPool;
	TaskDistributor _distributor;
};