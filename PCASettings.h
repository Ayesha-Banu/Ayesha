#ifndef PCASETTINGS_H
#define PCASETTINGS_H

#include <QObject>
#include <QString>
#include <QSettings>
#include <QtXml/QtXml>
struct PCASettingsData;
struct XmlNode;

class PCASettings : public QObject
{
    Q_OBJECT
public:
    explicit PCASettings(QObject *parent = nullptr){}
    ~PCASettings();
    QHash<QString,QVariant> read_Config_Xml();

     /* keep create_Config_Xml into private */
    /* write IP Port settings functions */
    /* write comments */


signals:

public slots:
    void init();

private:
    PCASettingsData *d;

    int create_Config_Xml();
    bool readSettingsXml(QIODevice &device, QMap<QString, QVariant> &map);
    bool writeSettingsXml(QIODevice &device, const QMap<QString, QVariant> &map);
    QString fullPath(const XmlNode *cur) const;
};

#endif // PCASETTINGS_H
