#include "HistoryPage.h"
#include "ui_historypage.h"

HistoryPage::HistoryPage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HistoryPage)
{
    ui->setupUi(this);
    count = 0;
    CreateFont();
    ui->groupBox->setFont(_font);
    LoadHist();
}

HistoryPage::~HistoryPage()
{
    delete ui;
}

void HistoryPage::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    LoadHist();
}

void HistoryPage::LoadHist(){
    for(auto& his:hist){
        if(count == 0){
            ui->lnDate1->setText(his.date.toString());
            ui->lnOpp1->setText(his.opponent);
            ui->lnRes1->setText(his.result);
            QString rounds;
            for(auto& rnd:his.rounds){
                rounds+=rnd;
                rounds+="-";
            }
            ui->lnRnd1->setText(rounds);
            count++;
            continue;
        }
        if(count == 1){
            ui->lnDate2->setText(his.date.toString());
            ui->lnOpp2->setText(his.opponent);
            ui->lnRes2->setText(his.result);
            QString rounds;
            for(auto& rnd:his.rounds){
                rounds+=rnd;
                rounds+="-";
            }
            ui->lnRnd2->setText(rounds);
            count++;
            continue;
        }
        if(count == 2){
            ui->lnDate3->setText(his.date.toString());
            ui->lnOpp3->setText(his.opponent);
            ui->lnRes3->setText(his.result);
            QString rounds;
            for(auto& rnd:his.rounds){
                rounds+=rnd;
                rounds+="-";
            }
            ui->lnRnd3->setText(rounds);
            count++;
        }
    }
    count = 0;
}

void HistoryPage::CreateFont(){
    int fontId = QFontDatabase::addApplicationFont(":/Font/balatro.ttf");
    QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont customFont(family, 16, true);
    _font = customFont;

}
