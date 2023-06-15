#ifndef GRAPH_H
#define GRAPH_H

#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>
#include <QObject>
#include <QAbstractItemModel>
#include <QWidget>

#include "plot.h"
#include "axis.h"


class Plot;
class Axis;
class Axes;

namespace Ui {
class Graph;
}

class Graph : public QWidget
{
    Q_OBJECT

public:
    explicit Graph(QWidget *parent = nullptr);
    ~Graph();

    void setPlot(Plot* plot, Axis* yAxis);
    void setPlot(int column, Axis* yAxis);

    QMap<Axis*, Plot>& plots();
    Plot* plot(int column);
    Axis* xAxis();

    //Model
    void setModel(QAbstractItemModel *model);
    void append(const QModelIndex &topLeft, const QModelIndex &bottomRight);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void onRefresh();
    void onAutoScaleUpdate();
    void onDataUpdate();
    void onYAxesVisibleChange(bool visible);
    void onXAxesVisibleChange(bool visible);

    //! Model
    //!TODO:Model below



private:
    Ui::Graph *ui;

    void refreshPixMap();
    void drawGrid(QPainter *painter);
    void drawCurves(QPainter *painter);

    enum class MarkType{
        Margin = 10,
        TickMarksWidth = 5,
    };Q_ENUM(MarkType)

    //! member
    QMultiMap<Axis*, Plot*> mPlotMap;
    Axes* mAxes;
    QPixmap mPixMap;
    int mVisibleYAxesCount;
    QRect mRect;
    QAbstractItemModel *mModel;
    QMap<int, Plot*> mPlots;
};

#endif // GRAPH_H
