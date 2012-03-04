#ifndef Knotwrap_HEADER
#define Knotwrap_HEADER
#include <QObject>
#include <QPoint>

extern "C"
{
    #include "puzzles.h"
    
    /*
     * Mid-end to front-end calls
     */
    void get_random_seed(void **randseed, int *randseedsize);
    void activate_timer(frontend *fe);
    void deactivate_timer(frontend *fe);
    void fatal(char *fmt, ...);
    void frontend_default_colour(frontend *fe, float *output);
}

class KnotMidend : public QObject
{
    Q_OBJECT
public:
    KnotMidend(Knotplasm* parent);
    virtual ~KnotMidend();
    
public slots:
    void newGame();
    void size(int* portX, int* portY);
    void redraw();
    void forceRedraw();
    void color(QColor color);

    void pressButton (QPoint pos, Qt::MouseButtons btn);
    void releaseButton (QPoint pos, Qt::MouseButtons btn);
    void dragButton (QPoint pos, Qt::MouseButtons btn);
    
signals:
    void drawRect(int x, int y, int w, int h, int colour);
    void drawText(int x, int y, bool monospace, int fontsize,
        int align, int colour, const QString& text);
    void drawLine(int x1, int y1, int x2, int y2,
        int colour);
    void drawPolygon(const QPolygon& polygon, int fillcolour,
        int outlinecolour);
    void drawCircle(int cx, int cy, int radius,
        int fillcolour, int outlinecolour);
    void drawThickLine(float thickness,
        float x1, float y1, float x2, float y2,
        int colour);
    void clip (int x, int y, int w, int h);
    void unclip ();
    
    void statusBar(const QString& text);
    void setColor(const QList<QColor> colorList);
private:
    midend* m_me;
    QColor m_color;

friend void knotplasm_draw_rect(void *handle, int x, int y, int w, int h, int colour);
friend void knotplasm_draw_text(void *handle, int x, int y, int fonttype, int fontsize,
    int align, int colour, char *text);
friend void knotplasm_status_bar(void *handle, char *text);
friend void knotplasm_draw_line(void *handle, int x1, int y1, int x2, int y2,
    int colour);
friend void knotplasm_draw_polygon(void *handle, int *coords, int npoints,
            int fillcolour, int outlinecolour);
friend void knotplasm_draw_circle(void *handle, int cx, int cy, int radius,
        int fillcolour, int outlinecolour);
friend void frontend_default_colour(frontend *fe, float *output);
friend void knotplasm_draw_thick_line(void *handle, float thickness,
            float x1, float y1, float x2, float y2,
            int colour);
friend void knotplasm_clip(void *handle, int x, int y, int w, int h);
friend void knotplasm_unclip(void *handle);

};

#endif