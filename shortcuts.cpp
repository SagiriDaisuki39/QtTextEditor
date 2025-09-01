#include "shortcuts.h"

#include <QKeySequence>
#include <QFont>
#include <QWheelEvent>
#include <QApplication>


Shortcuts::Shortcuts(QMainWindow *parent, QTextEdit *editor)
    : QObject(parent), m_parent(parent), m_editor(editor)
{
    // 初始化快捷键表（统一集中管理）
    m_shortcutsMap = {
        {"new",     QKeySequence(Qt::CTRL | Qt::Key_T)},
        {"open",    QKeySequence(Qt::CTRL | Qt::Key_O)},
        {"save",    QKeySequence(Qt::CTRL | Qt::Key_S)},
        {"saveAs",  QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S)},
        {"zoomIn",  QKeySequence(Qt::CTRL | Qt::Key_Plus)},
        {"zoomOut", QKeySequence(Qt::CTRL | Qt::Key_Minus)},
        {"resetZoom", QKeySequence(Qt::CTRL | Qt::Key_0)}
    };

    // 安装事件过滤器，拦截 Ctrl+滚轮
    if (m_editor) {
        m_editor->installEventFilter(this);
    }
}

// 文件操作快捷键
void Shortcuts::setupFileShortcuts(QAction *newAction,
                                   QAction *openAction,
                                   QAction *saveAction,
                                   QAction *saveAsAction)
{
    if (newAction)    newAction->setShortcut(m_shortcutsMap["new"]);
    if (openAction)   openAction->setShortcut(m_shortcutsMap["open"]);
    if (saveAction)   saveAction->setShortcut(m_shortcutsMap["save"]);
    if (saveAsAction) saveAsAction->setShortcut(m_shortcutsMap["saveAs"]);
}

// 编辑器相关快捷键（放大/缩小/重置字号）
void Shortcuts::setupEditorShortcuts()
{
    auto zoomInShortcut    = new QShortcut(m_shortcutsMap["zoomIn"], m_parent);
    auto zoomOutShortcut   = new QShortcut(m_shortcutsMap["zoomOut"], m_parent);
    auto resetZoomShortcut = new QShortcut(m_shortcutsMap["resetZoom"], m_parent);

    connect(zoomInShortcut,    &QShortcut::activated, this, &Shortcuts::zoomIn);
    connect(zoomOutShortcut,   &QShortcut::activated, this, &Shortcuts::zoomOut);
    connect(resetZoomShortcut, &QShortcut::activated, this, &Shortcuts::resetZoom);
}

// ============ 字号调整 ================
void Shortcuts::zoomIn()
{
    QFont font = m_editor->font();
    font.setPointSize(font.pointSize() + 1);
    m_editor->setFont(font);
}

void Shortcuts::zoomOut()
{
    QFont font = m_editor->font();
    font.setPointSize(qMax(1, font.pointSize() - 1));
    m_editor->setFont(font);
}

void Shortcuts::resetZoom()
{
    QFont font = m_editor->font();
    font.setPointSize(12); // 默认字号
    m_editor->setFont(font);
}

// ============ Ctrl + 滚轮缩放 ================
// eventFilter 是 QObject 的虚函数，用来“拦截”某个对象的事件
// 参数： watched 表示谁发出的事件，event 表示事件本身
// 返回值：true 表示事件已处理（不会再传给控件本身），false 表示没处理（交给控件自己处理）
bool Shortcuts::eventFilter(QObject *watched, QEvent *event)
{
    // 只关心来自 m_editor 的滚轮事件
    if (watched == m_editor && event->type() == QEvent::Wheel)
    {
        auto *wheelEvent = static_cast<QWheelEvent *>(event); // 类型转换

        // 判断是否同时按下 Ctrl 键
        if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
            if (wheelEvent->angleDelta().y() > 0) {
                zoomIn();  // 向上滚轮 → 放大
            } else {
                zoomOut(); // 向下滚轮 → 缩小
            }
            return true; // 已处理，不再传递给原控件
        }
    }

    // 其他情况交给父类默认处理
    return QObject::eventFilter(watched, event);
}
