#include "simplemessagebox.h"

SimpleMessageBox::SimpleMessageBox()
{
    this->setWindowFlag(Qt::FramelessWindowHint);

    initUI();
    initControl();
}

void SimpleMessageBox::initUI()
{
    m_iconLbl.setObjectName(QStringLiteral("iconLbl"));
    m_titleLbl.setObjectName(QStringLiteral("titleLbl"));
    m_closeBtn.setObjectName(QStringLiteral("closeBtn"));
    m_mainLbl.setObjectName(QStringLiteral("mainLbl"));
    m_textLbl.setObjectName(QStringLiteral("textLbl"));
    m_okBtn.setObjectName(QStringLiteral("okBtn"));
    m_cancleBtn.setObjectName(QStringLiteral("cancleBtn"));

    //================ 组件属性设置
    m_iconLbl.setFixedSize(20, 20);
    m_titleLbl.setFixedHeight(24);

    m_closeBtn.setFlat(true);
    m_closeBtn.setFixedSize(14, 14);
    m_closeBtn.setToolTip(QStringLiteral("关闭"));

    m_mainLbl.setScaledContents(true);
    m_mainLbl.setFixedSize(64, 64);

    m_textLbl.setWordWrap(true);
    m_textLbl.setAlignment(Qt::AlignCenter);

    m_okBtn.setFocus();
    m_okBtn.setFixedSize(80, 35);
    m_cancleBtn.setFixedSize(80, 35);
    m_okBtn.setText(QStringLiteral("确定(&O)"));
    m_cancleBtn.setText(QStringLiteral("取消(&C)"));
    m_okBtn.setCursor(Qt::PointingHandCursor);
    m_cancleBtn.setCursor(Qt::PointingHandCursor);

    //================ 组件布局
    QWidget *titleWidget = new QWidget(this);
    QHBoxLayout *pLayout1 = new QHBoxLayout;
    titleWidget->setObjectName(QStringLiteral("titleWidget"));
    pLayout1->addSpacing(5);
    pLayout1->addWidget(&m_iconLbl);
    pLayout1->addSpacing(5);
    pLayout1->addWidget(&m_titleLbl);
    pLayout1->addStretch();
    pLayout1->addWidget(&m_closeBtn);
    pLayout1->addSpacing(5);
    pLayout1->setMargin(0);
    titleWidget->setFixedHeight(30);
    titleWidget->setLayout(pLayout1);

    QHBoxLayout *pLayout2 = new QHBoxLayout;
    pLayout2->addWidget(&m_mainLbl);
    pLayout2->addSpacing(10);
    pLayout2->addWidget(&m_textLbl);

    QHBoxLayout *pLayout3 = new QHBoxLayout;
    pLayout3->addStretch(true);
    pLayout3->addWidget(&m_okBtn);
    pLayout3->addWidget(&m_cancleBtn);

    QVBoxLayout *pLayout4 = new QVBoxLayout;
    pLayout4->addLayout(pLayout2);
    pLayout4->addLayout(pLayout3);
    pLayout4->setContentsMargins(10, 0, 10, 0);

    QVBoxLayout *pMainLayout = new QVBoxLayout;
    pMainLayout->addWidget(titleWidget);
    pMainLayout->addSpacing(10);
    pMainLayout->addLayout(pLayout4);
    pMainLayout->setContentsMargins(0, 0, 0, 10);

    //================ 样式表设置
    QFile file(QStringLiteral(":/simplemessagebox/simplemessagebox/black.qss"));
    file.open(QFile::ReadOnly);
    this->setStyleSheet(file.readAll());
    file.close();

    //================
    setLayout(pMainLayout);
    setMinimumSize(300, 40);
}

void SimpleMessageBox::initControl()
{
    connect(&m_okBtn, &QPushButton::clicked, [=](){
        done(QDialog::Accepted);
        close();
    });

    connect(&m_cancleBtn, &QPushButton::clicked, [=](){
        done(QDialog::Rejected);
        close();
    });

    connect(&m_closeBtn, &QPushButton::clicked, [=](){
        done(-1);
        close();
    });
}

int SimpleMessageBox::infomationMessageBox(const QString &text)
{
    SimpleMessageBox box;

    box.m_mainLbl.setPixmap(QPixmap(":/simplemessagebox/simplemessagebox/info.png"));
    box.m_titleLbl.setText(QStringLiteral("提示"));
    box.m_cancleBtn.setVisible(false);
    box.m_textLbl.setText(text);

    return box.exec();
}

int SimpleMessageBox::questionMessageBox(const QString &text)
{
    SimpleMessageBox box;

    box.m_mainLbl.setPixmap(QPixmap(":/simplemessagebox/simplemessagebox/question.png"));
    box.m_titleLbl.setText(QStringLiteral("询问"));
    box.m_textLbl.setText(text);

    return box.exec();
}

int SimpleMessageBox::errorMessageBox(const QString &text)
{
    SimpleMessageBox box;

    box.m_mainLbl.setPixmap(QPixmap(":/simplemessagebox/simplemessagebox/error.png"));
    box.m_titleLbl.setText(QStringLiteral("错误"));
    box.m_cancleBtn.setVisible(false);
    box.m_textLbl.setText(text);

    return box.exec();
}

void SimpleMessageBox::mouseMoveEvent(QMouseEvent *event)
{
    if (mousePressed)
    {
        this->move(event->globalPos() - mousePoint);
        event->accept();
    }
}

void SimpleMessageBox::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousePressed = true;
        mousePoint = event->globalPos() - this->pos();
        event->accept();
    }
}

void SimpleMessageBox::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    mousePressed = false;
}

