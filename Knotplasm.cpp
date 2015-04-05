#include "Knotplasm.h"
#include "Knotconfig.h"
#include "Knotwrap.h"
#include <QPainter>
#include <QFontMetrics>
#include <QSizeF>
#include <QWidget>
#include <QString>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneContextMenuEvent>
#include <QTimer>

#include <KDE/KConfigDialog>
#include <KDE/KIcon>
 
#include <Plasma/Svg>
#include <Plasma/Label>
#include <Plasma/PushButton>
#include <Plasma/Svg>
#include <Plasma/Theme>
#include <Plasma/Extender>
#include <Plasma/ExtenderItem>
#include <Plasma/ScrollWidget>

#include "Knotrenderer.h"
#include "Knotrenderer-primitive.h"
#include "Knotrenderer-plasma.h"
#include "Knotrenderer-batch.h"
#include "Knottimer.h"
#include "Knotmagic.h"

#include "Knotdebug.h"

class Knotplasm::Private
{
public:
    KIcon m_icon;
    
    KnotMidend* m_me;
    
    KnotRenderer* m_renderer;
    Plasma::Label* m_status;
    Plasma::PushButton* m_start;
    Plasma::PushButton* m_solve;
    KnotTimer* m_timer;

    KnotGameConfig *m_game_page;
    KnotDisplayConfig *m_display_page;
    
    KnotGameStateTracker *m_game_state_tracker;

    Private (): m_me(nullptr)
    {
    }
};

#ifdef KNOTPLASM_DEBUG
static Plasma::Label* __debug_label = nullptr;
static Plasma::ScrollWidget* __debug_area = nullptr;

static QMap<QString,QVector<QPair<QString,qint16> > > __debug_history;

#include <QMap>

void knotDebugClear (QString category)
{
    __debug_history[category].clear();
}

void knotDebugAppend (QString category, QString s)
{
    if (__debug_history[category].empty() || s != __debug_history[category].at(__debug_history[category].size() - 1).first)
    {
        __debug_history[category].push_back(qMakePair(s, (qint16)1));
    }
    else
    {
        __debug_history[category][__debug_history[category].size() - 1].second ++;
    }
}

void knotDebugFlush ()
{
    QString m_debug_text = "";
    for (QMap<QString, QVector<QPair<QString,qint16> > >::iterator jt = __debug_history.begin(); jt != __debug_history.end(); ++jt)
    {
        m_debug_text = m_debug_text + jt.key() + ":\n";
        for (QVector<QPair<QString,qint16> >::iterator it = (jt.value()).begin(); it != (jt.value()).end(); ++ it)
        {
            if (it->second==1)
                m_debug_text = m_debug_text + QString("%1\n").arg(it->first);
            else
                m_debug_text = m_debug_text + QString("%1 (x%2)\n").arg(it->first).arg(it->second);
        }
        m_debug_text = m_debug_text + "\n\n";
    }
    
    __debug_label->setText(m_debug_text);
}
#endif

Knotplasm::Knotplasm(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
    ,d(new Private)
{
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    setAspectRatioMode( Plasma::IgnoreAspectRatio );

    QGraphicsLinearLayout* mainLayout = new QGraphicsLinearLayout(Qt::Vertical, this);
    
    {
        /* Vertical component 1: the gaming area */
        d->m_renderer = new KnotRendererPlasma(this);
        mainLayout->addItem(d->m_renderer);
    }
        
    {
        /* Vertical component 2: the status bar */
        QGraphicsLinearLayout* statusBarLayout = new QGraphicsLinearLayout(Qt::Horizontal, mainLayout);
        
        {
            /* Status information */
            d->m_status = new Plasma::Label(this);
            d->m_status->setPreferredWidth(32);
            d->m_status->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            statusBarLayout->addItem(d->m_status);
        }
        
        {
            /* "New game" button */
            d->m_start = new Plasma::PushButton(this);
            d->m_start->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed, QSizePolicy::PushButton);
            d->m_start->setText("New");
            statusBarLayout->addItem(d->m_start);
        }
        
        {
            /* "Solve" button */
            d->m_solve = new Plasma::PushButton(this);
            d->m_solve->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed, QSizePolicy::PushButton);
            d->m_solve->setText("Solve");
            statusBarLayout->addItem(d->m_solve);
        }
        
        statusBarLayout->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
        mainLayout->addItem(statusBarLayout);
    }
    
#ifdef KNOTPLASM_DEBUG
    {
        /* Vertical component 3: the silly debug info area*/
        
        if (__debug_area == nullptr)
        {
            __debug_area = new Plasma::ScrollWidget(this);
            __debug_label = new Plasma::Label(this);
            __debug_area->setWidget(__debug_label);

            __debug_label->setTextSelectable(true);
            __debug_label->setWordWrap(false);
            __debug_area->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed, QSizePolicy::Frame);
            __debug_area->setPreferredHeight(120);

            mainLayout->addItem(__debug_area);
        }
    }
#endif
    
    setLayout(mainLayout);

    d->m_timer=new KnotTimer(this);
    d->m_timer->setInterval(KNOTPLASM_TIMER_INTERVAL);

    connect(d->m_start, SIGNAL(clicked()), d->m_renderer, SLOT(updateAll()));
    
    setBackgroundHints(StandardBackground);
//    setBackgroundHints(TranslucentBackground);
}
 
Knotplasm::~Knotplasm()
{
    if (hasFailedToLaunch()) {
        // Do some cleanup here
    } else {
        // Save settings
    }
}
 
void Knotplasm::init()
{
    {    
        KConfigGroup cg = config();

        d->m_game_state_tracker = new KnotGameStateTracker(cg);
    }
    
    configChanged();
}

KnotMidend* Knotplasm::midend() const
{
    return d->m_me;
}
 
void Knotplasm::createConfigurationInterface(KConfigDialog *parent)
{
    KConfigGroup cg = config();

    d->m_game_page = new KnotGameConfig(parent, cg);
    d->m_display_page = new KnotDisplayConfig(parent, cg);
    
    parent->addPage(d->m_game_page, i18n("Game"), "applications-games-strategy");
    parent->addPage(d->m_display_page, i18n("Display"), "preferences-desktop");
    
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
}

void Knotplasm::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    if (event->reason()==QGraphicsSceneContextMenuEvent::Mouse &&
        d->m_renderer->contains(mapToItem(d->m_renderer, event->pos())))
        event->accept();
    else
        QGraphicsItem::contextMenuEvent(event);
}

void Knotplasm::configAccepted()
{
    if (d->m_game_page->saveConfig())
        emit (configNeedsSaving());
}

void Knotplasm::setStatus(QString s)
{
    d->m_status->setText(s);
}

void Knotplasm::configChanged()
{
    KConfigGroup cg = config();

    Plasma::Applet::configChanged();
    
    int gameId = KnotConfig::getGameId(cg);
    int presetId = KnotConfig::getPresetId(cg);
    
    bool loadSavedGame = (d->m_me == nullptr);
    
    if (d->m_me != nullptr)
        delete d->m_me;

    d->m_me = new KnotMidend(this, gameId);

    if (presetId == -1)
    {
        KnotGameParamList list = d->m_me->getConfig();
        for (KnotGameParamList::iterator it = list.begin(); it != list.end(); ++ it)
            KnotConfig::setKnotGameParam (cg, *it);
        d->m_me->setConfig(list);
    }
    else
        d->m_me->setPreset(presetId);
    
    connect(d->m_start, SIGNAL(clicked()), d->m_me, SLOT(newGame()));
    connect(d->m_start, SIGNAL(clicked()), d->m_renderer, SLOT(geometryChangedHandler()));          /* This is needed because newGaming with different params need an additional call to size()*/
    connect(d->m_start, SIGNAL(clicked()), this, SLOT(grabFocus()));          /* This is needed because newGaming with different params need an additional call to size()*/

    connect(d->m_solve, SIGNAL(clicked()), d->m_me, SLOT(solve()));

    connect(d->m_renderer, SIGNAL(redrawRequest()), d->m_me, SLOT(redraw()));
    connect(d->m_renderer, SIGNAL(forceRedrawRequest()), d->m_me, SLOT(forceRedraw()));
    connect(d->m_renderer, SIGNAL(sizeRequest(int*,int*)), d->m_me, SLOT(size(int*,int*)));
    connect(d->m_renderer, SIGNAL(colorRequest(QColor)), d->m_me, SLOT(color(QColor)));
    connect(d->m_me, SIGNAL(drawText(int,int,bool,int,int,int,QString)), d->m_renderer, SLOT(drawText(int,int,bool,int,int,int,QString)));
    connect(d->m_me, SIGNAL(drawRect(int,int,int,int,int)), d->m_renderer, SLOT(drawRect(int,int,int,int,int)));
    connect(d->m_me, SIGNAL(drawLine(int,int,int,int,int)), d->m_renderer, SLOT(drawLine(int,int,int,int,int)));
    connect(d->m_me, SIGNAL(drawPolygon(QPolygon,int,int)), d->m_renderer, SLOT(drawPolygon(QPolygon,int,int)));
    connect(d->m_me, SIGNAL(drawCircle(int,int,int,int,int)), d->m_renderer, SLOT(drawCircle(int,int,int,int,int)));
    connect(d->m_me, SIGNAL(drawThickLine(float,float,float,float,float,int)), d->m_renderer, SLOT(drawThickLine(float,float,float,float,float,int)));
    connect(d->m_me, SIGNAL(clip(int,int,int,int)), d->m_renderer, SLOT(clip(int,int,int,int)));
    connect(d->m_me, SIGNAL(unclip()), d->m_renderer, SLOT(unclip()));
    connect(d->m_me, SIGNAL(startDraw()), d->m_renderer, SLOT(startDraw()));
    connect(d->m_me, SIGNAL(endDraw()), d->m_renderer, SLOT(endDraw()));
    connect(d->m_me, SIGNAL(drawUpdate(int,int,int,int)), d->m_renderer, SLOT(drawUpdate(int,int,int,int)));

    connect(d->m_me, SIGNAL(setColor(QList<QColor>)), d->m_renderer, SLOT(setColor(QList<QColor>)));

    connect(d->m_me, SIGNAL(statusBar(QString)), this, SLOT(setStatus(QString)));
    
    connect(d->m_renderer, SIGNAL(mousePressed(QPoint,Qt::MouseButton)), d->m_me, SLOT(pressButton(QPoint,Qt::MouseButton)));
    connect(d->m_renderer, SIGNAL(mouseReleased(QPoint,Qt::MouseButton)), d->m_me, SLOT(releaseButton(QPoint,Qt::MouseButton)));
    connect(d->m_renderer, SIGNAL(mouseDragged(QPoint,Qt::MouseButtons)), d->m_me, SLOT(dragButton(QPoint,Qt::MouseButtons)));
    connect(d->m_renderer, SIGNAL(keyPressed(int,Qt::KeyboardModifiers)), d->m_me, SLOT(pressKey(int,Qt::KeyboardModifiers)));
    
    connect(d->m_me, SIGNAL(activateTimer()), d->m_timer, SLOT(start()));
    connect(d->m_me, SIGNAL(deactivateTimer()), d->m_timer, SLOT(stop()));
    connect(d->m_timer, SIGNAL(tick(qreal)), d->m_me, SLOT(tickTimer(qreal)));
    
    connect(d->m_me, SIGNAL(stateChanged(QString)), d->m_game_state_tracker, SLOT(gameStateChanged(QString)));
    connect(d->m_me, SIGNAL(stateChanged(QString)), this, SIGNAL(configNeedsSaving()));

    if (loadSavedGame && d->m_game_state_tracker->gameState() != "")
    {
        if (!d->m_me->deserialize(d->m_game_state_tracker->gameState()))
        {
            d->m_me->newGame();
        }
    }
    else
    {
        d->m_me->newGame();
    }
    
    setStatus("");
    d->m_renderer->configChangedHandler(cg);
    grabFocus();
}


void Knotplasm::grabFocus()
{
    d->m_renderer->setFocus();
}

void Knotplasm::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    grabFocus();
}

K_EXPORT_PLASMA_APPLET(Knotplasm, Knotplasm)
 
#include "Knotplasm.moc"
