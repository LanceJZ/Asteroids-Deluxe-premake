#include "Timer.h"

void Timer::Update(float deltaTime)
{
    Seconds += deltaTime;
}

void Timer::Set(float amountInSeconds)
{
    Amount = amountInSeconds;
}

void Timer::Reset()
{
    Seconds = 0;
}

void Timer::Reset(float amoutInSeconds)
{
    Reset();
    Set(amoutInSeconds);
}

bool Timer::Elapsed()
{
    return Seconds > Amount;
}
