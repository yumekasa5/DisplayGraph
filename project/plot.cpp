#include "plot.h"

/**
 * @fn
 * コンストラクタ
 * @brief Plotクラスコンストラクタ
 * @param [int] section
 * @param [QAbstractItemModel] *model
 * @param [QObject] *parent
 * @detail 詳細な説明
 */
Plot::Plot(int section, QAbstractItemModel *model, QObject *parent)
    : QObject{parent},
    mSection(section),
    mModel(model),
    mVisible(true),
    mLineColor(Qt::red),
    mLineWidth(1.0),
    mMinData(std::numeric_limits<qreal>::max(), std::numeric_limits<qreal>::max()),
    mMaxData(std::numeric_limits<qreal>::min(), std::numeric_limits<qreal>::min()),
    mPlottedCount(0),
    mIsRecalculateMinMax(false)
{

}

QPointF Plot::minData() const
{
    return mMinData;
}

QPointF Plot::maxData() const
{
    return mMaxData;
}

bool Plot::visible() const
{
    return mVisible;
}

void Plot::setVisible(bool visible)
{
    if(mVisible != visible){
        mVisible = visible;
        emit refreshed();
    }
}

QColor Plot::lineColor() const
{
    return mLineColor;
}

void Plot::setLineColor(const QColor &lineColor)
{

}

qreal Plot::lineWidth() const
{
    return mLineWidth;
}

void Plot::setLineWidth(qreal lineWidth)
{
    if(mLineWidth != lineWidth){
        mLineWidth = lineWidth;
        emit refreshed();
    }
}

int Plot::count() const
{
    return mModel->rowCount();
}

void Plot::clear()
{

}

double Plot::yData(int index) const
{
    return mModel->index(index, mSection).data().toDouble();
}

double Plot::xData(int index) const
{
    return mModel->index(index, 0).data().toDouble();
}

int Plot::plottedPoint() const
{

}

void Plot::setPlottedPoint(int plottedCount)
{

}

void Plot::clearPlottedPoint()
{
    mPlottedCount = 0;
}

/**
 * @fn
 * 最大最小値を各軸に対して更新する
 * @brief 要約説明
 * @param (yIndex) 引数の説明
 * @param (xIndex) 引数の説明
 * @return (minMaxChange) 最大値または最小値の更新実績フラグ
 * @sa 参照すべき関数を書けばリンクが貼れる
 * @detail 詳細な説明
 */
bool Plot::calculateMinMaxData(const QModelIndex &yIndex, const QModelIndex &xIndex)
{
    bool minMaxChange = false;

    // X Axis
    double xData = xIndex.data().toDouble();
    if(mMaxData.x() < xData){
        mMaxData.setX(xData);
        minMaxChange = true;
    }
    if(mMinData.x() > xData){
        mMinData.setX(xData);
        minMaxChange = true;
    }

    // Y Axis
    double yData = yIndex.data().toDouble();
    if(mMaxData.y() < yData){
        mMaxData.setY(yData);
        minMaxChange = true;
    }
    if(mMinData.y() > yData){
        mMinData.setY(yData);
        minMaxChange = true;
    }
    return minMaxChange;
}

/**
 * @fn
 * 各軸の再計算が必要かどうかチェックを行う
 * @brief 要約説明
 * @param (yIndex) 引数の説明
 * @param (xIndex) 引数の説明
 * @return 最大最小値の更新を行うフラグ
 * @sa 参照すべき関数を書けばリンクが貼れる
 * @detail 詳細な説明
 */
void Plot::checkMinMaxDeleteData(const QModelIndex &yIndex, const QModelIndex &xIndex)
{
    bool recalculateX = true;
    bool recalculateY = true;

    // X Axis
    double xData = xIndex.data().toDouble();
    if(mMinData.x() < xData && xData < mMaxData.x()){
        recalculateX = false;
    }

    // Y Axis
    double yData = yIndex.data().toDouble();
    if(mMinData.y() < yData && yData < mMaxData.y()){
        recalculateY = false;
    }

    mIsRecalculateMinMax = (recalculateX || recalculateY);

}

/**
 * @fn
 * 各軸の最大値・最小値の更新が必要な場合は、処理を行う
 * @brief 要約説明
 * @param (isForce) 強制的に実行するフラグ, default:false
 * @return 最大最小値の更新実績フラグ
 * @sa 参照すべき関数を書けばリンクが貼れる
 * @detail 詳細な説明
 */
bool Plot::recalculateMinMaxData(bool isForce)
{
    if(mIsRecalculateMinMax || isForce){
        clear();
        double data;
        for(int row = 0; row < count(); row++){
            //Y Axis
            data = mModel->index(row, mSection).data().toDouble();
            if(mMaxData.y() < data){
                mMaxData.setY(data);
            }
            if(mMinData.y() > data){
                mMinData.setY(data);
            }

            //X Axis
            data = mModel->index(row, 0).data().toDouble();
            if(mMaxData.x() < data){
                mMaxData.setX(data);
            }
            if(mMinData.x() > data){
                mMinData.setX(data);
            }
        }
        return true;
    }
    return false;
}
