#include <QDateTime>

#include "Knotplasm.h"
#include "Knotwrap.h"
#include <Plasma/Theme>

/*
 * Forward declaration
 */
extern const struct drawing_api knotplasm_drawing;

/*
 * Knotplasm functions interacting with the wrapper:
 */

struct frontend
{
};

/*
 * A simple C++ wrapper for the middle end
 */
KnotMidend::KnotMidend (Knotplasm* parent)
{
    m_me = midend_new((frontend*)this, gamelist[1], &knotplasm_drawing, (void *) this);
}
KnotMidend::~KnotMidend() {}
    
void KnotMidend::newGame()
{
    midend_new_game(m_me);
}

void KnotMidend::size(int* portX, int* portY)
{
    midend_size(m_me,portX,portY,TRUE);
}

void KnotMidend::redraw()
{
    midend_redraw(m_me);
}

void KnotMidend::forceRedraw()
{
    midend_force_redraw(m_me);
}

void KnotMidend::color(QColor color)
{
    int n_colors;
    float *f_colors;
    QList<QColor> colorList;
    
    m_color = color;

    f_colors = midend_colours(m_me, &n_colors);
    
    for (int i = 0; i < n_colors; i ++)
        colorList.append(QColor::fromRgbF(f_colors[i*3], f_colors[i*3+1], f_colors[i*3+2]));
    
    emit setColor(colorList);
}

void KnotMidend::pressButton(QPoint pos, Qt::MouseButton btn)
{
    int x = pos.x();
    int y = pos.y();
    
    if (btn==Qt::LeftButton)
        midend_process_key(m_me, x, y, LEFT_BUTTON);
    if (btn==Qt::MiddleButton)
        midend_process_key(m_me, x, y, MIDDLE_BUTTON);
    if (btn==Qt::RightButton)
        midend_process_key(m_me, x, y, RIGHT_BUTTON);
}

void KnotMidend::releaseButton(QPoint pos, Qt::MouseButton btn)
{
    int x = pos.x();
    int y = pos.y();
    
    if (btn==Qt::LeftButton)
        midend_process_key(m_me, x, y, LEFT_RELEASE);
    if (btn==Qt::MiddleButton)
        midend_process_key(m_me, x, y, MIDDLE_RELEASE);
    if (btn==Qt::RightButton)
        midend_process_key(m_me, x, y, RIGHT_RELEASE);
}

void KnotMidend::dragButton(QPoint pos, Qt::MouseButtons btn)
{
    int x = pos.x();
    int y = pos.y();
    
    if (btn&Qt::LeftButton)
        midend_process_key(m_me, x, y, LEFT_DRAG);
    else if (btn&Qt::MiddleButton)
        midend_process_key(m_me, x, y, MIDDLE_DRAG);
    else if (btn&Qt::RightButton)
        midend_process_key(m_me, x, y, RIGHT_DRAG);
}

void KnotMidend::tickTimer(qreal tplus)
{
    midend_timer (this->m_me, (float)tplus);
}

/*
 * Drawing APIs we provide to the midend
 */

void knotplasm_draw_text(void *handle, int x, int y, int fonttype, int fontsize,
            int align, int colour, char *text)
{
    emit ((KnotMidend*)handle)-> drawText(x,y,fonttype==FONT_FIXED,fontsize,align,colour,QString(text));
}

void knotplasm_draw_rect(void *handle, int x, int y, int w, int h, int colour)
{
    emit ((KnotMidend *)handle)-> drawRect(x,y,w,h,colour);
}
void knotplasm_draw_line(void *handle, int x1, int y1, int x2, int y2,
            int colour)
{
    emit ((KnotMidend *)handle)-> drawLine(x1,y1,x2,y2,colour);
}
void knotplasm_draw_polygon(void *handle, int *coords, int npoints,
            int fillcolour, int outlinecolour)
{
    QVector<QPoint> n_coords;
    for (int i = 0; i < npoints; i ++)
        n_coords.append(QPoint(coords[i * 2], coords[i * 2 + 1]));
    emit ((KnotMidend *)handle)-> drawPolygon(QPolygon(n_coords),fillcolour,outlinecolour);
}
void knotplasm_draw_circle(void *handle, int cx, int cy, int radius,
        int fillcolour, int outlinecolour)
{
    emit ((KnotMidend *)handle)-> drawCircle(cx,cy,radius,fillcolour,outlinecolour);
}
void knotplasm_draw_update(void *handle, int x, int y, int w, int h)
{
//    ((Knotplasm *)handle)->drawUpdate(x,y,w,h);
}
void knotplasm_clip(void *handle, int x, int y, int w, int h)
{
    emit ((KnotMidend *)handle)->clip(x,y,w,h);
}
void knotplasm_unclip(void *handle)
{
    emit ((KnotMidend *)handle)->unclip();
}
void knotplasm_start_draw(void *handle)
{
//    ((Knotplasm *)handle)->startDraw();
}
void knotplasm_end_draw(void *handle)
{
//    ((Knotplasm *)handle)->endDraw();
}
void knotplasm_status_bar(void *handle, char *text)
{
    ((KnotMidend *)handle)->statusBar(QString(text));
}
blitter *knotplasm_blitter_new(void *handle, int w, int h)
{
    return NULL;
}
void knotplasm_blitter_free(void *handle, blitter *bl)
{
}
void knotplasm_blitter_save(void *handle, blitter *bl, int x, int y)
{
}
void knotplasm_blitter_load(void *handle, blitter *bl, int x, int y)
{
}
void knotplasm_line_dotted(void *handle, int dotted)
{
}
char *knotplasm_text_fallback(void *handle, const char *const *strings,
            int nstrings)
{
    return NULL;
}
void knotplasm_draw_thick_line(void *handle, float thickness,
            float x1, float y1, float x2, float y2,
            int colour)
{
    ((KnotMidend *)handle)->drawThickLine(thickness,x1,y1,x2,y2,colour);
}

const struct drawing_api knotplasm_drawing = {
    knotplasm_draw_text,
    knotplasm_draw_rect,
    knotplasm_draw_line,
    knotplasm_draw_polygon,
    knotplasm_draw_circle,
    knotplasm_draw_update,
    knotplasm_clip,
    knotplasm_unclip,
    knotplasm_start_draw,
    knotplasm_end_draw,
    knotplasm_status_bar,
    knotplasm_blitter_new,
    knotplasm_blitter_free,
    knotplasm_blitter_save,
    knotplasm_blitter_load,
    NULL, NULL, NULL, NULL, NULL, NULL, /* {begin,end}_{doc,page,puzzle} */
    NULL, 
    knotplasm_line_dotted,                /* line_width */
    knotplasm_text_fallback,
    knotplasm_draw_thick_line,
};

/*
 * Mid-end to front-end calls
 */
void get_random_seed(void **randseed, int *randseedsize)
{
    *randseedsize = sizeof (qint64);
    *randseed = smalloc(*randseedsize);
    *(qint64 *)*randseed = QDateTime::currentMSecsSinceEpoch ();
}

void activate_timer(frontend *fe)
{
    emit ((KnotMidend *)fe)->activateTimer();
}

void deactivate_timer(frontend *fe)
{
    emit ((KnotMidend *)fe)->deactivateTimer();
}

void fatal(char *fmt, ...)
{
}

void frontend_default_colour(frontend *fe, float *output)
{
    output[0] = ((KnotMidend*)fe)->m_color.redF();
    output[1] = ((KnotMidend*)fe)->m_color.greenF();
    output[2] = ((KnotMidend*)fe)->m_color.blueF();
}

#include "Knotwrap.moc"
