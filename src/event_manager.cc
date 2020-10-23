#pragma once

#include "event_manager.h"

EventManager::EventManager()
{
    next_event_id_ = 0;
    next_process_id_ = 0;
}
