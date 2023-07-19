#ifndef LOG4QTTESTWGT_H
#define LOG4QTTESTWGT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Log4QtTestWgt; }
QT_END_NAMESPACE

class Log4QtTestWgt : public QWidget
{
    Q_OBJECT

public:
    Log4QtTestWgt(QWidget *parent = nullptr);
    ~Log4QtTestWgt();

private slots:
    void on_debugBtn_clicked();
    void on_infoBtn_clicked();
    void on_warnBtn_clicked();
    void on_errorBtn_clicked();

    void slot_onLogStr(int level, const QString &msg);

private:
    Ui::Log4QtTestWgt *ui;
};
#endif // LOG4QTTESTWGT_H
