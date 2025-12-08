#pragma once
#include "TCB.h"
#include <queue>
using namespace std;

struct Mutex {
    bool lock;
    queue<TCB*> tasks;
};
