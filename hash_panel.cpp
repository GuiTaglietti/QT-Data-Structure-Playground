/** @author Guilherme Martinelli Taglietti
 *  @file   hash_panel.cpp
 *  @brief  Hash table panel impl
 */
#include "hash_panel.h"
#include "texts.h"
#include "storage_manager.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QCryptographicHash>

HashPanel::HashPanel(QWidget* parent) : BasePanel(tr("Tabela Hash"), about_hash_pt(), parent), table(17){
    set_kind("hash");
    build_controls(controls_bar);
}

void HashPanel::build_controls(QHBoxLayout* bar){
    input_key=new QLineEdit(this);
    input_key->setPlaceholderText(tr("Chave (texto)"));
    put_btn=new QPushButton(tr("Inserir"),this);
    get_btn=new QPushButton(tr("Buscar"),this);
    clear_btn=new QPushButton(tr("Limpar"),this);
    bar->addWidget(new QLabel(tr("Chave:"),this));
    bar->addWidget(input_key);
    bar->addWidget(put_btn);
    bar->addWidget(get_btn);
    bar->addWidget(clear_btn);
    bar->addStretch();
    connect(put_btn,&QPushButton::clicked,this,&HashPanel::on_put);
    connect(get_btn,&QPushButton::clicked,this,&HashPanel::on_get);
    connect(clear_btn,&QPushButton::clicked,this,&HashPanel::on_clear);
}

int HashPanel::H(const QString& k) const{
    return qAbs((int)qHash(k))%table.size();
}

void HashPanel::redraw(int hi){
    view->clear_scene();
    int w=180,h=40,gap=8,x=20,y=20;
    for(int i=0;i<table.size();++i){
        bool hl=i==hi;
        auto fill=hl? color_node_fill_highlight: (table[i].used? QColor("#c6ffb3"):QColor("#2a2a2a"));
        auto* r=view->scene_ptr()->addRect(x,y+i*(h+gap),w,h,QPen(Qt::black),QBrush(fill));
        QString label=QString::number(i)+" "+(table[i].used? QString("(%1 → %2)").arg(table[i].key).arg(table[i].val):QString("-"));
        auto* t=view->scene_ptr()->addText(label);
        t->setDefaultTextColor(Qt::black);
        t->setPos(x+6,y+i*(h+gap)+8);
    }
    view->auto_layout_now();
}

QJsonObject HashPanel::capture() const{
    QJsonArray arr;
    for(const auto& it:table){
        QJsonObject o; o["used"]=it.used; o["key"]=it.key; o["val"]=it.val; arr.append(o);
    }
    QJsonObject out; out["table"]=arr; return out;
}

void HashPanel::restore(const QJsonObject& o){
    auto arr=o.value("table").toArray();
    table.resize(arr.size());
    for(int i=0;i<arr.size();++i){
        auto obj=arr[i].toObject();
        table[i].used=obj.value("used").toBool();
        table[i].key=obj.value("key").toString();
        table[i].val=obj.value("val").toString();
    }
    redraw();
}

void HashPanel::on_put(){
    QString v=input_value->text().trimmed();
    QString k=input_key->text().trimmed();
    if(k.isEmpty()) return;
    int i=H(k);
    int hi=-1;
    for(int c=0;c<table.size();++c,i=(i+1)%table.size()){
        if(!table[i].used||table[i].key==k){
            table[i].used=true; table[i].key=k; table[i].val=v;
            hi=i; break;
        }
    }
    set_status(tr("put(")+k+")");
    redraw(hi);
}

void HashPanel::on_get(){
    QString k=input_key->text().trimmed();
    if(k.isEmpty()) return;
    int i=H(k);
    int hi=-1;
    QString ans;
    for(int c=0;c<table.size();++c,i=(i+1)%table.size()){
        if(table[i].used&&table[i].key==k){ans=table[i].val;hi=i;break;}
    }
    set_status(ans.isEmpty()? tr("Não encontrado"):tr("Valor: ")+ans);
    redraw(hi);
}

void HashPanel::on_clear(){
    table.fill(Item{});
    redraw();
}
