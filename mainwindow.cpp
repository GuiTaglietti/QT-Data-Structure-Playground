/** @author Guilherme Martinelli Taglietti
 *  @file   mainwindow.cpp
 *  @brief  Main window impl
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stack_panel.h"
#include "queue_panel.h"
#include "hash_panel.h"
#include "sll_panel.h"
#include "dll_panel.h"
#include "graph_panel.h"
#include "bst_panel.h"
#include "rbt_panel.h"
#include "base_panel.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QToolBar>
#include <QMessageBox>
#include <QAction>
#include <QBuffer>

static QString css_dark(){
    return
        "QMainWindow{background:#0f1115;}"
        "QTabWidget::pane{border:0;}"
        "QToolBar{background:#0b0d12;border:0;}"
        "QLabel{color:#e6eaf2;}"
        "QPushButton{background:#171a21;color:#e6eaf2;border:1px solid #2a2f3a;border-radius:12px;padding:9px 16px;}"
        "QPushButton:hover{background:#1f2430;}"
        "QPushButton:pressed{background:#0e1117;}"
        "QLineEdit,QPlainTextEdit,QComboBox{padding:9px;border-radius:10px;border:1px solid #2a2f3a;background:#12151c;color:#e6eaf2;}"
        "QComboBox QAbstractItemView{background:#12151c;color:#e6eaf2;border:1px solid #2a2f3a;}"
        "QScrollBar:vertical{background:#0f1115;width:10px;margin:0;}QScrollBar::handle:vertical{background:#2a2f3a;border-radius:5px;min-height:20px;}"
        "QScrollBar:horizontal{background:#0f1115;height:10px;margin:0;}QScrollBar::handle:horizontal{background:#2a2f3a;border-radius:5px;min-width:20px;}"
        "QGroupBox{border:1px solid #1c2230;border-radius:12px;margin-top:10px;color:#e6eaf2;}QGroupBox::title{subcontrol-origin:margin;left:10px;padding:0 4px;}";
}

static QString css_light(){
    return
        "QMainWindow{background:#f6f8fb;}"
        "QTabWidget::pane{border:0;}"
        "QToolBar{background:#ffffff;border:0;}"
        "QLabel{color:#1c2430;}"
        "QPushButton{background:#ffffff;color:#1c2430;border:1px solid #d9dde6;border-radius:12px;padding:9px 16px;}"
        "QPushButton:hover{background:#f2f4f8;}"
        "QPushButton:pressed{background:#e9edf5;}"
        "QLineEdit,QPlainTextEdit,QComboBox{padding:9px;border-radius:10px;border:1px solid #d9dde6;background:#ffffff;color:#1c2430;}"
        "QComboBox QAbstractItemView{background:#ffffff;color:#1c2430;border:1px solid #d9dde6;}"
        "QScrollBar:vertical{background:#f6f8fb;width:10px;margin:0;}QScrollBar::handle:vertical{background:#c9d1e3;border-radius:5px;min-height:20px;}"
        "QScrollBar:horizontal{background:#f6f8fb;height:10px;margin:0;}QScrollBar::handle:horizontal{background:#c9d1e3;border-radius:5px;min-width:20px;}"
        "QGroupBox{border:1px solid #d9dde6;border-radius:12px;margin-top:10px;color:#1c2430;}QGroupBox::title{subcontrol-origin:margin;left:10px;padding:0 4px;}";
}

QIcon MainWindow::icon_sun() const{
    static const char* svg=
        "<svg width='24' height='24' viewBox='0 0 24 24' fill='none' xmlns='http://www.w3.org/2000/svg'>"
        "<circle cx='12' cy='12' r='4' fill='#FDB813'/>"
        "<g stroke='#FDB813' stroke-width='2'><line x1='12' y1='1' x2='12' y2='5'/>"
        "<line x1='12' y1='19' x2='12' y2='23'/><line x1='1' y1='12' x2='5' y2='12'/>"
        "<line x1='19' y1='12' x2='23' y2='12'/><line x1='4' y1='4' x2='6.8' y2='6.8'/>"
        "<line x1='17.2' y1='17.2' x2='20' y2='20'/><line x1='4' y1='20' x2='6.8' y2='17.2'/>"
        "<line x1='17.2' y1='6.8' x2='20' y2='4'/></g></svg>";
    QPixmap pm; pm.loadFromData((const uchar*)svg,strlen(svg),"SVG");
    return QIcon(pm);
}

QIcon MainWindow::icon_moon() const{
    static const char* svg=
        "<svg width='24' height='24' viewBox='0 0 24 24' fill='none' xmlns='http://www.w3.org/2000/svg'>"
        "<path d='M21 12.5A9 9 0 0111.5 3 8 8 0 1021 12.5z' fill='#E6EAF2'/></svg>";
    QPixmap pm; pm.loadFromData((const uchar*)svg,strlen(svg),"SVG");
    return QIcon(pm);
}

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent),ui(new Ui::MainWindow),dark_mode(true){
    ui->setupUi(this);

    tabs=new QTabWidget(this);
    tabs->addTab(make_stack(),tr("Pilha"));
    tabs->addTab(make_queue(),tr("Fila"));
    tabs->addTab(make_hash(),tr("Tabela Hash"));
    tabs->addTab(make_sll(),tr("Lista Encadeada"));
    tabs->addTab(make_dll(),tr("Lista Duplamente Encadeada"));
    tabs->addTab(make_graph(),tr("Grafo"));
    tabs->addTab(make_bst(),tr("Árvore BST"));
    tabs->addTab(make_rbt(),tr("Árvore Vermelho-Preto"));
    setCentralWidget(tabs);
    tabs->tabBar()->hide();

    auto* topbar=new QWidget(this);
    auto* hl=new QHBoxLayout(topbar);
    hl->setContentsMargins(12,6,12,6);

    auto* title=new QLabel(tr("QT DSA Playground"),topbar);
    QFont f=title->font(); f.setPointSizeF(f.pointSizeF()+1.5); f.setBold(true); title->setFont(f);

    pick_btn=new QPushButton(tr("Escolher Estrutura"),topbar);
    about_btn=new QPushButton(tr("Sobre o Projeto"),topbar);
    theme_btn=new QPushButton(topbar);
    theme_btn->setToolTip(tr("Alternar tema (Ctrl+T)"));

    hl->addWidget(title);
    hl->addStretch();
    hl->addWidget(pick_btn);
    hl->addWidget(about_btn);
    hl->addWidget(theme_btn);

    addToolBarBreak();
    auto* tb=new QToolBar(this);
    tb->setMovable(false);
    tb->addWidget(topbar);
    addToolBar(Qt::TopToolBarArea,tb);

    connect(pick_btn,&QPushButton::clicked,this,&MainWindow::on_pick_structure);
    connect(about_btn,&QPushButton::clicked,this,&MainWindow::on_about_project);
    connect(theme_btn,&QPushButton::clicked,this,&MainWindow::on_toggle_theme);

    auto* act_toggle=new QAction(this);
    act_toggle->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_T));
    addAction(act_toggle);
    connect(act_toggle,&QAction::triggered,this,&MainWindow::on_toggle_theme);

    setWindowTitle(tr("QT DSA Playground"));
    resize(1160,760);
    apply_theme();
}

MainWindow::~MainWindow(){
    delete ui;
}

QWidget* MainWindow::make_stack(){return new StackPanel(this);}
QWidget* MainWindow::make_queue(){return new QueuePanel(this);}
QWidget* MainWindow::make_hash(){return new HashPanel(this);}
QWidget* MainWindow::make_sll(){return new SllPanel(this);}
QWidget* MainWindow::make_dll(){return new DllPanel(this);}
QWidget* MainWindow::make_graph(){return new GraphPanel(this);}
QWidget* MainWindow::make_bst(){return new BstPanel(this);}
QWidget* MainWindow::make_rbt(){return new RbtPanel(this);}

void MainWindow::apply_theme(){
    setStyleSheet(dark_mode? css_dark():css_light());
    theme_btn->setIcon(dark_mode? icon_sun():icon_moon());
    auto panels=findChildren<BasePanel*>();
    for(auto* p:panels) p->apply_theme(dark_mode);
}

void MainWindow::on_toggle_theme(){
    dark_mode=!dark_mode;
    apply_theme();
}

void MainWindow::on_pick_structure(){
    QDialog d(this);
    d.setWindowTitle(tr("Escolha a Estrutura"));
    auto* vl=new QVBoxLayout(&d);

    auto make_group=[&](const QString& title,const QList<QPair<QString,int>>& items){
        auto* gb=new QGroupBox(title,&d);
        auto* gl=new QGridLayout(gb);
        int r=0,c=0;
        for(const auto& it:items){
            auto* b=new QPushButton(it.first,gb);
            b->setMinimumHeight(40);
            gl->addWidget(b,r,c);
            int tab_index=it.second;
            connect(b,&QPushButton::clicked,&d,[this,&d,tab_index](){tabs->setCurrentIndex(tab_index); d.accept();});
            if(++c==3){c=0;++r;}
        }
        vl->addWidget(gb);
    };

    make_group(tr("Listas e Sequências"),{{tr("Pilha"),0},{tr("Fila"),1},{tr("Lista Encadeada"),3},{tr("Lista Duplamente Encadeada"),4}});
    make_group(tr("Mapeamentos"),{{tr("Tabela Hash"),2}});
    make_group(tr("Grafos"),{{tr("Grafo"),5}});
    make_group(tr("Árvores"),{{tr("Árvore BST"),6},{tr("Árvore Vermelho-Preto"),7}});

    d.exec();
}

void MainWindow::on_about_project(){
    QString t=
        tr("Autor: Guilherme Martinelli Taglietti\n"
           "Projeto: QT DSA Playground\n\n"
           "Objetivo: auxiliar a compreensão e o estudo de estruturas de dados com visualizações em tempo real,"
           " animações e exportação de exemplos. Este projeto continuará evoluindo com foco educacional e de pesquisa.\n\n"
           "Origem: inicialmente desenvolvido como trabalho final da disciplina de Laboratório de Programação III"
           " do curso de Ciência da Computação.");
    QMessageBox::information(this,tr("Sobre o Projeto"),t);
}
