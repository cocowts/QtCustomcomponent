#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QLabel>
#include <QMargins>
#include <QRect>
#include <QVector>

/* GridWidget 实现的功能
 * 1. 可任意插入、追加 QWidget 类组控件 [推荐QLabel]
 * 2. 可自由拖动子组件，并保持移动后所有子相对组件位置不变
 * 3. 可设置子组件大小、子组件行列间距和子组件与父组件边框的间距
 * 4. 可自动根据父组件宽度调整子组件布局
 * 5. 可对父组件管理的子组件进行查找、移除等操作
 */

/*
 * 说明：当父组件开始管理子组件时，子组件的生命周期便交由父组件管理，外部不可进行随意 delete 操作
 *          可使用 removeAt removeOne clear 进行处理
 * 说明：子组件原功能不受任何影响，因此可单独配置后结合父组件实现更丰富的功能
 */

class GridWidget : public QLabel
{
    Q_OBJECT

    Q_PROPERTY(QMargins margins READ margins WRITE setMargins)
    Q_PROPERTY(QSize    size    READ size    WRITE setSize)
    Q_PROPERTY(QSize    spacing READ spacing WRITE setSpacing)

public:
    explicit GridWidget(QWidget *parent = nullptr);
    ~GridWidget();

    QMargins margins()                                   const;     // 返回子组件与父组件间距
    QSize size()                                         const;     // 返回子组件大小
    QSize spacing()                                      const;     // 返回子组件间行、列间距

    int  count()                                         const;     // 返回当前父组件中子组件的数量
    bool contains(QWidget *widget)                       const;     // 返回当前父组件中是否包含此组件 （参数：widget 地址）
    bool contains(const QString &objectName)             const;     // 返回当前父组件中是否包含此组件 （参数：widget objectName）
    int indexOf(QWidget* widget, int from = 0)           const;     // 返回值在父组件中第一次出现的索引位置，从索引位置向前搜索 （参数：widget 地址）
    int indexOf(const QString &objectName, int from = 0) const;     // 返回值在父组件中第一次出现的索引位置，从索引位置向前搜索 （参数：widget objectName）
    const QWidget *at(int index)                         const;     // 返回父组件中索引位置i处的项

public slots:
    void append(QWidget* widget);                                    // 将参数组件附加到父组件。
    void setSize(QSize size);                                        // 设置子组件大小
    void setMargins(QMargins margins);                               // 设置子组件与父组件的边距
    void setSpacing(QSize spaceing);                                 // 设置子组件间的行、列距离
    void setRowSpacing(int spaceing);                                // 设置子组件的行间距
    void setColumnSpacing(int spaceing);                             // 设置子组件的列间距
    void insert(int i, QWidget* widget);                             // 在指定索引处插入参数组件
    QWidget *takeAt(int index);                                      // 删除索引位置i处的子组件并返回它
    void removeAt(int index);                                        // 移除指定索引处的子组件
    bool removeOne(QWidget *widget);                                 // 移除指定索引处的子组件 （参数：widget 地址）
    bool removeOne(const QString &objectName);                       // 移除指定索引处的子组件 （参数：widget objectName）
    void clear();                                                    // 清空父组件中的所有子组件
    void setAnimationEnabled(bool enable);                           // 设置鼠标点击动画使能

protected:
    bool eventFilter(QObject *object, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event)          override;
    void resizeEvent(QResizeEvent *event)            override;

    void rearrangeSubWidget();

private:
    QMargins          m_margins        = {10, 10, 10, 10};
    QSize             m_size           = {60, 60};
    QSize             m_spacing        = {10, 10};
    QVector<QWidget*> m_vector;

    bool m_isAnimation = false;

    QPoint m_movePos;
    QPoint m_offsetPos;

    int m_moveIndex = -1;
};

#endif // GRIDWIDGET_H
