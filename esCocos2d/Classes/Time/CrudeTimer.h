#pragma  once 
#include <ctime>

#define Clock CrudeTimer::Instance()

class CrudeTimer
{
 
public:

  static CrudeTimer* Instance();
 
  long GetCurrentTimes();
 
};
 