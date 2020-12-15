#include "gridwidget.h"

#include <QMouseEvent>
#include <QEvent>

#include <QDebug>

GridWidget::GridWidget(QWidget *parent) : QLabel(parent)
{ }

GridWidget::~GridWidget()
{ }

bool GridWidget::eventFilter(QObject *object, QEvent *event)
{
    for (int i=0; i<m_vector.size(); ++i)
    {
        if (m_vector[i] == object)
        {
            QMouseEvent *me = dynamic_cast<QMouseEvent*>(event);
            if (me != nullptr)
            {
                if (me->type() == QEvent::MouseButtonPress && m_moveIndex == -1)
                {
                    m_moveIndex = i;
                    m_offsetPos = me->pos();
                    m_movePos = m_vector[i]->pos();

                    if (m_isAnimation)
                        m_vector[i]->setFixedSize(m_size.width()*1.3, m_size.height()*1.3);
                }
                else if (me->type() == QEvent::MouseButtonRelease && m_moveIndex != -1)
                {
                    if (m_isAnimation)
                        m_vector[i]->setFixedSize(m_size.width(), m_size.height());
                    m_vector[m_moveIndex]->move(m_movePos);
                    m_moveIndex = -1;
                }
                break;
            }
        }
    }

    return QWidget::eventFilter(object, event);
}

void GridWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_moveIndex != -1)
    {
        m_vector[m_moveIndex]->move(event->pos() - m_offsetPos);

        for (int i=0; i<m_vector.size(); ++i)
        {
            if (i != m_moveIndex && m_vector[i]->geometry().contains(event->pos()))
            {
                QPoint ptmp = m_vector[i]->pos();
                m_vector[i]->move(m_movePos);
                m_movePos = ptmp;

                QWidget *tmp = m_vector[m_moveIndex];
                m_vector[m_moveIndex] = m_vector[i];
                m_vector[i] = tmp;

                m_moveIndex = i;

                break;
            }
        }
    }

    return QWidget::mouseMoveEvent(event);
}

void GridWidget::resizeEvent(QResizeEvent *event)
{
    rearrangeSubWidget();

    return QWidget::resizeEvent(event);
}

QMargins GridWidget::margins() const
{
    return m_margins;
}

QSize GridWidget::size() const
{
    return m_size;
}

QSize GridWidget::spacing() const
{
    return m_spacing;
}

int GridWidget::count() const
{
    return m_vector.size();
}

bool GridWidget::contains(QWidget *widget) const
{
    return m_vector.contains(widget);
}

bool GridWidget::contains(const QString &objectName) const
{
    return (indexOf(objectName) == -1 ? false : true);
}

void GridWidget::append(QWidget* widget)
{
    widget->setFixedSize(m_size);
    widget->setParent(this);
    widget->installEventFilter(this);
    m_vector.append(widget);
    rearrangeSubWidget();
}

void GridWidget::setSize(QSize size)
{
    m_size = size;

    rearrangeSubWidget();
}

void GridWidget::setMargins(QMargins margins)
{
    m_margins = margins;

    rearrangeSubWidget();
}

void GridWidget::setSpacing(QSize spaceing)
{
    m_spacing = spaceing;

    rearrangeSubWidget();
}

void GridWidget::setRowSpacing(int spaceing)
{
    m_spacing.setHeight(spaceing);

    rearrangeSubWidget();
}

void GridWidget::setColumnSpacing(int spaceing)
{
    m_spacing.setWidth(spaceing);

    rearrangeSubWidget();
}

int GridWidget::indexOf(QWidget* widget, int from) const
{
    return m_vector.indexOf(widget, from);
}

int GridWidget::indexOf(const QString &objectName, int from) const
{
    for (int i=0; i<m_vector.size(); ++i)
    {
        if (i >= from && m_vector[i]->objectName() == objectName)
            return i;
    }

    return -1;
}

void GridWidget::insert(int i, QWidget* widget)
{
    widget->setFixedSize(m_size);
    widget->setParent(this);
    widget->installEventFilter(this);
    m_vector.insert(i, widget);
    rearrangeSubWidget();
}

const QWidget *GridWidget::at(int index) const
{
    return m_vector.at(index);
}

QWidget *GridWidget::takeAt(int index)
{
    QWidget *ret = nullptr;

    ret = m_vector.takeAt(index);

    rearrangeSubWidget();

    return ret;
}

void GridWidget::removeAt(int index)
{
    delete m_vector[index];

    m_vector.removeAt(index);

    rearrangeSubWidget();
}

bool GridWidget::removeOne(QWidget *widget)
{
    bool ret = false;

    ret = m_vector.removeOne(widget);

    if (ret)
        delete widget;

    rearrangeSubWidget();

    return ret;
}

bool GridWidget::removeOne(const QString &objectName)
{
    bool ret = false;

    for (auto item : m_vector)
    {
        if (item->objectName() == objectName)
        {
            ret = m_vector.removeOne(item);

            if (ret)
                delete item;

            rearrangeSubWidget();

            break;
        }
    }

    return ret;
}

void GridWidget::clear()
{
    qDeleteAll(m_vector);

    m_vector.clear();

    rearrangeSubWidget();
}

void GridWidget::setAnimationEnabled(bool enable)
{
    m_isAnimation = enable;
}

void GridWidget::rearrangeSubWidget()
{
    int w       = this->width();

    int x_start = m_margins.left();
    int x_end   = w - m_margins.right();

    int y_start = m_margins.top();

    int x       = x_start;
    int y       = y_start;

    int rows = 0;
    for (auto item : m_vector)
    {
        item->move(x, y);

        x += m_size.width() + m_spacing.width();

        if ((x + m_size.width()/2) >= x_end && item != m_vector.last())
        {
            ++ rows;
            x = x_start;
            y += m_size.height() + m_spacing.height();
        }
    }

     setMinimumHeight(m_margins.top() + m_margins.bottom() + (rows+1) * (m_size.height()+m_spacing.height()) - m_spacing.height());
}
