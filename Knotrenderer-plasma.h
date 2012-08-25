#ifndef Knotrenderer_plasma_HEADER
#define Knotrenderer_plasma_HEADER

#include <Plasma/FrameSvg>

#include "Knotrenderer-batch.h"

struct PaintInterfaceData;

class KnotRendererPlasma: public KnotRendererBatch
{
    Q_OBJECT
public:
    KnotRendererPlasma (QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~KnotRendererPlasma();
protected:
    virtual void preprocessBatch(QList<QColor> colorList);
    void preprocessGalaxies(QList<QColor> colorList);
    
    struct KnotPlasmaRectAction : public KnotBatchAction
    {
        int x, y, w, h, style, edge;
        
        KnotPlasmaRectAction(int n_x, int n_y, int n_w, int n_h, int n_style, int n_edge):
            x(n_x), y(n_y), w(n_w), h(n_h), style(n_style), edge(n_edge){}

        virtual QString getType () {return "plasma";}
        virtual QString toString () {return QString("plasma-rect at %1 %2 %3 %4, style %5, edge %6").arg(x).arg(y).arg(w).arg(h).arg(style).arg(edge);}
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
    };

private:
    class Private;
    Private *d;
};

#endif