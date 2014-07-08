#include "Knotplasmagamehandler.h"
#include "Knotconfig.h"
#include <cmath>
#include <typeinfo>
#include "QPainter"
#include "KIconEffect"

class KnotplasmaFrameThemedCircleAction : public KnotPlasmaCircleAction
{
public:
    KnotplasmaFrameThemedCircleAction(int cx, int cy, int radius, int style, bool canGrow, StyleHint styleHint):
        KnotPlasmaCircleAction(cx, cy, radius, style, canGrow, styleHint) {}
    virtual ~KnotplasmaFrameThemedCircleAction () {}

    virtual QString toString () {return QString("plasma-circle-frame-themed at %1 %2 radius %3, style %4").arg(cx).arg(cy).arg(radius).arg(style);}
    virtual void apply (KnotRendererBatch::PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
};

class DefaultCircleActionFactory : public KnotPlasmaCircleActionFactory
{
public:
    virtual ~DefaultCircleActionFactory () {}
    virtual KnotPlasmaCircleAction* getAction (int cx, int cy, int radius, int style, bool canGrow, KnotPlasmaCircleAction::StyleHint styleHint)
    {
        return new KnotplasmaFrameThemedCircleAction(cx, cy, radius, style, canGrow, styleHint);
    }
};

class KnotplasmaThemedRectAction : public KnotPlasmaRectAction
{
public:
    KnotplasmaThemedRectAction(int n_x, int n_y, int n_w, int n_h, bool n_t, bool n_b, bool n_l, bool n_r, KnotplasmaThemedRectAction::StyleHint n_styleHint, Colorizer* n_colorizer):
        KnotPlasmaRectAction(n_x, n_y, n_w, n_h, n_t, n_b, n_l, n_r, n_styleHint, n_colorizer) {}
    virtual ~KnotplasmaThemedRectAction () {}

    virtual QString toString () {return QString("plasma-rect-themed at %1 %2 %3 %4, style %5, edge %6/%7/%8/%9").arg(x).arg(y).arg(w).arg(h).arg((int)styleHint).arg(t).arg(b).arg(l).arg(r);}
    virtual void apply (KnotRendererBatch::PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
};

class DefaultRectActionFactory : public KnotPlasmaRectActionFactory
{
public:
    virtual ~DefaultRectActionFactory () {}
    virtual KnotPlasmaRectAction* getAction (int x, int y, int w, int h, 
                                             bool t = true, bool b = true, bool l = true, bool r = true, 
                                             KnotPlasmaRectAction::StyleHint styleHint = KnotPlasmaRectAction::DEFAULT, Colorizer* colorizer = nullptr)
    {
        return new KnotplasmaThemedRectAction(x, y, w, h, t, b, l, r, styleHint, colorizer);
    }
};

class KnotplasmaThemedBlockAction : public KnotPlasmaBlockAction
{
public:
    KnotplasmaThemedBlockAction(int n_x, int n_y, int n_w, int n_h):
        KnotPlasmaBlockAction(n_x, n_y, n_w, n_h) {}
    virtual ~KnotplasmaThemedBlockAction () {}

    virtual QString toString () {return QString("plasma-block-themed at %1 %2 %3 %4, style %5, edge %6").arg(x).arg(y).arg(w).arg(h);}
    virtual void apply (KnotRendererBatch::PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
};

class DefaultBlockActionFactory : public KnotPlasmaBlockActionFactory
{
public:
    virtual ~DefaultBlockActionFactory () {}
    virtual KnotPlasmaBlockAction* getAction (int x, int y, int w, int h)
    {
        return new KnotplasmaThemedBlockAction(x, y, w, h);
    }
};

class KnotplasmaThemedArrowAction : public KnotPlasmaArrowAction
{
public:
    KnotplasmaThemedArrowAction(int n_cx, int n_cy, int n_radius, Type n_type):
        KnotPlasmaArrowAction(n_cx, n_cy, n_radius, n_type) {}

    virtual QString toString () {return QString("plasma-arrow-themed at %1 %2 %3").arg(cx).arg(cy).arg(radius);}
    virtual void apply (KnotRendererBatch::PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
};

class DefaultArrowActionFactory : public KnotPlasmaArrowActionFactory
{
public:
    virtual ~DefaultArrowActionFactory () {}
    virtual KnotPlasmaArrowAction* getAction (int cx, int cy, int radius, KnotPlasmaArrowAction::Type type)
    {
        return new KnotplasmaThemedArrowAction(cx, cy, radius, type);
    }
};

DefaultGameHandler::DefaultGameHandler(const GameHandlerFactories& factories): m_factories(factories)
{
}

void DefaultGameHandler::genericRemoveSpace(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    if (typeid(**batch.begin()) == typeid(KnotRendererBatch::KnotBatchRectAction))
    {
        batch.erase(batch.begin());
    }
}

bool DefaultGameHandler::containsByElements(const QPointF& point, const QList< std::shared_ptr< KnotRendererBatch::KnotBatchAction > >& batch, const QSizeF& size)
{
    for (auto it = batch.begin(); it != batch.end(); ++it)
        if ((*it)->contains(point))
            return true;
    return false;
}

bool DefaultGameHandler::containsByPreprocessedElements(const QPointF& point, const QList< std::shared_ptr< KnotRendererBatch::KnotBatchAction > >& batch, const QSizeF& size)
{
    QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>> copy = batch;
    
    preprocessBatch(copy);
    return containsByElements(point, copy, size);
}

void DefaultGameHandler::getRealDimensionByBoundingBox(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    double x1 = x, x2 = 0, y1 = y, y2 = 0;
    
    for (int i = 0; i < batch.size(); ++i)
    {
        QRectF bbox;
        
        bbox = batch[i]->boundingBox();
        x1 = qMin(x1, floor(bbox.left()));
        x2 = qMax(x2, ceil(bbox.right()));
        y1 = qMin(y1, floor(bbox.top()));
        y2 = qMax(y2, ceil(bbox.bottom()));
    }
    
    if (x1 <= x2)
    {
        x = ceil(x2) - floor(x1);
        y = ceil(y2) - floor(y1);
        ox = floor(x1);
        oy = floor(y1);
    }
}

DefaultGameHandler::~DefaultGameHandler()
{

}

bool DefaultGameHandler::contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return (QRectF(QPointF(0,0), size).contains(point));
}

void DefaultGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    this->preprocessBatch(batch);
    this->getRealDimensionByBoundingBox(x, y, ox, oy, batch);
}

void DefaultGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    genericRemoveSpace(batch);
}

void DefaultGameHandler::free()
{
    delete this;
}

class GameHandlerFactoryImpl::Private
{
public:
    static GameHandlerFactoryImpl* instance;
};

GameHandlerFactoryImpl* GameHandlerFactoryImpl::Private::instance = nullptr;

GameHandlerFactoryImpl::GameHandlerFactoryImpl()
{
}

GameHandlerFactoryImpl::~GameHandlerFactoryImpl()
{

}

KnotRendererPlasma::GameHandler* GameHandlerFactoryImpl::getGameHandler(const KConfigGroup& cg)
{
    QString gameName = KnotConfig::getGameName(cg);
    GameHandlerFactories factories;
    
    factories.block_factory = new DefaultBlockActionFactory();
    factories.circle_factory = new DefaultCircleActionFactory();
    factories.rect_factory = new DefaultRectActionFactory();
    factories.arrow_factory = new DefaultArrowActionFactory();
    
    if (gameName == "Black Box")
        return new BlackBoxGameHandler(factories);
    if (gameName == "Bridges")
        return new BridgesGameHandler(factories);
    if (gameName == "Cube")
        return new CubeGameHandler(factories);
    if (gameName == "Dominosa")
        return new DominosaGameHandler(factories);
    if (gameName == "Fifteen")
        return new FifteenGameHandler(factories);
    if (gameName == "Filling")
        return new FillingGameHandler(factories);
    if (gameName == "Flip")
        return new FlipGameHandler(factories);
    if (gameName == "Galaxies")
        return new GalaxiesGameHandler(factories);
    if (gameName == "Inertia")
        return new InertiaGameHandler(factories);
    if (gameName == "Loopy")
        return new LoopyGameHandler(factories);
    if (gameName == "Map")
        return new MapGameHandler(factories);
    if (gameName == "Mines")
        return new MinesGameHandler(factories);
    if (gameName == "Pearl")
        return new PearlGameHandler(factories);
    if (gameName == "Range")
        return new RangeGameHandler(factories);
    if (gameName == "Same Game")
        return new SameGameGameHandler(factories);
    if (gameName == "Signpost")
        return new SignpostGameHandler(factories);
    if (gameName == "Singles")
        return new SinglesGameHandler(factories);
    if (gameName == "Sixteen")
        return new SixteenGameHandler(factories);
    if (gameName == "Slant")
        return new SlantGameHandler(factories);
    if (gameName == "Slide")
        return new SlideGameHandler(factories);
    if (gameName == "Solo")
        return new SoloGameHandler(factories);
    if (gameName == "Towers")
        return new TowersGameHandler(factories);
    if (gameName == "Undead")
        return new UndeadGameHandler(factories);
    if (gameName == "Unequal")
        return new UnequalGameHandler(factories);
    if (gameName == "Unruly")
        return new UnrulyGameHandler(factories);
    if (gameName == "Untangle")
        return new UntangleGameHandler(factories);
    
    return new DefaultGameHandler(factories);
}


GameHandlerFactoryImpl* GameHandlerFactoryImpl::instance()
{
    if (Private::instance == nullptr)
        Private::instance = new GameHandlerFactoryImpl();
    return Private::instance;
}

KnotPlasmaCircleActionFactory::~KnotPlasmaCircleActionFactory()
{
}

KnotPlasmaRectActionFactory::~KnotPlasmaRectActionFactory()
{
}

KnotPlasmaBlockActionFactory::~KnotPlasmaBlockActionFactory()
{
}

KnotPlasmaArrowActionFactory::~KnotPlasmaArrowActionFactory()
{
}

KnotPlasmaCircleAction::~KnotPlasmaCircleAction()
{

}

void KnotplasmaThemedBlockAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    Plasma::Svg *fifteen;
    
    fifteen = new Plasma::Svg(nullptr);
    fifteen->setImagePath("fifteenPuzzle/blanksquare");
    fifteen->paint(paint_interface->p, x, y, w, h);
}

void KnotplasmaFrameThemedCircleAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    Plasma::FrameSvg *round;

/*
        d->round = new Plasma::FrameSvg(this);
        d->round->setImagePath("widgets/circular-background");
        d->round->setEnabledBorders(Plasma::FrameSvg::NoBorder);
*/
    round = new Plasma::FrameSvg(nullptr);
    if (styleHint == DEFAULT)
    {
        round->setImagePath("widgets/background");
    }
    else if (styleHint == TRANSLUCENT)
    {
        round->setImagePath("widgets/translucentbackground");
    }
    else if (styleHint == PLAIN)
    {
        round->setImagePath("widgets/frame");
        round->setElementPrefix("plain");
    }
    else if (styleHint == RAISED)
    {
        round->setImagePath("widgets/frame");
        round->setElementPrefix("raised");
    }
    else
    {
        round->setImagePath("widgets/frame");
        round->setElementPrefix("sunken");
    }
    round->setEnabledBorders(Plasma::FrameSvg::AllBorders);
    
    if (round->isValid())
    {
        qreal left, top, right, bottom;
        
        round->getMargins(left, top, right, bottom);
        if (radius * 2 >= left + right && radius * 2 >= top + bottom || canGrow)
        {
            qreal realRadius = radius;
            if ((left+right)/2 > realRadius)
                realRadius = (left+right) / 2;
            if ((top+bottom)/2 > realRadius)
                realRadius = (top+bottom) / 2;
            round->resizeFrame(QSizeF(realRadius * 2, realRadius * 2));
            round->paintFrame(paint_interface->p, QPointF(cx-realRadius, cy-realRadius));
            
            delete round;

            return;
        }
    }
    else
    {
        delete round;
    }
    
    // Some fallback here.
}

void KnotplasmaThemedRectAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    Plasma::FrameSvg *rect;

    rect = new Plasma::FrameSvg(nullptr);
    if (styleHint == DEFAULT)
    {
        rect->setImagePath("widgets/background");
    }
    else if (styleHint == TRANSLUCENT)
    {
        rect->setImagePath("widgets/translucentbackground");
    }
    else if (styleHint == PLAIN)
    {
        rect->setImagePath("widgets/frame");
        rect->setElementPrefix("plain");
    }
    else if (styleHint == RAISED)
    {
        rect->setImagePath("widgets/frame");
        rect->setElementPrefix("raised");
    }
    else
    {
        rect->setImagePath("widgets/frame");
        rect->setElementPrefix("sunken");
    }
    
    rect->setEnabledBorders(
        (t? Plasma::FrameSvg::TopBorder: Plasma::FrameSvg::NoBorder) |
        (b? Plasma::FrameSvg::BottomBorder: Plasma::FrameSvg::NoBorder) |
        (l? Plasma::FrameSvg::LeftBorder: Plasma::FrameSvg::NoBorder) |
        (r? Plasma::FrameSvg::RightBorder: Plasma::FrameSvg::NoBorder)
        );
    
    if (rect->isValid())
    {
        qreal left, top, right, bottom;
        
        rect->getMargins(left, top, right, bottom);
        
        //QPointF location(x - left, y - top);

        //rect->resizeFrame(QSizeF(w + left + right, h + top + bottom));
        
        QPointF location(x, y);

        rect->resizeFrame(QSizeF(w, h));
        
        if (colorizer)
        {
            QPixmap p = rect->framePixmap();
            QImage itmp = p.toImage();
            KIconEffect::colorize(itmp, colorizer->color, colorizer->strength);
            p = p.fromImage(itmp);
            
            paint_interface->p->drawPixmap(location, p);
        }
        else
        {
            rect->paintFrame(paint_interface->p, location);
        }
            
        delete rect;
        return;
    }
    else
    {
        delete rect;
    }
    
    // Some fallback here.
}

void KnotplasmaThemedArrowAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    Plasma::Svg *arrow;
    QString element;

    switch (type)
    {
        case UP:
            element = "up-arrow";
            break;
        case DOWN:
            element = "down-arrow";
            break;
        case LEFT:
            element = "left-arrow";
            break;
        case RIGHT:
            element = "right-arrow";
            break;
    }
    
    arrow = new Plasma::Svg(nullptr);
    arrow->setImagePath("widgets/arrows");

    QSize oldSize = arrow->elementSize(element);
    qreal multiplier = sqrt(radius*radius*4 / oldSize.height() / oldSize.width());

 //   arrow->resize(multiplier, multiplier);
    arrow->resize(arrow->size() * multiplier);
    QSize newSize = arrow->elementSize(element);
    arrow->paint(paint_interface->p, cx - newSize.width() / 2, cy - newSize.height() / 2, element);
}


#include "Knotplasmagamehandler.moc"
