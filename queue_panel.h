/** @author Guilherme Martinelli Taglietti
 *  @file   queue_panel.h
 *  @brief  Queue panel
 */
#pragma once

#include "base_panel.h"
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>

class QueuePanel:public BasePanel{
    Q_OBJECT

public:
    explicit QueuePanel(QWidget* parent=nullptr);

private:
    QVector<QString> data;
    QPushButton* enq_btn;
    QPushButton* deq_btn;
    QPushButton* clear_btn;

    void build_controls(QHBoxLayout* bar) override;
    void redraw();
    QJsonObject capture() const override;
    void restore(const QJsonObject& obj) override;

private slots:
    void on_enq();
    void on_deq();
    void on_clear();
};
