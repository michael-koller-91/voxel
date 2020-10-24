#pragma once

#include <map>
#include <memory>
#include <string>
#include <typeinfo>

#include "process.h"

class ProcessManager
{
public:
    void Update();

    template <typename T>
    void RegisterProcess(int priority)
    {
        // assert that the process has not already been registered
        assert(processes_.find(priority) == processes_.end() && "Process already registered.");
        // priority is also the identifier of a process
        process_type_to_priority_map_.insert({typeid(T).name(), priority});
        // pointer to process
        processes_.insert({priority, std::make_shared<T>()});
    }

    template <typename T>
    int PriorityOf()
    {
        return process_type_to_priority_map_[typeid(T).name()];
    }

    // cast from base class (IProcess) to derived class
    template <typename T>
    std::shared_ptr<T> GetProcess()
    {
        return std::static_pointer_cast<T>(processes_[PriorityOf<T>()]);
    }

private:
    std::map<std::string, int> process_type_to_priority_map_;
    std::map<int, std::shared_ptr<IProcess>> processes_;
};
