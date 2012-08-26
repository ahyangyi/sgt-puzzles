#include "Knotconfig.h"

#include <QVBoxLayout>
#include <KDE/KLocalizedString>
#include <kconfiggroup.h>

#include "puzzles.h"

KnotGameConfig::KnotGameConfig(QWidget *parent, KConfigGroup cg) :QWidget(parent), m_cg(cg)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    m_mainLayout = new QGridLayout();
    
    m_game = new KComboBox(this);
    m_game->setEditable(false);
    for (int i = 0; i < gamecount; i ++)
        m_game->addItem(i18n(gamelist[i]->name));
#ifndef KNOT_FINAL
        i18n("blackbox");
        i18n("bridges");
        i18n("cube");
        i18n("galaxies");
        i18n("fifteen");
        i18n("solo");
#endif
    m_game->setCurrentIndex(m_cg.readEntry("Game", 0));
    m_gameLabel = new QLabel(this);
    m_gameLabel->setText(i18n("Game:"));
    m_mainLayout->addWidget(m_gameLabel, 0, 0); 
    m_mainLayout->addWidget(m_game, 0, 1);
    
    connect (m_game, SIGNAL(currentIndexChanged(int)), parent, SLOT(settingsModified()));
    
    layout->addLayout(m_mainLayout);
    layout->addStretch();
    
    setLayout(layout);
}

int KnotGameConfig::game()
{
    return m_game->currentIndex();
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
