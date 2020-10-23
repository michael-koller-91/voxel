#pragma once

#include "process_manager.h"

void ProcessManager::Update()
{
    for (auto const& process: processes_)
    {
        process.second->Update();
    }
}
