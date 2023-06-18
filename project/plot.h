#ifndef PLOT_H
#define PLOT_H

#include <QObject>
#include <QColor>
#include <QAbstractItemModel>

class Plot : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Plot)
public:
    explicit Plot(int section, QAbstractItemModel *model = 0, QObject *parent = nullptr);
    virtual ~Plot(){}

    QPointF minData() const;
    QPointF maxData() const;

    bool visible() const;
    void setVisible(bool visible);

    QColor lineColor() const;
    void setLineColor(const QColor &lineColor);

    qreal lineWidth() const;
    void setLineWidth(qreal lineWidth);

    int count() const;
    void clear();
    double yData(int index) const;
    double xData(int index) const;

    inline int plottedPoint() const;
    inline void setPlottedPoint(int plottedCount);
    void clearPlottedPoint();

    bool calculateMinMaxData(const QModelIndex &yIndex, const QModelIndex &xIndex = QModelIndex());
    void checkMinMaxDeleteData(const QModelIndex &yIndex, const QModelIndex &xIndex = QModelIndex());
    bool recalculateMinMaxData(bool isForce = false);

signals:
    void refreshed();
    void autoScaleUpdated();
    void dataUpdated();

private:
    int mSection;
    QAbstractItemModel* mModel;
    bool mVisible;
    QColor mLineColor;
    qreal mLineWidth;
    QPointF mMinData;
    QPointF mMaxData;
    int mPlottedCount;
    bool mIsRecalculateMinMax;
};

#endif // PLOT_H
