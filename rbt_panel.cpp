/** @author Guilherme Martinelli Taglietti
 *  @file   rbt_panel.cpp
 *  @brief  Red-Black Tree panel impl
 */
#include "rbt_panel.h"
#include "texts.h"
#include <QGraphicsTextItem>

RbtPanel::RbtPanel(QWidget* parent):BasePanel("Árvore Vermelho-Preto",about_rbt_pt(),parent){
    set_kind("rbt");
    build_controls(controls_bar);
    connect(&anim_timer,&QTimer::timeout,this,&RbtPanel::on_anim_step);
}

void RbtPanel::build_controls(QHBoxLayout* bar){
    insert_btn=new QPushButton("Inserir",this);
    find_btn=new QPushButton("Buscar",this);
    remove_btn=new QPushButton("Remover",this);
    clear_btn=new QPushButton("Limpar",this);
    bar->addWidget(insert_btn);
    bar->addWidget(find_btn);
    bar->addWidget(remove_btn);
    bar->addWidget(clear_btn);
    bar->addStretch();
    connect(insert_btn,&QPushButton::clicked,this,&RbtPanel::on_insert);
    connect(find_btn,&QPushButton::clicked,this,&RbtPanel::on_find);
    connect(remove_btn,&QPushButton::clicked,this,&RbtPanel::on_remove);
    connect(clear_btn,&QPushButton::clicked,this,&RbtPanel::on_clear);
}

void RbtPanel::draw_node(Node* n,int x,int y,int dx,Node* flash){
    if(!n) return;
    int r=22;
    QColor fill=n->c==RED? QColor("#d73737"):QColor("#1b1b1b");
    if(n==flash) fill=QColor("#ffdf70");
    auto* e=view->scene_ptr()->addEllipse(x-r,y-r,2*r,2*r,QPen(Qt::black),QBrush(fill));
    auto f=view->fit_font_for_width(n->v,2*r-8,16);
    auto* t=view->scene_ptr()->addText(n->v,f);
    t->setDefaultTextColor(n->c==BLACK? QColor("#fff"):QColor("#000"));
    if(n==flash) t->setDefaultTextColor(QColor("#000"));
    t->setPos(x-t->boundingRect().width()/2,y-t->boundingRect().height()/2);
    if(n->l){
        QPointF a(x,y), b(x-dx,y+70);
        QLineF L(a,b); L.setLength(L.length()-r); QPointF p1=L.p2();
        QLineF Lb(b,a); Lb.setLength(Lb.length()-r); QPointF p2=Lb.p2();
        view->scene_ptr()->addLine(p1.x(),p1.y(),p2.x(),p2.y(),QPen(Qt::white,2));
        draw_node(n->l,b.x(),b.y(),dx/2,flash);
    }
    if(n->r){
        QPointF a(x,y), b(x+dx,y+70);
        QLineF L(a,b); L.setLength(L.length()-r); QPointF p1=L.p2();
        QLineF Lb(b,a); Lb.setLength(Lb.length()-r); QPointF p2=Lb.p2();
        view->scene_ptr()->addLine(p1.x(),p1.y(),p2.x(),p2.y(),QPen(Qt::white,2));
        draw_node(n->r,b.x(),b.y(),dx/2,flash);
    }
}

void RbtPanel::redraw(Node* flash){
    view->clear_scene();
    if(root) draw_node(root,380,60,160,flash);
    view->auto_layout_now();
}

void RbtPanel::clear_all(Node* n){
    if(!n) return;
    clear_all(n->l);
    clear_all(n->r);
    delete n;
}

RbtPanel::Node* RbtPanel::insert_bst(Node* n,const QString& v,Node* p){
    if(!n) return new Node{v,RED,p,nullptr,nullptr};
    if(v<n->v) n->l=insert_bst(n->l,v,n);
    else if(v>n->v) n->r=insert_bst(n->r,v,n);
    return n;
}

void RbtPanel::rotate_left(Node* x){
    Node* y=x->r;
    x->r=y->l; if(y->l) y->l->p=x;
    y->p=x->p;
    if(!x->p) root=y; else if(x==x->p->l) x->p->l=y; else x->p->r=y;
    y->l=x; x->p=y;
    anim_nodes={x,y}; anim_index=0; anim_timer.start(220);
}

void RbtPanel::rotate_right(Node* x){
    Node* y=x->l;
    x->l=y->r; if(y->r) y->r->p=x;
    y->p=x->p;
    if(!x->p) root=y; else if(x==x->p->r) x->p->r=y; else x->p->l=y;
    y->r=x; x->p=y;
    anim_nodes={x,y}; anim_index=0; anim_timer.start(220);
}

void RbtPanel::fix_insert(Node* z){
    while(z->p&&z->p->c==RED){
        Node* gp=z->p->p; if(!gp) break;
        if(z->p==gp->l){
            Node* y=gp->r;
            if(y&&y->c==RED){ z->p->c=BLACK; y->c=BLACK; gp->c=RED; z=gp; }
            else{
                if(z==z->p->r){ z=z->p; rotate_left(z); }
                z->p->c=BLACK; gp->c=RED; rotate_right(gp);
            }
        }else{
            Node* y=gp->l;
            if(y&&y->c==RED){ z->p->c=BLACK; y->c=BLACK; gp->c=RED; z=gp; }
            else{
                if(z==z->p->l){ z=z->p; rotate_right(z); }
                z->p->c=BLACK; gp->c=RED; rotate_left(gp);
            }
        }
    }
    if(root) root->c=BLACK;
}

bool RbtPanel::find_rec(Node* n,const QString& v,QVector<Node*>* path){
    if(!n) return false;
    if(path) path->push_back(n);
    if(v==n->v) return true;
    return v<n->v? find_rec(n->l,v,path):find_rec(n->r,v,path);
}

void RbtPanel::remove_value(const QString& v){
    QVector<QString> vals;
    QVector<Node*> st; if(root) st.push_back(root);
    while(!st.isEmpty()){
        Node* n=st.back(); st.pop_back();
        if(n->v!=v) vals.push_back(n->v);
        if(n->r) st.push_back(n->r);
        if(n->l) st.push_back(n->l);
    }
    clear_all(root); root=nullptr;
    for(const auto& s:vals){
        root=insert_bst(root,s,nullptr);
        Node* cur=root;
        while(cur){
            if(s<cur->v){ if(!cur->l) break; cur=cur->l; }
            else if(s>cur->v){ if(!cur->r) break; cur=cur->r; }
            else break;
        }
        fix_insert(cur);
    }
}

QJsonObject RbtPanel::capture() const{
    QJsonArray arr;
    QVector<Node*> stack;
    if(root) stack.push_back(root);
    while(!stack.isEmpty()){
        Node* n=stack.back(); stack.pop_back();
        QJsonObject o; o["v"]=n->v; o["c"]=(n->c==RED?"R":"B"); arr.append(o);
        if(n->r) stack.push_back(n->r);
        if(n->l) stack.push_back(n->l);
    }
    QJsonObject j; j["rbt"]=arr; return j;
}

void RbtPanel::restore(const QJsonObject& j){
    clear_all(root); root=nullptr;
    for(auto v:j.value("rbt").toArray()){
        QString s=v.toObject().value("v").toString();
        root=insert_bst(root,s,nullptr);
        Node* cur=root;
        while(cur){
            if(s<cur->v){ if(!cur->l) break; cur=cur->l; }
            else if(s>cur->v){ if(!cur->r) break; cur=cur->r; }
            else break;
        }
        fix_insert(cur);
    }
    redraw();
}

void RbtPanel::on_insert(){
    QString v=input_value->text().trimmed(); if(v.isEmpty()) return;
    root=insert_bst(root,v,nullptr);
    Node* cur=root;
    while(cur){
        if(v<cur->v){ if(!cur->l) break; cur=cur->l; }
        else if(v>cur->v){ if(!cur->r) break; cur=cur->r; }
        else break;
    }
    fix_insert(cur);
    redraw();
}

void RbtPanel::on_find(){
    QString v=input_value->text().trimmed(); if(v.isEmpty()) return;
    anim_nodes.clear();
    if(find_rec(root,v,&anim_nodes)){anim_index=0; anim_timer.start(260);}
    else set_status("Não encontrado");
}

void RbtPanel::on_remove(){
    QString v=input_value->text().trimmed(); if(v.isEmpty()) return;
    QVector<Node*> path; find_rec(root,v,&path);
    anim_nodes=path; anim_index=0; anim_timer.start(220);
    remove_value(v);
    redraw();
}

void RbtPanel::on_clear(){
    clear_all(root); root=nullptr; redraw();
}

void RbtPanel::on_anim_step(){
    if(anim_index>=anim_nodes.size()){anim_timer.stop(); redraw(); return;}
    redraw(anim_nodes[anim_index]); anim_index++;
}
