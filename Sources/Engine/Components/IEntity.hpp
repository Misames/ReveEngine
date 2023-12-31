#pragma once

#include <vector>
#include "IComponent.hpp"

using namespace std;

class IEntity
{
protected:
    uint64_t id;
    bool initialized = false;
    vector<IComponent *> components;

public:
    bool enabled = false;

    virtual ~IEntity() = default;
    virtual void Initialize() = 0;
    virtual void Release() = 0;
    uint64_t GetId() const;
    vector<IComponent *> GetAllComponents() const;
};