#pragma once
#include "GameObject.h"

#define Speed_0    0
#define Speed_1    1
#define Speed_2    2
#define Speed_3    3
#define Speed_4    4
#define Speed_5    5
#define Speed_6    6
#define Speed_7    7

class SpeedBar :
    public CGameObject
{
public:
    SpeedBar();
    ~SpeedBar() {};
    
    void Render(float x, float y, float vx);
    virtual void Render() {};
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {};

};

