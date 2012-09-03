#include <QDateTime>
#include <QString>

extern "C"
{
#include "puzzles.h"
#include "malloc.h"
}

#include "Knotplasm.h"
#include "Knotwrap.h"
#include "Knotdebug.h"
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
 * KnotGameParams
 */
KnotGameParams::KnotGameParams()
{

}

KnotGameParams::KnotGameParams(game_params* params)
{
    m_params = params;
}

/*
 * KnotGameParamList
 */

class KnotGameParamItem::Private
{
public:
    static KnotGameParamItem::ConfigItemType adaptConfigType (int type)
    {
        switch (type)
        {
            case C_STRING:
                return KnotGameParamItem::CONFIG_STRING;
            case C_BOOLEAN:
                return KnotGameParamItem::CONFIG_BOOLEAN;
            case C_CHOICES:
                return KnotGameParamItem::CONFIG_CHOICES;
        }
        
        return KnotGameParamItem::CONFIG_ERROR;
    }
    
    static QStringList adaptConfigChoices (int type, char *sval)
    {
        if (type == C_CHOICES)
        {
            return QString(sval + 1).split(*sval);
        }
        return QStringList();
    }
};

KnotGameParamItem::KnotGameParamItem(): type(CONFIG_ERROR)
{

}

KnotGameParamItem::KnotGameParamItem(const config_item& config): type(Private::adaptConfigType(config.type)), name(config.name), choices(Private::adaptConfigChoices(config.type, config.sval))
{
    switch (type)
    {
        case CONFIG_BOOLEAN:
            bVal = config.ival;
            break;
        case CONFIG_STRING:
            sVal = QString(config.sval);
            break;
        case CONFIG_CHOICES:
            iVal = config.ival;
            break;
    }
}

KnotGameParamList::KnotGameParamList()
{

}

KnotGameParamList::KnotGameParamList(config_item* configList, char *n_title): m_title(n_title)
{
    while (configList->type != C_END)
    {
        this->push_back(KnotGameParamItem(*configList));
        configList ++;
    }
}

QString KnotGameParamList::title()
{
    return m_title;
}

/*
 * A simple C++ wrapper for the middle end
 */
KnotMidend::KnotMidend (QObject* parent, int game)
{
    if (game < 0 || game >= gamecount)
        game = 0;
    m_game_id = game;
    m_me = midend_new((frontend*)this, gamelist[game], &knotplasm_drawing, (void *) this);
}
KnotMidend::~KnotMidend() {}
    
void KnotMidend::newGame()
{
    midend_new_game(m_me);
}

void KnotMidend::solve()
{
    midend_solve(m_me);
}

int KnotMidend::gameId()
{
    return m_game_id;
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

void KnotMidend::pressKey(int key, Qt::KeyboardModifiers modifier)
{
    int myKey = 0;
    
    if (key == Qt::Key_Up)
        myKey = CURSOR_UP;
    if (key == Qt::Key_Down)
        myKey = CURSOR_DOWN;
    if (key == Qt::Key_Left)
        myKey = CURSOR_LEFT;
    if (key == Qt::Key_Right)
        myKey = CURSOR_RIGHT;
    if (key == Qt::Key_Return)
        myKey = '\n';

    if (key == Qt::Key_0)
        myKey = MOD_NUM_KEYPAD | '0';
    if (key == Qt::Key_1)
        myKey = MOD_NUM_KEYPAD | '1';
    if (key == Qt::Key_2)
        myKey = MOD_NUM_KEYPAD | '2';
    if (key == Qt::Key_3)
        myKey = MOD_NUM_KEYPAD | '3';
    if (key == Qt::Key_4)
        myKey = MOD_NUM_KEYPAD | '4';
    if (key == Qt::Key_5)
        myKey = MOD_NUM_KEYPAD | '5';
    if (key == Qt::Key_6)
        myKey = MOD_NUM_KEYPAD | '6';
    if (key == Qt::Key_7)
        myKey = MOD_NUM_KEYPAD | '7';
    if (key == Qt::Key_8)
        myKey = MOD_NUM_KEYPAD | '8';
    if (key == Qt::Key_9)
        myKey = MOD_NUM_KEYPAD | '9';
    
    if (key == Qt::Key_Backspace || key == Qt::Key_Delete)
        myKey = '\177';
    
    if (key >= Qt::Key_Space && key <= 0xff)
        myKey = key;

    if (myKey)
    {
        if (modifier & Qt::Key_Control)
            myKey |= MOD_CTRL;
        if (modifier & Qt::Key_Shift)
            myKey |= MOD_SHFT;
        midend_process_key(m_me, 0, 0, myKey);
    }
}

void KnotMidend::tickTimer(qreal tplus)
{
    midend_timer (this->m_me, (float)tplus);
}

QStringList KnotMidend::presetList()
{
    QStringList re;
    int n = midend_num_presets (this->m_me);
    char *str;
    game_params *params;
    
    for (int i = 0; i < n; i ++)
    {
        midend_fetch_preset(this->m_me, i, &str, &params);
        re.push_back(QString(str));
    }
    
    return re;
}

void KnotMidend::setPreset(int preset)
{
    game_params *params;
    char *str;
    
    midend_num_presets (this->m_me); // This must be called. Don't ask why.
    midend_fetch_preset(this->m_me, preset, &str, &params);
    
    midend_set_params(this->m_me, params);
}

KnotGameParamList KnotMidend::getConfig()
{
    KnotGameParamList paramList;
    
    config_item* itemList;
    char *title;
    
    itemList = midend_get_config(m_me, CFG_SETTINGS, &title);
    paramList = KnotGameParamList(itemList, title);
    
    free(title);
    free_cfg(itemList);
    
    return paramList;
}

void KnotMidend::setConfig(KnotGameParamList config)
{
    config_item* itemList, *p;
    char *title;
    
    itemList = midend_get_config(m_me, CFG_SETTINGS, &title);
    p = itemList;
    for (KnotGameParamList::iterator it = config.begin(); it != config.end(); ++ it)
    {
        switch (it->type)
        {
            case KnotGameParamItem::CONFIG_STRING:
                free(p->sval);
                p->sval = (char*)malloc(it->sVal.size() + 1);
                memcpy(p->sval, it->sVal.toAscii().constData(), it->sVal.size());
                p->sval[it->sVal.size()] = 0;
                break;
            case KnotGameParamItem::CONFIG_BOOLEAN:
                p->ival = it->bVal;
                break;
            case KnotGameParamItem::CONFIG_CHOICES:
                p->ival = it->iVal;
                break;
        }
        p ++;
    }
    midend_set_config(m_me, CFG_SETTINGS, itemList);
    
    free(title);
    free_cfg(itemList);
}

bool KnotMidend::canConfig()
{
    return gamelist[this->m_game_id]->can_configure;
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
    emit ((KnotMidend *)handle)->drawUpdate(x,y,w,h);
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
    emit ((KnotMidend *)handle)->startDraw();
}
void knotplasm_end_draw(void *handle)
{
    emit ((KnotMidend *)handle)->endDraw();
}
void knotplasm_status_bar(void *handle, char *text)
{
    emit ((KnotMidend *)handle)->statusBar(QString(text));
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
