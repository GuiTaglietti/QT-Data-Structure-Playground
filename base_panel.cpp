/** @author Guilherme Martinelli Taglietti
 *  @file   base_panel.cpp
 *  @brief  Base panel impl
 */
#include "base_panel.h"
#include "code_exporter.h"
#include "storage_manager.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

BasePanel::BasePanel(const QString& title_pt,const QString& about_pt,QWidget* parent)
    : QWidget(parent),about_text(about_pt),title_text(title_pt){
    auto* root=new QVBoxLayout(this);

    auto* header=new QHBoxLayout;
    title_label=new QLabel(title_pt,this);
    header->addWidget(title_label);
    header->addStretch();

    export_lang=new QComboBox(this);
    export_lang->addItems(CodeExporter::languages_pt());
    export_btn=new QPushButton(tr("Exportar Código"),this);

    save_btn=new QPushButton(tr("Salvar"),this);
    load_btn=new QPushButton(tr("Carregar"),this);
    saved_list=new QComboBox(this);

    header->addWidget(export_lang);
    header->addWidget(export_btn);
    header->addSpacing(12);
    header->addWidget(saved_list);
    header->addWidget(load_btn);
    header->addWidget(save_btn);
    root->addLayout(header);

    auto* split=new QSplitter(Qt::Vertical,this);

    auto* top=new QWidget(this);
    auto* top_layout=new QVBoxLayout(top);

    controls_bar=new QHBoxLayout;
    input_value=new QLineEdit(this);
    input_value->setPlaceholderText(tr("Valor (pode ser texto)"));
    controls_bar->addWidget(new QLabel(tr("Valor:"),this));
    controls_bar->addWidget(input_value);
    top_layout->addLayout(controls_bar);

    view=new DiagramView(this);
    top_layout->addWidget(view);

    auto* bottom=new QWidget(this);
    auto* bottom_layout=new QVBoxLayout(bottom);

    bottom_layout->addWidget(new QLabel(tr("Sobre a estrutura"),this));
    about_box=new QPlainTextEdit(this);
    about_box->setPlainText(about_text);
    about_box->setReadOnly(true);
    bottom_layout->addWidget(about_box);

    bottom_layout->addWidget(new QLabel(tr("Saída"),this));
    output_box=new QPlainTextEdit(this);
    output_box->setReadOnly(true);
    bottom_layout->addWidget(output_box);

    split->addWidget(top);
    split->addWidget(bottom);
    split->setStretchFactor(0,3);
    split->setStretchFactor(1,1);
    root->addWidget(split);

    status_label=new QLabel("",this);
    root->addWidget(status_label);

    connect(export_btn,&QPushButton::clicked,this,&BasePanel::on_export);
    connect(save_btn,&QPushButton::clicked,this,&BasePanel::on_save);
    connect(load_btn,&QPushButton::clicked,this,&BasePanel::on_load);

    StorageManager::ensure_env();
    apply_theme(true);
}

void BasePanel::apply_theme(bool dark){
    dark_mode=dark;
    set_highlight_palette(dark_mode);
    title_label->setStyleSheet(QString("font-size:20px;font-weight:600;color:%1;")
                                   .arg(dark_mode? "#e6eaf2":"#1c2430"));
    about_box->setStyleSheet(QString("background:%1;color:%2;border-radius:10px;padding:10px;border:1px solid %3;")
                                 .arg(dark_mode? "#141720":"#ffffff")
                                 .arg(dark_mode? "#d9deea":"#1c2430")
                                 .arg(dark_mode? "#252b36":"#d9dde6"));
    output_box->setStyleSheet(QString("background:%1;color:%2;border:1px solid %3;border-radius:10px;padding:10px;font-family:monospace;")
                                  .arg(dark_mode? "#0f1115":"#ffffff")
                                  .arg(dark_mode? "#e6eaf2":"#1c2430")
                                  .arg(dark_mode? "#252b36":"#d9dde6"));
    view->set_background(dark_mode? QColor("#1b1e25"):QColor("#f0f3f8"));
}

void BasePanel::build_controls(QHBoxLayout*){}

QJsonObject BasePanel::capture() const{
    QJsonObject o; o["placeholder"]=true; return o;
}

void BasePanel::restore(const QJsonObject&){
}

void BasePanel::set_highlight_palette(bool){
    color_node_fill_normal=QColor("#cce0ff");
    color_node_fill_highlight=QColor("#ffdf70");
    color_text_normal=QColor("#000");
    color_text_highlight=QColor("#000");
    color_edge=QColor("#aaaaaa");
}

void BasePanel::set_status(const QString& s){
    output_box->appendPlainText(s);
}

void BasePanel::export_snippet(const QString& structure_pt){
    QString lang=export_lang->currentText();
    QString code=CodeExporter::snippet_for(structure_pt,lang);
    QString ext=CodeExporter::language_ext(lang);
    QString fn=QFileDialog::getSaveFileName(this,tr("Salvar exemplo"),structure_pt+"."+ext,"*.*");
    if(fn.isEmpty()) return;
    QFile f(fn);
    if(f.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        QTextStream out(&f);
        out<<code;
        f.close();
        set_status(tr("Arquivo exportado: ")+fn);
    }
}

void BasePanel::on_export(){
    export_snippet(title_text);
}

void BasePanel::set_kind(const QString& k){
    structure_kind=k;
    saved_list->clear();
    saved_list->addItems(StorageManager::list_names(structure_kind));
}

void BasePanel::on_save(){
    QString name=QFileDialog::getSaveFileName(this,tr("Salvar como"),StorageManager::base_dir()+"/"+structure_kind+"_nome.json","*.json");
    if(name.isEmpty()) return;
    name=QFileInfo(name).baseName();
    QJsonObject obj=capture();
    if(StorageManager::save_json(structure_kind,name,obj)){
        set_status(tr("Salvo: ")+name);
        set_kind(structure_kind);
    }else set_status(tr("Falha ao salvar"));
}

void BasePanel::on_load(){
    QString name=saved_list->currentText();
    if(name.isEmpty()) return;
    QJsonObject obj=StorageManager::load_json(structure_kind,name);
    if(obj.isEmpty()) set_status(tr("Nada carregado"));
    else{
        restore(obj);
        set_status(tr("Carregado: ")+name);
    }
}
