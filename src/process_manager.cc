#pragma once

#include "process_manager.h"

void ProcessManager::update()
{
    for (auto const& process: processes_)
    {
        process.second->update();
    }
}
