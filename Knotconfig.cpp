#include "Knotconfig.h"
#include "Knotdebug.h"

#include <QString>
#include <QVBoxLayout>
#include <KDE/KLocalizedString>
#include <kconfiggroup.h>

struct KnotGameConfig::Private
{
    QLabel* m_gameLabel;
    QLabel* m_presetLabel;
    KComboBox* m_game;
    KComboBox* m_preset;
    QGroupBox* m_params_group;
    
    QGridLayout *m_mainLayout;
    KConfigGroup m_cg;
};

int KnotConfig::getGameId(KConfigGroup cg)
{
    return cg.readEntry("Game", 0);
}

int KnotConfig::getPresetId(KConfigGroup cg)
{
    int gameId = getGameId(cg);
    int presetId = cg.readEntry(QString("Preset_%1").arg(gameId), 0);
    
    knotDebugAppend ("KnotConfig::getPresetId", QString("gameid is %1, presetid is %2").arg(gameId).arg(presetId));
    
    return presetId;
}

int KnotConfig::getPresetId(KConfigGroup cg, int gameId)
{
    int presetId = cg.readEntry(QString("Preset_%1").arg(gameId), 0);
    
    knotDebugAppend ("KnotConfig::getPresetId", QString("gameid is %1, presetid is %2").arg(gameId).arg(presetId));
    
    return presetId;
}

KnotGameConfig::KnotGameConfig(QWidget *parent, KConfigGroup cg) :QWidget(parent)
{
    int gameId = KnotConfig::getGameId(cg);
    
    d = new Private();

    d->m_cg = cg;
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    d->m_mainLayout = new QGridLayout();
    
    d->m_game = new KComboBox(this);
    d->m_game->setEditable(false);
    for (int i = 0; i < gamecount; i ++)
        d->m_game->addItem(i18n(gamelist[i]->name));
#ifndef KNOT_FINAL
        i18n("blackbox");
        i18n("bridges");
        i18n("cube");
        i18n("galaxies");
        i18n("fifteen");
        i18n("solo");
#endif
    d->m_game->setCurrentIndex(gameId);
    d->m_game->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect (d->m_game, SIGNAL(currentIndexChanged(int)), this, SLOT(gameChanged(int)));

    d->m_preset = new KComboBox(this);
    d->m_preset->setEditable(false);
    d->m_preset->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    d->m_gameLabel = new QLabel(this);
    d->m_gameLabel->setText(i18n("Game:"));
    d->m_presetLabel = new QLabel(this);
    d->m_presetLabel->setText(i18n("Preset:"));
    
    d->m_params_group = new QGroupBox(this);
    d->m_params_group->setTitle(i18n("Parameters"));
    d->m_params_group->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    
    d->m_mainLayout->addWidget(d->m_gameLabel, 0, 0); 
    d->m_mainLayout->addWidget(d->m_game, 0, 1);
    d->m_mainLayout->addWidget(d->m_presetLabel, 1, 0); 
    d->m_mainLayout->addWidget(d->m_preset, 1, 1);
    d->m_mainLayout->addWidget(d->m_params_group, 2, 0, 1, 2);
    
    connect (d->m_game, SIGNAL(currentIndexChanged(int)), parent, SLOT(settingsModified()));
    connect (d->m_preset, SIGNAL(currentIndexChanged(int)), parent, SLOT(settingsModified()));
    
    layout->addLayout(d->m_mainLayout);
    layout->addStretch();
    
    setLayout(layout);

    gameChanged(cg.readEntry("Game", 0));
}

bool KnotGameConfig::saveConfig()
{
    int gameId = d->m_game->currentIndex();
    int presetId = d->m_preset->currentIndex();
    
    d->m_cg.writeEntry("Game", gameId);
    d->m_cg.writeEntry(QString("Preset_%1").arg(gameId), presetId);
    
    return true;
}

void KnotGameConfig::gameChanged(int id)
{
    d->m_preset->clear();
    
    KnotMidend *me = new KnotMidend(NULL, id);
    QList<QPair<QString, KnotGameParams> > list = me->presetList();
    for (QList<QPair<QString, KnotGameParams> >::iterator it = list.begin(); it != list.end(); ++ it)
        d->m_preset->addItem(it->first);

    int presetId = KnotConfig::getPresetId(d->m_cg, id);
    d->m_preset->setCurrentIndex(presetId);
    
    delete me;
}

KnotGameConfig::~KnotGameConfig()
{
}

KnotDisplayConfig::KnotDisplayConfig(QWidget* parent, KConfigGroup cg): QWidget(parent), m_cg(cg)
{

}

KnotDisplayConfig::~KnotDisplayConfig()
{

}

#include "Knotconfig.moc"
