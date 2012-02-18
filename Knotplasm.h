#ifndef Knotplasm_HEADER
#define Knotplasm_HEADER

#include <KIcon>
 
#include <Plasma/Applet>
#include <Plasma/Svg>
#include <Plasma/Label>
#include <Plasma/PushButton>

#include "Knotrenderer.h"
 
struct frontend;
class KnotMidEnd;

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
        void paintInterface(QPainter *p,
                const QStyleOptionGraphicsItem *option,
                const QRect& contentsRect);
        QSize sizeHint() const;
        QSize minimalSizeHint() const;

        // Handlers of various drawing events
        void drawText(int x, int y, int fonttype, int fontsize,
            int align, int colour, char *text);
        void drawRect(int x, int y, int w, int h, int colour);
        void drawLine(int x1, int y1, int x2, int y2,
                    int colour);
        void drawPolygon(int *coords, int npoints,
            int fillcolour, int outlinecolour);
        void drawCircle(int cx, int cy, int radius,
            int fillcolour, int outlinecolour);
        void drawUpdate(int x, int y, int w, int h);
        void clip(int x, int y, int w, int h);
        void unclip();
        void startDraw();
        void endDraw();
        void statusBar(char *text);
        void drawThickLine(float thickness,
            float x1, float y1, float x2, float y2,
            int colour);

        // Interact with wrappers
        frontend* toFrontend() const;
        void* toDrawHandle() const;
        
    protected:
        
    private:
        Plasma::FrameSvg m_svg[4][4];
        KIcon m_icon;
        
        frontend* m_fe;
        KnotMidEnd* m_me;
        
        KnotRenderer* m_renderer;
        Plasma::Label* m_status;
        Plasma::PushButton* m_start;
        
        int m_port_x, m_port_y;
        bool m_drawing;
        
#ifdef KNOTPLASM_DEBUG
        QString m_debug_text;
        QVector<QPair<QString,qint16> > m_debug_history;
        void kpdebug (QString s);
#endif
};

#endif
