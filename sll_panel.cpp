/** @author Guilherme Martinelli Taglietti
 *  @file   sll_panel.cpp
 *  @brief  SLL panel impl
 */
#include "sll_panel.h"
#include "texts.h"
#include <QGraphicsTextItem>

SllPanel::SllPanel(QWidget* parent):BasePanel(tr("Lista Encadeada"),about_sll_pt(),parent){
    set_kind("sll");
    build_controls(controls_bar);
}

void SllPanel::build_controls(QHBoxLayout* bar){
    push_front_btn=new QPushButton(tr("Inserir Início"),this);
    pop_front_btn=new QPushButton(tr("Remover Início"),this);
    remove_sel_btn=new QPushButton(tr("Remover Selecionado"),this);
    clear_btn=new QPushButton(tr("Limpar"),this);
    bar->addWidget(push_front_btn);
    bar->addWidget(pop_front_btn);
    bar->addWidget(remove_sel_btn);
    bar->addWidget(clear_btn);
    bar->addStretch();
    connect(push_front_btn,&QPushButton::clicked,this,&SllPanel::on_push_front);
    connect(pop_front_btn,&QPushButton::clicked,this,&SllPanel::on_pop_front);
    connect(remove_sel_btn,&QPushButton::clicked,this,&SllPanel::on_remove_sel);
    connect(clear_btn,&QPushButton::clicked,this,&SllPanel::on_clear);
}

void SllPanel::redraw(){
    view->clear_scene();
    selected_index=-1;
    int r=26, gap=40;
    int x=60, y=120;
    for(int i=0;i<data.size();++i){
        auto* e=view->scene_ptr()->addEllipse(x+i*(2*r+gap),y,2*r,2*r,QPen(Qt::black),QBrush(QColor("#e6dbff")));
        e->setFlag(QGraphicsItem::ItemIsSelectable,true);
        e->setData(0,i);
        QString s=data[i];
        auto f=view->fit_font_for_width(s,2*r-8,18);
        auto* t=view->scene_ptr()->addText(s,f);
        t->setDefaultTextColor(QColor("#1a1a1a"));
        t->setPos(x+i*(2*r+gap)+r-t->boundingRect().width()/2,y+r-t->boundingRect().height()/2);
        if(i+1<data.size()){
            int x1=x+i*(2*r+gap)+2*r, x2=x+(i+1)*(2*r+gap);
            view->scene_ptr()->addLine(x1,y+r,x2,y+r,QPen(Qt::black,3));
        }
        QObject::connect(view->scene_ptr(),&QGraphicsScene::selectionChanged,[this](){
            auto sel=view->scene_ptr()->selectedItems();
            selected_index=sel.isEmpty()? -1:sel.first()->data(0).toInt();
        });
    }
    view->auto_layout_now();
}

QJsonObject SllPanel::capture() const{
    QJsonArray a; for(auto& s:data) a.append(s);
    QJsonObject o; o["sll"]=a; return o;
}

void SllPanel::restore(const QJsonObject& o){
    data.clear();
    for(auto v:o.value("sll").toArray()) data.push_back(v.toString());
    redraw();
}

void SllPanel::on_push_front(){
    QString v=input_value->text().trimmed(); if(v.isEmpty()) return;
    data.push_front(v);
    redraw();
}

void SllPanel::on_pop_front(){
    if(data.isEmpty()) return;
    set_status(tr("pop_front → %1").arg(data.front()));
    data.pop_front();
    redraw();
}

void SllPanel::on_remove_sel(){
    if(selected_index<0||selected_index>=data.size()) return;
    set_status(tr("remove_at → %1").arg(QString::number(selected_index)));
    data.removeAt(selected_index);
    redraw();
}

void SllPanel::on_clear(){
    data.clear(); redraw();
}
