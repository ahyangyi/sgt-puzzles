#include "Knotconfig.h"
#include "Knotdebug.h"
#include "Knotwrap.h"

#include <cassert>
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

struct KnotGameStateTracker::Private
{
    KConfigGroup m_cg;
};

int KnotConfig::getGameId(KConfigGroup cg)
{
    QString re = cg.readEntry("Game", "");
    
    for (int i = 0; i < gamecount; i ++)
    {
        if (re == gamelist[i]->name)
            return i;
    }
    
    // Fallback to the first game. Crash is acceptable if we have 0 games compiled in :)
    return 0;
}

QString KnotConfig::getGameName(KConfigGroup cg)
{
    QString re = cg.readEntry("Game", "");
    
    for (int i = 0; i < gamecount; i ++)
    {
        if (re == gamelist[i]->name)
            return re;
    }
    
    return gamelist[0]->name;
}

int KnotConfig::getPresetId(KConfigGroup cg)
{
    int gameId = getGameId(cg);
    int presetId = cg.readEntry(QString("Preset_%1").arg(sanitize(gamelist[gameId]->name)), 0);
    
    return presetId;
}

int KnotConfig::getPresetId(KConfigGroup cg, int gameId)
{
    int presetId = cg.readEntry(QString("Preset_%1").arg(sanitize(gamelist[gameId]->name)), 0);
    
    return presetId;
}

QString KnotConfig::sanitize(QString s)
{
    QString re;
    for (QString::iterator it = s.begin(); it != s.end(); ++it)
        if (*it >= 'A' && *it <= 'Z' || *it >= 'a' && *it <= 'z')
            re.push_back(*it);
    return re;
}

void KnotConfig::setKnotGameParam(KConfigGroup cg, KnotGameParamItem& item)
{
    int gameId = getGameId(cg);
    QString key = QString("Option_%1_%2").arg(sanitize(gamelist[gameId]->name)).arg(sanitize(item.name));
    
    switch (item.type)
    {
        case KnotGameParamItem::CONFIG_STRING:
            item.sVal = cg.readEntry(key, "");
            break;
        case KnotGameParamItem::CONFIG_BOOLEAN:
            item.bVal = cg.readEntry(key, false);
            break;
        case KnotGameParamItem::CONFIG_CHOICES:
            item.iVal = cg.readEntry(key, 0);
            break;
    }
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

    gameChanged(KnotConfig::getGameId(cg));
}

bool KnotGameConfig::saveConfig()
{
    int gameId = d->m_game->currentIndex();
    int presetId = d->m_preset->currentIndex();

    if (d->m_preset_custom_id == presetId)
        presetId = -1;
    
    d->m_cg.writeEntry("Game", gamelist[gameId]->name);
    d->m_cg.writeEntry(QString("Preset_%1").arg(KnotConfig::sanitize(gamelist[gameId]->name)), presetId);
    if (presetId == -1)
    {
        KnotMidend *me = new KnotMidend(nullptr, gameId);

        KnotGameParamList paramList = me->getConfig();

        int x = 0;
        for (KnotGameParamList::iterator it = paramList.begin(); it != paramList.end(); ++it, ++x)
        {
            QString key = QString("Option_%1_%2").arg(KnotConfig::sanitize(gamelist[gameId]->name)).arg(KnotConfig::sanitize(it->name));
            
            switch (it->type)
            {
                case KnotGameParamItem::CONFIG_STRING:
                {
                    KLineEdit *edit = (KLineEdit *)d->m_params_list[x].second;
                    d->m_cg.writeEntry(key, edit->text());
                }
                    break;
                case KnotGameParamItem::CONFIG_BOOLEAN:
                {
                    QCheckBox *check = (QCheckBox *)d->m_params_list[x].second;
                    d->m_cg.writeEntry(key, check->isChecked());
                }
                    break;
                case KnotGameParamItem::CONFIG_CHOICES:
                {
                    KComboBox *combo = (KComboBox *)d->m_params_list[x].second;
                    d->m_cg.writeEntry(key, combo->currentIndex());
                }
                    break;
            }
        }
        
        delete me;
    }
    
    return true;
}

void KnotGameConfig::gameChanged(int id)
{
    disconnect (d->m_preset, SIGNAL(currentIndexChanged(int)), this, SLOT(paramChanged()));

    d->m_preset->clear();
    
    KnotMidend *me = new KnotMidend(nullptr, id);
    QStringList list = me->presetList();
    for (QStringList::iterator it = list.begin(); it != list.end(); ++ it)
        d->m_preset->addItem(*it);
    
    for (auto it = d->m_params_list.begin(); it != d->m_params_list.end(); ++ it)
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
            
            QWidget *widget = nullptr;
            
            switch (it->type)
            {
                case KnotGameParamItem::CONFIG_STRING:
                {
                    KLineEdit *edit = new KLineEdit(this);
                    
                    edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

                    d->m_params_layout->addWidget(edit, x, 1);
                    connect(edit, SIGNAL(textChanged(QString)), parent(), SLOT(settingsModified()));
                    widget = edit;
                }
                    break;
                case KnotGameParamItem::CONFIG_BOOLEAN:
                {
                    QCheckBox *check = new QCheckBox(this);
                    
                    d->m_params_layout->addWidget(check, x, 1);
                    connect(check, SIGNAL(toggled(bool)), parent(), SLOT(settingsModified()));
                    widget = check;
                }
                    break;
                case KnotGameParamItem::CONFIG_CHOICES:
                {
                    KComboBox *combo = new KComboBox(this);
                    
                    for (QStringList::const_iterator jt = it->choices.begin(); jt != it->choices.end(); ++jt)
                        combo->addItem(*jt);
                    combo->setEditable(false);
                    
                    d->m_params_layout->addWidget(combo, x, 1);
                    connect(combo, SIGNAL(currentIndexChanged(int)), parent(), SLOT(settingsModified()));
                    widget = combo;
                }
                    break;
            }
            
            d->m_params_list.push_back(qMakePair<QLabel*, QWidget*>(label, widget));
        }
        d->m_params_group->updateGeometry();
    }
    else
    {
        d->m_preset_custom_id = -1;
        
        QLabel* label;
        
        label = new QLabel(this);
        label->setText(i18n("No configuration available."));
        d->m_params_layout->addWidget(label, 0, 0);
    }

    int presetId = KnotConfig::getPresetId(d->m_cg, id);
    if (presetId == -1)
    {
        d->m_preset->setCurrentIndex(d->m_preset_custom_id);
    }
    else
    {
        d->m_preset->setCurrentIndex(presetId);
    }
    delete me;

    connect (d->m_preset, SIGNAL(currentIndexChanged(int)), this, SLOT(paramChanged()));
    paramChanged();
}

void KnotGameConfig::paramCustomized()
{
    disconnect (d->m_preset, SIGNAL(currentIndexChanged(int)), this, SLOT(paramChanged()));
    d->m_preset->setCurrentIndex(d->m_preset_custom_id);
    connect (d->m_preset, SIGNAL(currentIndexChanged(int)), this, SLOT(paramChanged()));
}

void KnotGameConfig::paramChanged()
{
    int gameId = d->m_game->currentIndex();
    int presetId = d->m_preset->currentIndex();
    
    if (presetId == d->m_preset_custom_id)
        presetId = -1;
    
    KnotMidend *me = new KnotMidend(nullptr, gameId);

    if (presetId != -1)
        me->setPreset(presetId);
    KnotGameParamList paramList = me->getConfig();
    
    QList<QPair<QLabel*, QWidget*> >::iterator jt = d->m_params_list.begin();
    for (KnotGameParamList::iterator it = paramList.begin(); it != paramList.end(); ++it, ++jt)
    {
        QString key = QString("Option_%1_%2").arg(KnotConfig::sanitize(gamelist[gameId]->name)).arg(KnotConfig::sanitize(it->name));
        
        switch (it->type)
        {
            case KnotGameParamItem::CONFIG_STRING:
            {
                KLineEdit *edit = dynamic_cast<KLineEdit*>(jt->second);
                if (edit == nullptr)
                    break;
                QString str;
                if (presetId == -1 && d->m_cg.hasKey(key))
                {
                    str = d->m_cg.readEntry(key, "");
                }
                else
                {
                    str = it->sVal;
                }
                disconnect(edit, SIGNAL(textChanged(QString)), this, SLOT(paramCustomized()));
                edit->setText(str);
                connect(edit, SIGNAL(textChanged(QString)), this, SLOT(paramCustomized()));
            }
                break;
            case KnotGameParamItem::CONFIG_BOOLEAN:
            {
                QCheckBox *check = dynamic_cast<QCheckBox*>(jt->second);
                if (check == nullptr)
                    break;
                bool status;
                
                if (presetId == -1 && d->m_cg.hasKey(key))
                {
                    status = d->m_cg.readEntry(key, false);
                }
                else
                {
                    status = it->bVal;
                }
                disconnect(check, SIGNAL(toggled(bool)), this, SLOT(paramCustomized()));
                check->setChecked(status);
                connect(check, SIGNAL(toggled(bool)), this, SLOT(paramCustomized()));
            }
                break;
            case KnotGameParamItem::CONFIG_CHOICES:
            {
                KComboBox *combo = dynamic_cast<KComboBox*>(jt->second);
                if (combo == nullptr)
                    break;
                int choice;
                
                if (presetId == -1 && d->m_cg.hasKey(key))
                {
                    choice = d->m_cg.readEntry(key, 0);
                }
                else
                {
                    choice = it->iVal;
                }
                disconnect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(paramCustomized()));
                combo->setCurrentIndex(choice);
                connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(paramCustomized()));
            }
                break;
        }
    }
    
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

KnotGameStateTracker::KnotGameStateTracker(KConfigGroup cg)
{
    d = new Private();

    d->m_cg = cg;
}

void KnotGameStateTracker::gameStateChanged(const QString& state)
{
    d->m_cg.writeEntry("GameState", state);
}

QString KnotGameStateTracker::gameState()
{
    return d->m_cg.readEntry("GameState", "");
}


#include "Knotconfig.moc"
