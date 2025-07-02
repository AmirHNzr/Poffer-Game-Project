#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include <QDialog>
#include"History.h"
#include <QFont>
#include <QFontDatabase>

namespace Ui {
class HistoryPage;
}

class HistoryPage : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryPage(QWidget *parent = nullptr);
    ~HistoryPage();
protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::HistoryPage *ui;
    int count;
    QFont _font;
    void CreateFont();
    void LoadHist();
};

#endif // HISTORYPAGE_H
