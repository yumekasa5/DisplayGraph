#include "axis.h"

Axis::Axis(QObject *parent)
    : QObject{parent}
{

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
