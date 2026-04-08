#include "snazzcraft-engine/input-handler/event.hpp"
#include "snazzcraft-engine/input-handler/data.hpp"

SnazzCraft::Event::Event(uint8_t IType)
    : EventData(new SnazzCraft::Data()), Type(IType)
{

}

SnazzCraft::Event::~Event()
{
    delete this->EventData;
}


