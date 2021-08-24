#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QScroller>
#include <QTimer>
#include <QPropertyAnimation>

/* 滑动窗口
 * 1. 支持小部件追加
 * 2. 支持获取小部件的数量
 * 3. 支持获取当前小部件的索引和小部分地址
 * 4. 支持查询给定的小部件索引
 * 5. 支持设置首页小部件
 * 6. 支持小部件移除
 * 7. 支持背景图片是否可滑动切换 ！！！
 *     SliderWidget(QWidget *parent = nullptr, const QString &picture = "");
 *     当 picture 传入图片地址时， 背景图片将一直保持不可切换
 */

class SliderWidget : public QWidget
{
    Q_OBJECT

public:
    SliderWidget(QWidget *parent = nullptr, const QString &picture = "");
    ~SliderWidget();

    int addWidget(QWidget *page);
    int count() const;
    int currentIndex() const;
    QWidget *currentWidget() const;
    int indexOf(QWidget *widget) const;

public slots:
    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget *widget);

    void removeWidget(int index);
    void removeWidget(QWidget *widget);

protected:
    void resizeEvent(QResizeEvent *ev) override;

protected slots:
    void onScrollerStateChanged(QScroller::State state);
    void onSliderTimerTimeout();

signals:
    void currentChanged(int index);

private:
    void initUi();
    void initCtrl();
    void updateIndicator(int index);

private:
    QWidget *m_pMainWidget;
    QScrollArea *m_pScrollArea;
    QHBoxLayout *m_pMainLayout;
    QWidget *m_pIndicatorWidget;
    QHBoxLayout *m_pIndicatorLayout;

    QPropertyAnimation *m_pAnimation;
    QTimer *m_pSlidingTimer;

    bool m_sliderFlag = false;
    int m_sliderPressedValue = 0;
    int m_sliderReleaseValue = 0;
    int m_curPageIndex = -1;
    int m_nextPageIndex = -1;

    QString m_background;
};
#endif // MAINWINDOW_H
