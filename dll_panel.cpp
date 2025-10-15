/** @author Guilherme Martinelli Taglietti
 *  @file   dll_panel.cpp
 *  @brief  DLL panel impl
 */
#include "dll_panel.h"
#include "texts.h"
#include <QGraphicsTextItem>

DllPanel::DllPanel(QWidget* parent):BasePanel("Lista Duplamente Encadeada",about_dll_pt(),parent){
    set_kind("dll");
    build_controls(controls_bar);
}

void DllPanel::build_controls(QHBoxLayout* bar){
    push_front_btn=new QPushButton("Inserir Início",this);
    pop_back_btn=new QPushButton("Remover Fim",this);
    remove_sel_btn=new QPushButton("Remover Selecionado",this);
    clear_btn=new QPushButton("Limpar",this);
    bar->addWidget(push_front_btn);
    bar->addWidget(pop_back_btn);
    bar->addWidget(remove_sel_btn);
    bar->addWidget(clear_btn);
    bar->addStretch();
    connect(push_front_btn,&QPushButton::clicked,this,&DllPanel::on_push_front);
    connect(pop_back_btn,&QPushButton::clicked,this,&DllPanel::on_pop_back);
    connect(remove_sel_btn,&QPushButton::clicked,this,&DllPanel::on_remove_sel);
    connect(clear_btn,&QPushButton::clicked,this,&DllPanel::on_clear);
}

void DllPanel::redraw(){
    view->clear_scene();
    selected_index=-1;
    int w=60,h=52,gap=34;
    int x=60,y=120;
    for(int i=0;i<data.size();++i){
        auto* r=view->scene_ptr()->addRect(x+i*(w+gap),y,w,h,QPen(Qt::black),QBrush(QColor("#ffd6e6")));
        r->setFlag(QGraphicsItem::ItemIsSelectable,true);
        r->setData(0,i);
        QString s=data[i];
        auto f=view->fit_font_for_width(s,w-10,16);
        auto* t=view->scene_ptr()->addText(s,f);
        t->setDefaultTextColor(QColor("#1a1a1a"));
        t->setPos(x+i*(w+gap)+w/2-t->boundingRect().width()/2,y+h/2-t->boundingRect().height()/2);
        if(i+1<data.size()){
            int x1=x+i*(w+gap)+w, x2=x+(i+1)*(w+gap);
            auto pen=QPen(Qt::black,3);
            view->scene_ptr()->addLine(x1,y+h/2,x2,y+h/2,pen);
            view->scene_ptr()->addLine(x2,y+h/2,x1,y+h/2,pen);
        }
        QObject::connect(view->scene_ptr(),&QGraphicsScene::selectionChanged,[this](){
            auto sel=view->scene_ptr()->selectedItems();
            selected_index=sel.isEmpty()? -1:sel.first()->data(0).toInt();
        });
    }
    view->auto_layout_now();
}

QJsonObject DllPanel::capture() const{
    QJsonArray a; for(auto& s:data) a.append(s);
    QJsonObject o; o["dll"]=a; return o;
}

void DllPanel::restore(const QJsonObject& o){
    data.clear();
    for(auto v:o.value("dll").toArray()) data.push_back(v.toString());
    redraw();
}

void DllPanel::on_push_front(){
    QString v=input_value->text().trimmed(); if(v.isEmpty()) return;
    data.push_front(v);
    redraw();
}

void DllPanel::on_pop_back(){
    if(data.isEmpty()) return;
    set_status("pop_back → "+data.back());
    data.pop_back();
    redraw();
}

void DllPanel::on_remove_sel(){
    if(selected_index<0||selected_index>=data.size()) return;
    set_status("remove_at → "+QString::number(selected_index));
    data.removeAt(selected_index);
    redraw();
}

void DllPanel::on_clear(){
    data.clear(); redraw();
}
