#include "axes.h"

Axes::Axes(QObject *parent)
    : QObject{parent}
{

}

Axis* Axes::xAxis()
{
    return mXAxis;
}

Axis* Axes::yAxes(int key)
{
    return mYAxes[key];
}

QVector<Axis *> &Axes::yAxes()
{
    return mYAxes;
}
