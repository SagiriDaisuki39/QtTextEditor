#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "shortcuts.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QRegularExpression>
#include <QShortcut>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("我的记事本");
    connect(ui->textEditor,SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()));
    connect(ui->textEditor, &QTextEdit::textChanged, this, &MainWindow::onTextChanged);

    // 初始化快捷键管理
    m_shortcuts = new Shortcuts(this, ui->textEditor);

    // 文件快捷键
    m_shortcuts->setupFileShortcuts(ui->action_new,
                                    ui->action_open,
                                    ui->action_save,
                                    ui->action_saveAs);

    // 编辑器快捷键（包含 Ctrl+滚轮）
    m_shortcuts->setupEditorShortcuts();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::maybeSave()
{
    // 检查文本编辑器内容是否被修改过，如果没被修改不用提示保存
    if (!ui->textEditor->document()->isModified())
        return true;

    // 如果被修改过，提示保存
    auto ret = QMessageBox::warning(
        this, tr("未保存的更改"),
        tr("当前文件已被修改，是否保存？"),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    // 点击“是”，保存，返回 true 表示状态已处理
    // 点击“取消”，返回 false 表示状态未处理
    // 点击“否”，直接返回 true 表示状态已处理
    if (ret == QMessageBox::Yes)
    {
        return saveFile();
    }
    else if (ret == QMessageBox::Cancel)
    {
        return false;
    }
    return true;
}

bool MainWindow::saveFile()
{
    QString text = ui->textEditor->toPlainText();
    if (m_fileHandler.filePath().isEmpty()) {
        return saveFileAs();
    }

    if (!m_fileHandler.saveFile(text)) {
        QMessageBox::critical(this, tr("保存失败"), tr("无法写入文件：\n%1").arg(m_fileHandler.filePath()));
        return false;
    }

    ui->textEditor->document()->setModified(false);
    setWindowTitle(m_fileHandler.filePath());
    return true;
}

bool MainWindow::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("文件另存为"),
        m_fileHandler.dirPath(),
        tr("Text (*.txt *.doc)")
        );
    if (fileName.isEmpty()) return false;

    QString text = ui->textEditor->toPlainText();
    if (!m_fileHandler.saveFileAs(fileName, text)) {
        QMessageBox::critical(this, tr("保存失败"), tr("无法写入文件：\n%1").arg(fileName));
        return false;
    }

    ui->textEditor->document()->setModified(false);
    setWindowTitle(fileName);
    return true;
}

void MainWindow::on_action_save_triggered()
{
    saveFile();
}

void MainWindow::on_action_saveAs_triggered()
{
    saveFileAs();
}

void MainWindow::on_action_open_triggered()
{
    // 打开新文件前先确认当前内容是否要保存
    if (!maybeSave()) return;

    // 文件选择栏
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("打开文件"),
        m_fileHandler.dirPath(),
        tr("Text (*.txt)")
        );
    if (fileName.isEmpty()) return;

    QString content;
    if (!m_fileHandler.loadFile(fileName, content))
    {
        QMessageBox::critical(this, tr("打开失败"), tr("无法打开文件：\n%1").arg(fileName));
        return;
    }

    ui->textEditor->setPlainText(content);
    ui->textEditor->document()->setModified(false);
    setWindowTitle(fileName);
}

void MainWindow::on_action_new_triggered()
{
    if (!maybeSave()) return;
    m_fileHandler.setFilePath("");
    ui->textEditor->clear();
    setWindowTitle("新创建文件");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}



// 显示光标位置和设置当前行高亮
void MainWindow::onCursorPositionChanged()
{
    // 显示光标位置
    QTextCursor cursor = ui->textEditor->textCursor();
    // qDebug() << cursor.blockNumber()+1 << ", " << cursor.columnNumber()+1;
    QString blockNum = QString::number(cursor.blockNumber()+1);
    QString columnNum = QString::number(cursor.columnNumber()+1);
    const QString labelMes = "第 "+blockNum+" 行，第 "+columnNum+" 列";
    ui->label_location->setText(labelMes);

    // 设置当前行高亮
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection ext;
    // 1. 获取当前行
    ext.cursor = ui->textEditor->textCursor();
    // 2. 颜色
    QBrush qBrush(Qt::lightGray);
    ext.format.setBackground(qBrush);
    // 配置段属性，整行显示
    ext.format.setProperty(QTextFormat::FullWidthSelection, true);
    // 3. 设置
    extraSelections.append(ext);
    ui->textEditor->setExtraSelections(extraSelections);
}

// 实时统计字数
void MainWindow::onTextChanged()
{
    QString text = ui->textEditor->toPlainText();

    // 统计字符数（含空格和换行）
    int charCount = text.length();

    // 统计单词数（以空格/换行分隔）
    // QStringList words = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    // int wordCount = words.size();

    // 更新状态栏或者标签
    // QString msg = QString("字数: %1 | 单词数: %2").arg(charCount).arg(wordCount);
    QString msg = QString(" 字数: %1").arg(charCount);
    ui->label_counting->setText(msg);
}
