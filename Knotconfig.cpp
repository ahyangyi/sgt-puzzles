#include "Knotconfig.h"
#include "Knotdebug.h"
#include "Knotwrap.h"

#include <QString>
#include <QVBoxLayout>
#include <KDE/KLocalizedString>
#include <KDE/KLineEdit>
#include <QCheckBox>
#include <kconfiggroup.h>

struct KnotGameConfig::Private
{
    QLabel* m_gameLabel;
    QLabel* m_presetLabel;
    KComboBox* m_game;
    KComboBox* m_preset;
    QGroupBox* m_params_group;
    QGridLayout *m_params_layout;
    QList<QPair<QLabel*, QWidget*> > m_params_list;
    
    QGridLayout *m_mainLayout;
    KConfigGroup m_cg;
    
    int m_preset_custom_id;
};

int KnotConfig::getGameId(KConfigGroup cg)
{
    return cg.readEntry("Game", 0);
}

int KnotConfig::getPresetId(KConfigGroup cg)
{
    int gameId = getGameId(cg);
    int presetId = cg.readEntry(QString("Preset_%1").arg(gamelist[gameId]->name), 0);
    
    return presetId;
}

int KnotConfig::getPresetId(KConfigGroup cg, int gameId)
{
    int presetId = cg.readEntry(QString("Preset_%1").arg(gamelist[gameId]->name), 0);
    
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
    
    d->m_params_layout = new QGridLayout(this);
    d->m_params_group->setLayout(d->m_params_layout);
    
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

    if (d->m_preset_custom_id == presetId)
        presetId = -1;
    
    d->m_cg.writeEntry("Game", gameId);
    d->m_cg.writeEntry(QString("Preset_%1").arg(gamelist[gameId]->name), presetId);
    
    return true;
}

void KnotGameConfig::gameChanged(int id)
{
    d->m_preset->clear();
    
    KnotMidend *me = new KnotMidend(NULL, id);
    QList<QPair<QString, KnotGameParams> > list = me->presetList();
    for (QList<QPair<QString, KnotGameParams> >::iterator it = list.begin(); it != list.end(); ++ it)
        d->m_preset->addItem(it->first);
    
    for (QList<QPair<QLabel*, QWidget*> >::iterator it = d->m_params_list.begin(); it != d->m_params_list.end(); ++ it)
    {
        delete it->first;
        if (it->second)
        {
            delete it->second;
        }
    }
    d->m_params_list.clear();
    
    if (me->canConfig())
    {
        d->m_preset->addItem(i18n("Custom"));
        d->m_preset_custom_id = list.size();
        
        KnotGameParamList paramList = me->getConfig();
        
        int x = 0;
        for (KnotGameParamList::iterator it = paramList.begin(); it != paramList.end(); ++it, ++x)
        {
            QLabel* label;
            
            label = new QLabel(this);
            label->setText(it->name);
            d->m_params_layout->addWidget(label, x, 0);
            
            QWidget *widget = NULL;
            
            switch (it->type)
            {
                case KnotGameParamItem::CONFIG_STRING:
                {
                    KLineEdit *edit = new KLineEdit(this);
                    
                    edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
                    edit->setText(it->sVal);
                    
                    d->m_params_layout->addWidget(edit, x, 1);
                    widget = edit;
                }
                    break;
                case KnotGameParamItem::CONFIG_BOOLEAN:
                {
                    QCheckBox *check = new QCheckBox(this);
                    
                    check->setChecked(it->bVal);
                    
                    d->m_params_layout->addWidget(check, x, 1);
                    widget = check;
                }
                    break;
                case KnotGameParamItem::CONFIG_CHOICES:
                {
                    KComboBox *combo = new KComboBox(this);
                    
                    for (QStringList::const_iterator jt = it->choices.begin(); jt != it->choices.end(); ++jt)
                        combo->addItem(*jt);
                    combo->setEditable(false);
                    
                    combo->setCurrentIndex(it->iVal);

                    d->m_params_layout->addWidget(combo, x, 1);
                    widget = combo;
                }
                    break;
            }
            
            d->m_params_list.push_back(qMakePair<QLabel*, QWidget*>(label, widget));
        }
//        d->m_params_group->setTitle(QString("There are %1 options").arg(x));
        d->m_params_group->updateGeometry();
    }
    else
    {
        d->m_preset_custom_id = -1;
        
        // TODO: Add some text about there's no config available
    }

    int presetId = KnotConfig::getPresetId(d->m_cg, id);
    if (presetId == -1)
        d->m_preset->setCurrentIndex(d->m_preset_custom_id);
    else
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
