/** @author Guilherme Martinelli Taglietti
 *  @file   graph_panel.h
 *  @brief  Graph panel
 */
#pragma once

#include "base_panel.h"
#include <QVector>
#include <QSet>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>
#include <QComboBox>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>

class DraggableNode;

class GraphPanel:public BasePanel{
    Q_OBJECT

public:
    explicit GraphPanel(QWidget* parent=nullptr);

private:
    struct Node{QString label; QPointF pos;};
    QVector<Node> nodes;
    QVector<QPair<int,int>> edges;

    QLineEdit* input_label;
    QLineEdit* input_u;
    QLineEdit* input_v;
    QComboBox* start_combo;
    QPushButton* add_node_btn;
    QPushButton* add_edge_btn;
    QPushButton* remove_node_btn;
    QPushButton* remove_edge_btn;
    QPushButton* bfs_btn;
    QPushButton* dfs_btn;
    QPushButton* clear_btn;

    QTimer anim_timer;
    QVector<int> anim_order;
    int anim_index;

    DraggableNode* make_node_item(int idx);
    struct Visual{DraggableNode* n; QGraphicsTextItem* t;};
    QVector<Visual> visuals;
    QVector<QGraphicsLineItem*> edge_items;
    bool manual_layout;

    void build_controls(QHBoxLayout* bar) override;
    void refresh_combos();
    void ensure_items(const QSet<int>& highlight=QSet<int>());
    void update_geometry(const QSet<int>& highlight=QSet<int>());
    int index_of(const QString& label) const;
    void bfs(int s,QList<int>& order);
    void dfs(int s,QSet<int>& vis,QList<int>& order);
    void start_anim(const QList<int>& ord,const QString& title);
    void on_node_moved(int idx,const QPointF& center);

    QJsonObject capture() const override;
    void restore(const QJsonObject& obj) override;

private slots:
    void on_add_node();
    void on_add_edge();
    void on_remove_node();
    void on_remove_edge();
    void on_bfs();
    void on_dfs();
    void on_clear();
    void on_anim_step();

private:
    friend class DraggableNode;
};
