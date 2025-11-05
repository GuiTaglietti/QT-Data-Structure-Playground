#pragma once

#include "base_panel.h"
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

class BstPanel:public BasePanel{
    Q_OBJECT

public:
    explicit BstPanel(QWidget* parent=nullptr);

private:
    struct Node{QString v;Node* l;Node* r;};
    Node* root=nullptr;
    QPushButton* insert_btn;
    QPushButton* find_btn;
    QPushButton* remove_btn;
    QPushButton* clear_btn;

    QTimer anim_timer;
    QVector<Node*> anim_path;
    int anim_i=0;
    bool anim_pending=false;
    QString anim_pending_msg;

    void build_controls(QHBoxLayout* bar) override;
    void redraw(Node* flash=nullptr);
    Node* insert_rec(Node* n,const QString& v);
    bool find_with_path(Node* n,const QString& v,QVector<Node*>* path);
    Node* remove_rec(Node* n,const QString& v);
    Node* min_node(Node* n);
    void draw_node(Node* n,int x,int y,int dx,Node* flash);
    void clear_all(Node* n);
    static int bst_compare(const QString& a, const QString& b);

    QJsonObject capture() const override;
    void restore(const QJsonObject& obj) override;

private slots:
    void on_insert();
    void on_find();
    void on_remove();
    void on_clear();
    void on_anim_step();
};
