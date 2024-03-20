#include "matplotlibcpp.h"
#include <QCoreApplication>

namespace plt = matplotlibcpp;

int main(int argc, char *argv[])
  {
  QCoreApplication a(argc, argv);

  plt::plot({1,3,2,4});
  plt::show();

  return a.exec();
  }
