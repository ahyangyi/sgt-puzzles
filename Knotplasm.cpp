#include "Knotplasm.h"
#include "Knotconfig.h"
#include "Knotwrap.h"
#include <QPainter>
#include <QFontMetrics>
#include <QSizeF>
#include <QWidget>
#include <KDE/KConfigDialog>
 
#include <plasma/svg.h>
#include <plasma/theme.h>

#include "Knotrenderer-primitive.h"

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
    ,m_drawing(false)
{
    m_fe = new frontend();
    m_fe->plasm = this;
    
    m_renderer = new KnotrendererPrimitive();

    static const int BorderSample[4] = {1, 0, 2, 3};
    
    for (int i = 0; i < 4; i ++)
        for (int j = 0; j < 4; j ++)
        {
            m_svg[i][j].setParent(this);
            m_svg[i][j].setImagePath("widgets/background");
            m_svg[i][j].setEnabledBorders(Plasma::FrameSvg::EnabledBorders(BorderSample[i]*4+BorderSample[j]));
        }
    // this will get us the standard applet background, for free!
    setBackgroundHints(TranslucentBackground);
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
    m_me = new KnotMidEnd(this);
    m_me->newGame();
    m_port_x = 200;
    m_port_y = 200;
    m_me->size(&m_port_x, &m_port_y);
    m_me->redraw();
}
 
void Knotplasm::paintInterface(QPainter *p,
        const QStyleOptionGraphicsItem *option, const QRect &contentsRect)
{
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->setRenderHint(QPainter::Antialiasing);

    for (int i = 0; i < 4; i ++)
    {
        for (int j = 0; j < 4; j ++)
        {
            m_svg[i][j].resizeFrame(contentsRect.size() / 4);
            m_svg[i][j].paintFrame(p, contentsRect.topLeft() + QPointF(contentsRect.size().width() / 4 * i, contentsRect.size().height() / 4 * j));
        }
    }
    
    m_drawing = true;
    
    // Force the redraw
    
    m_drawing = false;
 
    // We place the icon and text
//    p->drawPixmap(7, 0, m_icon.pixmap((int)contentsRect.width(),(int)contentsRect.width()-14));
#ifdef KNOTPLASM_DEBUG
    p->save();
    p->setPen(Qt::white);
    p->drawText(contentsRect,
        Qt::AlignBottom | Qt::AlignHCenter,
        m_debug_text);
    p->restore();
#endif KNOTPLASM_DEBUG
}

void Knotplasm::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *page = new KnotConfig(parent);
    
    parent->addPage(page, i18n("General"), icon());
    
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
}

QSize Knotplasm::sizeHint() const
{
    return QSize(200, 200);
}

QSize Knotplasm::minimalSizeHint() const
{
    return QSize(80, 80);
}

void Knotplasm::drawText(int x, int y, int fonttype, int fontsize,
    int align, int colour, char *text)
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

void Knotplasm::drawPolygon(int *coords, int npoints,
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

void Knotplasm::statusBar(char *text)
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
