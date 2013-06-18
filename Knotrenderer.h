#ifndef Knotrenderer_HEADER
#define Knotrenderer_HEADER

#include <QGraphicsWidget>
#include <Plasma/Frame>
#include <KConfigGroup>

class KnotRenderer: public QGraphicsWidget
{
Q_OBJECT
public:
    
    KnotRenderer (QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    virtual void paintInterface(QPainter *p,
            const QStyleOptionGraphicsItem *option,
            const QRectF& contentsRect) = 0;

    virtual ~KnotRenderer ();
    
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * e);
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * e);
    virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * e);
    
    virtual void keyPressEvent ( QKeyEvent * e );
    
    bool event ( QEvent * event );

public slots:
    // Responding to desktop events
    virtual void geometryChangedHandler ();
    virtual void themeChangedHandler ();
    virtual void configChangedHandler (KConfigGroup cg);
    
    // Real SGT APIs
    virtual void drawText(int x, int y, bool monospace, int fontsize,
        int align, int colour, const QString& text) = 0;
    virtual void drawRect(int x, int y, int w, int h, int colour) = 0;
    virtual void drawLine(int x1, int y1, int x2, int y2,
        int colour) = 0;
    virtual void drawPolygon(const QPolygon& polygon,
        int fillcolour, int outlinecolour) = 0;
    virtual void drawCircle(int cx, int cy, int radius,
        int fillcolour, int outlinecolour) = 0;
    virtual void drawUpdate(int x, int y, int w, int h) = 0;
    virtual void updateAll () = 0;
    virtual void clip(int x, int y, int w, int h) = 0;
    virtual void unclip() = 0;
    virtual void startDraw() = 0;
    virtual void endDraw() = 0;
    virtual void drawThickLine(float thickness,
        float x1, float y1, float x2, float y2,
        int colour) = 0;
    
    // Pseudo APIs which interacts in the same way
    virtual void notStarted() = 0;
    virtual void setColor(QList<QColor> colorList) = 0;
    
signals:
    void redrawRequest ();
    void forceRedrawRequest ();
    void sizeRequest (int* x, int* y);
    void colorRequest (QColor color);
    
    void mousePressed (QPoint pos, Qt::MouseButton btn);
    void mouseReleased (QPoint pos, Qt::MouseButton btn);
    void mouseDragged (QPoint pos, Qt::MouseButtons btn);
    
    void keyPressed (int key, Qt::KeyboardModifiers modifier);

protected:
    virtual QPointF getOffset();
    virtual void setOffset(const QPointF& offset);
    bool m_initialized;
    
private:
    virtual void paint (QPainter * painter, 
        const QStyleOptionGraphicsItem * option,
        QWidget * widget = 0);

    QPointF m_offset;
};

#endif