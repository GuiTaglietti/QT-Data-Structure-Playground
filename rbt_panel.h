#pragma once

#include "base_panel.h"
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

class RbtPanel:public BasePanel{
    Q_OBJECT

public:
    explicit RbtPanel(QWidget* parent=nullptr);

private:
    enum Color{RED,BLACK};
    struct Node{QString v;Color c;Node* p;Node* l;Node* r;};
    Node* root=nullptr;

    QPushButton* insert_btn;
    QPushButton* find_btn;
    QPushButton* remove_btn;
    QPushButton* clear_btn;

    QTimer anim_timer;
    QVector<Node*> anim_nodes;
    int anim_index=0;
    bool anim_pending=false;
    QString anim_pending_msg;

    void build_controls(QHBoxLayout* bar) override;
    void redraw(Node* flash=nullptr);
    void draw_node(Node* n,int x,int y,int dx,Node* flash);
    void clear_all(Node* n);

    Node* insert_bst(Node* n,const QString& v,Node* p);
    void fix_insert(Node* z);
    void rotate_left(Node* x);
    void rotate_right(Node* x);

    bool find_rec(Node* n,const QString& v,QVector<Node*>* path=nullptr);

    void remove_value(const QString& v);

    QJsonObject capture() const override;
    void restore(const QJsonObject& obj) override;

private slots:
    void on_insert();
    void on_find();
    void on_remove();
    void on_clear();
    void on_anim_step();
};
