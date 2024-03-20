#include "Tube.h"

Tube::Tube()
  : mFlap(0)
  {
  for( int i = 0; i < 100; i++ )
    mTube.enqueue(0);
  }
