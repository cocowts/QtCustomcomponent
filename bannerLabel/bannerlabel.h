#ifndef BANNERLABEL_H
#define BANNERLABEL_H

#include <QButtonGroup>
#include <QFont>
#include <QHBoxLayout>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QSpacerItem>
#include <QTimer>
#include <QToolButton>
#include <QWidget>

/**
 * 横幅轮播空间
 * 参考：https://zhuanlan.zhihu.com/p/64257336
 *      https://www.cnblogs.com/feiyangqingyun/p/10990260.html
 * 1. 可随机添加图片路径及提示信息集合
 * 2. 可清空图片路径及提示信息集合
 * 3. 可设置图片切换时间
 * 4. 可设置提示信息文字颜色
 * 5. 可设置指示器位置 左边 + 中间 + 右边
 * 6. 可设置鼠标悬停停止播放
 * 7. 可设置组件固定大小（需要与图片尺寸匹配）
 * 8. 图片切换时，可发出切换信号
 * 9. 当前图片被点击时，可发出点击信号
 *
 * 使用时需注意图标资源文件路径 border-image:url(:/image/point.png);
 */

class BannerLabel : public QWidget
{
    Q_OBJECT

    Q_ENUMS(IndicatorPosition)

    Q_PROPERTY(int interval READ getInterval WRITE setInterval);
    Q_PROPERTY(bool hoverStop READ getHoverStop WRITE setHoverStop);
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor);
    Q_PROPERTY(IndicatorPosition indicatorPosition READ getIndicatorPosition WRITE setIndicatorPosition);

public:
    enum IndicatorPosition {
        IndicatorLeft = Qt::AlignLeft | Qt::AlignVCenter,
        IndicatorCenter = Qt::AlignHCenter | Qt::AlignVCenter,
        IndicatorRight = Qt::AlignRight | Qt::AlignVCenter
    };

public slots:
    /* 设置图片切换间隔 */
    void setInterval(int _interval);
    /* 设置鼠标悬停停止播放 */
    void setHoverStop(bool isHoverStop);
    /* 设置提示信息文字颜色 */
    void setTextColor(const QColor &_textColor);
    /* 设置指示器位置 左边 + 中间 + 右边 */
    void setIndicatorPosition(IndicatorPosition _indicatorPosition);
    /* 清空图片路径及提示信息集合 */
    void imageClear();

public:
    explicit BannerLabel(QWidget *parent = nullptr);
    virtual ~BannerLabel();

    /* 随机添加图片路径及提示信息集合 */
    void addImage(const QList<std::pair<QString, QString>> &imagePairList);
    /* 获取图片路径及提示信息集合 */
    const QList<std::pair<QString, QString>> &getImages() const;
    /* 可设置组件固定大小（需要与图片尺寸匹配） */
    void setFixedSize(const QSize &s);
    void setFixedSize(int w, int h);
    /* 获取图片切换间隔 */
    int getInterval() const;
    /* 鼠标悬停是否停止播放  */
    bool getHoverStop() const;
    /* 获取提示信息文字颜色 */
    QColor getTextColor() const;
    /* 获取指示器位置 左边 + 中间 + 右边 */
    IndicatorPosition getIndicatorPosition() const;

signals:
    /* 图片切换时，发出切换信号 */
    void imageChange(int index, const QString &imagePath, const QString &tipText);
    /* 图片被点击时，发出点击信号 */
    void imageclicked(int index, const QString &imagePath, const QString &tipText);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void onImageValueChange(const QVariant &variant);
    void onExtendBtnValueChange(const QVariant &varient);
    void onShrinkBtnValueChange(const QVariant &varient);
    void onButtonClicked(int index);
    void onImageTimerOut();

private:
    void initUI();
    void initControl();
    void initAnimation();

private:
    QWidget *m_pIndicatorWidget          = nullptr;
    QHBoxLayout *m_pIndicatorLayout      = nullptr;

    QPropertyAnimation *m_pImageAnimation      = nullptr;
    QPropertyAnimation *m_pShrinkBtnAnimation  = nullptr;
    QPropertyAnimation *m_pExtendBtnAnimation  = nullptr;
    QParallelAnimationGroup *m_pAnimationGroup = nullptr;

    QButtonGroup *m_pBtnGroup = new QButtonGroup(this);
    QList<std::pair<QString, QString>> m_imagePairList;
    QTimer *m_pTimer = new QTimer(this);

    const static int BtnShrinkWidth = 16;
    const static int BtnExtendWidth = 48;

    int m_currIndex = 0;
    int m_preIndex = 0;
    int m_offset = 0;

    int interval = 2000;
    bool hoverStop = false;
    QColor textColor = Qt::gray;
    IndicatorPosition indicatorPosition = IndicatorLeft;
};

#endif // BANNERLABEL_H
