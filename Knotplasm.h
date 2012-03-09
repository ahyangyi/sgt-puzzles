#ifndef Knotplasm_HEADER
#define Knotplasm_HEADER

#include <Plasma/Applet>
 
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
        class Private;
        
        Private* d;
};

#endif
