/** @author Guilherme Martinelli Taglietti
 *  @file   queue_panel.cpp
 *  @brief  Queue panel impl
 */
#include "queue_panel.h"
#include "texts.h"
#include "storage_manager.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

QueuePanel::QueuePanel(QWidget* parent) : BasePanel("Fila", about_queue_pt(), parent){
    set_kind("queue");
    build_controls(controls_bar);
}

void QueuePanel::build_controls(QHBoxLayout* bar){
    enq_btn=new QPushButton("Enfileirar",this);
    deq_btn=new QPushButton("Desenfileirar",this);
    clear_btn=new QPushButton("Limpar",this);
    bar->addWidget(enq_btn);
    bar->addWidget(deq_btn);
    bar->addWidget(clear_btn);
    bar->addStretch();
    connect(enq_btn,&QPushButton::clicked,this,&QueuePanel::on_enq);
    connect(deq_btn,&QPushButton::clicked,this,&QueuePanel::on_deq);
    connect(clear_btn,&QPushButton::clicked,this,&QueuePanel::on_clear);
}

void QueuePanel::redraw(){
    view->clear_scene();
    int w=100,h=40,gap=10,x=20,y=40;
    for(int i=0;i<data.size();++i){
        view->scene_ptr()->addRect(x+i*(w+gap),y,w,h,QPen(Qt::black),QBrush(color_node_fill_normal));
        auto* t=view->scene_ptr()->addText(data[i]);
        t->setDefaultTextColor(color_text_normal);
        t->setPos(x+i*(w+gap)+10,y+8);
    }
    view->auto_layout_now();
}

QJsonObject QueuePanel::capture() const{
    QJsonArray arr; for(const auto& s:data) arr.append(s);
    QJsonObject o; o["queue"]=arr; return o;
}

void QueuePanel::restore(const QJsonObject& o){
    data.clear();
    for(auto v:o.value("queue").toArray()) data.push_back(v.toString());
    redraw();
}

void QueuePanel::on_enq(){
    QString v=input_value->text().trimmed();
    if(v.isEmpty()) return;
    data.push_back(v);
    redraw();
}

void QueuePanel::on_deq(){
    if(data.isEmpty()) return;
    set_status("deq → "+data.front());
    data.pop_front();
    redraw();
}

void QueuePanel::on_clear(){
    data.clear();
    redraw();
}
