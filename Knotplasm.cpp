#include "Knotplasm.h"
#include <QPainter>
#include <QFontMetrics>
#include <QSizeF>
 
#include <plasma/svg.h>
#include <plasma/theme.h>
 
Knotplasm::Knotplasm(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
    m_icon("document")
{
    static const int BorderSample[4] = {1, 0, 2, 3};
    
    for (int i = 0; i < 4; i ++)
        for (int j = 0; j < 4; j ++)
        {
            m_svg[i][j].setParent(this);
            m_svg[i][j].setImagePath("widgets/background");
//            m_svg[i][j].setEnabledBorders(Plasma::FrameSvg::AllBorders);
            m_svg[i][j].setEnabledBorders(Plasma::FrameSvg::EnabledBorders(BorderSample[i]*4+BorderSample[j]));
//            this->setPos();
        }
    // this will get us the standard applet background, for free!
    setBackgroundHints(TranslucentBackground);
    resize(200, 200);
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
 
    // A small demonstration of the setFailedToLaunch function
//    if (m_icon.isNull()) {
//        setFailedToLaunch(true, "No world to say hello");
//    }
} 
 
 
void Knotplasm::paintInterface(QPainter *p,
        const QStyleOptionGraphicsItem *option, const QRect &contentsRect)
{
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->setRenderHint(QPainter::Antialiasing);

    // Now we draw the applet, starting with our svg
    for (int i = 0; i < 4; i ++)
    {
        for (int j = 0; j < 4; j ++)
        {
            m_svg[i][j].resizeFrame(contentsRect.size() / 4);
            m_svg[i][j].paintFrame(p, contentsRect.topLeft() + QPointF(contentsRect.size().width() / 4 * i, contentsRect.size().height() / 4 * j));
        }
    }
 
    // We place the icon and text
//    p->drawPixmap(7, 0, m_icon.pixmap((int)contentsRect.width(),(int)contentsRect.width()-14));
    p->save();
    p->setPen(Qt::white);
    p->drawText(contentsRect,
        Qt::AlignBottom | Qt::AlignHCenter,
        "Hello Plasmoid!");
    p->restore();
}
 
// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(Knotplasm, Knotplasm)
 
#include "Knotplasm.moc"
