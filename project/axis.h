#ifndef AXIS_H
#define AXIS_H

#include <QObject>

#include "plot.h"

class Axis : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Axis)
public:
    explicit Axis(QObject *parent = nullptr);
    virtual ~Axis(){}

    bool visible() const;
    void setVisible(bool visible);

    qreal min() const;
    void setMin(qreal value);
    qreal max() const;
    void setMax(qreal value);
    bool autoScale() const;
    void setAutoScale(bool autoscale);

    QColor lineColor() const;
    void setLineColor(const QColor &lineColor);

    enum UpdateAdjust{
        AutoScale,
        Forced
    };

    void adjust(qreal min, qreal max, UpdateAdjust updateadjust = AutoScale);
    bool autoScaleAdjustX(QList<Plot*> axes);
    bool auotScaleAdjustY(QList<Plot*> axes);

    int numTicks() const;
    qreal span() const;

    void clearMaxTickLabelWidth();
    void setMaxTickLabelWidth(int width);
    int maxTickLabelWidth() const;

signals:
    void visibleChanged(bool visible);
    void autoScaleChanged(bool autoScale);
    void minMaxChanged();

private:
    bool adjustAxis(qreal min, qreal max, UpdateAdjust updateadjust);

    struct AdjustSettings{
        qreal min;
        qreal max;
        int numTicks;
    }mAdjustSettings;

    bool mVisible;
    bool mAutoScale;
    QString mCaption;
    QColor mLineColor;
    int mMaxTickLabelWidth;

};

#endif // AXIS_H
