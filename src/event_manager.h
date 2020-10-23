/*
TODO:
Currently, unsubcribing followed by re-subscribing increases the next_process_id counter and does not reuse the old
process ID. This could be changed using a queue similar to what is done for entities. However, there should not be exist
many processes anyway and unsubcribing and re-subscribing should not happen.
*/
#pragma once

#include <functional>
#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>

#include "callback_map.h"
#include "type.h"

class EventManager
{
public:
    EventManager();

    template <typename TEvent, typename TProcess>
    void Subscribe(std::shared_ptr<TProcess> process)
    {
        const std::string& event_type_name = typeid(TEvent).name();
        const std::string& process_type_name = typeid(TProcess).name();

        // initialization if the event type is not known yet
        if (event_type_to_id_map_.find(event_type_name) == event_type_to_id_map_.end())
        {
            // event type has not already been registered
            event_type_to_id_map_.insert({event_type_name, next_event_id_});
            
            // make (a pointer to) a map which can hold callbacks for the new event type
            callbacks_map_.insert({next_event_id_, std::make_shared<CallbackMap<TEvent>>()});

            ++next_event_id_;
        }

        // initialization if the process type is not known yet
        if (process_type_to_id_map_.find(process_type_name) == process_type_to_id_map_.end())
        {
            // process type has not already been registered
            process_type_to_id_map_.insert({process_type_name, next_process_id_});

            ++next_process_id_;
        }

        // add the subscriber, i.e., insert callback in CallbackMap
        auto cb = std::bind(static_cast<void(TProcess::*)(TEvent&)>(&TProcess::Receive), process, std::placeholders::_1);
        GetCallbacks<TEvent>().insert({ProcessIdOf<TProcess>(), cb});
    }

    template <typename TEvent, typename TProcess>
    void Unsubscribe(std::shared_ptr<TProcess> process)
    {
        const std::string& event_type_name = typeid(TEvent).name();
        const std::string& process_type_name = typeid(TProcess).name();

        // nothing to remove if the event type is not known
        if (event_type_to_id_map_.find(event_type_name) == event_type_to_id_map_.end())
        {
            return;
        }

        // nothing to remove if the process type is not known
        if (process_type_to_id_map_.find(process_type_name) == process_type_to_id_map_.end())
        {
            return;
        }

        // remove the subscriber, i.e., erase callback from CallbackMap
        auto& cb = GetCallbacks<TEvent>();
        auto it = cb.find(ProcessIdOf<TProcess>());
        if (it != cb.end())
        {
            cb.erase(it);
        }
    }

    template <typename T>
    EventIdType EventIdOf()
    {
        return event_type_to_id_map_[typeid(T).name()];
    }

    template <typename T>
    ProcessIdType ProcessIdOf()
    {
        return process_type_to_id_map_[typeid(T).name()];
    }

    template <typename T>
    void Publish(T& event)
    {
        for (auto const& cb : GetCallbacks<T>())
        {
            cb.second(event);
        }
    }

    template <typename T>
    std::map<ProcessIdType, std::function<void(T)>>& GetCallbacks()
    {
        return std::static_pointer_cast<CallbackMap<T>>(callbacks_map_[EventIdOf<T>()])->map_;
    }

private:
    EventIdType next_event_id_;
    ProcessIdType next_process_id_;
    std::unordered_map<std::string, EventIdType> event_type_to_id_map_;
    std::unordered_map<std::string, ProcessIdType> process_type_to_id_map_;
    std::unordered_map<EventIdType, std::shared_ptr<ICallbackMap>> callbacks_map_;
};
