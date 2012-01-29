#include "Knotconfig.h"

#include <QVBoxLayout>
#include <KDE/KLocalizedString>

KnotConfig::KnotConfig(QWidget *parent) :QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    m_mainLayout = new QGridLayout();
    
    m_game = new KComboBox(this);
    m_game->addItem(i18n("galaxies"));
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
