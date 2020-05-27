/*****************************************************************/
/*Author: Joaquin Bilbao				                         */
/*Brief description:	    									 */
/*****************************************************************/

#pragma once 

#include <string>
#include <typeinfo>
#include <map> //std::map
#include <vector> //std::vector

//fwd declaration
class GameObject;

namespace Messaging
{

	class Event
	{
	public:
		Event() {}
		virtual ~Event() {}

	private:

	};


	class HandlerFunction
	{
	public:
		virtual void handle(const Event& event);

		virtual void call(const Event& event) = 0;

	private:
	};

	template <typename T, typename EVENT>
	class MemberFunctionHandler : public HandlerFunction
	{
	public:
		//typedef to save function pointers as memberFunction
		typedef void (T::*MemberFunction)(const EVENT&);
		/*!
		*****************************************************************************
		\fn
		MemberFunctionHandler
		\brief
		constructor for MemberFunctionHandler
		\param
		T* object: instance of the class
		MemberFunction fn: function pointer to the way to resolve the event
		*******************************************************************************/
		MemberFunctionHandler(T* object, const MemberFunction fn) : instance(object), function(fn)
		{}

		/*!
		*****************************************************************************
		\fn
		virtual void call(const Event* event)
		\brief
		calls the event to be handled
		\param
		const Event* event: the event to be handled
		\return
		none, void
		*******************************************************************************/
		virtual void call(const Event& event)
		{
			//instance: who
			//*function: how
			//event: what
			(instance->*function)(static_cast<const EVENT &> (event));
		}
	private:
		T* instance; //pointer to the instance of the type of the handler
		MemberFunction function; // function pointer to the way to resolve the event
	};


	class EventHandler
	{
	public:

		/*!
		*****************************************************************************
		\fn
		template <typename T, typename EVENT>
		void register_handler(T* object, void (T::*fn)(const EVENT&))
		\brief
		registers a MemberFunctionHandler in the handlers std::map
		\param
		T* object: object whose member function will be stored
		void (T::*fn)(const EVENT&): pointer to the member function to be stored
		\return
		none
		*******************************************************************************/
		template <typename T, typename EVENT>
		void register_handler(T* object, void (T::*fn)(const EVENT&))
		{
			//JOKINNEW
			//stores a member function/instance
			//associated with the type of the event that the function is taking
			handlers[typeid(EVENT).name()].push_back(new MemberFunctionHandler<T, EVENT>(object, fn));
		}

		/*!
		*****************************************************************************
		\fn
		template <typename T, typename EVENT>
		void register_handler(T& object, void (T::*fn)(const EVENT&))
		\brief
		Same as above but takes a reference instead of a ptr
		\param
		T& object: object whose member function will be stored
		void (T::*fn)(const EVENT&): pointer to the member function to be stored
		\return
		none
		*******************************************************************************/
		template <typename T, typename EVENT>
		void register_handler(T& object, void (T::*fn)(const EVENT&))
		{
			register_handler(&object, fn);
		}
		EventHandler() = default;
		EventHandler(GameObject& object);
		~EventHandler();

		/*!
		*****************************************************************************
		\fn
		template <typename EVENT>
		void handle(const EVENT event)
		\brief
		looks for the handler function for the type of event passed and calls handle on
		that event
		\param
		const EVENT event: the type of event we want to handle
		\return
		none
		*******************************************************************************/
		template <typename EVENT>
		void handle(const EVENT& passedEvent)
		{
			if (!mOwner)
			{
				std::cout << "Prevented a crash on event handler" << std::endl;
				return;
			}

			if (!mOwner->m_alive)
			{
				mOwner = nullptr;

				std::cout << "Prevented a crash on event handler" << std::endl;
				return;
			}

			//find the event
			auto it = handlers.find(typeid(passedEvent).name());

			//if the event is found we call all function handlers of said event
			if (it != handlers.end())
			{
				for (HandlerFunction* currentFunction : it->second)
					currentFunction->handle(passedEvent);
			}
		}


		GameObject* getOwner();

	private:
		//maps object types with member functions
		//of said type that take a specific event
		std::map<std::string, std::vector<HandlerFunction *> > handlers;
		GameObject* mOwner;
	};

}