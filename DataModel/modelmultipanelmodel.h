#ifndef MODELMULTIPANELMODEL_H
#define MODELMULTIPANELMODEL_H

#include <QObject>
#include <QSettings>
#include <QVariant>

class ModelMultiPanelModel : public QObject
{
    Q_OBJECT
public:
    explicit ModelMultiPanelModel(QObject *parent = 0);
    quint32 getLayoutStyle(quint32 fmIndex);
    void setLayoutStyle(quint32 fmIndex, quint32 styleId);
    QString getLayoutName(quint32 fmIndex);
    void setLayoutName(quint32 fmIndex, QString fmName);
    quint32 getPanelType(quint32 fmIndex, int paIndex);
    void setPanelType(quint32 fmIndex, int paIndex, quint32 PanelId);
    QVariant getPanelParam(quint32 fmIndex, int paIndex);
    void setPanelParam(quint32 fmIndex, int paIndex, QVariant value);

private:
    QSettings Settings;
};

#endif // MODELMULTIPANELMODEL_H
