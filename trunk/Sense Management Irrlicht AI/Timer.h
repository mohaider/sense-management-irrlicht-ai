#ifndef TIMER_H
#define TIMER_H

class Timer
{
public:
   Timer();
   ~Timer();

   
   float timeSinceCreation();

      float timeSinceLastFrame();


private:
  
   float m_timeOfLastCall;
};

#endif