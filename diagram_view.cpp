/** @author Guilherme Martinelli Taglietti
 *  @file   diagram_view.cpp
 *  @brief  DiagramView impl
 */
#include "diagram_view.h"

DiagramView::DiagramView(QWidget* parent):QGraphicsView(parent){
    auto* s=new QGraphicsScene(this);
    setScene(s);
    setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing|QPainter::SmoothPixmapTransform);
    setDragMode(QGraphicsView::NoDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setBackgroundBrush(QColor("#1b1e25"));
}

void DiagramView::set_background(const QColor& c){
    setBackgroundBrush(c);
}

QGraphicsScene* DiagramView::scene_ptr() const{
    return scene();
}

void DiagramView::clear_scene(){
    scene()->clear();
}

void DiagramView::auto_layout_now(){
    scene()->setSceneRect(scene()->itemsBoundingRect().adjusted(-40,-40,40,40));
}

void DiagramView::zoom_reset(){
    resetTransform();
}

qreal DiagramView::scale_factor() const{
    return transform().m11();
}

QFont DiagramView::fit_font_for_width(const QString& text,int target_px_width,int base_pt) const{
    int lo=8, hi=base_pt, ans=lo;
    while(lo<=hi){
        int mid=(lo+hi)/2;
        QFont f; f.setPointSize(mid);
        QFontMetrics fm(f);
        int w=fm.horizontalAdvance(text);
        if(w<=target_px_width){ans=mid; lo=mid+1;}
        else hi=mid-1;
    }
    QFont f; f.setPointSize(ans);
    return f;
}

void DiagramView::wheelEvent(QWheelEvent* e){
    const double step=1.12;
    double next = scale_factor() * (e->angleDelta().y()>0 ? step : 1.0/step);
    if(next<min_scale) next=min_scale;
    if(next>max_scale) next=max_scale;
    double factor = next/scale_factor();
    scale(factor,factor);
}

void DiagramView::mousePressEvent(QMouseEvent* e){
    if(e->button()==Qt::MiddleButton||e->button()==Qt::RightButton){
        panning=true; last_pan_pos=e->pos(); setCursor(Qt::ClosedHandCursor);
    }else QGraphicsView::mousePressEvent(e);
}

void DiagramView::mouseMoveEvent(QMouseEvent* e){
    if(panning){
        QPointF d=mapToScene(last_pan_pos)-mapToScene(e->pos());
        translate(d.x(),d.y());
        last_pan_pos=e->pos();
    }else QGraphicsView::mouseMoveEvent(e);
}

void DiagramView::mouseReleaseEvent(QMouseEvent* e){
    if(panning&&(e->button()==Qt::MiddleButton||e->button()==Qt::RightButton)){
        panning=false; setCursor(Qt::ArrowCursor);
    }else QGraphicsView::mouseReleaseEvent(e);
}

void DiagramView::mouseDoubleClickEvent(QMouseEvent* e){
    if(e->button()==Qt::RightButton) zoom_reset();
    else QGraphicsView::mouseDoubleClickEvent(e);
}
