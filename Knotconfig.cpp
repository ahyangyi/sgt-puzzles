#include "Knotconfig.h"

#include <QVBoxLayout>
#include <KDE/KLocalizedString>

#include "puzzles.h"

KnotConfig::KnotConfig(QWidget *parent) :QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    m_mainLayout = new QGridLayout();
    
    m_game = new KComboBox(this);
    for (int i = 0; i < gamecount; i ++)
        m_game->addItem(i18n(gamelist[i]->name));
#ifndef KNOT_FINAL
        i18n("fifteen");
        i18n("galaxies");
#endif
    m_gameLabel = new QLabel(this);
    m_gameLabel->setText(i18n("Game:"));
    m_mainLayout->addWidget(m_gameLabel, 0, 0);
    m_mainLayout->addWidget(m_game, 0, 1);

    layout->addLayout(m_mainLayout);
    layout->addStretch();
    
    setLayout(layout);
}

KnotConfig::~KnotConfig()
{
}

#include "Knotconfig.moc"
