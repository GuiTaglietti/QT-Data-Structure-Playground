/** @author Guilherme Martinelli Taglietti
 *  @file   main.cpp
 *  @brief  Application entry point with pre-launch language picker
 */
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>

struct LangText{QString title; QString prompt; QString start;};
static const QMap<QString,LangText> k_texts{
    {"pt_BR",{ "Escolha o idioma", "Escolha o idioma da aplicação:", "Iniciar" }},
    {"en_US",{ "Choose the language", "Choose the application language:", "Start" }},
    {"es_ES",{ "Elige el idioma", "Elige el idioma de la aplicación:", "Iniciar" }}
};

static QString locale_from_index(int i){
    switch(i){
    case 0: return "pt_BR";
    case 1: return "en_US";
    case 2: return "es_ES";
    default: return "en_US";
    }
}
static int index_from_locale(const QString& loc){
    if(loc=="pt_BR") return 0;
    if(loc=="en_US") return 1;
    if(loc=="es_ES") return 2;
    return 1;
}

int main(int argc,char* argv[]){
    QApplication a(argc,argv);
    QApplication::setApplicationName("QT Data Structure Playground");

    QSettings settings;
    QString remembered=settings.value("ui/locale").toString();
    if(remembered.isEmpty()){
        const QString sys=QLocale::system().name();
        remembered=(sys=="pt_BR"||sys=="es_ES")? sys:"en_US";
    }

    QDialog pick(nullptr);
    pick.setWindowFlag(Qt::MSWindowsFixedSizeDialogHint,true);
    pick.setWindowFlag(Qt::WindowContextHelpButtonHint,false);
    pick.setModal(true);

    auto* vl=new QVBoxLayout(&pick);
    auto* label=new QLabel(&pick);
    auto* combo=new QComboBox(&pick);
    auto* btn=new QPushButton(&pick);

    combo->addItem("Português (Brasil)");
    combo->addItem("English (US)");
    combo->addItem("Español (España)");
    combo->setCurrentIndex(index_from_locale(remembered));

    auto reapply=[&](){
        const QString loc=locale_from_index(combo->currentIndex());
        const auto txt=k_texts.value(loc,k_texts.value("en_US"));
        pick.setWindowTitle(txt.title);
        label->setText(txt.prompt);
        btn->setText(txt.start);
    };
    reapply();
    QObject::connect(combo,&QComboBox::currentIndexChanged,&pick,[&](){reapply();});

    auto* hl=new QHBoxLayout;
    hl->addStretch();
    hl->addWidget(btn);
    vl->addWidget(label);
    vl->addWidget(combo);
    vl->addLayout(hl);
    QObject::connect(btn,&QPushButton::clicked,&pick,&QDialog::accept);

    if(pick.exec()!=QDialog::Accepted) return 0;

    const QString chosen=locale_from_index(combo->currentIndex());
    settings.setValue("ui/locale",chosen);

    static QTranslator translator;
    const QString qmPath = QCoreApplication::applicationDirPath()
                           + QString("/QTDataStructurePlayground_%1.qm").arg(chosen);
    translator.load(qmPath);
    a.installTranslator(&translator);

    MainWindow w;
    w.show();
    return a.exec();
}
