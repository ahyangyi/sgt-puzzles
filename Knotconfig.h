#ifndef Knotconfig_HEADER
#define Knotconfig_HEADER

#include <KDE/KComboBox>
#include <KDE/KConfigGroup>
#include <QLabel>
#include <QGridLayout>
 
class KnotGameConfig : public QWidget
{
    Q_OBJECT
    public:
        // Basic Create/Destroy
        KnotGameConfig(QWidget *parent, KConfigGroup cg);
        ~KnotGameConfig();
        
        int game();
        
    protected:
    private:
        QLabel* m_gameLabel;
        KComboBox* m_game;
        
        QGridLayout *m_mainLayout;
        KConfigGroup m_cg;
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
