#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "krotciphers.h"
#include <QFile>
#include <QMessageBox>
#include <numeric>

QString prevtext;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_5->setHidden(true);
    ui->textEdit->setHidden(true);
    prevtext = ui->text->text();
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::vector<std::vector<int> > tabls;

QString printmatr(const std::vector<std::vector<int> > &tables)
{
    QString text = "";
    for(int i = 0;i < 5;i++)
    {
        for(int j = 0; j < 5; j++)
        {
            text.append(QChar(tables[0][i * 5 + j]+'a'));
            text.append(" ");
        }
        if(i == 4)
        {
            text.append("/");
            text.append(QChar(tables[0][25]+'a'));
            text.append("  ");
        }
        text.append("\t");
        for(int j = 0; j < 5; j++)
        {
            text.append(QChar(tables[1][i * 5 + j]+'a'));
            text.append(" ");
        }
        if(i == 4)
        {
            text.append("/");
            text.append(QChar(tables[1][25]+'a'));
        }
        text.append("\n");
    }
    text.append("\n");
    for(int i = 0;i < 5;i++)
    {
        for(int j = 0; j < 5; j++)
        {
            text.append(QChar(tables[2][i * 5 + j]+'a'));
            text.append(" ");
        }
        if(i == 4)
        {
            text.append("/");
            text.append(QChar(tables[2][25]+'a'));
            text.append("  ");
        }
        text.append("\t");
        for(int j = 0; j < 5; j++)
        {
            text.append(QChar(tables[3][i * 5 + j]+'a'));
            text.append(" ");
        }
        if(i == 4)
        {
            text.append("/");
            text.append(QChar(tables[3][25]+'a'));
        }
        text.append("\n");
    }
    return text;
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->playfer->isChecked())
    {
        std::string str = ui->text->text().toStdString();
        std::string enc = EnglishTextToPlayfair(str, tabls);
        ui->result->setText(QString(QString::fromStdString(enc)));
    }
    else if(ui->vigenere->isChecked())
    {
        std::u16string str = ui->text->text().toStdU16String();
        std::u16string key = rusfilter(ui->key->text().toStdU16String());
        if(key.size() == 0)
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle("error");
            msgBox.setText("no alphabet characters in the key");
            msgBox.exec();
            return;
        }
        std::u16string enc = RussianTextToVigenere(str, key);
        ui->result->setText(QString(reinterpret_cast<const QChar *>(enc.data()), enc.size()));
    }
    else if(ui->decimation->isChecked())
    {
        int key = keytoint(ui->key->text().toStdString());
        if(ui->engl->isChecked())
        {
            if(std::gcd(key, ENGLENGTH) != 1)
            {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setWindowTitle("error");
                msgBox.setText("gcd must be == 1");
                msgBox.exec();
                return;
            }
            std::string str = ui->text->text().toStdString();
            std::string enc = EnglishTextToDecimation(str, key);
            ui->result->setText(QString(QString::fromStdString(enc)));
        }
        else if(ui->rus->isChecked())
        {
            if(std::gcd(key, RUSLENGTH) != 1)
            {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setWindowTitle("error");
                msgBox.setText("gcd must be == 1");
                msgBox.exec();
                return;
            }
            std::u16string str = ui->text->text().toStdU16String();
            std::u16string enc = RussianTextToDecimation(str, key);
            ui->result->setText(QString(reinterpret_cast<const QChar *>(enc.data()), enc.size()));
        }
    }
}

void MainWindow::on_vigenere_clicked()
{
    ui->key->setHidden(false);
    ui->label_2->setHidden(false);
    ui->textEdit->setHidden(true);
    ui->label_5->setHidden(true);
    ui->rus->setEnabled(true);
    ui->rus->setChecked(true);
    ui->engl->setEnabled(false);
}


void MainWindow::on_decimation_clicked()
{
    ui->key->setHidden(false);
    ui->label_2->setHidden(false);
    ui->textEdit->setHidden(true);
    ui->label_5->setHidden(true);
    ui->engl->setEnabled(true);
    ui->rus->setEnabled(true);
}


void MainWindow::on_playfer_clicked()
{
    tabls.clear();
    for(int i = 0;i < 4;i++)
        tabls.emplace_back(shuff(ENGLENGTH));
    ui->textEdit->setHidden(false);
    ui->label_5->setHidden(false);
    ui->key->setHidden(true);
    ui->label_2->setHidden(true);
    ui->textEdit->setText(printmatr(tabls));
    ui->engl->setEnabled(true);
    ui->engl->setChecked(true);
    ui->rus->setEnabled(false);
}


void MainWindow::on_UIinput_clicked()
{
    ui->text->setText(prevtext);
    ui->text->setEnabled(true);
}


void MainWindow::on_fileinput_clicked()
{
    prevtext = ui->text->text();
    ui->text->setEnabled(false);
    QFile file("D:/Admin/Documents/firstQT/example/TEXT.txt");
    QString word;
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        word = QString::fromLocal8Bit(data);
        file.close();
    }
    ui->text->setText(word);
}


void MainWindow::on_decrypt_clicked()
{
    if(ui->playfer->isChecked())
    {
        std::string str = ui->text->text().toStdString();
        std::string dec = PlayfairToEnglishText(str, tabls);
        ui->result->setText(QString(QString::fromStdString(dec)));
    }
    else if(ui->vigenere->isChecked())
    {
        std::u16string str = ui->text->text().toStdU16String();
        std::u16string key = rusfilter(ui->key->text().toStdU16String());
        if(key.size() == 0)
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle("error");
            msgBox.setText("no alphabet characters in the key");
            msgBox.exec();
            return;
        }
        std::u16string dec = VigenereToRussianText(str, key);
        ui->result->setText(QString(reinterpret_cast<const QChar *>(dec.data()), dec.size()));
    }
    else if(ui->decimation->isChecked())
    {
        int key = keytoint(ui->key->text().toStdString());
        if(ui->engl->isChecked())
        {
            if(std::gcd(key, ENGLENGTH) != 1)
            {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setWindowTitle("error");
                msgBox.setText("gcd must be == 1");
                msgBox.exec();
                return;
            }
            std::string str = ui->text->text().toStdString();
            std::string dec = DecimationToEnglishText(str, key);
            ui->result->setText(QString(QString::fromStdString(dec)));
        }
        else if(ui->rus->isChecked())
        {
            if(std::gcd(key, RUSLENGTH) != 1)
            {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setWindowTitle("error");
                msgBox.setText("gcd must be == 1");
                msgBox.exec();
                return;
            }
            std::u16string str = ui->text->text().toStdU16String();
            std::u16string dec = DecimationToRussianText(str, key);
            ui->result->setText(QString(reinterpret_cast<const QChar *>(dec.data()), dec.size()));
        }
    }
}

