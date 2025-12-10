#pragma once

#include <QObject>
#include <QSettings>
#include <QStandardPaths>
#include <QString>

class GSRSettings : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString outputDirectory READ getOutputDir WRITE setOutputDir NOTIFY outputDirectoryChanged FINAL)
    Q_PROPERTY(bool categorizeByTitle READ getCategorizeByTitle WRITE setCategorizeByTitle NOTIFY categorizeByTitleChanged FINAL)

public:
    static GSRSettings& instance() {
        static GSRSettings INSTANCE;
        return INSTANCE;
    }

    static constexpr const char* OUTPUT_DIR_KEY = "output/directory";
    static constexpr const char* CATEGORIZE_BY_TITLE_KEY = "output/categorize_by_title";

    QString getOutputDir() const;
    void setOutputDir(const QString& path);
    Q_INVOKABLE void resetOutputDir();
    Q_INVOKABLE QString getDefaultOutputDirPath() const;

    bool getCategorizeByTitle() const;
    void setCategorizeByTitle(bool);
    Q_INVOKABLE bool getDefaultCategorizeByTitle() const;

signals:
    void outputDirectoryChanged();
    void categorizeByTitleChanged();

private:
    GSRSettings(QObject* parent = nullptr);
    ~GSRSettings() override = default;

    GSRSettings(const GSRSettings&) = delete;
    GSRSettings& operator=(const GSRSettings&) = delete;
    GSRSettings(GSRSettings&&) = delete;
    GSRSettings& operator=(GSRSettings&&) = delete;

    QString calculateDefaultOutputDir() const;

    QSettings m_settings;
};
