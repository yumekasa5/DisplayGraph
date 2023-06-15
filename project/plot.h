#ifndef PLOT_H
#define PLOT_H

#include <QObject>
#include <QAbstractItemModel>

class Plot : public QObject
{
    Q_OBJECT
public:
    explicit Plot(QObject *parent = nullptr);

signals:
    void refreshed();
    void autoScaleUpdated();
    void dataUpdated();

private:
    int mSection;
    QAbstractItemModel* mModel;
    bool mVisible;
//    QColor mLineColor;
    qreal mLineWidth;
    QPointF mMinData;
    QPointF mMaxData;
    int mPlottedCount;
    bool mIsRecalculateMinMax;
};

#endif // PLOT_H
