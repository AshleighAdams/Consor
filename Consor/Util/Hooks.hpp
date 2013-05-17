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
		typedef HookHandle<Args...>			handle_t;
		typedef std::function<void(Args...)>	function_t;
		typedef	std::shared_ptr<handle_t>		handle_p;
	private:
		std::list<handle_p> _Subscribed;
		std::list<handle_p> _ToRemove;
		std::mutex _CallMutex;
		std::thread* _pLastCallThread;

		bool _Itterating;
	public:
		Hook()
		{
			_pLastCallThread = nullptr;
		}

		~Hook()
		{
			// leak memory yay
			//delete _pLastCallThread;
		}

		inline handle_p operator+=(const function_t& Function)
		{
			handle_t* handle = new handle_t(this, Function);

			handle_p sp(handle);

			_Subscribed.push_back(sp);
			return sp;
		}

		inline void operator-=(handle_t* pHandle)
		{
			_Subscribed.remove_if([&](handle_p other)
			{
				handle_t* pOther = &*other;
				return pHandle == pOther;
			});
		}

		inline void operator-=(handle_p Handle)
		{
			if(_Itterating)
			{
				_ToRemove.push_back(Handle);
			}

			_Subscribed.remove(*Handle);
			Handle->Remove();
		}

		inline void operator()(Args... args)
		{
			bool canresume = false;

			std::thread* pThrd = new std::thread([&]()
			{
				//std::thread* pThread = pThrd;
				canresume = true;

				_CallMutex.lock();

				// fuck it, lets just leak memory
				//delete _pLastCallThread;
				//_pLastCallThread = nullptr;

				for(handle_p hand : _ToRemove)
					hand->Unregister();
				_ToRemove.clear();

				_Itterating = true;
				for(handle_p hand : _Subscribed)
				{
					_CallMutex.unlock();
					(*hand)(args...);
					_CallMutex.lock();
				}
				_Itterating = false;
				//_pLastCallThread = pThread;
				_CallMutex.unlock();
			});

			while(!canresume)
				;
		}
	};
};

#endif