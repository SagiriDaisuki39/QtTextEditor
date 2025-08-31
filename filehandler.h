#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QString>
#include <QFile>
#include <QTextStream>

class FileHandler
{
public:
    FileHandler();

    // 打开文件，读取内容
    bool loadFile(const QString& path, QString& outText);

    // 保存到当前文件（必须先 setFilePath）
    bool saveFile(const QString& text);

    // 另存为
    bool saveFileAs(const QString& path, const QString& text);

    // 当前文件路径
    QString filePath() const { return m_filePath; }
    void setFilePath(const QString& path) { m_filePath = path; }

    QString dirPath() const { return m_fileDir; }
    void setDirPath(const QString& path) { m_fileDir = path; }

private:
    QString m_filePath;
    QString m_fileDir = "../../files";
};

#endif // FILEHANDLER_H
