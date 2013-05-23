/*
	A small hook/event class, fully functional:
	Hook<arg1, arg2, arg3...> hook;

	auto pHandle = hook += function;

	delete pHandle; // to remove the hook, either let it be deleted, or:
	pHandle->Unregister(); // or:
	hook -= pHandle
*/

#ifndef HOOKS_HPP
#define HOOKS_HPP

#include <list>
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <thread>
#include <mutex>

namespace Consor
{
	template<class... Args>
	class Hook; // fwd decleration

	template<class... Args>
	struct HookHandle
	{
		typedef std::function<void(Args...)>	function_t;
		typedef Hook<Args...>					hook_t;

		hook_t*		_pHook;
		function_t	_Func;

	public:
		HookHandle(hook_t* pHook, const function_t& Func)
		{
			_pHook = pHook;
			_Func = Func;
		}

		~HookHandle()
		{
			Unregister();
		}

		inline void Unregister()
		{
			if(!_pHook)
				return;
			
			(*_pHook) -= this;
			_pHook = 0;
		}
		inline void operator()(Args... args)
		{
			_Func(args...);
		}
	};

	template<class... Args>
	class Hook
	{
	public:
		typedef HookHandle<Args...>             handle_t;
		typedef std::function<void(Args...)>    function_t;
		typedef	std::shared_ptr<handle_t>       handle_p;
	private:
		std::list<handle_p> _Subscribed;
		std::mutex _CallMutex;
		std::list<std::thread> _Threads;
	public:
		Hook()
		{
		}

		~Hook()
		{
			for(std::thread& t : _Threads)
				if(t.joinable())
					t.join();
			
			for(const handle_p& handle : _Subscribed)
			{
				handle_t* hand = handle.get();
				hand->_pHook = nullptr;
			}
		}

		inline handle_p operator+=(const function_t& Function)
		{
			_Subscribed.emplace_back(new handle_t(this, Function));
			return _Subscribed.back();
		}

		inline void operator-=(handle_t* pHandle)
		{
			//_CallMutex.lock();
			
			_Subscribed.remove_if([&](const handle_p& other)
			{
				return pHandle == other.get();
			});
			
			//_CallMutex.unlock();
		}		
		
		inline void operator()(Args... args)
		{
			auto func = [&](Args... subargs)
			{
				_CallMutex.lock();
				
				// remove all of the threads that've completed
				_Threads.remove_if([](const std::thread& t)
				{
					return !t.joinable();
				});
				
				for(handle_p hand : _Subscribed)
				{
					(*hand)(subargs...);
				}
				
				_CallMutex.unlock();
			};
			
			auto bound_func = std::bind(func, args...);
			_Threads.push_back(std::thread(bound_func));
		}
	};
};

#endif
