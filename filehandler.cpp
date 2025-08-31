#include "FileHandler.h"

FileHandler::FileHandler() : m_filePath("") {}

bool FileHandler::loadFile(const QString& path, QString& outText)
{
    // 打开文件
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream in(&file);
    outText = in.readAll();
    file.close();

    m_filePath = path;
    return true;
}

bool FileHandler::saveFile(const QString& text)
{
    if (m_filePath.isEmpty())
        return false; // 未设置文件路径

    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        return false;
    }

    QTextStream out(&file);
    out << text;
    file.close();
    return true;
}

bool FileHandler::saveFileAs(const QString& path, const QString& text)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        return false;
    }

    QTextStream out(&file);
    out << text;
    file.close();

    m_filePath = path; // 更新当前文件路径
    return true;
}
