#ifndef Knotconfig_HEADER
#define Knotconfig_HEADER

#include <KDE/KComboBox>
#include <QLabel>
#include <QGridLayout>
 
class KnotConfig : public QWidget
{
    Q_OBJECT
    public:
        // Basic Create/Destroy
        KnotConfig(QWidget *parent);
        ~KnotConfig();
        
    protected:
    private:
        QLabel* m_gameLabel;
        KComboBox* m_game;
        
        QGridLayout *m_mainLayout;
};

#endif
