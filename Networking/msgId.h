#pragma once
#include <iostream>


enum class Id : uint32_t {
    
    JoinRequest,
    JoinAccept,
    JoinDenied,
    PlayerJoined,
    PlayerLeft,
    GameStarted
    //...
};