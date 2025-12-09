#pragma once

#include <QObject>
#include <QSettings>
#include <QStandardPaths>
#include <QString>

class GSRSettings : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString outputDirectory READ getOutputDir WRITE setOutputDir NOTIFY outputDirectoryChanged FINAL)

public:
    static GSRSettings& instance() {
        static GSRSettings INSTANCE;
        return INSTANCE;
    }

    static constexpr const char* OUTPUT_DIR_KEY = "output/directory";
    
    QString getOutputDir() const;
    void setOutputDir(const QString& path);
    Q_INVOKABLE void resetOutputDir();

signals:
    void outputDirectoryChanged();

private:
    GSRSettings(QObject* parent = nullptr);
    ~GSRSettings() override = default;
    
    QString calculateDefaultOutputDir() const;

    QSettings m_settings;
};

