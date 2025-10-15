/** @author Guilherme Martinelli Taglietti
 *  @file   dll_panel.h
 *  @brief  Doubly linked list panel
 */
#pragma once

#include "base_panel.h"
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>

class DllPanel:public BasePanel{
    Q_OBJECT

public:
    explicit DllPanel(QWidget* parent=nullptr);

private:
    QVector<QString> data;
    QPushButton* push_front_btn;
    QPushButton* pop_back_btn;
    QPushButton* remove_sel_btn;
    QPushButton* clear_btn;

    int selected_index=-1;

    void build_controls(QHBoxLayout* bar) override;
    void redraw();
    QJsonObject capture() const override;
    void restore(const QJsonObject& obj) override;

private slots:
    void on_push_front();
    void on_pop_back();
    void on_remove_sel();
    void on_clear();
};
