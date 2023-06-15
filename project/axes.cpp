#include "axes.h"

Axes::Axes(QObject *parent)
    : QObject{parent}
{

}

Axes* Axes::xAxes()
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
