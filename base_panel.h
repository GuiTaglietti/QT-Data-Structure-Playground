/** @author Guilherme Martinelli Taglietti
 *  @file   base_panel.h
 *  @brief  Base panel for controls and diagram
 */
#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QTimer>
#include <QJsonObject>
#include "diagram_view.h"

class BasePanel:public QWidget{
    Q_OBJECT

public:
    explicit BasePanel(const QString& title_pt,const QString& about_pt,QWidget* parent=nullptr);
    void apply_theme(bool dark);

protected:
    DiagramView* view;
    QLineEdit* input_value;
    QLabel* title_label;
    QPlainTextEdit* about_box;
    QComboBox* export_lang;
    QPushButton* export_btn;
    QHBoxLayout* controls_bar;
    QPlainTextEdit* output_box;
    QPushButton* save_btn;
    QPushButton* load_btn;
    QComboBox* saved_list;
    QString structure_kind;

    virtual void build_controls(QHBoxLayout* bar);
    virtual QJsonObject capture() const;
    virtual void restore(const QJsonObject& obj);
    void set_status(const QString& s);
    void set_highlight_palette(bool dark);
    void export_snippet(const QString& structure_pt);
    void set_kind(const QString& k);

    QColor color_node_fill_normal;
    QColor color_node_fill_highlight;
    QColor color_text_normal;
    QColor color_text_highlight;
    QColor color_edge;

private slots:
    void on_export();
    void on_save();
    void on_load();

private:
    QLabel* status_label;
    QString about_text;
    QString title_text;
    bool dark_mode=false;
};
