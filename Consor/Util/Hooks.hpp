/*
	A small hook/event class, fully functional:
	CHook<arg1, arg2, arg3...> hook;

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

namespace Consor
{
	template<class... Args>
	class CHook; // fwd decleration

	template<class... Args>
	struct CHookHandle
	{
		typedef std::function<void(Args...)>	function_t;
		typedef CHook<Args...>					hook_t;

		hook_t*		m_pHook;
		function_t	m_Func;

	public:
		CHookHandle(hook_t* pHook, const function_t& Func)
		{
			m_pHook = pHook;
			m_Func = Func;
		}

		~CHookHandle()
		{
			Unregister();
		}

		inline void Unregister()
		{
			if(!m_pHook)
				return;

			(*m_pHook) -= this;
			m_pHook = 0;
		}
		inline void operator()(Args... args)
		{
			m_Func(args...);
		}
	};

	template<class... Args>
	class CHook
	{
	public:
		typedef CHookHandle<Args...>			handle_t;
		typedef std::function<void(Args...)>	function_t;
		typedef	std::shared_ptr<handle_t>		handle_p;
	private:
		std::list<handle_p> m_Subscribed;
		std::list<handle_p> m_ToRemove;

		bool m_Itterating;
	public:

		inline handle_p operator+=(const function_t& Function)
		{
			handle_t* handle = new handle_t(this, Function);

			handle_p sp(handle);

			m_Subscribed.push_back(sp);
			return sp;
		}

		inline void operator-=(handle_t* pHandle)
		{
			m_Subscribed.remove_if([&](handle_p other)
			{
				handle_t* pOther = &*other;
				return pHandle == pOther;
			});
		}

		inline void operator-=(handle_p Handle)
		{
			if(m_Itterating)
			{
				m_ToRemove.push_back(Handle);
			}

			m_Subscribed.remove(*Handle);
			Handle->Remove();
		}

		inline void operator()(Args... args)
		{
			for(handle_p hand : m_ToRemove)
				hand->Unregister();
			m_ToRemove.clear();

			m_Itterating = true;
			for(handle_p hand : m_Subscribed)
				(*hand)(args...);
			m_Itterating = false;
		}
	};
};

#endif