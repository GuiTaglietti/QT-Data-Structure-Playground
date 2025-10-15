/** @author Guilherme Martinelli Taglietti
 *  @file   diagram_view.h
 *  @brief  QGraphicsView with zoom/pan and helpers
 */
#pragma once

#include <QGraphicsView>
#include <QWheelEvent>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QFont>

class DiagramView:public QGraphicsView{
    Q_OBJECT

public:
    explicit DiagramView(QWidget* parent=nullptr);

    QGraphicsScene* scene_ptr() const;
    void clear_scene();
    void auto_layout_now();
    void zoom_reset();
    qreal scale_factor() const;
    void set_background(const QColor& c);

    QFont fit_font_for_width(const QString& text,int target_px_width,int base_pt=14) const;

protected:
    void wheelEvent(QWheelEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void mouseDoubleClickEvent(QMouseEvent* e) override;

private:
    QPoint last_pan_pos;
    bool panning=false;
    qreal min_scale=0.25;
    qreal max_scale=3.0;
};
