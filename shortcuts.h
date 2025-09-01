#ifndef SHORTCUTS_H
#define SHORTCUTS_H

#include <QObject>
#include <QShortcut>
#include <QAction>
#include <QMainWindow>
#include <QTextEdit>

class Shortcuts : public QObject
{
    Q_OBJECT
public:
    explicit Shortcuts(QMainWindow *parent, QTextEdit *editor);
    // 初始化文件操作和编辑器快捷键
    void setupFileShortcuts(QAction *newAction,
                            QAction *openAction,
                            QAction *saveAction,
                            QAction *saveAsAction);

    void setupEditorShortcuts();

    // 提供外部获取快捷键表的接口（以后可以用于设置界面）
    const QMap<QString, QKeySequence>& shortcutsMap() const { return m_shortcutsMap; }

private slots:
    void zoomIn();
    void zoomOut();
    void resetZoom();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QMainWindow *m_parent;
    QTextEdit *m_editor;

    // 快捷键配置表
    QMap<QString, QKeySequence> m_shortcutsMap;
};


#endif // SHORTCUTS_H
