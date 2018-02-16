/********************************************************************************
** Form generated from reading UI file 'ai.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AI_H
#define UI_AI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLineEdit *text_edit;
    QPlainTextEdit *main;
    QPlainTextEdit *main_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(891, 618);
        QPalette palette;
        QLinearGradient gradient(0, 0, 1, 1);
        gradient.setSpread(QGradient::PadSpread);
        gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient.setColorAt(0, QColor(190, 210, 230, 255));
        gradient.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush(gradient);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        QLinearGradient gradient1(0, 0, 1, 1);
        gradient1.setSpread(QGradient::PadSpread);
        gradient1.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient1.setColorAt(0, QColor(190, 210, 230, 255));
        gradient1.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush1(gradient1);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        QLinearGradient gradient2(0, 0, 1, 1);
        gradient2.setSpread(QGradient::PadSpread);
        gradient2.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient2.setColorAt(0, QColor(190, 210, 230, 255));
        gradient2.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush2(gradient2);
        palette.setBrush(QPalette::Active, QPalette::Window, brush2);
        QLinearGradient gradient3(0, 0, 1, 1);
        gradient3.setSpread(QGradient::PadSpread);
        gradient3.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient3.setColorAt(0, QColor(190, 210, 230, 255));
        gradient3.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush3(gradient3);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush3);
        QLinearGradient gradient4(0, 0, 1, 1);
        gradient4.setSpread(QGradient::PadSpread);
        gradient4.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient4.setColorAt(0, QColor(190, 210, 230, 255));
        gradient4.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush4(gradient4);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        QLinearGradient gradient5(0, 0, 1, 1);
        gradient5.setSpread(QGradient::PadSpread);
        gradient5.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient5.setColorAt(0, QColor(190, 210, 230, 255));
        gradient5.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush5(gradient5);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush5);
        QLinearGradient gradient6(0, 0, 1, 1);
        gradient6.setSpread(QGradient::PadSpread);
        gradient6.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient6.setColorAt(0, QColor(190, 210, 230, 255));
        gradient6.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush6(gradient6);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush6);
        QLinearGradient gradient7(0, 0, 1, 1);
        gradient7.setSpread(QGradient::PadSpread);
        gradient7.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient7.setColorAt(0, QColor(190, 210, 230, 255));
        gradient7.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush7(gradient7);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        QLinearGradient gradient8(0, 0, 1, 1);
        gradient8.setSpread(QGradient::PadSpread);
        gradient8.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient8.setColorAt(0, QColor(190, 210, 230, 255));
        gradient8.setColorAt(1, QColor(255, 255, 255, 255));
        QBrush brush8(gradient8);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush8);
        MainWindow->setPalette(palette);
        MainWindow->setStyleSheet(QLatin1String("* { \n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(190, 210, 230, 255), stop:1 rgba(255, 255, 255, 255)) \n"
"}\n"
"*:checked { \n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(150, 150, 200, 255), stop:1 rgba(255, 255, 255, 255)); color: white \n"
"}"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        text_edit = new QLineEdit(centralWidget);
        text_edit->setObjectName(QStringLiteral("text_edit"));
        text_edit->setGeometry(QRect(10, 590, 871, 21));
        main = new QPlainTextEdit(centralWidget);
        main->setObjectName(QStringLiteral("main"));
        main->setGeometry(QRect(10, 10, 641, 581));
        main->setLineWidth(1);
        main->setMidLineWidth(0);
        main->setReadOnly(true);
        main_2 = new QPlainTextEdit(centralWidget);
        main_2->setObjectName(QStringLiteral("main_2"));
        main_2->setGeometry(QRect(650, 10, 231, 581));
        main_2->setLineWidth(1);
        main_2->setMidLineWidth(0);
        main_2->setReadOnly(true);
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Console", 0));
        text_edit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AI_H
