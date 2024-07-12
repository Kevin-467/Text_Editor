#include "stdafx.h"
#include "mainwindow.h"

Mainwindow::Mainwindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::mainwindowClass())
{
    currentFilePath = "";
    ui->setupUi(this);
    ui->textEdit->clear();

    QSpinBox* font_size_box = new QSpinBox(this);
    font_size_box->setMinimum(5);
    font_size_box->setMaximum(72);
    font_size_box->setValue(ui->textEdit->font().pointSize());
    font_size_box->setMinimumWidth(50);
    ui->mainToolBar->addWidget(font_size_box);

    QFontComboBox* font_box = new QFontComboBox(this);
    font_box->setMinimumWidth(150);
    font_box->setFont(ui->textEdit->font());
    ui->mainToolBar->addWidget(font_box);

    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionClose);

    labelFile = new QLabel(this);
    labelFile->setMinimumWidth(150);
    labelFile->setText(tr("文件名:"));
    ui->statusBar->addWidget(labelFile);

    progressbar = new QProgressBar(this);
    progressbar->setMinimumWidth(5);
    progressbar->setMaximum(72);
    progressbar->setValue(ui->textEdit->font().pointSize());
    ui->statusBar->addWidget(progressbar);
    labelInfo = new QLabel("Permanent");
    ui->statusBar->addPermanentWidget(labelInfo);

    connect(font_size_box, &QSpinBox::valueChanged, this, &Mainwindow::on_fontsize_changed);
    connect(font_box, &QFontComboBox::currentFontChanged, this, &Mainwindow::on_font_changed);
}

Mainwindow::~Mainwindow()
{
    delete labelFile;
    delete labelInfo;
    delete ui;
}

void Mainwindow::on_actionNew_File_triggered()
{
    currentFilePath = "";
    ui->textEdit->clear();
    ui->textEdit->setPlaceholderText("请输入文字");
    ui->textEdit->document()->setModified(false);
    labelFile->setText(QString::asprintf("新建的文件").toUtf8().constData());
    QDateTime currentDateTime = QDateTime::currentDateTime();
    labelInfo->setText(QString::asprintf("新建时间：%s", currentDateTime.toString("yyyy-MM-dd HH:mm:ss").toUtf8().constData()));
}

void Mainwindow::on_actionOpen_File_triggered()
{
    labelInfo->clear();
    ui->textEdit->clear();
    filter = "文本文件(*.txt *.md);;文档文件(*.docx *.doc *.xlsx *.xls *.pptx *.ppt *.pdf);;代码文件(*.py *.c *.h *.cpp *.java *.json *.css *.html *.xml *.js *.sln)";
    QString filePath = QFileDialog::getOpenFileName(this, "打开文件", "", filter);
    readFile(filePath);
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();
    labelFile->setText(QString::asprintf("文件名：%s", fileName.toUtf8().constData()));
    currentFilePath = filePath.toUtf8().constData();
}

void Mainwindow::on_actionPrint_triggered()
{
    labelInfo->clear();
    QMessageBox::warning(this, "温馨提示", "该功能暂不可用", "确认(&C)");
}

void Mainwindow::on_actionSave_triggered()
{
    labelInfo->clear();
    filter = "文本文件(*.txt *.md);;文档文件(*.docx *.doc);;代码文件(*.py *.c *.h *.cpp *.java *.json *.css *.html *.xml *.js)";
    QString filePath = currentFilePath;
    if (filePath.isEmpty()) {
        filePath = QFileDialog::getSaveFileName(this, "保存文件", "", filter);
        if (filePath.isEmpty()) {
            return;
        }
    }
    SaveFile(filePath);
}

void Mainwindow::on_actionSave_as_triggered()
{

    filter = "文本文件(*.txt *.md);;文档文件(*.docx *.doc);;代码文件(*.py *.c *.h *.cpp *.java *.json *.css *.html *.xml *.js)";
    QString filePath = QFileDialog::getSaveFileName(this, "保存文件", "", filter);
    if (filePath.isEmpty()) {
        return;
    }
    SaveFile(filePath);
}

void Mainwindow::on_textEdit_selectionChanged()
{
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    ui->actionBold->setChecked(fmt.font().bold());
    ui->actionItalic->setChecked(fmt.font().italic());
    ui->actionUnderLine->setChecked(fmt.font().underline());
}

void Mainwindow::on_actionBold_triggered(bool checked)
{
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    if (checked) {
        fmt.setFontWeight(QFont::Bold);
    }
    else { fmt.setFontWeight(QFont::Normal); }
    ui->textEdit->setCurrentCharFormat(fmt);
}

void Mainwindow::on_actionItalic_triggered(bool arg)
{
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    fmt.setFontItalic(arg);
    ui->textEdit->setCurrentCharFormat(fmt);
}

void Mainwindow::on_actionUnderLine_triggered(bool arg)
{
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    fmt.setFontUnderline(arg);
    ui->textEdit->setCurrentCharFormat(fmt);

}

void Mainwindow::on_actionAbout_A_triggered()
{
    QMessageBox* msg = new QMessageBox(this);
    msg->setWindowTitle("About");
    msg->setText("Written by Kevin Chan\nData:2024/7/11 Thursday\nThanks for using!");
    msg->show();

}

void Mainwindow::on_fontsize_changed(int fontsize)
{
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    fmt.setFontPointSize(fontsize);
    ui->textEdit->setCurrentCharFormat(fmt);
    progressbar->setFormat("%v");
    progressbar->setValue(fontsize);
}

void Mainwindow::on_font_changed(const QFont& font)
{
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    fmt.setFont(font);
    ui->textEdit->setCurrentCharFormat(fmt);
    labelInfo->setText("字体名称：" + font.family());
}




void Mainwindow::readFile(const QString& filePath)
{
    QFile selected_file(filePath);
    if (!selected_file.open(QIODevice::ReadOnly | QIODevice::Text))  return;
    QTextStream in(&selected_file);
    in.setEncoding(QStringConverter::Utf8);
    QString fileContent = in.readAll();
    ui->textEdit->setPlainText(fileContent);
    selected_file.close();
}

void Mainwindow::SaveFile(const QString& filePath)
{
retry_save:
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox msgBox;
        msgBox.setText("无法保存文件: " + file.errorString());
        msgBox.addButton("重试(&R)", QMessageBox::AcceptRole);
        msgBox.addButton(QMessageBox::Cancel);

        int ret = msgBox.exec();
        if (ret == QMessageBox::Retry) {
            goto retry_save;
        }
    }
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << ui->textEdit->toPlainText();
    file.close();
    QMessageBox::information(this, "", "文件保存成功");
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();
    currentFilePath = filePath;
    labelFile->setText(QString::asprintf("文件名：%s", fileName.toUtf8().constData()));
}

