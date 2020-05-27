#include "Engine/Platform/EventSystem/event.h"
#include "Engine/GameObject/GameObject.h"
#include <iostream>


namespace Messaging
{
    /*!
    *****************************************************************************
    \fn
    EventHandler::EventHandler(GameObject& object)
    \brief
    ctor for the event handler
    \param
    object: the owner of the event handler
    \return
    none.
    *******************************************************************************/
    EventHandler::EventHandler(GameObject& object)
    {
		mOwner = &object;
    }

    /*!
    *****************************************************************************
    \fn
    GameObject* EventHandler::getOwner()
    \brief
    gettor for a ptr to the owner;
    \param
    none
    \return
    the owner of the handler
    *******************************************************************************/
    GameObject* EventHandler::getOwner()
    {
        return mOwner;
    }

    /*!
    *****************************************************************************
    \fn
    void HandlerFunction::handle(const cs225::Event& event)
    \brief
    handles a non member function
    \param
    const Event& event: the event to handle
    \return
    none.
    *******************************************************************************/
    void HandlerFunction::handle(const Event& event)
    {
        call(event);
    }
    /*!
    *****************************************************************************
    \fn
    EventHandler::~EventHandler()
    \brief
    destructor for EventHandler, clears the handlers std::map
    \param
    none
    \return
    none
    *******************************************************************************/
    EventHandler::~EventHandler()
    {

        //cleanup new'ed function handlers
        for (auto it = handlers.begin(); it != handlers.end(); it++)
        {
			for(HandlerFunction* handlerFunction: it->second)
            delete handlerFunction;
        }
        handlers.clear();
    }
}