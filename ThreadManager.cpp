#include "ThreadManager.h"
#include <chrono>
#include <thread>
#include "Def.h"
#include "Log.h"

NSP_STD
NSP_CHR

const unsigned ThreadManager::Threads::_concurrency = thread::hardware_concurrency();

ThreadManager::ThreadManager() : 
	_distributor(),
	_threadPool(&_distributor)
{
	_threadPool.start();
}

void ThreadManager::registerCallable(CallableObject * obj)
{
	_distributor.registerCallable(obj);
}

void ThreadManager::addTask(function<int()>func)
{
	_distributor.addTask(func);
}

ThreadManager::~ThreadManager()
{}

ThreadManager::Threads::Threads()
{
	_running = false;
	_threads.clear();
	_distributor = nullptr;
}

ThreadManager::Threads::Threads(ThreadManager::TaskDistributor* distributor) :
	_distributor(distributor), 
	_running(false)
{
	_threads.clear();
}

void ThreadManager::Threads::start()
{
	_running = true;
	for (unsigned char i = 0; i < _concurrency; i++)
		_threads.push_back(move(thread()));
	for (unsigned char i = 0; i < _concurrency; i++)
		_threads[i].swap(thread([this] {run(); }));
}

void ThreadManager::Threads::run() //terminating for some reason
{
	int err = 0;
	while (err == 0 && _running)
	{
		lastTick = chrono::system_clock::now();

		function<int()> task = _distributor->getTask();
		err = task();

		this_thread::sleep_for(milliseconds(20 - _distributor->getTasks()) - duration_cast<chrono::milliseconds>(system_clock::now() - lastTick));
	}
}

ThreadManager::Threads::~Threads()
{
	_running = false;
	for (unsigned char i = 0; i < _concurrency; i++)
		_threads[i].join();
}


ThreadManager::TaskDistributor::TaskDistributor() : _registry(), _tasks()
{
	std::lock_guard<std::mutex> lk(_lock);
}

void ThreadManager::TaskDistributor::registerCallable(CallableObject * callable)
{
	std::lock_guard<std::mutex> lk(_lock);
	_registry.push_back(callable);
}

void ThreadManager::TaskDistributor::addTask(function<int()> task)
{
	std::lock_guard<std::mutex> lk(_lock);
	_tasks.push(task);
}

ThreadManager::TaskDistributor::~TaskDistributor()
{
	std::lock_guard<std::mutex> lk(_lock);
	while (!_tasks.empty())
		_tasks.pop();

	_registry.clear();
}

function<int()> ThreadManager::TaskDistributor::getTask()
{
	std::lock_guard<std::mutex> lk(_lock);
	if (_tasks.empty())
	{
		for (CallableObject* obj : _registry)
			_tasks.push([obj] { return obj->run(); });
	}
	if (!_tasks.empty())
	{
		function<int()> task = _tasks.front();
		_tasks.pop();
		return task;
	}
	
	return
		[] {return 0; };
}
