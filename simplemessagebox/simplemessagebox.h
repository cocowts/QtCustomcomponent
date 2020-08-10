#ifndef COMMONMESSAGEBOX_H
#define COMMONMESSAGEBOX_H

#include <QDialog>
#include <QFile>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPoint>
#include <QPushButton>
#include <QVBoxLayout>

/**
 * @brief 简易的的消息对话框
 * 1. 消息提示对话框
 * 2. 询问提示对框框
 * 3. 错误提示对话框
 * 使用时需要注意资源文件路径！！
 */
class SimpleMessageBox : public QDialog
{
    Q_OBJECT
public:
    /* 消息提示对话框 */
    static int infomationMessageBox(const QString &text);
    /* 询问提示对框框 */
    static int questionMessageBox(const QString &text);
    /* 错误提示对话框 */
    static int errorMessageBox(const QString &text);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    SimpleMessageBox();

    void initUI();
    void initControl();

private:
    QPoint mousePoint;
    bool mousePressed = false;

    QLabel m_iconLbl;
    QLabel m_titleLbl;
    QPushButton m_closeBtn;
    QLabel m_mainLbl;
    QLabel m_textLbl;
    QPushButton m_okBtn;
    QPushButton m_cancleBtn;
};

#endif // COMMONMESSAGEBOX_H
