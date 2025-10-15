/** @author Guilherme Martinelli Taglietti
 *  @file   mainwindow.h
 *  @brief  Main window
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui{class MainWindow;}
QT_END_NAMESPACE

class MainWindow:public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget* parent=nullptr);
    ~MainWindow();

private slots:
    void on_pick_structure();
    void on_about_project();
    void on_toggle_theme();

private:
    Ui::MainWindow* ui;
    QTabWidget* tabs;
    QPushButton* pick_btn;
    QPushButton* about_btn;
    QPushButton* theme_btn;
    bool dark_mode;
    QWidget* make_stack();
    QWidget* make_queue();
    QWidget* make_hash();
    QWidget* make_sll();
    QWidget* make_dll();
    QWidget* make_graph();
    QWidget* make_bst();
    QWidget* make_rbt();
    void apply_theme();
    QIcon icon_sun() const;
    QIcon icon_moon() const;
};
#endif
