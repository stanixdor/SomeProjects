#include "Engine/Platform/EventSystem/event_dispatcher.h"
#include "Engine\GameObject\GameObject.h"

namespace Messaging
{
    /*!
    *****************************************************************************
    \fn
    static EventDispatcher& get_instance()
    \brief
    this gets the only instance the class EventDispatcher will have, if there is none, it is created
    \param
    none
    \return
    a reference to the instance of the dispatcher
    *******************************************************************************/
    EventDispatcher& EventDispatcher::get_instance()
    {
        //compiler checks if dispatcher already has a value before doing the new for the second time
        static  EventDispatcher* dispatcher = new EventDispatcher();
        return *dispatcher;
        
    }

    /*!
    *****************************************************************************
    \fn
    void EventDispatcher::subscribe(Listener& object, const TypeInfo& type)
    \brief
    subscribes an object to a type of event
    \param
    Listener& object: the object to subscribe
    const TypeInfo& type: to what type of event to subscribe
    \return
    none
    *******************************************************************************/
    void EventDispatcher::subscribe(EventHandler& objectsEventHanlder, const TypeInfo& type)
    {
       
        subscriberList[type.get_name()].push_back(&objectsEventHanlder);
    }

    /*!
    *****************************************************************************
    \fn
    void EventDispatcher::unsubscribe(Listener& object, const TypeInfo& type)
    \brief
    unsubscribes an object from an event type
    \param
    Listener& object: object to unsubscribe
    const TypeInfo& type: type of the event to unsuscribe from
    \return
    none
    *******************************************************************************/
    void EventDispatcher::unsubscribe(EventHandler& objectsEventHanlder, const TypeInfo& type)
    {
        //find the type of the event
        std::string eventType = type.get_name();
        
        auto mapIterator = subscriberList.find(eventType);

        //now that we have the event we find the object to unsubscribe
        if (mapIterator != subscriberList.end())
        {
            //get an iterator to go through the vector
            auto vectorIterator = std::find(mapIterator->second.begin(), mapIterator->second.end(), &objectsEventHanlder);

            //if we found the object
            if (vectorIterator != mapIterator->second.end())
            {
                //delete the object from the vector
                mapIterator->second.erase(vectorIterator);
            }
        }
    }

    /*!
    *****************************************************************************
    \fn
    void EventDispatcher::clear()
    \brief
    clears the subscriber list
    \param
    none
    \return
    none
    *******************************************************************************/
    void EventDispatcher::clear()
    {
        subscriberList.clear();
    }

    /*!
    *****************************************************************************
    \fn
    std::ostream& operator<<( std::ostream& output, const EventDispatcher& dispatcher)
    \brief
    overloading of operator << to make debugging easier.
    \param
    std::ostream& output: output will be saved here
    const EventDispatcher& dispatcher: pointer to the event dispatcher
    \return
    the output
    *******************************************************************************/
    std::ostream& operator<<(std::ostream& output, const EventDispatcher& dispatcher)
    {
        //EventDispatcher& eventDisp = dispatcher.get_instance();

        auto mapIterator = dispatcher.subscriberList.begin();

        //iterate through the map
        for (mapIterator = dispatcher.subscriberList.begin(); mapIterator != dispatcher.subscriberList.end(); mapIterator++)
        {
            //we write the text
            output << "The event type " << mapIterator->first << " has the following subscribers:\n";

            //in every event get the subscribers it has
            auto vectorIterator = mapIterator->second.begin();

            for (vectorIterator = mapIterator->second.begin(); vectorIterator != mapIterator->second.end(); vectorIterator++)
            {
                EventHandler* tempHandler = *vectorIterator;
                output << "\tAn instance of type " << tempHandler->getOwner()->m_name << "\n";
            }
        }

        return output;
    }

    /*!
    *****************************************************************************
    \fn
    void EventDispatcher::trigger_event(const Event& event)
    \brief
    triggers the event, which calls the listeners that are subscribed to the event
    \param
    const Event& event: the event to be triggered
    \return
    none
    *******************************************************************************/
    void EventDispatcher::trigger_event(const Event& mEvent)
    {
        //find the type of the event
        std::string eventType = typeid(mEvent).name();
        auto mapIterator = subscriberList.find(eventType);

        //if there are listeners subscribed to the event
        if (mapIterator != subscriberList.end())
        {
            //get an iterator to go through the vector
           auto vectorIterator = mapIterator->second.begin();

            //loops through the subscribers
            for (vectorIterator = mapIterator->second.begin(); vectorIterator != mapIterator->second.end(); vectorIterator++)
            {
                //every listener handles the event
                EventHandler* tempHandler = *vectorIterator;
                tempHandler->handle(mEvent);
            }
        }
    }

    void EventDispatcher::trigger_event_for_handler(EventHandler& objectsEventHanlder, const Event& mEvent)
    {
        //find the type of the event
        std::string eventType = typeid(mEvent).name();
        auto mapIterator = subscriberList.find(eventType);

        //if there are listeners subscribed to the event
        if (mapIterator != subscriberList.end())
        {
            std::vector<EventHandler*> vectorOfHandlers = mapIterator->second;

            auto vectorIterator = std::find(vectorOfHandlers.begin(), vectorOfHandlers.end(), &objectsEventHanlder);
            
            if (vectorIterator != vectorOfHandlers.end())
            {
                EventHandler* tempHandler = *vectorIterator;
                tempHandler->handle(mEvent);
            }
        }
    }
    /*!
    *****************************************************************************
    \fn
    void trigger_event(const Event& event)
    \brief
    wrapper function to call EventDispatcher's trigger event
    \param
    const Event& event: the event to be triggered
    \return
    none
    *******************************************************************************/
    void trigger_event(const Event& event)
    {
        EventDispatcher::get_instance().trigger_event(event);
    }
}