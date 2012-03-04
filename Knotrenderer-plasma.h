
#ifndef Knotrenderer_plasma_HEADER
#define Knotrenderer_plasma_HEADER

#include <Plasma/FrameSvg>

#include "Knotrenderer.h"

struct PaintInterfaceData;

class KnotRendererPlasma: public KnotRenderer
{
    Q_OBJECT
public:
    KnotRendererPlasma (QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~KnotRendererPlasma();

    virtual void paintInterface(QPainter *p,
            const QStyleOptionGraphicsItem *option,
            const QRectF& contentsRect);

public slots:
    // Real SGT APIs
    virtual void drawText(int x, int y, int fonttype, int fontsize,
        int align, int colour, const QString& text);
    virtual void drawRect(int x, int y, int w, int h, int colour);
    virtual void drawLine(int x1, int y1, int x2, int y2,
                int colour);
    virtual void drawPolygon(const QPolygon& polygon,
        int fillcolour, int outlinecolour);
    virtual void drawCircle(int cx, int cy, int radius,
        int fillcolour, int outlinecolour);
    virtual void drawUpdate(int x, int y, int w, int h);
    virtual void clip(int x, int y, int w, int h);
    virtual void unclip();
    virtual void startDraw();
    virtual void endDraw();
    virtual void drawThickLine(float thickness,
        float x1, float y1, float x2, float y2,
        int colour);
    
    // Pseudo APIs which interacts in the same way
    virtual void notStarted();
    virtual void setColor(QList<QColor> colorList);
    
private:
    class Private;
    
    Private* d;
};

#endif