#ifndef Knotconfig_HEADER
#define Knotconfig_HEADER

#include <KDE/KComboBox>
#include <KDE/KConfigGroup>
#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>

#include "Knotwrap.h"

struct KnotGameParams;

class KnotConfig
{
public:
    static int getGameId (KConfigGroup cg);
    static QString getGameName (KConfigGroup cg);
    static int getPresetId (KConfigGroup cg);
    static int getPresetId (KConfigGroup cg, int gameId);
    static void setKnotGameParam (KConfigGroup cg, KnotGameParamItem& item);
    static QString sanitize (QString s);
};

class KnotGameConfig : public QWidget
{
    Q_OBJECT
public:
    // Basic Create/Destroy
    KnotGameConfig(QWidget *parent, KConfigGroup cg);
    ~KnotGameConfig();
    
    bool saveConfig ();
    
protected slots:
    void gameChanged (int id);
    void paramChanged ();
    void paramCustomized ();
private:
    struct Private;
    Private *d;
};

class KnotDisplayConfig : public QWidget
{
    Q_OBJECT
public:
    // Basic Create/Destroy
    KnotDisplayConfig(QWidget *parent, KConfigGroup cg);
    ~KnotDisplayConfig();
    
protected:
private:
    struct Private;
    Private *d;
};

class KnotGameStateTracker : public QObject
{  
    Q_OBJECT
public:
    KnotGameStateTracker(KConfigGroup cg);

protected slots:
    void gameStateChanged (const QString& state);
public:
    QString gameState ();
private:
    struct Private;
    Private *d;
};

#endif
