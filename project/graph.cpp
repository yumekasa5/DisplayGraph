#include "graph.h"
#include "ui_graph.h"

#include <QStylePainter>
#include <QStyleOptionFocusRect>

Graph::Graph(QMultiMap<Axis*, Plot*>& plotMap, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Graph),
    mPlotMap(plotMap)
{
    ui->setupUi(this);

    setBackgroundRole(QPalette::Dark);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->disconnect();

    mAxes = new Axes(0, this);

    connect(xAxis(), &Axis::visibleChanged, this, &Graph::onXAxesVisibleChange);
    connect(xAxis(), &Axis::autoScaleChanged, this, &Graph::onAutoScaleUpdate);
    connect(xAxis(), &Axis::minMaxChanged, this, &Graph::onRefresh);
}

Graph::~Graph()
{
    delete ui;
}

void Graph::setPlot(Plot *plot, Axis *yAxis)
{
    mPlotMap.insert(yAxis, plot);
    mAxes->yAxes().append(yAxis);

    // Plot
    connect(plot, &Plot::refreshed, this, &Graph::onRefresh);
    connect(plot, &Plot::autoScaleUpdated, this, &Graph::onAutoScaleUpdate);
    connect(plot, &Plot::dataUpdated, this, &Graph::onDataUpdate);

    // Y Axis
    connect(yAxis, &Axis::visibleChanged, this, &Graph::onYAxesVisibleChange);
    connect(yAxis, &Axis::autoScaleChanged, this, &Graph::onAutoScaleUpdate);
    connect(yAxis, &Axis::minMaxChanged, this, &Graph::onRefresh);

    mVisibleYAxesCount = 0;
    foreach (Axis* yAxis, mAxes->yAxes()) {
        if(yAxis->visible()){
            mVisibleYAxesCount++;
        }
    }

    refreshPixMap();
}

void Graph::setPlot(int column, Axis *yAxis)
{
    setPlot(plot(column), yAxis);
}

Axis *Graph::xAxis()
{
    return mAxes->xAxis();
}

void Graph::paintEvent(QPaintEvent *event)
{
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, mPixMap);

    if(hasFocus()){
        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.backgroundColor = palette().dark().color();
        painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
    }

}

void Graph::resizeEvent(QResizeEvent *event)
{
    refreshPixMap();
}

QMap<Axis *, Plot*> &Graph::plots()
{
    return mPlotMap;
}

Plot *Graph::plot(int column)
{
    return mPlots.value(column, nullptr);
}

void Graph::onRefresh()
{
    refreshPixMap();
}

void Graph::setModel(QAbstractItemModel *model)
{
    mPlots.clear();
    if(model){
        int section = 1;
        QVariant header;
        do{
            header = model->headerData(section, Qt::Horizontal);
            if(header.toString().size()){
                mPlots.insert(section, new Plot(section, model, this));
            }
            section++;

        }while(header.toString().size());
    }
    mModel = model;

    connect(model, &QAbstractItemModel::modelReset, this, &Graph::onResetModel);
    connect(model, &QAbstractItemModel::rowsInserted, this, &Graph::onRowsInsert);
    connect(model, &QAbstractItemModel::rowsAboutToBeRemoved, this, &Graph::onRowsAboutToBeRomove);
    connect(model, &QAbstractItemModel::rowsRemoved, this, &Graph::onRowsRemove);
    connect(model, &QAbstractItemModel::dataChanged, this, &Graph::onDataChange);

}

void Graph::append(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    bool minMaxChanged = false;
    for(int column = topLeft.column(); column <= bottomRight.column(); column++){
        if(column != 0){ // Y Axis
            for(int row = topLeft.row(); row <= bottomRight.row(); row++){
                QModelIndex yIndex = mModel->index(row, column);
                QModelIndex xIndex = mModel->index(row, 0);
                minMaxChanged += mPlots.value(column)->calculateMinMaxData(yIndex, xIndex);
            }
        }
    }

    if(minMaxChanged){
        emit mPlots.value(bottomRight.column())->autoScaleUpdated();
    }
    else{
        emit mPlots.value(bottomRight.column())->dataUpdated();
    }
}

/**
 * @fn
 * 各軸の調整
 * @brief 要約説明
 * @param (min) 引数の説明
 * @param (max) 引数の説明
 * @return 最大最小値の更新を行うフラグ
 * @detail 詳細な説明
 */
void Graph::onAutoScaleUpdate()
{
    bool updateGrid = false;
    Axis* xAxis = mAxes->xAxis();
    if(xAxis->autoScale()){
        if(xAxis->autoScaleAdjustX(mPlotMap.values())){
            updateGrid = true;
        }
    }

    foreach(Axis* yAxes, mAxes->yAxes()){
        if(yAxes->autoScale()){
            if(yAxes->auotScaleAdjustY(mPlotMap.values(yAxes))){
                updateGrid = true;
            }
        }
    }

    if(updateGrid){
        onRefresh();
    }
    else{
        onDataUpdate();
    }

}

/**
 * @fn
 * 各軸の調整
 * @brief 要約説明
 * @param (min) 引数の説明
 * @param (max) 引数の説明
 * @return 最大最小値の更新を行うフラグ
 * @detail 詳細な説明
 */
void Graph::onDataUpdate()
{
    QPainter painter(&mPixMap);
    drawCurves(&painter);
    update();
}

/**
 * @fn
 * 各軸の調整
 * @brief 要約説明
 * @param (min) 引数の説明
 * @param (max) 引数の説明
 * @return 最大最小値の更新を行うフラグ
 * @detail 詳細な説明
 */
void Graph::onYAxesVisibleChange(bool visible)
{
    visible ? mVisibleYAxesCount++ : mVisibleYAxesCount--;
    refreshPixMap();
}

/**
 * @fn
 * 各軸の調整
 * @brief 要約説明
 * @param (min) 引数の説明
 * @param (max) 引数の説明
 * @return 最大最小値の更新を行うフラグ
 * @detail 詳細な説明
 */
void Graph::onXAxesVisibleChange(bool visible)
{
    refreshPixMap();

}


/**
 * @fn
 * 各軸の調整
 * @brief 要約説明
 * @param (min) 引数の説明
 * @param (max) 引数の説明
 * @return 最大最小値の更新を行うフラグ
 * @detail 詳細な説明
 */
void Graph::onDataChange(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    for(int column = topLeft.column(); column <= bottomRight.column(); column++){
        if(column != 0){
            for(int row = topLeft.row(); row <= bottomRight.row(); row++){
                this->mPlots.value(column)->recalculateMinMaxData(true);
            }
        }
    }
    onAutoScaleUpdate();
    onRefresh();

}

void Graph::onRowsInsert(const QModelIndex &parent, int first, int last)
{
    bool minMaxChanged = false;
    bool isAppendModel = true;

    QMapIterator<int, Plot*> i(mPlots);
    while(i.hasNext()){
        i.next();
        int column = i.key();
        if(column != 0){
            if((first + 1) != i.value()->count()){
                isAppendModel = false;
            }

            for(int row = first; row <= last; row++){
                QModelIndex yIndex = mModel->index(row, column, parent);
                QModelIndex xIndex = mModel->index(row, 0, parent);
                minMaxChanged += i.value()->calculateMinMaxData(yIndex, xIndex);
            }
        }
    }

    if(isAppendModel){
        if(minMaxChanged){
            onAutoScaleUpdate();
        }
        else{
            if(minMaxChanged){
                onAutoScaleUpdate();
            }
            onRefresh();
        }

    }
}

void Graph::onRowsAboutToBeRomove(const QModelIndex &parent, int first, int last)
{
    QMapIterator<int, Plot*> i(mPlots);
    while (i.hasNext()) {
        i.next();
        int column = i.key();
        if (column != 0) { // Y Axis
            for (int row = first; row <= last; row++) {
                QModelIndex yIndex = mModel->index(row, column, parent);
                QModelIndex xIndex = mModel->index(row, 0, parent);
                i.value()->checkMinMaxDeleteData(yIndex, xIndex);
            }
        }
    }
}

void Graph::onRowsRemove(const QModelIndex &parent, int first, int last)
{
    bool minMaxChanged = false;

    QMapIterator<int, Plot*> i(mPlots);
    while(i.hasNext()){
        i.next();
        int column = i.key();
        if(column != 0){
            minMaxChanged += i.value()->recalculateMinMaxData();
        }
    }
    if(minMaxChanged){
        onAutoScaleUpdate();
    }
    onRefresh();
}

void Graph::onResetModel()
{
    QMapIterator<int, Plot*> i(mPlots);
    while(i.hasNext()){
        i.next();
        i.value()->clear();
    }
    onAutoScaleUpdate();
    onRefresh();
}

void Graph::refreshPixMap()
{
    mPixMap = QPixmap(size());
    mPixMap.fill(Qt::black);

    QPainter painter(&mPixMap);
    painter.initFrom(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    drawGrid(&painter);
    drawCurves(&painter);
    update();
}

void Graph::drawGrid(QPainter *painter)
{
    //TODO: SiPrefixes class has not been completed.
    QFontMetrics fm(font());

    QVector<QString> labels;
//    SiPrefixes siLabel;
    int yAxisMargin = 0;
    foreach(Axis* yAxis, mAxes->yAxes()){
        yAxis->clearMaxTickLabelWidth();
        if(yAxis->visible()){
            for(int j = 0; j <= yAxis->numTicks(); ++j){
                double labelVal = yAxis->min() + (j * yAxis->span() / yAxis->numTicks());
//                siLabel.setValue(labelVal);
//                labels.append(siLabel.text());
//                yAxis->setMaxTickLabelWidth(fm.width(siLabel.text()) + TickMarksWidth + 5);
            }
            yAxisMargin += yAxis->maxTickLabelWidth();
        }
    }

    QRect rect(yAxisMargin + Margin,
               Margin,
               width() - (yAxisMargin + (2 * Margin)),
               height() - (TickMarksWidth + (xAxis()->visible() * fm.height() + (2 * Margin))));
    mRect = rect;

    if(!rect.isValid()){
        return;
    }

    QPen quiteDark = QPen(palette().dark().color().light(), 1, Qt::DotLine);
    QPen light = QPen(palette().light().color(), 0.4);

    if(mAxes->xAxis()->visible()){
        int previousTextEndPoint = std::numeric_limits<int>::min();
        for(int i = 0; i <= mAxes->xAxis()->numTicks(); ++i){
            int x = rect.left() + (i * (rect.width() - 1) / mAxes->xAxis()->numTicks());
            double lavelVal = mAxes->xAxis()->min();
//            siLabel.setvalue(lavelVal);
//            int textWidth = fm.width(siLabel.text());
            int textWidth = 0;
            int textXPoint = x - textWidth / 2;

            // X Ticks
            painter->setPen(light);
            painter->drawLine(x, rect.top(), x, rect.bottom());
            painter->drawLine(x, rect.bottom(), x, rect.bottom());

            if(previousTextEndPoint < textXPoint){ // ラベルが重なる場合は表示しない
                // X Ticks
                painter->drawLine(x, rect.bottom(), x, rect.bottom());
                painter->drawLine(x, rect.bottom(), x, rect.bottom() + TickMarksWidth);

                // X Label
                painter->drawText(textXPoint,
                                  rect.bottom() + TickMarksWidth,
                                  textWidth,
                                  fm.height(),
                                  Qt::AlignTop,
                                  QString("temp")); //! TODO: to be fixed.
//                                  siLabel.text());
                previousTextEndPoint = x + textWidth / 2 + 5;
            }
        }
    }

    int yAxesNum = 0;
    int yAxisLabelsOffset = 0;
    int yAxisTicksOffset = 0;
    foreach (const Axis* yAxis, mAxes->yAxes()) {
        if(yAxis->visible()){
            int previousTextYTopPoint = std::numeric_limits<int>::max();
            yAxisLabelsOffset += yAxis->maxTickLabelWidth();
            for(int j = 0; j <= yAxis->numTicks(); ++j){
                int y = rect.bottom() - (j * (rect.height() - 1) / yAxis->numTicks());

                if(yAxesNum == 0){ // first
                    // Grid Line
                    painter->setPen(light);
                    painter->drawLine(rect.left(), y, rect.height(), y);
                }

                // Ticks
                QPen pen(yAxis->lineColor(), 1.0);
                painter->setPen(pen);
                painter->drawLine(rect.left() - yAxisTicksOffset - TickMarksWidth, y,
                                  rect.left() - yAxisTicksOffset, rect.bottom());

                // Label
                painter->setPen(light);
                QString label = labels.takeFirst();
                if((y + fm.height() / 2) < previousTextYTopPoint){
                    int textYPoint =  y - fm.height() / 2;
                    painter->drawText(rect.left() - yAxisLabelsOffset,
                                      textYPoint,
                                      yAxis->maxTickLabelWidth() - TickMarksWidth,
                                      fm.height(),
                                      Qt::AlignRight,
                                      label);
                    previousTextYTopPoint = textYPoint;
                }
            }
            yAxisTicksOffset = yAxisLabelsOffset;
            yAxesNum++;
        }
    }
    painter->drawRect(rect.adjusted(0, 0, -1, -1));

    QMapIterator<Axis*, Plot*> i(mPlotMap);
    while(i.hasNext()){
        i.next();
        Plot* plot = i.value();
        plot->clearPlottedPoint();
    }
}

void Graph::drawCurves(QPainter *painter)
{

    if(!mRect.isValid()){
        return;
    }
    painter->setClipRect(mRect.adjusted(+1, +1, -1, -1));

    QMapIterator<Axis*, Plot*> i(mPlotMap);
    while(i.hasNext()){
        i.next();

        Plot* plot = i.value();
        if(plot->visible()){
            Axis* yAxis = i.key();
            int plotEndPoint = plot->count();
            QPolygonF polyline(plotEndPoint - plot->plottedPoint());

            int n = 0;
            for(int j = plot->plottedPoint(); j < plotEndPoint; ++j){
                double dx = plot->xData(j) - mAxes->xAxis()->min();
                double dy = plot->yData(j) - yAxis->min();
                double x = mRect.left() + (dx * (mRect.width() - 1))
                                              / mAxes->xAxis()->span();
                double y = mRect.bottom() - (dy * (mRect.height() - 1))
                                              / yAxis->span();
                polyline[n++] = QPointF(x, y);
            }
            plot->setPlottedPoint(plotEndPoint - 1);

            QPen pen(plot->lineColor(), plot->lineWidth());
            painter->setPen(pen);
            painter->drawPolyline(polyline);
        }
    }
}
