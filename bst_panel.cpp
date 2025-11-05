#include "bst_panel.h"
#include "texts.h"
#include <QGraphicsTextItem>

BstPanel::BstPanel(QWidget* parent) : BasePanel(tr("Árvore BST"),about_bst_pt(),parent){
    set_kind("bst");
    build_controls(controls_bar);
    connect(&anim_timer,&QTimer::timeout,this,&BstPanel::on_anim_step);
}

static void draw_edge(QGraphicsScene* sc,const QPointF& a,const QPointF& b,int r,const QPen& pen){
    QLineF L(a,b); L.setLength(L.length()-r); QPointF p1=L.p2();
    QLineF Lb(b,a); Lb.setLength(Lb.length()-r); QPointF p2=Lb.p2();
    sc->addLine(p1.x(),p1.y(),p2.x(),p2.y(),pen);
}

void BstPanel::build_controls(QHBoxLayout* bar){
    insert_btn=new QPushButton(tr("Inserir"),this);
    find_btn=new QPushButton(tr("Buscar"),this);
    remove_btn=new QPushButton(tr("Remover"),this);
    clear_btn=new QPushButton(tr("Limpar"),this);
    bar->addWidget(insert_btn);
    bar->addWidget(find_btn);
    bar->addWidget(remove_btn);
    bar->addWidget(clear_btn);
    bar->addStretch();
    connect(insert_btn,&QPushButton::clicked,this,&BstPanel::on_insert);
    connect(find_btn,&QPushButton::clicked,this,&BstPanel::on_find);
    connect(remove_btn,&QPushButton::clicked,this,&BstPanel::on_remove);
    connect(clear_btn,&QPushButton::clicked,this,&BstPanel::on_clear);
}

void BstPanel::draw_node(Node* n,int x,int y,int dx,Node* flash){
    if(!n) return;
    int r=22;
    auto* e=view->scene_ptr()->addEllipse(x-r,y-r,2*r,2*r,QPen(Qt::black),QBrush(QColor("#f3f3a6")));
    auto f=view->fit_font_for_width(n->v,2*r-8,16);
    auto* t=view->scene_ptr()->addText(n->v,f);
    t->setDefaultTextColor(QColor("#1a1a1a"));
    t->setPos(x-t->boundingRect().width()/2,y-t->boundingRect().height()/2);
    if(n==flash){
        e->setBrush(QBrush(QColor("#ffdf70")));
        t->setDefaultTextColor(Qt::black);
    }
    if(n->l){
        QPointF a(x,y), b(x-dx,y+70);
        draw_edge(view->scene_ptr(),a,b,r,QPen(Qt::black,2));
        draw_node(n->l,b.x(),b.y(),dx/2,flash);
    }
    if(n->r){
        QPointF a(x,y), b(x+dx,y+70);
        draw_edge(view->scene_ptr(),a,b,r,QPen(Qt::black,2));
        draw_node(n->r,b.x(),b.y(),dx/2,flash);
    }
}

void BstPanel::redraw(Node* flash){
    view->clear_scene();
    if(root) draw_node(root,380,60,160,flash);
    view->auto_layout_now();
}

int BstPanel::bst_compare(const QString& a,const QString& b){
    bool okA=false, okB=false;
    qlonglong ia=a.toLongLong(&okA);
    qlonglong ib=b.toLongLong(&okB);
    if(okA && okB){
        if(ia<ib) return -1;
        if(ia>ib) return 1;
        return 0;
    }
    if(a<b) return -1;
    if(a>b) return 1;
    return 0;
}

BstPanel::Node* BstPanel::insert_rec(Node* n,const QString& v){
    if(!n) return new Node{v,nullptr,nullptr};
    int c=bst_compare(v,n->v);
    if(c<0) n->l=insert_rec(n->l,v);
    else if(c>0) n->r=insert_rec(n->r,v);
    return n;
}

bool BstPanel::find_with_path(Node* n,const QString& v,QVector<Node*>* path){
    Node* cur=n;
    while(cur){
        if(path) path->push_back(cur);
        int c=bst_compare(v,cur->v);
        if(c==0) return true;
        cur = (c<0)? cur->l : cur->r;
    }
    return false;
}

BstPanel::Node* BstPanel::min_node(Node* n){
    while(n && n->l) n=n->l;
    return n;
}

BstPanel::Node* BstPanel::remove_rec(Node* n,const QString& v){
    if(!n) return n;

    int c=bst_compare(v,n->v);

    if(c<0) n->l=remove_rec(n->l,v);
    else if(c>0) n->r=remove_rec(n->r,v);
    else{
        if(!n->l){
            auto* r=n->r;
            delete n;
            return r;
        }else if(!n->r){
            auto* l=n->l;
            delete n;
            return l;
        }
        Node* m=min_node(n->r);
        n->v=m->v;
        n->r=remove_rec(n->r,m->v);
    }
    return n;
}

QJsonObject BstPanel::capture() const{
    QJsonArray a;
    QVector<Node*> st; if(root) st.push_back(root);
    while(!st.isEmpty()){
        Node* n=st.back(); st.pop_back();
        a.append(n->v);
        if(n->r) st.push_back(n->r);
        if(n->l) st.push_back(n->l);
    }
    QJsonObject o; o["bst_pre"]=a; return o;
}

void BstPanel::restore(const QJsonObject& o){
    clear_all(root); root=nullptr;
    for(auto v:o.value("bst_pre").toArray()){
        const QString s=v.toString();
        root=insert_rec(root,s);
    }
    redraw();
}

void BstPanel::clear_all(Node* n){
    if(!n) return;
    clear_all(n->l); clear_all(n->r); delete n;
}

void BstPanel::on_insert(){
    QString v=input_value->text().trimmed(); if(v.isEmpty()) return;
    root=insert_rec(root,v);
    redraw();
}

void BstPanel::on_find(){
    QString v=input_value->text().trimmed(); if(v.isEmpty()) return;
    anim_path.clear();
    bool ok=find_with_path(root,v,&anim_path);
    anim_i=0;
    anim_pending=true;
    QStringList seq; for(auto* n:anim_path) seq<<n->v;
    QString pathText=seq.join(" -> ");
    if(ok) anim_pending_msg=tr("Caminho: %1").arg(pathText);
    else anim_pending_msg=tr("Não encontrado") + QString(" — ") + tr("Caminho: %1").arg(pathText);
    anim_timer.start(260);
}

void BstPanel::on_remove(){
    QString v=input_value->text().trimmed(); if(v.isEmpty()) return;
    root=remove_rec(root,v); redraw();
}

void BstPanel::on_clear(){
    clear_all(root); root=nullptr; redraw();
}

void BstPanel::on_anim_step(){
    if(anim_i>=anim_path.size()){
        anim_timer.stop();
        redraw();
        if(anim_pending){ set_status(anim_pending_msg); anim_pending=false; anim_pending_msg.clear(); }
        return;
    }
    redraw(anim_path[anim_i]);
    anim_i++;
}
