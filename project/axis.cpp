#include "axis.h"

Axis::Axis(QObject *parent)
    : QObject{parent},
    mAdjustSettings({0.0, 10.0, 5}),
    mVisible(true),
    mAutoScale(true),
    mCaption(QString()),
    mLineColor(Qt::white),
    mMaxTickLabelWidth(0)
{

}

bool Axis::visible() const
{
    return mVisible;
}

void Axis::setVisible(bool visible)
{
    if(mVisible != visible){
        mVisible = visible;

        emit visibleChanged(visible);
    }
}

qreal Axis::min() const
{
    return mAdjustSettings.min;
}

void Axis::setMin(qreal value)
{
    if(mAdjustSettings.min != value && !mAutoScale){
        if(adjustAxis(value, mAdjustSettings.max, Forced)){
            emit minMaxChanged();
        }
    }
}

qreal Axis::max() const
{
    return mAdjustSettings.max;
}

void Axis::setMax(qreal value)
{
    if(mAdjustSettings.max != value && !mAutoScale){
        if(adjustAxis(mAdjustSettings.min, value, Forced)){
            emit minMaxChanged();
        }
    }
}

bool Axis::autoScale() const
{
    return mAutoScale;
}

void Axis::setAutoScale(bool autoscale)
{
    if(mAutoScale != autoscale){
        mAutoScale = autoscale;

        emit autoScaleChanged(autoscale);
    }
}

QColor Axis::lineColor() const
{
    return mLineColor;
}

void Axis::setLineColor(const QColor &lineColor)
{
    if(mLineColor != lineColor){
        mLineColor = lineColor;
        emit minMaxChanged();
    }
}

/**
 * @fn
 * 各軸の調整
 * @brief 要約説明
 * @param (min) 引数の説明
 * @param (max) 引数の説明
 * @return 最大最小値の更新を行うフラグ
 * @sa 参照すべき関数を書けばリンクが貼れる
 * @detail 詳細な説明
 */
void Axis::adjust(qreal min, qreal max, UpdateAdjust updateadjust)
{
    adjustAxis(min, max, updateadjust);
}

bool Axis::autoScaleAdjustX(QList<Plot *> axes)
{
    qreal min = std::numeric_limits<qreal>::max();
    qreal max = std::numeric_limits<qreal>::min();

    foreach(Plot* plot, axes){
        if(min > plot->minData().x()){
            min = plot->minData().x();
        }
        if(max < plot->maxData().x()){
            max = plot->maxData().x();
        }
    }
    return adjustAxis(min, max, AutoScale);
}

bool Axis::auotScaleAdjustY(QList<Plot *> axes)
{
    qreal min = std::numeric_limits<qreal>::max();
    qreal max = std::numeric_limits<qreal>::min();

    foreach (Plot* plot, axes) {
        if (min > plot->minData().y()){
            min = plot->minData().y();
        }
        if (max < plot->maxData().y()){
            max = plot->maxData().y();
        }
    }

    return adjustAxis(min, max, AutoScale);
}

int Axis::numTicks() const
{
    return mAdjustSettings.numTicks;
}

qreal Axis::span() const
{
    return mAdjustSettings.max - mAdjustSettings.min;
}

void Axis::clearMaxTickLabelWidth()
{
    mMaxTickLabelWidth = 0;
}

void Axis::setMaxTickLabelWidth(int width)
{
    if(mMaxTickLabelWidth < width){
        mMaxTickLabelWidth = width;
    }
}

int Axis::maxTickLabelWidth() const
{
    return mMaxTickLabelWidth;
}

bool Axis::adjustAxis(qreal min, qreal max, UpdateAdjust updateadjust)
{
    bool updateGrid = false;
    if (updateadjust == Forced || mAutoScale) {
        const int MinTicks = 4;
        qreal grossStep = (max - min) / MinTicks;
        qreal step = pow(10.0, floor(log10(grossStep)));

        if (qIsNaN(step) || step == 0.0)
            return false;

        if ((5 * step) < grossStep)
            step *= 5;
        else if ((2 * step) < grossStep)
            step *= 2;

        mAdjustSettings.numTicks = int(ceil(max / step) - floor(min / step));
        if (mAdjustSettings.numTicks < MinTicks)
            mAdjustSettings.numTicks = MinTicks;
        min = floor(min / step) * step;
        max = ceil(max / step) * step;

        if (mAdjustSettings.min != min ||
            mAdjustSettings.max != max) {
            updateGrid = true;

            mAdjustSettings.min = min;
            mAdjustSettings.max = max;
        }
    }

    return updateGrid;
}
