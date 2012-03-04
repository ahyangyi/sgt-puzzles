#ifndef Knotplasm_HEADER
#define Knotplasm_HEADER

#include <KIcon>
 
#include <Plasma/Applet>
#include <Plasma/Svg>
#include <Plasma/Label>
#include <Plasma/PushButton>

#include "Knotrenderer.h"
 
struct frontend;
class KnotMidend;

// Define our plasma Applet
class Knotplasm : public Plasma::Applet
{
    Q_OBJECT
    public:
        // Basic Create/Destroy
        Knotplasm(QObject *parent, const QVariantList &args);
        ~Knotplasm();
 
        // Override
        void createConfigurationInterface(KConfigDialog *parent);
        void init();

        // Interact with wrappers
        KnotMidend* midend() const;
        
        virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    
    protected:
        
    private:
        KIcon m_icon;
        
        KnotMidend* m_me;
        
        KnotRenderer* m_renderer;
        Plasma::Label* m_status;
        Plasma::PushButton* m_start;
        
#ifdef KNOTPLASM_DEBUG
        QString m_debug_text;
        QVector<QPair<QString,qint16> > m_debug_history;
        void kpdebug (QString s);
#endif
};

#endif
