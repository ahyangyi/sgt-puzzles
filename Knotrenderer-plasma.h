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
    virtual void themeChangedHandler ();
public slots:
    virtual void initialize (KConfigGroup cg);
    virtual void geometryChangedHandler ();
protected:
    virtual void preprocessBatch();
    void genericRemoveSpace();
    void preprocessBridges();
    void preprocessCube();
    void preprocessDominosa();
    void preprocessFifteen();
    void preprocessFilling();
    void preprocessFlip();
    void preprocessGalaxies();
    void preprocessInertia();
    void preprocessLoopy();
    void preprocessMap();
    void preprocessMines();
    void preprocessPearl();
    void preprocessRange();
    void preprocessSameGame();
    void preprocessSignpost();
    void preprocessSingles();
    void preprocessSlant();
    void preprocessSlide();
    void preprocessSolo();
    void preprocessUnequal();
    void preprocessUndead();
    void preprocessUnruly();
    void preprocessUntangle();
    void getRealDimension(int &x, int &y, int &ox, int &oy);
    void getRealDimensionByBoundingBox(int &x, int &y, int &ox, int &oy);
    
    struct KnotPlasmaRectAction : public KnotBatchAction
    {
        int x, y, w, h, style, edge;
        KnotRendererPlasma* parent;
        
        KnotPlasmaRectAction(int n_x, int n_y, int n_w, int n_h, int n_style, int n_edge):
            x(n_x), y(n_y), w(n_w), h(n_h), style(n_style), edge(n_edge){}

        virtual QString toString () {return QString("plasma-rect at %1 %2 %3 %4, style %5, edge %6").arg(x).arg(y).arg(w).arg(h).arg(style).arg(edge);}
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
    };

    struct KnotPlasmaCircleAction : public KnotBatchAction
    {
        int cx, cy, radius, style;
        KnotRendererPlasma* parent;
        
        KnotPlasmaCircleAction(int n_cx, int n_cy, int n_radius, int n_style, KnotRendererPlasma* n_parent):
            cx(n_cx), cy(n_cy), radius(n_radius), style(n_style), parent(n_parent) {}

        virtual QString toString () {return QString("plasma-circle at %1 %2 radius %3, style %4").arg(cx).arg(cy).arg(radius).arg(style);}
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
    };

private:
    class Private;
    Private *d;
};

#endif
