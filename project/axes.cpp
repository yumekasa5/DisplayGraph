#include "axes.h"

Axes::Axes(QObject *parent)
    : QObject{parent}
{

}

Axes::Axes(int numAxes, QObject *parent)
{
    mXAxis = new Axis(parent);
    for(int n = 0; n < numAxes; n++){
        mYAxes.append(new Axis(parent));
    }
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
