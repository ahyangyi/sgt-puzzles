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
public slots:
    virtual void themeChangedHandler ();
protected:
    virtual void preprocessBatch(QList<QColor> colorList);
private:
    class Private;
    Private *d;
};

#endif