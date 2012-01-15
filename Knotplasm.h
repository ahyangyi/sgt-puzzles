#ifndef Knotplasm_HEADER
#define Knotplasm_HEADER

#include <KIcon>
 
#include <Plasma/Applet>
#include <Plasma/Svg>
 
class QSizeF;
 
// Define our plasma Applet
class Knotplasm : public Plasma::Applet
{
    Q_OBJECT
    public:
        // Basic Create/Destroy
        Knotplasm(QObject *parent, const QVariantList &args);
        ~Knotplasm();
 
        // The paintInterface procedure paints the applet to screen
        void paintInterface(QPainter *p,
                const QStyleOptionGraphicsItem *option,
                const QRect& contentsRect);
    void init();
 
    private:
        Plasma::FrameSvg m_svg[4][4];
        KIcon m_icon;
};
 
#endif
