#ifndef Knotrenderer_batch_HEADER
#define Knotrenderer_batch_HEADER

#include <Plasma/FrameSvg>

#include "Knotrenderer.h"

struct PaintInterfaceData;
struct KnotBatchAction;

class KnotRendererBatch: public KnotRenderer
{
    Q_OBJECT
public:
    KnotRendererBatch (QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~KnotRendererBatch();

    virtual void paintInterface(QPainter *p,
            const QStyleOptionGraphicsItem *option,
            const QRectF& contentsRect);

    virtual void themeChangedHandler ();

public slots:
    // Real SGT APIs
    virtual void drawText(int x, int y, bool monospace, int fontsize,
        int align, int colour, const QString& text);
    virtual void drawRect(int x, int y, int w, int h, int colour);
    virtual void drawLine(int x1, int y1, int x2, int y2,
                int colour);
    virtual void drawPolygon(const QPolygon& polygon,
        int fillcolour, int outlinecolour);
    virtual void drawCircle(int cx, int cy, int radius,
        int fillcolour, int outlinecolour);
    virtual void drawUpdate(int x, int y, int w, int h);
    virtual void updateAll ();
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
    
protected:
    virtual void preprocessBatch(QList<QColor> colorList);
    
    struct PaintInterfaceData
    {
        QPainter *p;
        const QStyleOptionGraphicsItem *option;
        
        PaintInterfaceData () {}
        PaintInterfaceData (QPainter *n_p, const QStyleOptionGraphicsItem *n_option):
            p(n_p), option(n_option){}
            
        void set(int fillColour, int outlineColour, int outlineWidth, const QList<QColor> colorList);
    };

    struct PaintInterfaceData* m_paint_interface;
    
    struct KnotBatchAction
    {
        virtual QString getType () = 0;
        virtual QString toString () = 0;
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list) = 0;
        
        virtual ~KnotBatchAction () {}
    };

    struct KnotBatchTextAction : public KnotBatchAction
    {
        int x, y, fontsize, align, colour;
        bool monospace;
        QString text;
        
        KnotBatchTextAction(int n_x, int n_y, bool n_monospace, int n_fontsize,
            int n_align, int n_colour, const QString& n_text): 
            x(n_x), y(n_y), fontsize(n_fontsize), align(n_align), colour(n_colour), text(n_text){}

        virtual QString getType () {return "text";}
        virtual QString toString () {return QString("text \"%1\" at %2 %3, size %4, align %5").arg(text).arg(x).arg(y).arg(fontsize).arg(align,4,16,QChar('0'));}
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
    };

    struct KnotBatchRectAction : public KnotBatchAction
    {
        int x, y, w, h, colour;
        
        KnotBatchRectAction(int n_x, int n_y, int n_w, int n_h, int n_colour):
            x(n_x), y(n_y), w(n_w), h(n_h), colour(n_colour){}

        virtual QString getType () {return "rect";}
        virtual QString toString () {return QString("rect at %1 %2 %3 %4, color %5").arg(x).arg(y).arg(w).arg(h).arg(colour);}
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
    };

    struct KnotBatchLineAction : public KnotBatchAction
    {
        int x1, y1, x2, y2, colour;
        
        KnotBatchLineAction(int n_x1, int n_y1, int n_x2, int n_y2,
                    int n_colour): x1(n_x1), y1(n_y1), x2(n_x2), y2(n_y2), colour(n_colour){}

        virtual QString getType () {return "line";}
        virtual QString toString () {return QString("line from %1 %2 to %3 %4").arg(x1).arg(y1).arg(x2).arg(y2);}
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
    };

    struct KnotBatchPolyAction : public KnotBatchAction
    {
        int fillColour, outlineColour;
        QPolygon polygon;
        
        KnotBatchPolyAction(const QPolygon& n_polygon,
            int n_fillcolour, int n_outlinecolour): 
            polygon(n_polygon), fillColour(n_fillcolour), outlineColour(n_outlinecolour){}

        virtual QString getType () {return "poly";}
        virtual QString toString () {return QString("polyon");}
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
    };

    struct KnotBatchCircleAction : public KnotBatchAction
    {
        int cx, cy, radius, fillColour, outlineColour;
        
        KnotBatchCircleAction(int n_cx, int n_cy, int n_radius,
            int n_fillcolour, int n_outlinecolour):
            cx(n_cx), cy(n_cy), radius(n_radius), fillColour(n_fillcolour), outlineColour(n_outlinecolour){}

        virtual QString getType () {return "circle";}
        virtual QString toString () {return QString("circle at %1 %2 radius %3").arg(cx).arg(cy).arg(radius);}
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
    };

    struct KnotBatchClipAction : public KnotBatchAction
    {
        int x, y, w, h;

        KnotBatchClipAction(int n_x, int n_y, int n_w, int n_h):
            x(n_x), y(n_y), w(n_w), h(n_h){}

        virtual QString getType () {return "clip";}
        virtual QString toString () {return QString("clip %1 %2 %3 %4").arg(x).arg(y).arg(w).arg(h);}
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
    };

    struct KnotBatchUnclipAction : public KnotBatchAction
    {
        virtual QString getType () {return "unclip";}
        virtual QString toString () {return "unclip";}
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
    };

    struct KnotBatchThickAction : public KnotBatchAction
    {
        float thickness, x1, y1, x2, y2;
        int colour;
        
        KnotBatchThickAction (float n_thickness,
            float n_x1, float n_y1, float n_x2, float n_y2,
            int n_colour): thickness(n_thickness), x1(n_x1),
            y1(n_y1), x2(n_x2), y2(n_y2), colour(n_colour){}

        virtual QString getType () {return "thick";}
        virtual QString toString () {return QString("thick line from %1 %2 to %3 %4").arg(x1).arg(y1).arg(x2).arg(y2);}
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
    };

    QList<KnotBatchAction*> m_batch;
    QList<QColor> m_color_list;
};

#endif