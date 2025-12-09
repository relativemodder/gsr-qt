#pragma once

#include <QString>
#include <QStringList>
#include <QMap>

class GSRArgs {
public:
    GSRArgs();

    // core options 
    // thx to LLMs for converting these from --help text to params

    /**
     * @brief sets the capture target (-w). required parameter.
     * @param target window id, 'monitor', 'focused', 'portal', 'region', etc.
     */
    void setWindowTarget(const QString& target);

    /**
     * @brief sets the container format (-c).
     * @param format 'mp4', 'mkv', 'flv', etc.
     */
    void setContainerFormat(const QString& format);

    /**
     * @brief sets the output resolution limit (-s WxH).
     * @param resolution resolution in WxH format, e.g., "1920x1080".
     */
    void setResolution(const QString& resolution);

    /**
     * @brief sets the region to capture (-region). only used with -w region.
     * @param region region in WxH+X+Y format, e.g., "640x480+100+100".
     */
    void setCaptureRegion(const QString& region);

    /**
     * @brief sets the frame rate (-f).
     * @param fps frame rate, e.g., 60.
     */
    void setFrameRate(int fps);

    /**
     * @brief adds an audio device or application to record from (-a).
     * can be called multiple times.
     * @param audioSource device name, 'default_output', 'app:Brave', etc.
     */
    void addAudioSource(const QString& audioSource);
    
    /**
     * @brief clears the list of audio sources.
     */
    void clearAudioSources();

    /**
     * @brief sets the video/image quality (-q).
     * @param quality 'medium', 'high', 'very_high', 'ultra' or bitrate in kbps (for -bm cbr).
     */
    void setQuality(const QString& quality);

    /**
     * @brief sets the output file path (-o). required for regular mode.
     * in replay mode (-r), this must be a directory.
     * @param path file/directory path.
     */
    void setOutputFile(const QString& path);

    /**
     * @brief sets the output directory for regular recordings in replay/streaming mode (-ro).
     * @param dirPath directory path.
     */
    void setRegularOutputDirectory(const QString& dirPath);

    // replay options

    /**
     * @brief sets the replay buffer time in seconds (-r).
     * @param seconds time in seconds (2 to 86400). if 0, buffer is disabled.
     */
    void setReplayBufferSize(int seconds);

    /**
     * @brief specifies where temporary replay is stored (-replay-storage).
     * @param storage 'ram' or 'disk'.
     */
    void setReplayStorage(const QString& storage);

    /**
     * @brief sets whether to restart replay on save (-restart-replay-on-save).
     * @param restart true for 'yes', false for 'no'.
     */
    void setRestartReplayOnSave(bool restart);

    // --- codec and mode options ---

    /**
     * @brief sets the video codec to use (-k).
     * @param codec 'auto', 'h264', 'hevc', 'av1', etc.
     */
    void setVideoCodec(const QString& codec);

    /**
     * @brief sets the audio codec to use (-ac).
     * @param codec 'aac', 'opus' or 'flac'.
     */
    void setAudioCodec(const QString& codec);

    /**
     * @brief sets the audio bitrate in kbps (-ab).
     * @param bitrate bitrate in kbps.
     */
    void setAudioBitrate(int bitrate);

    /**
     * @brief sets the framerate mode (-fm).
     * @param mode 'cfr', 'vfr' or 'content'.
     */
    void setFramerateMode(const QString& mode);

    /**
     * @brief sets the bitrate mode (-bm).
     * @param mode 'auto', 'qp', 'vbr' or 'cbr'.
     */
    void setBitrateMode(const QString& mode);

    // other options

    /**
     * @brief sets the color range (-cr).
     * @param range 'limited' or 'full'.
     */
    void setColorRange(const QString& range);

    /**
     * @brief tunes for performance or quality (-tune).
     * @param setting 'performance' or 'quality'.
     */
    void setTuning(const QString& setting);

    /**
     * @brief specifies the keyframe interval in seconds (-keyint).
     * @param seconds interval in seconds (float).
     */
    void setKeyframeInterval(double seconds);

    /**
     * @brief sets whether to record the cursor (-cursor).
     * @param record true for 'yes', false for 'no'.
     */
    void setRecordCursor(bool record);

    /**
     * @brief sets which device should be used for video encoding (-encoder).
     * @param device 'gpu' or 'cpu'.
     */
    void setEncoder(const QString& device);

    /**
     * @brief sets whether to organise replays in folders based on the current date (-df).
     * if true, the flag is added.
     */
    void setOrganizeReplaysByDate(bool enable);
    
    /**
     * @brief sets whether to overclock memory transfer rate (-oc). only for nvidia x11.
     * @param enable true for 'yes', false for 'no'.
     */
    void setOverclockMemory(bool enable);

    /**
     * @brief sets the script path to run on the saved video file (-sc).
     */
    void setScriptPath(const QString& path);

    /**
     * @brief adds a plugin (.so) to load (-p). can be called multiple times.
     */
    void addPlugin(const QString& path);

    /**
     * @brief restores the portal session (-restore-portal-session).
     */
    void setRestorePortalSession(bool enable);

    /**
     * @brief sets the portal session token filepath (-portal-session-token-filepath).
     */
    void setPortalSessionTokenFilepath(const QString& path);
    
    /**
     * @brief sets whether to fallback to cpu encoding if gpu is unavailable (-fallback-cpu-encoding).
     */
    void setFallbackCpuEncoding(bool enable);

    /**
     * @brief prints fps and damage info once per second (-v).
     */
    void setPrintFpsInfo(bool enable);

    /**
     * @brief prints opengl debug output (-gl-debug).
     */
    void setGlDebug(bool enable);

    /**
     * @brief generate the final argument list for qprocess.
     * @return qstringlist containing all set arguments.
     */
    QStringList toQStringList() const;

private:
    QMap<QString, QString> singleOptions;
    QMap<QString, QStringList> multiOptions;
    QMap<QString, bool> flags;

    // sets a single-value option
    void setOption(const QString& option, const QString& value);

    // adds an option that can be repeated (e.g., -a, -p)
    void addMultiOption(const QString& option, const QString& value);
    
    // sets a flag (an option without a value, if true)
    void setFlag(const QString& option, bool enable);
    
    // removes an option
    void removeOption(const QString& option);
};
