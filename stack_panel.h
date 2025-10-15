/** @author Guilherme Martinelli Taglietti
 *  @file   stack_panel.h
 *  @brief  Stack panel
 */
#pragma once

#include "base_panel.h"
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>

class StackPanel:public BasePanel{
    Q_OBJECT

public:
    explicit StackPanel(QWidget* parent=nullptr);

private:
    QVector<QString> data;
    QPushButton* push_btn;
    QPushButton* pop_btn;
    QPushButton* clear_btn;

    void build_controls(QHBoxLayout* bar) override;
    void redraw();
    QJsonObject capture() const override;
    void restore(const QJsonObject& obj) override;

private slots:
    void on_push();
    void on_pop();
    void on_clear();
};
