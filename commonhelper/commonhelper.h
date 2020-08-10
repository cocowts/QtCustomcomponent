#ifndef COMMONHELPER_H
#define COMMONHELPER_H

#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QFile>
#include <QObject>
#include <QProcess>
#include <QRegExp>
#include <QSettings>
#include <QStringLiteral>
#include <QTime>

/**
 * @brief 公共辅助类
 * 1. 设置皮肤样式
 * 2. 判断是否是IP地址
 * 3. 模糊延时
 * 4. 窗体剧中显示
 * 5. 设置为开机启动
 * 6. 设置为应用重启
 */
class CommonHelper : public QObject
{
public:
    CommonHelper() = delete;
    CommonHelper(const CommonHelper&) = delete;
    CommonHelper &operator=(const CommonHelper&) = delete;

    // 设置皮肤样式
    static void setStyleSheet(const QString &styleSheet)
    {
        QFile file(styleSheet);
        qApp->setStyleSheet(file.readAll());
        file.close();
    }

    // 判断是否是IP地址
    static bool isIP(const QString &ip)
    {
        QRegExp regExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
        return regExp.exactMatch(ip);
    }

    // 模糊延时
    static void sleep(unsigned long msec)
    {
        QTime deiTime = QTime::currentTime().addMSecs(msec);
        while (QTime::currentTime() < deiTime)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }

    // 窗体剧中显示
    static void formInCenter(QWidget &widget)
    {
        QDesktopWidget w;
        int deskWidth = w.width();
        int deskHeight = w.height();

        QPoint centerPoint((deskWidth - widget.width()) / 2, (deskHeight - widget.height()) / 2);
        widget.move(centerPoint);
    }

    // 设置为开机启动
    static void autoRunWithSystem(bool isAutoRun, const QString &appName, const QString &appPath)
    {
        QSettings reg(QStringLiteral("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);

        if (isAutoRun)
            reg.setValue(appName, appPath);
        else
            reg.setValue(appName, "");
    }

    // 应用重启
    static void reboot()
    {
        QString program = QApplication::applicationFilePath();
        QStringList argument = QApplication::arguments();
        QString workDirectory = QDir::currentPath();
        QProcess::startDetached(program, argument, workDirectory);
        QApplication::exit();
    }
};

#endif // COMMONHELPER_H
