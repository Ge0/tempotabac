#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDateTime>
#include <QTimer>
#include <QSettings>
#include "ui_widget.h"

//QDate(2017, 01, 12), QTime(9,0)
//1 20 6.9

class Widget : public QWidget, private Ui::Widget
{
    Q_OBJECT

private:
    QDateTime *m_last;
    qint64  m_secs;
    float  m_mins;
    float  m_hours;
    float  m_days;
    float  m_months;
    QTimer *m_timer;
    float m_monnay;
    QString m_totalString;
    QSettings *m_settings;

    float m_packetPerDay;
    int m_cigarettePerPacket;
    float m_packetPrice;
    float m_cigarettePrice;
    float m_cigarettePerDay;

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void updateDateTime();
    void on_lineEditPacketPerDay_textEdited();
    void on_lineEditCigarettePerPacket_textEdited();
    void on_lineEditPacketPrice_textEdited();
    void on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime);
    void on_pushButton_released();

private:
    void setMonths();
    void setDays();
    void setHour();
    void setMin();
    void setSecs();
    void setTotal();
    void setAll();
    void setMonnay();
    void calculCigarette();
    void loadSettings();
    void saveSettings();
    void initDefaultValue();
};

#endif // WIDGET_H
