#include <QPainter>

#include "Knotrenderer-plasma.h"
#include <Plasma/Theme>

class KnotRendererPlasma::Private
{
public:
    static bool isMagical (QColor color)
    {
        return color.red() >= 21 && color.red() <= 25 &&
            color.green() >= 12 && color.green() <= 16 &&
            color.blue() >= 79 && color.blue() <= 83;
    };
};

KnotRendererPlasma::KnotRendererPlasma(QGraphicsItem* parent, Qt::WindowFlags wFlags): KnotRendererBatch(parent, wFlags)
{

}

KnotRendererPlasma::~KnotRendererPlasma()
{

}

void KnotRendererPlasma::themeChangedHandler()
{
    emit colorRequest (QColor::fromRgb(23,14,81));
}

void KnotRendererPlasma::preprocessBatch(QList<QColor> colorList)
{
    /*
     * Remove-background
     */
    for (QList<KnotBatchAction*>::iterator it = m_batch.begin(); it != m_batch.end(); ++it)
    {
        {
            KnotBatchRectAction* rectAction;
            if ((rectAction = dynamic_cast<KnotBatchRectAction*>(*it)) != NULL && rectAction->colour != -1 && Private::isMagical (colorList[rectAction->colour]) > 0)
            {
                rectAction->colour = -1;
            }
        }
    }
}

#include "Knotrenderer-plasma.moc"