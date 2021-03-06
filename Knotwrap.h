#ifndef Knotwrap_HEADER
#define Knotwrap_HEADER
#include <QObject>
#include <QPoint>
#include <QPair>

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

struct KnotGameParamItem
{
public:
    KnotGameParamItem ();
    KnotGameParamItem (const config_item& config);
    enum ConfigItemType
        {CONFIG_STRING, CONFIG_BOOLEAN, CONFIG_CHOICES, CONFIG_ERROR, N_CONFIG=CONFIG_ERROR};

    const ConfigItemType type;
    const QString name;
    const QStringList choices;
    int iVal;
    QString sVal;
    bool bVal;
    
private:
    class Private;
};

class KnotGameParamList : public QList<KnotGameParamItem>
{
public:
    KnotGameParamList();
    KnotGameParamList(config_item* configList, char *title);
    QString title ();
private:
    QString m_title;
};

struct KnotGameParams
{
    game_params * m_params;
    
    KnotGameParams ();
    KnotGameParams (game_params *params);
};

class KnotMidend : public QObject
{
    Q_OBJECT
public:
    KnotMidend(QObject* parent, int game);
    virtual ~KnotMidend();
    
    int gameId ();
    QStringList presetList ();
    bool canConfig ();
    void setPreset (int preset);
    KnotGameParamList getConfig();
    void setConfig (KnotGameParamList config);
    
    QString serialize ();
    bool deserialize (const QString& str);
    
public slots:
    void newGame();
    void solve();
    void size(int* portX, int* portY);
    void redraw();
    void forceRedraw();
    void color(QColor color);

    void pressButton (QPoint pos, Qt::MouseButton btn);
    void releaseButton (QPoint pos, Qt::MouseButton btn);
    void dragButton (QPoint pos, Qt::MouseButtons btn);
    
    void pressKey (int key, Qt::KeyboardModifiers modifier);
    
    void tickTimer (qreal tplus);
    
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
    
    void drawUpdate (int x, int y, int w, int h);
    
    void activateTimer ();
    void deactivateTimer ();
    
    void statusBar(const QString& text);
    void setColor(const QList<QColor> colorList);
    
    void startDraw();
    void endDraw();
    
    void stateChanged (const QString& state);
    
private:
    midend* m_me;
    int m_game_id;
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

friend void activate_timer(frontend *fe);
friend void deactivate_timer(frontend *fe);

friend void knotplasm_draw_update(void *handle, int x, int y, int w, int h);

friend void knotplasm_start_draw(void *handle);
friend void knotplasm_end_draw(void *handle);
};

#endif