#pragma once

#include <JEngine/JEngine.h>
#include <JEngine/Util/Ref.h>
#include <JEngine/Audio/Sound.h>
#include <JEngine/Audio/Audio.h>
#include <random>

#define CELL 64

#define S_PLAYER {0, 0}
#define S_DOOR {1, 0}
#define S_MONSTER {2, 0}
#define S_WALL {0, 2}
#define S_KEY {1, 2}
#define S_CHEST {2, 2}
#define S_CHEST_OPEN {2, 3}
#define S_FIRE {2, 1}
#define S_AIM {0, 3}
#define S_AMULET {3, 2}
#define S_HEART {0, 1}
#define S_EMPTY_HEART {1, 1}
#define S_EMPTY_FIRE {3, 1}
#define S_BLUE_FIRE {1, 3}

#define FIRE_SPEED 40

static int random(int start, int end)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(start, end);
    
    return dist(gen);
}

#define SO_CHEST 0
#define SO_DEAD 1
#define SO_DOOR 2
#define SO_DROP 3
#define SO_FIRE 4
#define SO_HURT 5
#define SO_WIN 6
#define SO_KILL 7