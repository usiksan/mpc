#ifndef PROPAGATION_H
#define PROPAGATION_H

#include <QString>

class Propagation
  {
    const int    mTBase = 270;
    //Propagation *mPrev;
    Propagation *mNext;
    int          mDt;
  public:
    int          mQ;
    int          mKq;
    int          mKcool;

    Propagation() : mNext(nullptr), mDt(0), mQ(0), mKq(1), mKcool(1) {}

    Propagation *link( Propagation &next ) { mNext = &next; return mNext; }

    int temper() const { return mDt + mTBase; }

    void temperSet( int t )
      {

      }

    void heating( int dq )
      {
      if( mNext != nullptr ) {
        int qcool = qMax( 0, mDt - mNext->mDt ) * mKcool >> 8;
        mQ = qMax(0, mQ + dq - qcool );
        mDt = mQ * mKq >> 16;
        mNext->heating( qcool );
        }
      }
  };

#endif // PROPAGATION_H
