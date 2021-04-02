#pragma once

class WinCondition {
   public:
    virtual void update() = 0;
    virtual bool isMet() = 0;
};