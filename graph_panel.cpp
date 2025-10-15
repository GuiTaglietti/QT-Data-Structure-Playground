/** @author Guilherme Martinelli Taglietti
 *  @file   graph_panel.cpp
 *  @brief  Graph panel impl
 */
#include "graph_panel.h"
#include "texts.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QQueue>
#include <QtMath>

static QPointF trim_from_center(const QPointF& a,const QPointF& b,int r){
    QLineF L(a,b);
    if(L.length()<=r) return a;
    L.setLength(L.length()-r);
    return L.p2();
}

class DraggableNode:public QGraphicsEllipseItem{
public:
    int idx;
    class GraphPanel* owner;
    DraggableNode(int i,GraphPanel* o,int r):QGraphicsEllipseItem(-r,-r,2*r,2*r),idx(i),owner(o){
        setFlag(ItemIsMovable,true);
        setFlag(ItemIsSelectable,true);
        setFlag(ItemSendsGeometryChanges,true);
        setAcceptedMouseButtons(Qt::LeftButton);
        setCursor(Qt::OpenHandCursor);
        setZValue(1);
    }
    QVariant itemChange(GraphicsItemChange change,const QVariant& value){
        if(change==ItemPositionHasChanged && owner){
            owner->on_node_moved(idx,value.toPointF());
        }
        return QGraphicsEllipseItem::itemChange(change,value);
    }
    void mousePressEvent(QGraphicsSceneMouseEvent* e){setCursor(Qt::ClosedHandCursor); QGraphicsEllipseItem::mousePressEvent(e);}
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* e){setCursor(Qt::OpenHandCursor); QGraphicsEllipseItem::mouseReleaseEvent(e);}
};

GraphPanel::GraphPanel(QWidget* parent):BasePanel("Grafo",about_graph_pt(),parent),anim_index(0),manual_layout(false){
    set_kind("graph");
    build_controls(controls_bar);
    connect(&anim_timer,&QTimer::timeout,this,&GraphPanel::on_anim_step);
}

void GraphPanel::build_controls(QHBoxLayout* bar){
    input_label=new QLineEdit(this); input_label->setPlaceholderText("Nó");
    input_u=new QLineEdit(this); input_u->setPlaceholderText("u");
    input_v=new QLineEdit(this); input_v->setPlaceholderText("v");

    start_combo=new QComboBox(this);

    add_node_btn=new QPushButton("Adicionar Nó",this);
    add_edge_btn=new QPushButton("Adicionar Aresta",this);
    remove_node_btn=new QPushButton("Remover Nó",this);
    remove_edge_btn=new QPushButton("Remover Aresta",this);
    bfs_btn=new QPushButton("BFS",this);
    dfs_btn=new QPushButton("DFS",this);
    clear_btn=new QPushButton("Limpar",this);

    bar->addWidget(new QLabel("Valor:",this)); bar->addWidget(input_label); bar->addWidget(add_node_btn);
    bar->addSpacing(10);
    bar->addWidget(new QLabel("u:",this)); bar->addWidget(input_u);
    bar->addWidget(new QLabel("v:",this)); bar->addWidget(input_v);
    bar->addWidget(add_edge_btn);
    bar->addWidget(remove_edge_btn);
    bar->addSpacing(10);
    bar->addWidget(new QLabel("Partida:",this)); bar->addWidget(start_combo);
    bar->addWidget(bfs_btn); bar->addWidget(dfs_btn);
    bar->addWidget(remove_node_btn);
    bar->addWidget(clear_btn);
    bar->addStretch();

    connect(add_node_btn,&QPushButton::clicked,this,&GraphPanel::on_add_node);
    connect(add_edge_btn,&QPushButton::clicked,this,&GraphPanel::on_add_edge);
    connect(remove_node_btn,&QPushButton::clicked,this,&GraphPanel::on_remove_node);
    connect(remove_edge_btn,&QPushButton::clicked,this,&GraphPanel::on_remove_edge);
    connect(bfs_btn,&QPushButton::clicked,this,&GraphPanel::on_bfs);
    connect(dfs_btn,&QPushButton::clicked,this,&GraphPanel::on_dfs);
    connect(clear_btn,&QPushButton::clicked,this,&GraphPanel::on_clear);
}

void GraphPanel::refresh_combos(){
    start_combo->clear();
    for(auto& n:nodes) start_combo->addItem(n.label);
}

int GraphPanel::index_of(const QString& label) const{
    for(int i=0;i<nodes.size();++i) if(nodes[i].label==label) return i;
    return -1;
}

DraggableNode* GraphPanel::make_node_item(int idx){
    int r=18;
    auto* e=new DraggableNode(idx,this,r);
    e->setPen(QPen(Qt::black));
    e->setBrush(QBrush(color_node_fill_normal));
    view->scene_ptr()->addItem(e);

    auto* t=view->scene_ptr()->addText(nodes[idx].label,view->fit_font_for_width(nodes[idx].label,28,16));
    t->setDefaultTextColor(color_text_normal);
    t->setParentItem(e);

    visuals.push_back({e,t});
    e->setPos(nodes[idx].pos);
    QRectF br=t->boundingRect();
    t->setPos(-br.width()/2,-br.height()/2-1);
    return e;
}

void GraphPanel::ensure_items(const QSet<int>& hi){
    bool need_rebuild=(visuals.size()!=nodes.size())||(edge_items.size()!=edges.size());
    if(need_rebuild){
        view->clear_scene();
        visuals.clear();
        edge_items.clear();
        for(int i=0;i<edges.size();++i){
            auto* li=view->scene_ptr()->addLine(0,0,0,0,QPen(color_edge,2));
            li->setZValue(0);
            edge_items.push_back(li);
        }
        for(int i=0;i<nodes.size();++i) make_node_item(i);
        if(!manual_layout) view->auto_layout_now();
    }
    update_geometry(hi);
}

void GraphPanel::update_geometry(const QSet<int>& hi){
    int r=18;
    int n=qMin(visuals.size(),nodes.size());
    for(int i=0;i<n;++i){
        auto* e=visuals[i].n;
        auto* t=visuals[i].t;
        e->setBrush(QBrush(hi.contains(i)? color_node_fill_highlight:color_node_fill_normal));
        QFont f=view->fit_font_for_width(nodes[i].label,28,16);
        t->setFont(f);
        t->setDefaultTextColor(hi.contains(i)? color_text_highlight:color_text_normal);
        QRectF br=t->boundingRect();
        t->setPos(-br.width()/2,-br.height()/2-1);
        e->setPos(nodes[i].pos);
    }
    for(int k=0;k<edges.size() && k<edge_items.size();++k){
        int a=edges[k].first, b=edges[k].second;
        if(a<0||b<0||a>=n||b>=n) continue;
        QPointF ca=visuals[a].n->scenePos();
        QPointF cb=visuals[b].n->scenePos();
        QPointF p1=trim_from_center(ca,cb,r);
        QPointF p2=trim_from_center(cb,ca,r);
        edge_items[k]->setPen(QPen(color_edge,2));
        edge_items[k]->setLine(QLineF(p1,p2));
    }
}

void GraphPanel::on_node_moved(int idx,const QPointF& center){
    if(idx<0||idx>=nodes.size()) return;
    nodes[idx].pos=center;
    manual_layout=true;
    update_geometry();
}

void GraphPanel::on_add_node(){
    QString lab=input_label->text().trimmed();
    if(lab.isEmpty()) return;
    if(index_of(lab)>=0) return;
    int k=nodes.size();
    double ang=2.0*3.1415926535*(k%12)/qMax(12,k+1);
    QPointF p(360+130*cos(ang),160+130*sin(ang));
    nodes.push_back({lab,p});
    refresh_combos();
    ensure_items();
}

void GraphPanel::on_add_edge(){
    int iu=index_of(input_u->text().trimmed());
    int iv=index_of(input_v->text().trimmed());
    if(iu<0||iv<0||iu==iv) return;
    if(iu>iv) std::swap(iu,iv);
    for(auto e:edges) if(e.first==iu&&e.second==iv) return;
    edges.push_back({iu,iv});
    ensure_items();
}

void GraphPanel::on_remove_node(){
    int i=index_of(input_label->text().trimmed());
    if(i<0) return;
    QVector<QPair<int,int>> ne;
    for(auto e:edges) if(e.first!=i&&e.second!=i){
            int a=e.first>i? e.first-1:e.first;
            int b=e.second>i? e.second-1:e.second;
            ne.push_back({a,b});
        }
    edges=ne;
    nodes.removeAt(i);
    visuals.clear(); edge_items.clear();
    refresh_combos();
    ensure_items();
}

void GraphPanel::on_remove_edge(){
    int iu=index_of(input_u->text().trimmed());
    int iv=index_of(input_v->text().trimmed());
    if(iu<0||iv<0) return;
    if(iu>iv) std::swap(iu,iv);
    for(int k=0;k<edges.size();++k) if(edges[k].first==iu&&edges[k].second==iv){edges.removeAt(k);break;}
    visuals.clear(); edge_items.clear();
    ensure_items();
}

void GraphPanel::bfs(int s,QList<int>& order){
    if(s<0||s>=nodes.size()) return;
    QVector<QVector<int>> adj(nodes.size());
    for(auto e:edges){adj[e.first].push_back(e.second);adj[e.second].push_back(e.first);}
    QVector<bool> vis(nodes.size());
    QQueue<int> q;
    vis[s]=true; q.enqueue(s);
    while(!q.isEmpty()){
        int u=q.dequeue(); order.push_back(u);
        for(int v:adj[u]) if(!vis[v]){vis[v]=true; q.enqueue(v);}
    }
}

void GraphPanel::dfs(int s,QSet<int>& vis,QList<int>& order){
    if(s<0||s>=nodes.size()) return;
    vis.insert(s); order.push_back(s);
    for(auto& e:edges){
        int u=e.first, v=e.second;
        if(u==s&&!vis.contains(v)) dfs(v,vis,order);
        else if(v==s&&!vis.contains(u)) dfs(u,vis,order);
    }
}

void GraphPanel::start_anim(const QList<int>& ord,const QString& title){
    anim_order=ord.toVector();
    anim_index=0;
    set_status(title+": "+[&](){QString s; for(int i=0;i<ord.size();++i){if(i) s+=" "; s+=nodes[ord[i]].label;} return s;}());
    anim_timer.stop(); anim_timer.start(350);
}

void GraphPanel::on_bfs(){
    if(nodes.isEmpty()) return;
    int s=start_combo->currentIndex(); if(s<0) s=0;
    QList<int> ord; bfs(s,ord); start_anim(ord,"BFS");
}

void GraphPanel::on_dfs(){
    if(nodes.isEmpty()) return;
    int s=start_combo->currentIndex(); if(s<0) s=0;
    QSet<int> vis; QList<int> ord; dfs(s,vis,ord); start_anim(ord,"DFS");
}

void GraphPanel::on_clear(){
    anim_timer.stop();
    nodes.clear();
    edges.clear();
    visuals.clear();
    edge_items.clear();
    manual_layout=false;
    refresh_combos();
    view->clear_scene();
    set_status("Grafo limpo");
}


void GraphPanel::on_anim_step(){
    if(anim_index>=anim_order.size()){anim_timer.stop(); ensure_items(); return;}
    QSet<int> hi; for(int i=0;i<=anim_index&&i<anim_order.size();++i) hi.insert(anim_order[i]);
    ensure_items(hi); anim_index++;
}

QJsonObject GraphPanel::capture() const{
    QJsonArray jn, je;
    for(const auto& n:nodes){
        QJsonObject o; o["label"]=n.label; o["x"]=n.pos.x(); o["y"]=n.pos.y(); jn.append(o);
    }
    for(const auto& e:edges){
        QJsonObject o; o["u"]=e.first; o["v"]=e.second; je.append(o);
    }
    QJsonObject out; out["nodes"]=jn; out["edges"]=je; return out;
}

void GraphPanel::restore(const QJsonObject& o){
    nodes.clear(); edges.clear();
    for(auto v:o.value("nodes").toArray()){
        auto obj=v.toObject();
        nodes.push_back({obj.value("label").toString(), QPointF(obj.value("x").toDouble(), obj.value("y").toDouble())});
    }
    int N=nodes.size();
    for(auto v:o.value("edges").toArray()){
        auto obj=v.toObject();
        int u=obj.value("u").toInt(), w=obj.value("v").toInt();
        if(u>=0&&w>=0&&u<N&&w<N){
            if(u>w) std::swap(u,w);
            bool dup=false; for(auto& e:edges) if(e.first==u&&e.second==w){dup=true;break;}
            if(!dup) edges.push_back({u,w});
        }
    }
    visuals.clear(); edge_items.clear();
    refresh_combos();
    manual_layout=true;
    ensure_items();
}
