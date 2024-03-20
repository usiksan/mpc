#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Propagation.h"

#include <QDebug>

Propagation pTen;
Propagation pOven;
Propagation pEphir;

QCPRange rangeFromVector( const QVector<double> &v )
  {
  if( v.count() ) {
    double aMin = v.at(0);
    double aMax = aMin;
    for( auto val : v ) {
      if( val < aMin ) aMin = val;
      if( val > aMax ) aMax = val;
      }
    return QCPRange( aMin, aMax );
    }
  return QCPRange();
  }


int N = 120;
QVector<double> gx(N), gy(N); //Массивы координат точек


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  {
  ui->setupUi(this);

  //Рисуем график y=x*x
  //Сгенерируем данные
  //Для этого создадим два массива точек:
  //один для сохранения x координат точек,
  //а второй для y соответственно
  double a = -1; //Начало интервала, где рисуем график по оси Ox
  double b =  1; //Конец интервала, где рисуем график по оси Ox
  double h = 0.01; //Шаг, с которым будем пробегать по оси Ox
  //int N = (b - a) / h + 2; //Вычисляем количество точек, которые будем отрисовывать

  //Вычисляем наши данные
  int i=0;
  //Пробегаем по всем точкам
  for (double X = 0; X < 120; X += 1) {
    gx[i] = X;
    gy[i] = 27;//Формула нашей функции
    i++;
  }
  ui->plt->clearGraphs();//Если нужно, но очищаем все графики

  //Добавляем один график в widget
  ui->plt->addGraph();

  //Говорим, что отрисовать нужно график по нашим двум массивам x и y
  ui->plt->graph(0)->setData(gx, gy);

  //Подписываем оси Ox и Oy
  ui->plt->xAxis->setLabel("x");
  ui->plt->yAxis->setLabel("y");

  //Установим область, которая будет показываться на графике
  ui->plt->xAxis->setRange( rangeFromVector(gx) );//Для оси Ox
  ui->plt->yAxis->setRange( QCPRange( 20, 300 ) );//Для оси Oy

  //qDebug() << rangeFromVector(x);

  pTen.link( pOven )->link( pEphir );
  pTen.mKq = 8000;
  pTen.mKcool = 100;
  pOven.mKq = 350;
  pOven.mKcool = 100;
  ui->mTenTemper->setText( QString::number(pTen.temper()) );
  ui->mTenQ->setText( QString::number(pTen.mQ) );
  ui->mOvenTemper->setText( QString::number(pOven.temper()) );
  ui->mOvenQ->setText( QString::number(pOven.mQ) );
  connect( ui->mTenKQ, &QLineEdit::textEdited, this, [] (const QString &txt) { pTen.mKq = txt.toInt(); } );
  connect( ui->mTenKCool, &QLineEdit::textEdited, this, [] (const QString &txt) { pTen.mKcool = txt.toInt(); } );
  connect( ui->mOvenKQ, &QLineEdit::textEdited, this, [] (const QString &txt) { pOven.mKq = txt.toInt(); } );
  connect( ui->mOvenKCool, &QLineEdit::textEdited, this, [] (const QString &txt) { pOven.mKcool = txt.toInt(); } );

  QTimer *timer = new QTimer();
  connect( timer, &QTimer::timeout, this, [this] () {
    //Пропорционально-интегральный регулятор
    static int summ;

    //Получить значение ошибки
    int error = ui->mTarget->text().toInt() - pOven.temper();

    //Обновить сумму
    if( qAbs(error) > 3 )
      summ = qBound( 0, summ + error, 10000 );
    if( qAbs(error) > 200 )
      summ = 0;

    //Вычислить воздействие
    int mImpact = (error * 25 >> 3) + (summ * 2 >> 7);

    //Ограничить воздействие
    mImpact = qBound( 0, mImpact*2, 2000 );

    ui->mSumm->setText( QString::number(summ) );
    ui->mPwm->setText( QString::number(mImpact) );
    //Получить pwm
    //int pwm = ui->mPwm->text().toInt();
    pTen.heating( mImpact );

    ui->mTenTemper->setText( QString::number(pTen.temper()) );
    ui->mTenQ->setText( QString::number(pTen.mQ) );
    ui->mOvenTemper->setText( QString::number(pOven.temper()) );
    ui->mOvenQ->setText( QString::number(pOven.mQ) );

    //Обновить график
    for( int i = 0; i < 119; i++ )
      gy[i] = gy[i+1];
    gy[119] = double(pOven.temper()) / 10.0;
    ui->plt->graph(0)->setData(gx, gy);
    ui->plt->replot();
    });

  timer->start( 1000 );
  }

MainWindow::~MainWindow()
  {
  delete ui;
  }

