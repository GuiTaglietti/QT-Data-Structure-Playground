/** @author Guilherme Martinelli Taglietti
 *  @file   texts.h
 *  @brief  Translatable texts and titles
 */
#pragma once
#include <QString>
#include <QObject>

inline QString about_stack_pt(){
    return QObject::tr("Pilha (Stack)\n"
                       "Modelo LIFO: o último a entrar é o primeiro a sair. Operações: push, pop e topo.\n"
                       "Uso comum: desfazer/refazer, avaliação de expressões, navegação de telas, chamadas de função.");
}

inline QString about_queue_pt(){
    return QObject::tr("Fila (Queue)\n"
                       "Modelo FIFO: o primeiro a entrar é o primeiro a sair. Operações: enqueue e dequeue.\n"
                       "Uso comum: filas de impressão, processamento em lotes, simulação de atendimentos.");
}

inline QString about_hash_pt(){
    return QObject::tr("Tabela Hash\n"
                       "Mapeia chaves para índices via função de hash. Colisões resolvidas por sondagem linear.\n"
                       "Uso comum: dicionários, cache, contagem de frequência, índice rápido por chave.");
}

inline QString about_sll_pt(){
    return QObject::tr("Lista Encadeada\n"
                       "Cada nó aponta para o próximo. Inserções/remoções O(1) na cabeça. Acesso sequencial.\n"
                       "Uso comum: pilhas/filas simples, tabelas de dispersão por encadeamento.");
}

inline QString about_dll_pt(){
    return QObject::tr("Lista Duplamente Encadeada\n"
                       "Nós apontam para anterior e próximo. Facilita remoções locais. Acesso sequencial.\n"
                       "Uso comum: históricos de navegação, LRU cache, editores de texto.");
}

inline QString about_graph_pt(){
    return QObject::tr("Grafo simples\n"
                       "Representado por listas de adjacência. Suporta inserção de nós e arestas.\n"
                       "Demonstra BFS e DFS com animação de visita. Uso: redes, rotas, recomendações.");
}

inline QString about_bst_pt(){
    return QObject::tr("Árvore Binária de Busca (BST)\n"
                       "Para cada nó, menores à esquerda e maiores à direita. Operações: inserir, buscar, remover básica.\n"
                       "Uso: índices ordenados, dicionários, conjuntos, motores de busca simples.");
}

inline QString about_rbt_pt(){
    return QObject::tr("Árvore Vermelho-Preto\n"
                       "BST balanceada por regras de cores. Altura O(log n) com rotações e recolorações.\n"
                       "Uso: std::map/std::set, bancos de dados, sistemas de arquivos.");
}
