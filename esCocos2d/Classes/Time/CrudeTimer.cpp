#include "CrudeTimer.h"
#include <cassert>
#ifdef COCOS2D
#include "cocos2d.h"
using namespace cocos2d;
#endif // COCOS2D

CrudeTimer* CrudeTimer::Instance()
{
  static CrudeTimer instance;
 
  return &instance;
}

 long CrudeTimer::GetCurrentTimes()
 {
	 
	
  #ifdef COCOS2D
  struct cc_timeval tv;
  CCTime::gettimeofdayCocos2d(&tv,NULL); 
	 return tv.tv_sec * 1000 + tv.tv_usec / 1000;     
#endif // COCOS2D
	 assert(0);
	 return 0;
 
 }