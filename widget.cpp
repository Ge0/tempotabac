#include "widget.h"
#include <QStandardPaths>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

#define SEC_IN_MIN 60
#define SEC_IN_HOUR 60 * SEC_IN_MIN
#define SEC_IN_DAY 24 * SEC_IN_HOUR
#define SEC_IN_MONTH 31 * SEC_IN_DAY
#define SEC_IN_YEAR 366 * SEC_IN_DAY

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    loadSettings();
    calculCigarette();

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateDateTime()));
    m_timer->start(500);
}

Widget::~Widget()
{
    delete m_last;
    delete m_timer;
    delete m_settings;
}

void Widget::calculCigarette()
{
    m_cigarettePrice = m_packetPrice / ((float)m_cigarettePerPacket);
    m_cigarettePerDay = ((float)m_cigarettePerPacket)*m_packetPerDay;
}

void Widget::updateDateTime()
{
    //DateTime
    m_secs = m_last->secsTo(QDateTime::currentDateTime());
    m_mins = ((float)m_secs) / 60.0f;
    m_hours = m_mins / 60.0f;
    m_days = m_hours / 24.0f;
    m_months = m_days / 31.0f; //TODO : month are different

    //Monnay
    m_monnay = (((((m_cigarettePerDay / 24.0f)/60.0f)/60.0f) * ((float)m_secs)) * m_cigarettePrice);

    //Total DateTime
    qint64 years = 0;
    qint64 modYears = 0;

    qint64 months = 0;
    qint64 modMonths = 0;

    qint64 days = 0;
    qint64 modDays = 0;

    qint64 hours = 0;
    qint64 modHours = 0;

    qint64 mins = 0;
    qint64 modMins = 0;

    years = m_secs / ((qint64)SEC_IN_YEAR);
    modYears = m_secs % ((qint64)SEC_IN_YEAR);

    months = modYears / ((qint64)SEC_IN_MONTH);
    modMonths = modYears % ((qint64)SEC_IN_MONTH);

    days = modMonths / ((qint64)SEC_IN_DAY);
    modDays = modMonths % ((qint64)SEC_IN_DAY);

    hours = modDays / ((qint64)SEC_IN_HOUR);
    modHours = modDays % ((qint64)SEC_IN_HOUR);

    mins = modHours / ((qint64)SEC_IN_MIN);
    modMins = modHours % ((qint64)SEC_IN_MIN);

    m_totalString = QString("%1Y-%2M-%3-D %4H-%5M-%6S").arg(QString::number((int)years))
            .arg(QString::number((int)months)).arg(QString::number((int)days)).arg(QString::number((int)hours))
            .arg(QString::number((int)mins)).arg(QString::number((int)modMins));

    setAll();
}

void Widget::setAll()
{
    //DateTime
    setSecs();
    setMin();
    setHour();
    setDays();
    setMonths();
    setTotal();

    //Monnay
    setMonnay();
}

void Widget::setTotal()
{
    lineEditTotal->setText(m_totalString);
}

void Widget::setMonnay()
{
    lineEditMonnay->setText(QString::number(m_monnay));
}

void Widget::setMonths()
{
    lineEditMonth->setText(QString::number(m_months));
}

void Widget::setDays()
{
    lineEditDay->setText(QString::number(m_days));
}

void Widget::setHour()
{
    lineEditHour->setText(QString::number(m_hours));
}

void Widget::setMin()
{
    lineEditMin->setText(QString::number(m_mins));
}

void Widget::setSecs()
{
    lineEditSec->setText(QString::number(m_secs));
}

void Widget::on_lineEditPacketPerDay_textEdited()
{
    m_packetPerDay = lineEditPacketPerDay->text().toFloat();
    calculCigarette();
    //saveSettings();
}

void Widget::on_lineEditCigarettePerPacket_textEdited()
{
    m_cigarettePerPacket = lineEditCigarettePerPacket->text().toFloat();
    calculCigarette();
    //saveSettings();
}

void Widget::on_lineEditPacketPrice_textEdited()
{
    m_packetPrice = lineEditPacketPrice->text().toFloat();
    calculCigarette();
    //saveSettings();
}

void Widget::on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    m_last->setDate(dateTime.date());
    m_last->setTime(dateTime.time());
    updateDateTime();
    //saveSettings();
}

void Widget::initDefaultValue()
{
    m_last = new QDateTime(QDate(2000, 01, 01), QTime(0,0));
    dateTimeEdit->setDateTime(*m_last);

    m_packetPerDay = 0;
    m_cigarettePerPacket = 0;
    m_packetPrice = 0;

    lineEditPacketPerDay->setText(QString::number(m_packetPerDay));
    lineEditCigarettePerPacket->setText(QString::number(m_cigarettePerPacket));
    lineEditPacketPrice->setText(QString::number(m_packetPrice));
}

void Widget::loadSettings()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) ;
    QString filename = "configTabac.ini";
    QString completePathToFile = path + "/"+ filename;

    QFile file(completePathToFile);
    QFileInfo fileInfo(file);

    bool settingsExist = (fileInfo.exists() && fileInfo.isFile());

    m_settings = new QSettings(completePathToFile, QSettings::IniFormat);

    if(settingsExist)
    {
        QDateTime savedDateTime = m_settings->value("DateTime/last").toDateTime();
        m_last = new QDateTime(QDate(2000, 01, 01), QTime(0,0));
        m_last->setDate(savedDateTime.date());
        m_last->setTime(savedDateTime.time());
        dateTimeEdit->setDateTime(*m_last);

        m_packetPerDay = m_settings->value("Cigarette/packetPerDay").toFloat();
        m_cigarettePerPacket = m_settings->value("Cigarette/cigarettePerPacket").toInt();
        m_packetPrice = m_settings->value("Cigarette/packetPrice").toFloat();

        lineEditPacketPerDay->setText(QString::number(m_packetPerDay));
        lineEditCigarettePerPacket->setText(QString::number(m_cigarettePerPacket));
        lineEditPacketPrice->setText(QString::number(m_packetPrice));
    }
    else
    {
        initDefaultValue();
        saveSettings();
    }
}

void Widget::saveSettings()
{
    m_settings->setValue("DateTime/last", *m_last);
    m_settings->setValue("Cigarette/packetPerDay", m_packetPerDay);
    m_settings->setValue("Cigarette/cigarettePerPacket", m_cigarettePerPacket);
    m_settings->setValue("Cigarette/packetPrice", m_packetPrice);
}

void Widget::on_pushButton_released()
{
    saveSettings();
}
