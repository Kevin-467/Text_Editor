#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mainwindowClass; };
QT_END_NAMESPACE

class Mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    Mainwindow(QWidget* parent = nullptr);
    ~Mainwindow();

private slots:
    void on_actionNew_File_triggered();
    void on_actionOpen_File_triggered();
    void on_actionPrint_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();

    void on_textEdit_selectionChanged();

    void on_actionBold_triggered(bool checked);
    void on_actionItalic_triggered(bool arg);
    void on_actionUnderLine_triggered(bool arg);

    void on_actionAbout_A_triggered();
    void on_fontsize_changed(int fontsize);
    void on_font_changed(const QFont& font);


    void readFile(const QString& filePath);
    void SaveFile(const QString& filePath);


private:
    Ui::mainwindowClass* ui;
    QLabel* labelInfo, * labelFile;
    QString currentFilePath;
    QString filter;
    QProgressBar* progressbar;
};
