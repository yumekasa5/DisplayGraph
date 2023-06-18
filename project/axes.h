#ifndef AXES_H
#define AXES_H

#include <QObject>
#include <QVector>

#include "axis.h"

class Axes : public QObject
{
    Q_OBJECT
public:
    explicit Axes(QObject *parent = nullptr);
    explicit Axes(int numAxes, QObject *parent = nullptr);
    ~Axes();

    Axis* xAxis();
    Axis* yAxes(int key);
    QVector<Axis*>& yAxes();

private:
    Axis* mXAxis;
    QVector<Axis*> mYAxes;

};

#endif // AXES_H
