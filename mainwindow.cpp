#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("QThrobber Test");

    m_throbber = new QThrobber(ui->widget);
    m_throbber->setFixedSize(150,150);
    m_throbber->setText(ui->lineEdit->text());
    m_throbber->setIcon(QIcon(":/icons/icons8-temperature.svg"));
    ui->widget->layout()->addWidget(m_throbber);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(m_throbber->isPlaying())
    {
        m_throbber->stop();
        ui->pushButton->setText("Start");
    }
    else
    {
        m_throbber->start();
        ui->pushButton->setText("Stop");
    }
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    m_throbber->setText(arg1);
}


void MainWindow::on_throbberSizeSlider_valueChanged(int value)
{
    m_throbber->setThrobberWidth(value);
}


void MainWindow::on_iconSizeSlider_valueChanged(int value)
{
    m_throbber->setIconSize(QSize(value,value));
}


void MainWindow::on_iconScaleCheckBox_toggled(bool checked)
{
    m_throbber->setIconScaleAnimationEnabled(checked);
}


void MainWindow::on_iconOpacityCheckBox_toggled(bool checked)
{
    m_throbber->setIconOpacityAnimationEnabled(checked);
}


void MainWindow::on_colorEdit_textChanged(const QString &arg1)
{
    m_throbber->setThrobberColor(QColor(arg1));
}


void MainWindow::on_textColorEdit_textEdited(const QString &arg1)
{

}


void MainWindow::on_textColorEdit_textChanged(const QString &arg1)
{
    m_throbber->setTextColor(QColor(arg1));
}

