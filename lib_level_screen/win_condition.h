#pragma once

class WinCondition {
   public:
    virtual ~WinCondition(){};
    virtual bool isMet() = 0;
};