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
private:
    class Private;
    Private *d;
};

#endif
