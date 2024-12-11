#include "qthrobber.h"


QThrobber::QThrobber(QWidget *parent)
    : QWidget(parent)
{
    m_throbberRotationAnimation = new QPropertyAnimation(this, "throbberRotation");
    m_throbberRotationAnimation->setDuration(10000);

    m_throbberLengthAnimation = new QPropertyAnimation(this, "throbberLength");
    m_throbberLengthAnimation->setDuration(5000);

    m_iconOpacityAnimation = new QPropertyAnimation(this, "iconOpacity");
    m_iconOpacityAnimation->setDuration(5000);

    m_iconScaleAnimation = new QPropertyAnimation(this, "iconScale");
    m_iconScaleAnimation->setDuration(5000);

    m_animationGroup = new QParallelAnimationGroup(this);
    m_animationGroup->addAnimation(m_throbberRotationAnimation);

    connect(m_animationGroup, SIGNAL(finished()), this, SLOT(onAnimationGroupFinished()));

    m_secondaryAnimationGroup = new QParallelAnimationGroup(this);
    m_secondaryAnimationGroup->addAnimation(m_iconOpacityAnimation);
    m_secondaryAnimationGroup->addAnimation(m_throbberLengthAnimation);

    connect(m_secondaryAnimationGroup, SIGNAL(finished()), this, SLOT(onSecondaryAnimationGroupFinished()));
}

QThrobber::~QThrobber()
{
    stop();
    
    if(m_animationGroup)
        delete m_animationGroup;

    if(m_secondaryAnimationGroup)
        delete m_secondaryAnimationGroup;
}

void QThrobber::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if(!m_icon.isNull())
    {
        QPixmap pixmap;

        QSize size = m_iconSize;

        qreal x = 10;
        qreal y = 10;

        //qss/qproperty overrides
        if(property("icon-right").isValid())
            x -= property("icon-right").toInt();
        if(property("icon-left").isValid())
            x += property("icon-left").toInt();

        if(property("icon-top").isValid())
            y += property("icon-top").toInt();
        if(property("icon-bottom").isValid())
            y -= property("icon-bottom").toInt();

        if(property("icon-size").isValid())
            size = property("icon-size").toSize();

        if(m_iconScaleAnimationEnabled)
        {
            size.setHeight(size.height() * m_iconScale);
            size.setWidth(size.width() * m_iconScale);
        }

        if((m_iconAlignment & Qt::AlignHCenter) == Qt::AlignHCenter)
            x = (width() / 2) - (size.width() / 2);

        else if((m_iconAlignment & Qt::AlignLeft) == Qt::AlignLeft)
            x = 10;

        else if((m_iconAlignment & Qt::AlignRight) == Qt::AlignRight)
            x = (width() - size.width()) - 10;

        if((m_iconAlignment & Qt::AlignVCenter) == Qt::AlignVCenter)
            y = (height() / 2) - (size.height() / 2);

        else if((m_iconAlignment & Qt::AlignTop) == Qt::AlignTop)
            y = 10;

        else if((m_iconAlignment & Qt::AlignBottom) == Qt::AlignBottom)
            y = (height() - size.height()) - 10;

        if(isEnabled())
            pixmap = (m_icon.pixmap(size, QIcon::Normal, QIcon::On));
        else
            pixmap = (m_icon.pixmap(size, QIcon::Disabled, QIcon::On));

        if(m_iconOpacityAnimationEnabled)
            painter.setOpacity(m_iconOpacity);

        painter.drawPixmap(QRect(x, y, size.width(), size.height()), pixmap);

        painter.setOpacity(1);
    }

    painter.translate(qCeil(m_throbberWidth / 2), qCeil(m_throbberWidth / 2));

    QPen pen;

    if(m_throbberPenSet)
        pen = m_throbberPen;
    else
    {
        pen.setWidth(m_throbberWidth);
        pen.setColor(m_throbberColor);
        pen.setCapStyle(Qt::FlatCap);
    }

    //draw background
    painter.setPen(pen);
    painter.drawArc(QRectF(m_paddingX, m_paddingY, m_pathRadius - m_throbberWidth, m_pathRadius - m_throbberWidth), m_throbberRotation * 16, (m_throbberLength * 16));

    //draw filler
    painter.setPen(pen);
    //painter.drawArc(QRectF(m_paddingX, m_paddingY, m_pathRadius - m_pathWidth, m_pathRadius - m_pathWidth), 225 * 16, ((270*16) * m_progress) * -1);

    //painter.translate(0,0);

    qreal fontScale = m_fontSize;

    QFont font(QGuiApplication::font());
    font.setPointSize(fontScale);
    QFontMetrics fontMetrics(font);

    qreal textWidth = fontMetrics.horizontalAdvance(m_text);
    qreal textHeight = fontMetrics.lineSpacing();

    int x = 0, y = 0;

    //Get true center
    x = width() / 2;
    x -= textWidth / 2;

    y = (height()/2);

    if((m_iconAlignment & Qt::AlignHCenter) == Qt::AlignHCenter)
        x = (width() / 2) - (textWidth / 2);

    else if((m_iconAlignment & Qt::AlignLeft) == Qt::AlignLeft)
        x = 10;

    else if((m_iconAlignment & Qt::AlignRight) == Qt::AlignRight)
        x = (width() - textWidth) - 10;

    if((m_iconAlignment & Qt::AlignVCenter) == Qt::AlignVCenter)
        y = (height() / 2);

    else if((m_iconAlignment & Qt::AlignTop) == Qt::AlignTop)
        y = 10;

    else if((m_iconAlignment & Qt::AlignBottom) == Qt::AlignBottom)
        y = (height() - textHeight) - 10;

    pen.setColor(m_textColor);

    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(QPoint(x,y), m_text);
    painter.end();

    QWidget::paintEvent(event);
}

void QThrobber::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    calculateRect();
}

QThrobber::Speed QThrobber::speed() const
{
    return m_speed;
}

void QThrobber::setSpeed(Speed speed)
{
    if (m_speed == speed)
        return;

    m_speed = speed;

    switch(speed){

        case Fastest:
            m_throbberRotationAnimation->setDuration(1000);
            m_throbberLengthAnimation->setDuration(500);
            m_iconOpacityAnimation->setDuration(500);
            m_iconScaleAnimation->setDuration(500);
            break;
        case Fast:
            m_throbberRotationAnimation->setDuration(5000);
            m_throbberLengthAnimation->setDuration(1000);
            m_iconOpacityAnimation->setDuration(1000);
            m_iconScaleAnimation->setDuration(1000);
            break;
        case Normal:
            m_throbberRotationAnimation->setDuration(10000);
            m_throbberLengthAnimation->setDuration(5000);
            m_iconOpacityAnimation->setDuration(5000);
            m_iconScaleAnimation->setDuration(5000);
            break;
        case Slow:
            m_throbberRotationAnimation->setDuration(15000);
            m_throbberLengthAnimation->setDuration(7500);
            m_iconOpacityAnimation->setDuration(7500);
            m_iconScaleAnimation->setDuration(7500);
            break;
        case Slowest:
            m_throbberRotationAnimation->setDuration(20000);
            m_throbberLengthAnimation->setDuration(10000);
            m_iconOpacityAnimation->setDuration(10000);
            m_iconScaleAnimation->setDuration(10000);
            break;
    }

    if(m_isPlaying)
    {
        stop();
        start();
    }

    emit speedChanged();
}

Qt::Alignment QThrobber::iconAlignment() const
{
    return m_iconAlignment;
}

void QThrobber::setIconAlignment(const Qt::Alignment &iconAlignment)
{
    if (m_iconAlignment == iconAlignment)
        return;

    m_iconAlignment = iconAlignment;
    update();
    emit iconAlignmentChanged();
}

QColor QThrobber::textColor() const
{
    return m_textColor;
}

void QThrobber::setTextColor(const QColor &textColor)
{
    if (m_textColor == textColor)
        return;
    m_textColor = textColor;
    update();
    emit textColorChanged();
}

QString QThrobber::text() const
{
    return m_text;
}

void QThrobber::setText(const QString &text)
{
    if (m_text == text)
        return;
    m_text = text;
    update();
    emit textChanged();
}

Qt::Alignment QThrobber::textAlignment() const
{
    return m_textAlignment;
}

void QThrobber::setTextAlignment(const Qt::Alignment &textAlignment)
{
    if (m_textAlignment == textAlignment)
        return;
    m_textAlignment = textAlignment;
    emit textAlignmentChanged();
}

bool QThrobber::iconScaleAnimationEnabled() const
{
    return m_iconScaleAnimationEnabled;
}

void QThrobber::setIconScaleAnimationEnabled(bool iconScaleAnimationEnabled)
{
    if (m_iconScaleAnimationEnabled == iconScaleAnimationEnabled)
        return;

    m_iconScaleAnimationEnabled = iconScaleAnimationEnabled;
    emit iconScaleAnimationEnabledChanged();

    if(!m_iconScaleAnimationEnabled)
        m_secondaryAnimationGroup->removeAnimation(m_iconScaleAnimation);
    else
        m_secondaryAnimationGroup->addAnimation(m_iconScaleAnimation);
}

void QThrobber::onAnimationGroupFinished()
{
    if(m_isPlaying)
    {
        m_throbberRotationAnimation->setStartValue(0);
        m_throbberRotationAnimation->setEndValue(360 * 16);

        m_animationGroup->start();
    }
}

bool QThrobber::iconOpacityAnimationEnabled() const
{
    return m_iconOpacityAnimationEnabled;
}

void QThrobber::setIconOpacityAnimationEnabled(bool iconOpacityAnimationEnabled)
{
    if (m_iconOpacityAnimationEnabled == iconOpacityAnimationEnabled)
        return;

    m_iconOpacityAnimationEnabled = iconOpacityAnimationEnabled;
    emit iconOpacityAnimationEnabledChanged();

    if(!m_iconOpacityAnimationEnabled)
        m_secondaryAnimationGroup->removeAnimation(m_iconOpacityAnimation);
    else
        m_secondaryAnimationGroup->addAnimation(m_iconOpacityAnimation);
}

int QThrobber::throbberWidth() const
{
    return m_throbberWidth;
}

void QThrobber::setThrobberWidth(int throbberWidth)
{
    if (m_throbberWidth == throbberWidth)
        return;

    m_throbberWidth = throbberWidth;
    calculateRect();
    update();
    emit throbberWidthChanged();
}

QPen QThrobber::throbberPen() const
{
    return m_throbberPen;
}

void QThrobber::setThrobberPen(const QPen &throbberPen)
{
    if (m_throbberPen == throbberPen)
        return;

    m_throbberPen = throbberPen;
    m_throbberPenSet = true;

    setThrobberWidth(m_throbberPen.width());

    emit throbberPenChanged();
}

QColor QThrobber::throbberColor() const
{
    return m_throbberColor;
}

void QThrobber::setThrobberColor(const QColor &throbberColor)
{
    if (m_throbberColor == throbberColor)
        return;

    m_throbberColor = throbberColor;
    update();
    emit throbberColorChanged();
}

qreal QThrobber::iconOpacity() const
{
    return m_iconOpacity;
}

void QThrobber::setIconOpacity(qreal iconOpacity)
{
    if (qFuzzyCompare(m_iconOpacity, iconOpacity))
        return;

    m_iconOpacity = iconOpacity;
    update();
    emit iconOpacityChanged();
}

void QThrobber::calculateRect()
{
    if(this->width() > this->height())
    {
        m_paddingX = this->width() - this->height();
        m_paddingX /= 2;
        m_paddingY = m_throbberWidth / 2;
        m_pathRadius = this->height() - m_throbberWidth;
    }
    else
    {
        m_paddingY = this->height() - this->width();
        m_paddingY /= 2;
        m_paddingY += m_throbberWidth / 2;
        m_paddingX = m_throbberWidth / 2;
        m_pathRadius = this->width() - m_throbberWidth;
    }
}

qreal QThrobber::iconScale() const
{
    return m_iconScale;
}

void QThrobber::setIconScale(qreal iconScale)
{
    if (qFuzzyCompare(m_iconScale, iconScale))
        return;

    m_iconScale = iconScale;
    update();
    emit iconScaleChanged();
}

qreal QThrobber::throbberLength() const
{
    return m_throbberLength;
}

void QThrobber::setThrobberLength(qreal throbberLength)
{
    if (qFuzzyCompare(m_throbberLength, throbberLength))
        return;

    m_throbberLength = throbberLength;
    update();
    emit throbberLengthChanged();
}

void QThrobber::onSecondaryAnimationGroupFinished()
{
    if(m_animationGroup->state() != QParallelAnimationGroup::Running)
    {
        m_throbberLength = -270;
        m_throbberLengthGrowing = false;
        return;
    }

    if(!m_throbberLengthGrowing)
    {
        m_throbberLengthAnimation->setStartValue(-10);
        m_throbberLengthAnimation->setEndValue(-270);

        m_iconOpacityAnimation->setStartValue(0.25);
        m_iconOpacityAnimation->setEndValue(1);

        m_iconScaleAnimation->setStartValue(0.25);
        m_iconScaleAnimation->setEndValue(1);

        m_throbberLengthGrowing = true;
    }
    else
    {
        m_throbberLengthAnimation->setStartValue(-270);
        m_throbberLengthAnimation->setEndValue(-10);

        m_iconOpacityAnimation->setStartValue(1);
        m_iconOpacityAnimation->setEndValue(0.25);

        m_iconScaleAnimation->setStartValue(1);
        m_iconScaleAnimation->setEndValue(0.25);

        m_throbberLengthGrowing = false;
    }

    m_secondaryAnimationGroup->start();
}

bool QThrobber::isPlaying() const
{
    return m_isPlaying;
}

void QThrobber::setIsPlaying(bool isPlaying)
{
    if (m_isPlaying == isPlaying)
        return;

    m_isPlaying = isPlaying;
    emit isPlayingChanged();
}

qreal QThrobber::innerThrobberRotation() const
{
    return m_innerThrobberRotation;
}

void QThrobber::setInnerThrobberRotation(qreal innerThrobberRotation)
{
    if (qFuzzyCompare(m_innerThrobberRotation, innerThrobberRotation))
        return;

    m_innerThrobberRotation = innerThrobberRotation;
    update();
    emit innerThrobberRotationChanged();
}

qreal QThrobber::throbberRotation() const
{
    return m_throbberRotation;
}

void QThrobber::setThrobberRotation(qreal throbberRotation)
{
    if (qFuzzyCompare(m_throbberRotation, throbberRotation))
        return;

    m_throbberRotation = throbberRotation;
    update();
    emit throbberRotationChanged();
}

QIcon QThrobber::icon() const
{
    return m_icon;
}

void QThrobber::start()
{
    if(m_animationGroup->state() == QParallelAnimationGroup::Running)
        m_animationGroup->stop();

    m_throbberRotationAnimation->setStartValue(0);
    m_throbberRotationAnimation->setEndValue(360 * 16);

    m_throbberLengthAnimation->setStartValue(-270);
    m_throbberLengthAnimation->setEndValue(-16);

    m_iconOpacityAnimation->setStartValue(1);
    m_iconOpacityAnimation->setEndValue(0.25);

    m_iconScaleAnimation->setStartValue(1);
    m_iconScaleAnimation->setEndValue(0.25);

    setIsPlaying(true);
    m_animationGroup->start();
    m_secondaryAnimationGroup->start();
}

void QThrobber::stop()
{
    if(m_isPlaying)
    {
        setIsPlaying(false);
        m_animationGroup->stop();
        m_secondaryAnimationGroup->stop();
    }
}

void QThrobber::setIcon(const QIcon &icon)
{
    m_icon = icon;
    update();

    emit iconChanged();
}

QSize QThrobber::iconSize() const
{
    return m_iconSize;
}

void QThrobber::setIconSize(const QSize &iconSize)
{
    if (m_iconSize == iconSize)
        return;

    m_iconSize = iconSize;
    emit iconSizeChanged();
}

qreal QThrobber::fontSize() const
{
    return m_fontSize;
}

void QThrobber::setFontSize(qreal fontSize)
{
    if (qFuzzyCompare(m_fontSize, fontSize))
        return;

    m_fontSize = fontSize;
    emit fontSizeChanged();
}

