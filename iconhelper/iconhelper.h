#ifndef ICONHELPER_H
#define ICONHELPER_H

#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QLabel>
#include <QMutex>
#include <QObject>
#include <QAbstractButton>
#include <QListWidgetItem>

/**
 * @brief 字体图标辅助类
 * 1. 可设置 Label 字体图标
 * 2. 可设置 Button 字体图标
 * 3. 可设置 ListWidgetItem 字体图标
 * 注意：使用时需注意字体集路径：
 *      QFontDatabase::addApplicationFont(":/font/fontawesome-webfont.ttf");
 */
class IconHelper : public QObject
{ 
public:
    /* 设置 Label 字体图标 */
    static void setIcon(QLabel *label, QChar c, int size)
    {
        static IconHelper *_instance = instance();

        _instance->m_iconFont.setPointSize(size);
        label->setFont(_instance->m_iconFont);
        label->setText(c);
    }
    /* 设置 Button 字体图标 */
    static void setIcon(QAbstractButton *btn, QChar c, int size)
    {
        static IconHelper *_instance = instance();

        _instance->m_iconFont.setPointSize(size);
        btn->setFont(_instance->m_iconFont);
        btn->setText(c);
    }

    /* 设置 ListWidgetItem 字体图标 */
    static void setIcon(QListWidgetItem *item, QChar c, int size)
    {
        static IconHelper *_instance = instance();

        _instance->m_iconFont.setPointSize(size);
        item->setFont(_instance->m_iconFont);
        item->setText(c);
    }

private:
    IconHelper(const IconHelper&) = delete;
    IconHelper &operator=(const IconHelper&) = delete;

    explicit IconHelper(QObject *parent = nullptr) : QObject(parent)
    {
        int fontId = QFontDatabase::addApplicationFont(":/font/fontawesome-webfont.ttf");
        QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
        m_iconFont = QFont(fontName);
    }

    static IconHelper *instance()
    {
        static QMutex mutex;
        if (m_pInstance==nullptr)
        {
            QMutexLocker locker(&mutex);
            if (m_pInstance==nullptr)
            {
                m_pInstance = new IconHelper(qApp);
            }
        }

        return m_pInstance;
    }

private:
    QFont m_iconFont;
    static IconHelper *m_pInstance;
};

IconHelper *IconHelper::m_pInstance = nullptr;

#endif // ICONHELPER_H
