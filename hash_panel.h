/** @author Guilherme Martinelli Taglietti
 *  @file   hash_panel.h
 *  @brief  Hash table panel
 */
#pragma once

#include "base_panel.h"
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>

class HashPanel:public BasePanel{
    Q_OBJECT

public:
    explicit HashPanel(QWidget* parent=nullptr);

private:
    struct Item{bool used=false;QString key;QString val;};
    QVector<Item> table;
    QLineEdit* input_key;
    QPushButton* put_btn;
    QPushButton* get_btn;
    QPushButton* clear_btn;

    void build_controls(QHBoxLayout* bar) override;
    void redraw(int highlight_index=-1);
    int H(const QString& k) const;
    QJsonObject capture() const override;
    void restore(const QJsonObject& obj) override;

private slots:
    void on_put();
    void on_get();
    void on_clear();
};
