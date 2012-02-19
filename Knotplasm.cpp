#include "Knotplasm.h"
#include "Knotconfig.h"
#include "Knotwrap.h"
#include <QPainter>
#include <QFontMetrics>
#include <QSizeF>
#include <QWidget>
#include <QGraphicsLinearLayout>

#include <KDE/KConfigDialog>
 
#include <Plasma/Svg>
#include <Plasma/Theme>

#include "Knotrenderer-primitive.h"
#include "Knotrenderer-plasma.h"

#ifdef KNOTPLASM_DEBUG
void Knotplasm::kpdebug (QString s)
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
    ,m_icon("document")
{
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    setAspectRatioMode( Plasma::IgnoreAspectRatio );

    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical, this);
    m_renderer = new KnotRendererPlasma(this);
    layout->addItem(m_renderer);
    QGraphicsLinearLayout* statusBarLayout = new QGraphicsLinearLayout(Qt::Horizontal, layout);
    m_status = new Plasma::Label(this);
    m_status->setText("This is the status bar.");
    m_status->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed, QSizePolicy::Label);
    statusBarLayout->addItem(m_status);
    m_start = new Plasma::PushButton(this);
    m_start->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed, QSizePolicy::PushButton);
    m_start->setText("New");
    statusBarLayout->addItem(m_start);
    layout->addItem(statusBarLayout);
    setLayout(layout);

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
    m_me = new KnotMidend(this);
    
    connect(m_renderer, SIGNAL(redrawRequest()), m_me, SLOT(redraw()));
    connect(m_renderer, SIGNAL(sizeRequest(int*,int*)), m_me, SLOT(size(int*,int*)));
    connect(m_renderer, SIGNAL(colorRequest(QColor)), m_me, SLOT(color(QColor)));
    connect(m_me, SIGNAL(drawText(int,int,int,int,int,int,QString)), m_renderer, SLOT(drawText(int,int,int,int,int,int,QString)));
    connect(m_me, SIGNAL(drawRect(int,int,int,int,int)), m_renderer, SLOT(drawRect(int,int,int,int,int)));
    connect(m_me, SIGNAL(drawLine(int,int,int,int,int)), m_renderer, SLOT(drawLine(int,int,int,int,int)));
    connect(m_me, SIGNAL(drawPolygon(QPolygon,int,int)), m_renderer, SLOT(drawPolygon(QPolygon,int,int)));
    connect(m_me, SIGNAL(drawCircle(int,int,int,int,int)), m_renderer, SLOT(drawCircle(int,int,int,int,int)));
    
    connect(m_me, SIGNAL(setColor(QList<QColor>)), m_renderer, SLOT(setColor(QList<QColor>)));

    connect(m_me, SIGNAL(statusBar(QString)), m_status->nativeWidget(), SLOT(setText(QString)));
    
    m_me->newGame();
}
 
void Knotplasm::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *page = new KnotConfig(parent);
    
    parent->addPage(page, i18n("General"), icon());
    
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
}

void Knotplasm::drawText(int x, int y, int fonttype, int fontsize,
    int align, int colour, const QString& text)
{
    kpdebug("drawText() called!");
}

void Knotplasm::drawRect(int x, int y, int w, int h, int colour)
{
    kpdebug("drawRect() called!");
}

void Knotplasm::drawLine(int x1, int y1, int x2, int y2,
            int colour)
{
    kpdebug("drawLine() called!");
}

void Knotplasm::drawPolygon(const QPolygon& polygon,
    int fillcolour, int outlinecolour)
{
    kpdebug("drawPolygon() called!");
}

void Knotplasm::drawCircle(int cx, int cy, int radius,
    int fillcolour, int outlinecolour)
{
    kpdebug("drawCircle() called!");
}

void Knotplasm::drawUpdate(int x, int y, int w, int h)
{
    kpdebug("drawUpdate() called!");
}

void Knotplasm::clip(int x, int y, int w, int h)
{
    kpdebug("clip() called!");
}

void Knotplasm::unclip()
{
    kpdebug("unclip() called!");
}

void Knotplasm::startDraw()
{
    kpdebug("startDraw() called!");
}

void Knotplasm::endDraw()
{
    kpdebug("endDraw() called!");
}

void Knotplasm::statusBar(const QString& text)
{
    kpdebug(QString("statusBar() called with %1!").arg(text));
}

void Knotplasm::drawThickLine(float thickness,
    float x1, float y1, float x2, float y2,
    int colour)
{
    kpdebug("drawThickLine() called!");
}

// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(Knotplasm, Knotplasm)
 
#include "Knotplasm.moc"
