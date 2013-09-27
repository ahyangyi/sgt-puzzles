#ifndef Knotrenderer_plasma_HEADER
#define Knotrenderer_plasma_HEADER

#include "Knotrenderer-batch.h"

struct PaintInterfaceData;

class KnotRendererPlasma: public KnotRendererBatch
{
    Q_OBJECT
public:
    class GameHandler
    {
    public:
        virtual void preprocessBatch (QList<KnotBatchAction*>& batch) = 0;
        virtual void getRealDimension(int &x, int &y, int &ox, int &oy, QList<KnotBatchAction*>& batch) = 0;
        virtual bool contains (const QPointF& point, const QList<KnotBatchAction*>& batch, const QSizeF& size) = 0;
        virtual void free () = 0;
    };
    class GameHandlerFactory
    {
    public:
        virtual ~GameHandlerFactory() {}
        virtual GameHandler* getGameHandler(const KConfigGroup& cg) = 0;
    };
    KnotRendererPlasma (QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    KnotRendererPlasma (GameHandlerFactory* handlerFactory, QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~KnotRendererPlasma();
    virtual void themeChangedHandler ();
    
    // Override
    virtual bool contains(const QPointF& point) const;
public slots:
    virtual void geometryChangedHandler ();
    virtual void configChangedHandler (KConfigGroup cg);
protected:
    virtual void preprocessBatch();
    void genericRemoveSpace();
    void getRealDimension(int &x, int &y, int &ox, int &oy);
    void getRealDimensionByBoundingBox(int &x, int &y, int &ox, int &oy);
    
    GameHandlerFactory* m_handler_factory;
    GameHandler* m_handler;

public:
    struct KnotPlasmaRectAction : public KnotBatchAction
    {
        int x, y, w, h, style, edge;
        KnotRendererPlasma* parent;
        
        KnotPlasmaRectAction(int n_x, int n_y, int n_w, int n_h, int n_style, int n_edge, KnotRendererPlasma* n_parent):
            x(n_x), y(n_y), w(n_w), h(n_h), style(n_style), edge(n_edge), parent(n_parent){}

        virtual QString toString () {return QString("plasma-rect at %1 %2 %3 %4, style %5, edge %6").arg(x).arg(y).arg(w).arg(h).arg(style).arg(edge);}
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
    };

    struct KnotPlasmaBlockAction : public KnotBatchAction
    {
        int x, y, w, h;
        KnotRendererPlasma* parent;
        
        KnotPlasmaBlockAction(int n_x, int n_y, int n_w, int n_h, KnotRendererPlasma* n_parent):
            x(n_x), y(n_y), w(n_w), h(n_h), parent(n_parent){}

        virtual QString toString () {return QString("plasma-block at %1 %2 %3 %4, style %5, edge %6").arg(x).arg(y).arg(w).arg(h);}
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
        
        virtual QRectF boundingBox() {return QRectF(x, y, w, h);}
    };

    struct KnotPlasmaCircleAction : public KnotBatchAction
    {
        int cx, cy, radius, style;
        bool canGrow;
        
        KnotPlasmaCircleAction(int n_cx, int n_cy, int n_radius, int n_style, bool n_canGrow = false):
            cx(n_cx), cy(n_cy), radius(n_radius), style(n_style), canGrow(n_canGrow) {}

        virtual QString toString () {return QString("plasma-circle at %1 %2 radius %3, style %4").arg(cx).arg(cy).arg(radius).arg(style);}
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
    };
    
private:
    class Private;
    Private *d;
};

#endif
