/** @author Guilherme Martinelli Taglietti
 *  @file   sll_panel.h
 *  @brief  Singly linked list panel
 */
#pragma once

#include "base_panel.h"
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>

class SllPanel:public BasePanel{
    Q_OBJECT

public:
    explicit SllPanel(QWidget* parent=nullptr);

private:
    QVector<QString> data;
    QPushButton* push_front_btn;
    QPushButton* pop_front_btn;
    QPushButton* remove_sel_btn;
    QPushButton* clear_btn;

    int selected_index=-1;

    void build_controls(QHBoxLayout* bar) override;
    void redraw();
    QJsonObject capture() const override;
    void restore(const QJsonObject& obj) override;

private slots:
    void on_push_front();
    void on_pop_front();
    void on_remove_sel();
    void on_clear();
};
