#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "FileHandler.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_open_triggered();
    void on_action_save_triggered();
    void on_action_saveAs_triggered();
    void on_action_new_triggered();

    void onCursorPositionChanged();
    void onTextChanged();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;

    bool maybeSave();
    bool saveFile();
    bool saveFileAs();

    FileHandler m_fileHandler;  // 封装文件操作
};

#endif // MAINWINDOW_H
