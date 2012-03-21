#include "Knotplasm.h"
#include "Knotconfig.h"
#include "Knotwrap.h"
#include <QPainter>
#include <QFontMetrics>
#include <QSizeF>
#include <QWidget>
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

#include "Knotrenderer.h"
#include "Knotrenderer-primitive.h"
#include "Knotrenderer-plasma.h"
#include "Knottimer.h"

class Knotplasm::Private
{
public:
    KIcon m_icon;
    
    KnotMidend* m_me;
    
    KnotRenderer* m_renderer;
    Plasma::Label* m_status;
    Plasma::PushButton* m_start;
    KnotTimer* m_timer;
    
#ifdef KNOTPLASM_DEBUG
    QString m_debug_text;
    QVector<QPair<QString,qint16> > m_debug_history;
    void kpdebug (QString s);
#endif
};

#ifdef KNOTPLASM_DEBUG
void Knotplasm::Private::kpdebug (QString s)
{
    if (m_debug_history.empty() || s != m_debug_history.at(m_debug_history.size() - 1).first)
    {
        m_debug_history.push_back(qMakePair(s, (qint16)1));
    }
    else
    {
        m_debug_history[m_debug_history.size() - 1].second ++;
    }
    
    while (m_debug_history.size() > 16)
        m_debug_history.pop_front();
    
    m_debug_text = "";
    for (QVector<QPair<QString,qint16> >::iterator it = m_debug_history.end(); it != m_debug_history.begin();)
    {
        --it;
        if (it->second==1)
            m_debug_text = QString("%1\n").arg(it->first) + m_debug_text;
        else
            m_debug_text = QString("%1 (x%2)\n").arg(it->first).arg(it->second) + m_debug_text;
    }
}
#else
#define kpdebug(...)
#endif

Knotplasm::Knotplasm(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
    ,d(new Private)
{
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    setAspectRatioMode( Plasma::IgnoreAspectRatio );

    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical, this);
    d->m_renderer = new KnotRendererPrimitive(this);
    layout->addItem(d->m_renderer);
    QGraphicsLinearLayout* statusBarLayout = new QGraphicsLinearLayout(Qt::Horizontal, layout);
    d->m_status = new Plasma::Label(this);
    d->m_status->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed, QSizePolicy::Label);
    statusBarLayout->addItem(d->m_status);
    d->m_start = new Plasma::PushButton(this);
    d->m_start->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed, QSizePolicy::PushButton);
    d->m_start->setText("New");
    statusBarLayout->addItem(d->m_start);
    statusBarLayout->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    layout->addItem(statusBarLayout);
    setLayout(layout);
    
    d->m_timer=new KnotTimer(this);
    d->m_timer->setInterval(16);
    
    // this will get us the standard applet background, for free!
    setBackgroundHints(StandardBackground);
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
    d->m_me = new KnotMidend(this);
    d->m_me->newGame();
    
    connect(d->m_start, SIGNAL(clicked()), d->m_me, SLOT(newGame()));
    connect(d->m_start, SIGNAL(clicked()), d->m_renderer, SLOT(updateAll()));
    
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
    connect(d->m_me, SIGNAL(drawUpdate(int,int,int,int)), d->m_renderer, SLOT(drawUpdate(int,int,int,int)));
   
    connect(d->m_me, SIGNAL(setColor(QList<QColor>)), d->m_renderer, SLOT(setColor(QList<QColor>)));

    connect(d->m_me, SIGNAL(statusBar(QString)), d->m_status->nativeWidget(), SLOT(setText(QString)));
    
    connect(d->m_renderer, SIGNAL(mousePressed(QPoint,Qt::MouseButton)), d->m_me, SLOT(pressButton(QPoint,Qt::MouseButton)));
    connect(d->m_renderer, SIGNAL(mouseReleased(QPoint,Qt::MouseButton)), d->m_me, SLOT(releaseButton(QPoint,Qt::MouseButton)));
    connect(d->m_renderer, SIGNAL(mouseDragged(QPoint,Qt::MouseButtons)), d->m_me, SLOT(dragButton(QPoint,Qt::MouseButtons)));
    connect(d->m_renderer, SIGNAL(keyPressed(int,Qt::KeyboardModifiers)), d->m_me, SLOT(pressKey(int,Qt::KeyboardModifiers)));
    
    connect(d->m_me, SIGNAL(activateTimer()), d->m_timer, SLOT(start()));
    connect(d->m_me, SIGNAL(deactivateTimer()), d->m_timer, SLOT(stop()));
    connect(d->m_timer, SIGNAL(tick(qreal)), d->m_me, SLOT(tickTimer(qreal)));
    
    d->m_renderer->initialize();
}

KnotMidend* Knotplasm::midend() const
{
    return d->m_me;
}
 
void Knotplasm::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *page = new KnotConfig(parent);
    
    parent->addPage(page, i18n("General"), icon());
    
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
}

void Knotplasm::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    event->accept();
//    QGraphicsItem::contextMenuEvent(event);
}

void Knotplasm::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    d->m_renderer->setFocus();
}

// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(Knotplasm, Knotplasm)
 
#include "Knotplasm.moc"
