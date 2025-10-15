/** @author Guilherme Martinelli Taglietti
 *  @file   stack_panel.cpp
 *  @brief  Stack panel impl
 */
#include "stack_panel.h"
#include "texts.h"
#include "storage_manager.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

StackPanel::StackPanel(QWidget* parent) : BasePanel("Pilha", about_stack_pt(), parent){
    set_kind("stack");
    build_controls(controls_bar);
}

void StackPanel::build_controls(QHBoxLayout* bar){
    push_btn=new QPushButton("Empilhar",this);
    pop_btn=new QPushButton("Desempilhar",this);
    clear_btn=new QPushButton("Limpar",this);
    bar->addWidget(push_btn);
    bar->addWidget(pop_btn);
    bar->addWidget(clear_btn);
    bar->addStretch();
    connect(push_btn,&QPushButton::clicked,this,&StackPanel::on_push);
    connect(pop_btn,&QPushButton::clicked,this,&StackPanel::on_pop);
    connect(clear_btn,&QPushButton::clicked,this,&StackPanel::on_clear);
}

void StackPanel::redraw(){
    view->clear_scene();
    int w=120,h=40,gap=10;
    int x=20,y=20;
    for(int i=0;i<data.size();++i){
        auto* r=view->scene_ptr()->addRect(x,y+i*(h+gap),w,h,QPen(Qt::black),QBrush(color_node_fill_normal));
        auto* t=view->scene_ptr()->addText(data[i]);
        t->setDefaultTextColor(color_text_normal);
        t->setPos(x+10,y+i*(h+gap)+8);
    }
    view->auto_layout_now();
}

QJsonObject StackPanel::capture() const{
    QJsonArray arr;
    for(const auto& s:data) arr.append(s);
    QJsonObject o; o["stack"]=arr; return o;
}

void StackPanel::restore(const QJsonObject& o){
    data.clear();
    for(auto v:o.value("stack").toArray()) data.push_back(v.toString());
    redraw();
}

void StackPanel::on_push(){
    QString v=input_value->text().trimmed();
    if(v.isEmpty()) return;
    data.push_back(v);
    redraw();
}

void StackPanel::on_pop(){
    if(data.isEmpty()) return;
    set_status("pop → "+data.back());
    data.pop_back();
    redraw();
}

void StackPanel::on_clear(){
    data.clear();
    redraw();
}
