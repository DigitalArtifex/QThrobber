#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qthrobber.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void on_pushButton_clicked();

        void on_lineEdit_textChanged(const QString &arg1);

        void on_throbberSizeSlider_valueChanged(int value);

        void on_iconSizeSlider_valueChanged(int value);

        void on_iconScaleCheckBox_toggled(bool checked);

        void on_iconOpacityCheckBox_toggled(bool checked);

        void on_colorEdit_textChanged(const QString &arg1);

        void on_textColorEdit_textEdited(const QString &arg1);

        void on_textColorEdit_textChanged(const QString &arg1);

    private:
        Ui::MainWindow *ui;
        QThrobber *m_throbber = nullptr;
};
#endif // MAINWINDOW_H
