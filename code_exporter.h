/** @author Guilherme Martinelli Taglietti
 *  @file   code_exporter.h
 *  @brief  Snippet exporter
 */
#pragma once

#include <QString>
#include <QStringList>
#include <QMap>

enum class SnippetLang{
    C,
    CPP,
    Java,
    Python,
    JS
};

class CodeExporter{
public:
    static QStringList languages_pt();
    static QString language_ext(const QString& lang_pt);
    static QString snippet_for(const QString& structure,const QString& lang_pt);

private:
    static QString c_stack();
    static QString cpp_stack();
    static QString java_stack();
    static QString py_stack();
    static QString js_stack();

    static QString c_queue();
    static QString cpp_queue();
    static QString java_queue();
    static QString py_queue();
    static QString js_queue();

    static QString c_hash();
    static QString cpp_hash();
    static QString java_hash();
    static QString py_hash();
    static QString js_hash();

    static QString c_sll();
    static QString cpp_sll();
    static QString java_sll();
    static QString py_sll();
    static QString js_sll();

    static QString c_dll();
    static QString cpp_dll();
    static QString java_dll();
    static QString py_dll();
    static QString js_dll();

    static QString c_graph();
    static QString cpp_graph();
    static QString java_graph();
    static QString py_graph();
    static QString js_graph();

    static QString c_bst();
    static QString cpp_bst();
    static QString java_bst();
    static QString py_bst();
    static QString js_bst();

    static QString c_rbt();
    static QString cpp_rbt();
    static QString java_rbt();
    static QString py_rbt();
    static QString js_rbt();
};
