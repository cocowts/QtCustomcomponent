#ifndef POPUPMESSAGEBOX_H
#define POPUPMESSAGEBOX_H

#include <QLabel>
#include <QMouseEvent>
#include <QPoint>
#include <QPushButton>
#include <QTimer>
#include <QWidget>
#include <QPointer>
#include <QColor>
#include <QHBoxLayout>

/**
 * @brief 右下角提示对对话框
 * 1. 可设置标题
 * 2. 可设置标题图标
 * 3. 可设置提示文本
 * 4. 可设置提示图片
 * 5. 可设置提示动画
 * 6. 可设置操作按钮是否可见
 * 7. 可设置对话框入场动画时间
 * 8. 可设置对话框停留时间
 * 9. 可随时终止一个对话框
 * 10. 对话框退出时，发送退出信号
 * 11. 确定按钮被点击时，发送确认信息
 * 12. 取消按钮被点击时，发送取消信号
 * 使用时需要注意资源文件路径！！
 */
class PopupMessageBox : public QWidget
{
    Q_OBJECT

public:
    enum AnimationType
    {
        Animation_Drop = 0u,
        Animation_Shark,
        Animation_Opacity,
        Animation_Up,
    };

    Q_PROPERTY(QString title READ title WRITE setTitle);
    Q_PROPERTY(QPixmap titleIcon READ titleIcon WRITE setTitleIcon);
    Q_PROPERTY(QString text READ text WRITE setText);
    Q_PROPERTY(QPixmap icon READ icon WRITE setIcon400x270_400x215);
    Q_PROPERTY(bool isButtonVisible READ isButtonVisible WRITE setButtonVisible);

public:
    ~PopupMessageBox();

    /* 创建一个提示对话框 */
    static QPointer<PopupMessageBox> createInstance(QWidget *parent = nullptr);
    /* 获取标题 */
    QString title() const;
    /* 获取标题图标 */
    QPixmap titleIcon() const;
    /* 获取提示文本 */
    QString text() const;
    /* 获取提示动画 */
    QPixmap icon() const;
    /* 获取提示动画 */
    const QMovie *movie() const;
    /* 获取操作按钮是否可见 */
    bool isButtonVisible() const;

public slots:
    /* 设置标题 */
    void setTitle(const QString &text);
    /* 设置标题图标 */
    void setTitleIcon(const QPixmap &pixmap);
    /* 设置提示文本 */
    void setText(const QString &text);
    /* 设置提示文本颜色 */
    void setTextColor(const QColor &color);
    /* 设置提示图片 */
    void setIcon400x270_400x215(const QPixmap &pximap);
    /* 设置提示动画 */
    void setMovie400x270_400x215(const QString &fileName);
    /* 设置操作按钮是否可见 */
    void setButtonVisible(bool visible);
    /* 启动提示对话框 （keepTimeMs=-1；对话框不主动退出）*/
    void start(AnimationType type = Animation_Opacity, int keepTimeMs = 2000, int enterTimeMs = 1000);
    /* 终止提示对话框 */
    void terminate();

signals:
    /* 对话框退出 */
    void quit();
    /* 确定按钮被点击 */
    void accepted();
    /* 取消按钮被点击 */
    void rejected();

private:
    explicit PopupMessageBox(QWidget *parent = nullptr);
    void initUI();
    void initControl();
    void setFixedHeight(int h);
    void setFixedSize(const QSize &s);
    void setFixedSize(int w, int h);
    void setFixedWidth(int w);
    void animationDrop(int msecs);
    void animationShake(int msecs);
    void animationOpacity(int msecs);
    void animationUp(int msecs);
    void animationQuit(int msecs = 500);
    void animationKeep(int msecs);

private:
    QLabel m_iconLbl;
    QLabel m_titleLbl;
    QPushButton m_closeBtn;
    QLabel m_mainLbl;
    QPushButton m_okBtn;
    QPushButton m_cancleBtn;
    QHBoxLayout *mpBtnLayout = nullptr;

    QTimer m_timer;
};

#endif // POPUPMESSAGEBOX_H
