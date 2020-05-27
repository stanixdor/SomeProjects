/*****************************************************************/
/*Author: Joaquin Bilbao				                         */
/*Brief description:	    									 */
/*****************************************************************/ 
#pragma once
#include "Engine/Platform/EventSystem/type_info.h"
#include "Engine/Platform/EventSystem/event.h"

#include <vector>
#include <ostream>
#include <iostream>
#include <algorithm>

namespace Messaging
{
    /*
    class Listener
    {
    public:
        virtual ~Listener(){}

        virtual void handle_event(const Event & event);
        EventHandler mHandler;

    };
    */


    class EventDispatcher
    {
    public:
        // public interface

        static EventDispatcher& get_instance();

        void trigger_event(const Event& mEvent);

        void trigger_event_for_handler(EventHandler& objectsEventHanlder, const Event& mEvent);
        
        void subscribe(EventHandler& objectsEventHanlder, const TypeInfo& type);

        void unsubscribe(EventHandler& objectsEventHanlder, const TypeInfo& type);

        void clear();

        friend std::ostream& operator<<(std::ostream& output, const EventDispatcher& dispatcher);

    private:
        std::map<std::string, std::vector<EventHandler*> > subscriberList;

        // not copyable, in C++11 they should be = delete to delete the copy constructor and assignment operator
        EventDispatcher(const EventDispatcher &) = delete;
        void operator=(const EventDispatcher &) = delete;

    protected:
        // only with the getInstance function can be instantiable
        EventDispatcher(){}
    };

    void trigger_event(const Event& event);

}