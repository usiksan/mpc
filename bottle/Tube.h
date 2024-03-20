#ifndef TUBE_H
#define TUBE_H

#include <QQueue>

class Tube
  {
    double         mFlap;
    QQueue<double> mTube;
  public:
    Tube();

    void setFlap( double f ) { mFlap = f > 0 ? f : 0; }

    double act() { mTube.enqueue(mFlap); return mTube.dequeue(); }
  };

#endif // TUBE_H
