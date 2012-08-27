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
    static int getPresetId (KConfigGroup cg);
    static int getPresetId (KConfigGroup cg, int gameId);
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
    KConfigGroup m_cg;
};

#endif
