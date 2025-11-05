/** @author Guilherme Martinelli Taglietti
 *  @file   code_exporter.cpp
 *  @brief  Snippet exporter implementation
 */
#include "code_exporter.h"
#include <QObject>

QStringList CodeExporter::languages_pt(){
    // Nomes de linguagens permanecem universais; pode traduzir se quiser usando QObject::tr
    return {"C", "C++", "Java", "Python", "JavaScript"};
}

QString CodeExporter::language_ext(const QString& lang_pt){
    if(lang_pt=="C") return "c";
    if(lang_pt=="C++") return "cpp";
    if(lang_pt=="Java") return "java";
    if(lang_pt=="Python") return "py";
    return "js";
}

QString CodeExporter::snippet_for(const QString& s,const QString& l){
    // Comparar com títulos traduzidos; isso permite funcionar em pt-BR, en-US, es-ES.
    if(s==QObject::tr("Pilha")){
        if(l=="C") return c_stack();
        if(l=="C++") return cpp_stack();
        if(l=="Java") return java_stack();
        if(l=="Python") return py_stack();
        return js_stack();
    }

    if(s==QObject::tr("Fila")){
        if(l=="C") return c_queue();
        if(l=="C++") return cpp_queue();
        if(l=="Java") return java_queue();
        if(l=="Python") return py_queue();
        return js_queue();
    }

    if(s==QObject::tr("Tabela Hash")){
        if(l=="C") return c_hash();
        if(l=="C++") return cpp_hash();
        if(l=="Java") return java_hash();
        if(l=="Python") return py_hash();
        return js_hash();
    }

    if(s==QObject::tr("Lista Encadeada")){
        if(l=="C") return c_sll();
        if(l=="C++") return cpp_sll();
        if(l=="Java") return java_sll();
        if(l=="Python") return py_sll();
        return js_sll();
    }

    if(s==QObject::tr("Lista Duplamente Encadeada")){
        if(l=="C") return c_dll();
        if(l=="C++") return cpp_dll();
        if(l=="Java") return java_dll();
        if(l=="Python") return py_dll();
        return js_dll();
    }

    if(s==QObject::tr("Grafo")){
        if(l=="C") return c_graph();
        if(l=="C++") return cpp_graph();
        if(l=="Java") return java_graph();
        if(l=="Python") return py_graph();
        return js_graph();
    }

    if(s==QObject::tr("Árvore BST")){
        if(l=="C") return c_bst();
        if(l=="C++") return cpp_bst();
        if(l=="Java") return java_bst();
        if(l=="Python") return py_bst();
        return js_bst();
    }

    if(s==QObject::tr("Árvore Vermelho-Preto")){
        if(l=="C") return c_rbt();
        if(l=="C++") return cpp_rbt();
        if(l=="Java") return java_rbt();
        if(l=="Python") return py_rbt();
        return js_rbt();
    }

    return "";
}

static QString s_join(std::initializer_list<const char*> lines){
    QString out;
    for(auto *l : lines) out += QString(l)+"\n";
    return out;
}

QString CodeExporter::c_stack(){
    return s_join({
        "#include <stdio.h>",
        "#define N 16",
        "typedef struct{int a[N];int top;} Stack;",
        "void push(Stack* s,int v){if(s->top<N) s->a[s->top++]=v;}",
        "int pop(Stack* s){return s->top? s->a[--s->top]:-1;}",
        "int main(){Stack s={.top=0};push(&s,10);push(&s,20);printf(\"%d\\n\",pop(&s));return 0;}"
    });
}

QString CodeExporter::cpp_stack(){
    return s_join({
        "#include <iostream>",
        "#include <vector>",
        "struct Stack{std::vector<int> a;void push(int v){a.push_back(v);}int pop(){if(a.empty()) return -1;int v=a.back();a.pop_back();return v;}};",
        "int main(){Stack s;s.push(10);s.push(20);std::cout<<s.pop()<<\"\\n\";}"
    });
}

QString CodeExporter::java_stack(){
    return s_join({
        "import java.util.*;",
        "class Main{",
        "static class Stack{ArrayDeque<Integer> d=new ArrayDeque<>();void push(int v){d.push(v);}int pop(){return d.isEmpty()? -1:d.pop();}}",
        "public static void main(String[] a){Stack s=new Stack();s.push(10);s.push(20);System.out.println(s.pop());}}"
    });
}

QString CodeExporter::py_stack(){
    return s_join({
        "class Stack:",
        "    def __init__(self): self.a=[]",
        "    def push(self,v): self.a.append(v)",
        "    def pop(self): return self.a.pop() if self.a else -1",
        "s=Stack();s.push(10);s.push(20);print(s.pop())"
    });
}

QString CodeExporter::js_stack(){
    return s_join({
        "class Stack{constructor(){this.a=[]} push(v){this.a.push(v)} pop(){return this.a.length?this.a.pop():-1}}",
        "const s=new Stack();s.push(10);s.push(20);console.log(s.pop());"
    });
}

QString CodeExporter::c_queue(){
    return s_join({
        "#include <stdio.h>",
        "#define N 16",
        "typedef struct{int a[N];int h,t;} Queue;",
        "void enq(Queue* q,int v){if((q->t+1)%N!=q->h){q->a[q->t]=v;q->t=(q->t+1)%N;}}",
        "int deq(Queue* q){if(q->h==q->t) return -1;int v=q->a[q->h];q->h=(q->h+1)%N;return v;}",
        "int main(){Queue q={0};enq(&q,1);enq(&q,2);printf(\"%d\\n\",deq(&q));}"
    });
}

QString CodeExporter::cpp_queue(){
    return s_join({
        "#include <iostream>",
        "#include <deque>",
        "struct Queue{std::deque<int> d;void enq(int v){d.push_back(v);}int deq(){if(d.empty()) return -1;int v=d.front();d.pop_front();return v;}};",
        "int main(){Queue q;q.enq(1);q.enq(2);std::cout<<q.deq()<<\"\\n\";}"
    });
}

QString CodeExporter::java_queue(){
    return s_join({
        "import java.util.*;",
        "class Main{",
        "static class Queue{ArrayDeque<Integer> d=new ArrayDeque<>();void enq(int v){d.add(v);}int deq(){return d.isEmpty()? -1:d.remove();}}",
        "public static void main(String[] a){Queue q=new Queue();q.enq(1);q.enq(2);System.out.println(q.deq());}}"
    });
}

QString CodeExporter::py_queue(){
    return s_join({
        "from collections import deque",
        "class Queue:",
        "    def __init__(self): self.d=deque()",
        "    def enq(self,v): self.d.append(v)",
        "    def deq(self): return self.d.popleft() if self.d else -1",
        "q=Queue();q.enq(1);q.enq(2);print(q.deq())"
    });
}

QString CodeExporter::js_queue(){
    return s_join({
        "class Queue{constructor(){this.a=[]} enq(v){this.a.push(v)} deq(){return this.a.length?this.a.shift():-1}}",
        "const q=new Queue();q.enq(1);q.enq(2);console.log(q.deq());"
    });
}

QString CodeExporter::c_hash(){
    return s_join({
        "#include <stdio.h>",
        "#include <string.h>",
        "#define N 17",
        "typedef struct{int used;int k;int v;} Item;",
        "Item t[N];",
        "int h(int k){return (k%N+N)%N;}",
        "void put(int k,int v){int i=h(k);for(int c=0;c<N;c++,i=(i+1)%N) if(!t[i].used||t[i].k==k){t[i].used=1;t[i].k=k;t[i].v=v;return;}}",
        "int get(int k){int i=h(k);for(int c=0;c<N;c++,i=(i+1)%N) if(t[i].used&&t[i].k==k) return t[i].v;return -1;}",
        "int main(){put(10,42);printf(\"%d\\n\",get(10));}"
    });
}

QString CodeExporter::cpp_hash(){
    return s_join({
        "#include <iostream>",
        "#include <optional>",
        "#include <vector>",
        "struct Item{bool used=false;int k=0;int v=0;};",
        "struct Hash{std::vector<Item> t;Hash(size_t n):t(n){}int H(int k){return (k%(int)t.size()+(int)t.size())%(int)t.size();}void put(int k,int v){int i=H(k);for(size_t c=0;c<t.size();c++,i=(i+1)%t.size()) if(!t[i].used||t[i].k==k){t[i].used=true;t[i].k=k;t[i].v=v;return;}}int get(int k){int i=H(k);for(size_t c=0;c<t.size();c++,i=(i+1)%t.size()) if(t[i].used&&t[i].k==k) return t[i].v;return -1;}};",
        "int main(){Hash h(17);h.put(10,42);std::cout<<h.get(10)<<\"\\n\";}"
    });
}

QString CodeExporter::java_hash(){
    return s_join({
        "import java.util.*;",
        "class Main{",
        "static class Hash{static class It{boolean used;int k;int v;}It[] t;Hash(int n){t=new It[n];for(int i=0;i<n;i++) t[i]=new It();}int H(int k){int n=t.length;int r=k%n;return r<0?r+n:r;}void put(int k,int v){int i=H(k);for(int c=0;c<t.length;c++,i=(i+1)%t.length) if(!t[i].used||t[i].k==k){t[i].used=true;t[i].k=k;t[i].v=v;return;}}int get(int k){int i=H(k);for(int c=0;c<t.length;c++,i=(i+1)%t.length) if(t[i].used&&t[i].k==k) return t[i].v;return -1;}}",
        "public static void main(String[] a){Hash h=new Hash(17);h.put(10,42);System.out.println(h.get(10));}}"
    });
}

QString CodeExporter::py_hash(){
    return s_join({
        "class Hash:",
        "    class It:",
        "        def __init__(self): self.used=False;self.k=0;self.v=0",
        "    def __init__(self,n=17): self.t=[Hash.It() for _ in range(n)]",
        "    def H(self,k): n=len(self.t); r=k%n; return r+n if r<0 else r",
        "    def put(self,k,v): i=self.H(k);",
        "        \n        \n",
        "        c=0",
        "        while c<len(self.t):",
        "            if not self.t[i].used or self.t[i].k==k: self.t[i].used=True; self.t[i].k=k; self.t[i].v=v; return",
        "            i=(i+1)%len(self.t); c+=1",
        "    def get(self,k): i=self.H(k); c=0",
        "        \n        \n",
        "        while c<len(self.t):",
        "            if self.t[i].used and self.t[i].k==k: return self.t[i].v",
        "            i=(i+1)%len(self.t); c+=1",
        "        return -1",
        "h=Hash();h.put(10,42);print(h.get(10))"
    });
}

QString CodeExporter::js_hash(){
    return s_join({
        "class Hash{constructor(n=17){this.t=Array.from({length:n},()=>({used:false,k:0,v:0}))} H(k){const n=this.t.length;let r=k%n;return r<0?r+n:r} put(k,v){let i=this.H(k);for(let c=0;c<this.t.length;c++,i=(i+1)%this.t.length){if(!this.t[i].used||this.t[i].k===k){this.t[i]={used:true,k,v};return}}} get(k){let i=this.H(k);for(let c=0;c<this.t.length;c++,i=(i+1)%this.t.length){const it=this.t[i];if(it.used&&it.k===k)return it.v}return -1}}",
        "const h=new Hash();h.put(10,42);console.log(h.get(10));"
    });
}

QString CodeExporter::c_sll(){
    return s_join({
        "#include <stdio.h>",
        "#include <stdlib.h>",
        "typedef struct Node{int v;struct Node* next;} Node;",
        "Node* push_front(Node* head,int v){Node* n=malloc(sizeof(Node));n->v=v;n->next=head;return n;}",
        "Node* pop_front(Node* head){if(!head) return NULL;Node* n=head->next;free(head);return n;}",
        "int main(){Node* h=NULL;h=push_front(h,3);h=push_front(h,2);h=push_front(h,1);while(h){printf(\"%d \",h->v);h=pop_front(h);}return 0;}"
    });
}

QString CodeExporter::cpp_sll(){
    return s_join({
        "#include <iostream>",
        "struct Node{int v;Node* next;};",
        "Node* push_front(Node* h,int v){auto* n=new Node{v,h};return n;}",
        "Node* pop_front(Node* h){if(!h) return nullptr;auto* n=h->next;delete h;return n;}",
        "int main(){Node* h=nullptr;h=push_front(h,3);h=push_front(h,2);h=push_front(h,1);while(h){std::cout<<h->v<<\" \";h=pop_front(h);} }"
    });
}

QString CodeExporter::java_sll(){
    return s_join({
        "class Main{",
        "static class Node{int v;Node next;Node(int v,Node n){this.v=v;this.next=n;}}",
        "static Node pushFront(Node h,int v){return new Node(v,h);}",
        "static Node popFront(Node h){return h==null?null:h.next;}",
        "public static void main(String[] a){Node h=null;h=pushFront(h,3);h=pushFront(h,2);h=pushFront(h,1);for(Node n=h;n!=null;n=n.next) System.out.print(n.v+\" \");}}"
    });
}

QString CodeExporter::py_sll(){
    return s_join({
        "class Node:",
        "    def __init__(self,v,n=None): self.v=v; self.next=n",
        "def push_front(h,v): return Node(v,h)",
        "def pop_front(h): return h.next if h else None",
        "h=None",
        "h=push_front(h,3);h=push_front(h,2);h=push_front(h,1)",
        "n=h",
        "while n: print(n.v,end=' '); n=n.next"
    });
}

QString CodeExporter::js_sll(){
    return s_join({
        "class Node{constructor(v,n=null){this.v=v;this.next=n}}",
        "const pushFront=(h,v)=>new Node(v,h)",
        "const popFront=h=>h? h.next:null",
        "let h=null;h=pushFront(h,3);h=pushFront(h,2);h=pushFront(h,1);for(let n=h;n;n=n.next) process.stdout.write(n.v+' ');"
    });
}

QString CodeExporter::c_dll(){
    return s_join({
        "#include <stdio.h>",
        "#include <stdlib.h>",
        "typedef struct Node{int v;struct Node* prev;struct Node* next;} Node;",
        "Node* push_front(Node* h,int v){Node* n=malloc(sizeof(Node));n->v=v;n->prev=NULL;n->next=h;if(h) h->prev=n;return n;}",
        "Node* pop_back(Node* h){if(!h) return NULL;Node* t=h;while(t->next) t=t->next;Node* p=t->prev;if(p) p->next=NULL;free(t);return p? h:NULL;}",
        "int main(){Node* h=NULL;h=push_front(h,2);h=push_front(h,1);h=pop_back(h);for(Node* n=h;n;n=n->next) printf(\"%d \",n->v);}"
    });
}

QString CodeExporter::cpp_dll(){
    return s_join({
        "#include <iostream>",
        "struct Node{int v;Node* prev;Node* next;};",
        "Node* push_front(Node* h,int v){auto* n=new Node{v,nullptr,h};if(h) h->prev=n;return n;}",
        "Node* pop_back(Node* h){if(!h) return nullptr;Node* t=h;while(t->next) t=t->next;Node* p=t->prev;if(p) p->next=nullptr;delete t;return p? h:nullptr;}",
        "int main(){Node* h=nullptr;h=push_front(h,2);h=push_front(h,1);h=pop_back(h);for(Node* n=h;n;n=n->next) std::cout<<n->v<<\" \";}"
    });
}

QString CodeExporter::java_dll(){
    return s_join({
        "class Main{",
        "static class Node{int v;Node prev;Node next;Node(int v){this.v=v;}}",
        "static Node pushFront(Node h,int v){Node n=new Node(v);n.next=h;if(h!=null) h.prev=n;return n;}",
        "static Node popBack(Node h){if(h==null) return null;Node t=h;while(t.next!=null) t=t.next;Node p=t.prev;if(p!=null) p.next=null;return p!=null?h:null;}",
        "public static void main(String[] a){Node h=null;h=pushFront(h,2);h=pushFront(h,1);h=popBack(h);for(Node n=h;n!=null;n=n.next) System.out.print(n.v+\" \");}}"
    });
}

QString CodeExporter::py_dll(){
    return s_join({
        "class Node:",
        "    def __init__(self,v): self.v=v; self.prev=None; self.next=None",
        "def push_front(h,v): n=Node(v); n.next=h; h.prev=n if h else None; return n",
        "def pop_back(h):",
        "    if not h: return None",
        "    t=h",
        "    while t.next: t=t.next",
        "    p=t.prev",
        "    if p: p.next=None",
        "    return h if p else None",
        "h=None",
        "h=push_front(h,2);h=push_front(h,1);h=pop_back(h)",
        "n=h",
        "while n: print(n.v,end=' '); n=n.next"
    });
}

QString CodeExporter::js_dll(){
    return s_join({
        "class Node{constructor(v){this.v=v;this.prev=null;this.next=null}}",
        "const pushFront=(h,v)=>{const n=new Node(v);n.next=h;if(h)h.prev=n;return n}",
        "const popBack=h=>{if(!h)return null;let t=h;while(t.next)t=t.next;const p=t.prev;if(p)p.next=null;return p?h:null}",
        "let h=null;h=pushFront(h,2);h=pushFront(h,1);h=popBack(h);for(let n=h;n;n=n.next) process.stdout.write(n.v+' ');"
    });
}

QString CodeExporter::c_graph(){
    return s_join({
        "#include <stdio.h>",
        "#include <string.h>",
        "#define N 8",
        "int g[N][N];int q[64],h,t;",
        "void add(int u,int v){g[u][v]=g[v][u]=1;}",
        "void bfs(int s){int vis[N]={0};h=t=0;q[t++]=s;vis[s]=1;while(h<t){int u=q[h++];printf(\"%d \",u);for(int v=0;v<N;v++) if(g[u][v]&&!vis[v]){vis[v]=1;q[t++]=v;}}}",
        "int main(){add(0,1);add(1,2);add(2,3);bfs(0);}"
    });
}

QString CodeExporter::cpp_graph(){
    return s_join({
        "#include <bits/stdc++.h>",
        "int main(){int n=5;std::vector<std::vector<int>> g(n);auto add=[&](int u,int v){g[u].push_back(v);g[v].push_back(u);};add(0,1);add(1,2);add(2,3);std::queue<int> q;std::vector<int> vis(n);q.push(0);vis[0]=1;while(q.size()){int u=q.front();q.pop();std::cout<<u<<\" \";for(int v:g[u]) if(!vis[v]) vis[v]=1,q.push(v);} }"
    });
}

QString CodeExporter::java_graph(){
    return s_join({
        "import java.util.*;",
        "class Main{",
        "public static void main(String[] a){int n=5;List<List<Integer>> g=new ArrayList<>();for(int i=0;i<n;i++) g.add(new ArrayList<>());",
        "g.get(0).add(1);g.get(1).add(0);g.get(1).add(2);g.get(2).add(1);g.get(2).add(3);g.get(3).add(2);",
        "Queue<Integer> q=new ArrayDeque<>();boolean[] vis=new boolean[n];q.add(0);vis[0]=true;while(!q.isEmpty()){int u=q.remove();System.out.print(u+\" \");for(int v:g.get(u)) if(!vis[v]){vis[v]=true;q.add(v);}}}}"
    });
}

QString CodeExporter::py_graph(){
    return s_join({
        "from collections import deque",
        "n=5; g=[[] for _ in range(n)]",
        "def add(u,v): g[u].append(v); g[v].append(u)",
        "add(0,1); add(1,2); add(2,3)",
        "q=deque([0]); vis=[False]*n; vis[0]=True",
        "while q:",
        "    u=q.popleft(); print(u,end=' ')",
        "    for v in g[u]:",
        "        if not vis[v]: vis[v]=True; q.append(v)"
    });
}

QString CodeExporter::js_graph(){
    return s_join({
        "const n=5, g=Array.from({length:n},()=>[])",
        "const add=(u,v)=>{g[u].push(v);g[v].push(u)}",
        "add(0,1);add(1,2);add(2,3)",
        "const q=[0];const vis=Array(n).fill(false);vis[0]=true",
        "while(q.length){const u=q.shift();process.stdout.write(u+' ');for(const v of g[u]) if(!vis[v]){vis[v]=true;q.push(v)}}"
    });
}

QString CodeExporter::c_bst(){
    return s_join({
        "#include <stdio.h>",
        "#include <stdlib.h>",
        "typedef struct Node{int v;struct Node* l;struct Node* r;} Node;",
        "Node* ins(Node* n,int v){if(!n){n=malloc(sizeof(Node));n->v=v;n->l=n->r=NULL;return n;} if(v<n->v) n->l=ins(n->l,v); else if(v>n->v) n->r=ins(n->r,v); return n;}",
        "int find(Node* n,int v){if(!n) return 0; if(v==n->v) return 1; return v<n->v? find(n->l,v):find(n->r,v);}",
        "int main(){Node* r=NULL;r=ins(r,5);ins(r,2);ins(r,7);printf(\"%d\\n\",find(r,7));}"
    });
}

QString CodeExporter::cpp_bst(){
    return s_join({
        "#include <iostream>",
        "struct Node{int v;Node* l;Node* r;};",
        "Node* ins(Node* n,int v){if(!n) return new Node{v,nullptr,nullptr}; if(v<n->v) n->l=ins(n->l,v); else if(v>n->v) n->r=ins(n->r,v); return n;}",
        "bool find(Node* n,int v){if(!n) return false; if(v==n->v) return true; return v<n->v? find(n->l,v):find(n->r,v);}",
        "int main(){Node* r=nullptr;r=ins(r,5);ins(r,2);ins(r,7);std::cout<<find(r,7)<<\"\\n\";}"
    });
}
QString CodeExporter::java_bst(){
    return s_join({
        "class Main{",
        "static class Node{int v;Node l,r;Node(int v){this.v=v;}}",
        "static Node ins(Node n,int v){if(n==null) return new Node(v); if(v<n.v) n.l=ins(n.l,v); else if(v>n.v) n.r=ins(n.r,v); return n;}",
        "static boolean find(Node n,int v){if(n==null) return false; if(v==n.v) return true; return v<n.v? find(n.l,v):find(n.r,v);}",
        "public static void main(String[] a){Node r=null;r=ins(r,5);ins(r,2);ins(r,7);System.out.println(find(r,7));}}"
    });
}

QString CodeExporter::py_bst(){
    return s_join({
        "class Node:",
        "    def __init__(self,v): self.v=v; self.l=None; self.r=None",
        "def ins(n,v):",
        "    if not n: return Node(v)",
        "    if v<n.v: n.l=ins(n.l,v)",
        "    elif v>n.v: n.r=ins(n.r,v)",
        "    return n",
        "def find(n,v):",
        "    if not n: return False",
        "    if v==n.v: return True",
        "    return find(n.l,v) if v<n.v else find(n.r,v)",
        "r=None",
        "r=ins(r,5); ins(r,2); ins(r,7)",
        "print(find(r,7))"
    });
}

QString CodeExporter::js_bst(){
    return s_join({
        "class Node{constructor(v){this.v=v;this.l=null;this.r=null}}",
        "const ins=(n,v)=>{if(!n) return new Node(v); if(v<n.v) n.l=ins(n.l,v); else if(v>n.v) n.r=ins(n.r,v); return n}",
        "const find=(n,v)=>!n?false:(v===n.v?true:(v<n.v?find(n.l,v):find(n.r,v)))",
        "let r=null;r=ins(r,5);ins(r,2);ins(r,7);console.log(find(r,7))"
    });
}

QString CodeExporter::c_rbt(){
    return s_join({
        "#include <stdio.h>",
        "#include <stdlib.h>",
        "typedef enum{RED,BLACK} Color;",
        "typedef struct Node{int v;Color c;struct Node* l;struct Node* r;struct Node* p;} Node;",
        "Node* root=NULL;",
        "Node* nn(int v){Node* n=malloc(sizeof(Node));n->v=v;n->c=RED;n->l=n->r=n->p=NULL;return n;}",
        "void rot_l(Node* x){Node* y=x->r;x->r=y->l;if(y->l) y->l->p=x;y->p=x->p;if(!x->p) root=y;else if(x==x->p->l) x->p->l=y; else x->p->r=y;y->l=x;x->p=y;}",
        "void rot_r(Node* x){Node* y=x->l;x->l=y->r;if(y->r) y->r->p=x;y->p=x->p;if(!x->p) root=y;else if(x==x->p->r) x->p->r=y; else x->p->l=y;y->r=x;x->p=y;}",
        "void fix(Node* z){while(z->p&&z->p->c==RED){if(z->p==z->p->p->l){Node* y=z->p->p->r;if(y&&y->c==RED){z->p->c=BLACK;y->c=BLACK;z->p->p->c=RED;z=z->p->p;}else{if(z==z->p->r){z=z->p;rot_l(z);}z->p->c=BLACK;z->p->p->c=RED;rot_r(z->p->p);}}else{Node* y=z->p->p->l;if(y&&y->c==RED){z->p->c=BLACK;y->c=BLACK;z->p->p->c=RED;z=z->p->p;}else{if(z==z->p->l){z=z->p;rot_r(z);}z->p->c=BLACK;z->p->p->c=RED;rot_l(z->p->p);}}}root->c=BLACK;}",
        "void insert(int v){Node* z=nn(v);Node* y=NULL;Node* x=root;while(x){y=x;if(z->v<x->v) x=x->l; else x=x->r;}z->p=y;if(!y) root=z; else if(z->v<y->v) y->l=z; else y->r=z;fix(z);}",
        "int find(Node* n,int v){if(!n) return 0; if(v==n->v) return 1; return v<n->v? find(n->l,v):find(n->r,v);}",
        "int main(){insert(10);insert(5);insert(15);insert(7);printf(\"%d\\n\",find(root,7));}"
    });
}

QString CodeExporter::cpp_rbt(){
    return s_join({
        "#include <bits/stdc++.h>",
        "enum Color{RED,BLACK};",
        "struct Node{int v;Color c;Node* l;Node* r;Node* p;};",
        "Node* root=nullptr;",
        "Node* nn(int v){return new Node{v,RED,nullptr,nullptr,nullptr};}",
        "void rot_l(Node* x){Node* y=x->r;x->r=y->l;if(y->l) y->l->p=x;y->p=x->p;if(!x->p) root=y;else if(x==x->p->l) x->p->l=y; else x->p->r=y;y->l=x;x->p=y;}",
        "void rot_r(Node* x){Node* y=x->l;x->l=y->r;if(y->r) y->r->p=x;y->p=x->p;if(!x->p) root=y;else if(x==x->p->r) x->p->r=y; else x->p->l=y;y->r=x;x->p=y;}",
        "void fix(Node* z){while(z->p&&z->p->c==RED){if(z->p==z->p->p->l){Node* y=z->p->p->r;if(y&&y->c==RED){z->p->c=BLACK;y->c=BLACK;z->p->p->c=RED;z=z->p->p;}else{if(z==z->p->r){z=z->p;rot_l(z);}z->p->c=BLACK;z->p->p->c=RED;rot_r(z->p->p);}}else{Node* y=z->p->p->l;if(y&&y->c==RED){z->p->c=BLACK;y->c=BLACK;z->p->p->c=RED;z=z->p->p;}else{if(z==z->p->l){z=z->p;rot_r(z);}z->p->c=BLACK;z->p->p->c=RED;rot_l(z->p->p);}}}root->c=BLACK;}",
        "void insert(int v){Node* z=nn(v);Node* y=nullptr;Node* x=root;while(x){y=x;if(z->v<x->v) x=x->l; else x=x->r;}z->p=y;if(!y) root=z; else if(z->v<y->v) y->l=z; else y->r=z;fix(z);}",
        "bool find(Node* n,int v){if(!n) return false; if(v==n->v) return true; return v<n->v? find(n->l,v):find(n->r,v);}",
        "int main(){insert(10);insert(5);insert(15);insert(7);std::cout<<find(root,7)<<\"\\n\";}"
    });
}

QString CodeExporter::java_rbt(){
    return s_join({
        "class Main{",
        "enum Color{RED,BLACK}",
        "static class Node{int v;Color c;Node l,r,p;Node(int v){this.v=v;this.c=Color.RED;}}",
        "static Node root;",
        "static void rotL(Node x){Node y=x.r;x.r=y.l;if(y.l!=null) y.l.p=x;y.p=x.p;if(x.p==null) root=y;else if(x==x.p.l) x.p.l=y; else x.p.r=y;y.l=x;x.p=y;}",
        "static void rotR(Node x){Node y=x.l;x.l=y.r;if(y.r!=null) y.r.p=x;y.p=x.p;if(x.p==null) root=y;else if(x==x.p.r) x.p.r=y; else x.p.l=y;y.r=x;x.p=y;}",
        "static void fix(Node z){while(z.p!=null&&z.p.c==Color.RED){if(z.p==z.p.p.l){Node y=z.p.p.r;if(y!=null&&y.c==Color.RED){z.p.c=Color.BLACK;y.c=Color.BLACK;z.p.p.c=Color.RED;z=z.p.p;}else{if(z==z.p.r){z=z.p;rotL(z);}z.p.c=Color.BLACK;z.p.p.c=Color.RED;rotR(z.p.p);}}else{Node y=z.p.p.l;if(y!=null&&y.c==Color.RED){z.p.c=Color.BLACK;y.c=Color.BLACK;z.p.p.c=Color.RED;z=z.p.p;}else{if(z==z.p.l){z=z.p;rotR(z);}z.p.c=Color.BLACK;z.p.p.c=Color.RED;rotL(z.p.p);}}}root.c=Color.BLACK;}",
        "static void insert(int v){Node z=new Node(v);Node y=null;Node x=root;while(x!=null){y=x;if(z.v<x.v) x=x.l; else x=x.r;}z.p=y;if(y==null) root=z; else if(z.v<y.v) y.l=z; else y.r=z;fix(z);}",
        "static boolean find(Node n,int v){if(n==null) return false; if(v==n.v) return true; return v<n.v? find(n.l,v):find(n.r,v);}",
        "public static void main(String[] a){insert(10);insert(5);insert(15);insert(7);System.out.println(find(root,7));}}"
    });
}

QString CodeExporter::py_rbt(){
    return s_join({
        "RED,BLACK=0,1",
        "class Node:",
        "    def __init__(self,v): self.v=v; self.c=RED; self.l=None; self.r=None; self.p=None",
        "root=None",
        "def rot_l(x):",
        "    global root",
        "    y=x.r; x.r=y.l",
        "    if y.l: y.l.p=x",
        "    y.p=x.p",
        "    if not x.p: root=y",
        "    elif x==x.p.l: x.p.l=y",
        "    else: x.p.r=y",
        "    y.l=x; x.p=y",
        "def rot_r(x):",
        "    global root",
        "    y=x.l; x.l=y.r",
        "    if y.r: y.r.p=x",
        "    y.p=x.p",
        "    if not x.p: root=y",
        "    elif x==x.p.r: x.p.r=y",
        "    else: x.p.l=y",
        "    y.r=x; x.p=y",
        "def fix(z):",
        "    global root",
        "    while z.p and z.p.c==RED:",
        "        if z.p==z.p.p.l:",
        "            y=z.p.p.r",
        "            if y and y.c==RED: z.p.c=BLACK; y.c=BLACK; z.p.p.c=RED; z=z.p.p",
        "            else:",
        "                if z==z.p.r: z=z.p; rot_l(z)",
        "                z.p.c=BLACK; z.p.p.c=RED; rot_r(z.p.p)",
        "        else:",
        "            y=z.p.p.l",
        "            if y and y.c==RED: z.p.c=BLACK; y.c=BLACK; z.p.p.c=RED; z=z.p.p",
        "            else:",
        "                if z==z.p.l: z=z.p; rot_r(z)",
        "                z.p.c=BLACK; z.p.p.c=RED; rot_l(z.p.p)",
        "    root.c=BLACK",
        "def insert(v):",
        "    global root",
        "    z=Node(v); y=None; x=root",
        "    while x:",
        "        y=x",
        "        x=x.l if z.v<x.v else x.r",
        "    z.p=y",
        "    if not y: root=z",
        "    elif z.v<y.v: y.l=z",
        "    else: y.r=z",
        "    fix(z)",
        "def find(n,v):",
        "    if not n: return False",
        "    if v==n.v: return True",
        "    return find(n.l,v) if v<n.v else find(n.r,v)",
        "insert(10); insert(5); insert(15); insert(7)",
        "print(find(root,7))"
    });
}

QString CodeExporter::js_rbt(){
    return s_join({
        "const RED=0,BLACK=1;",
        "class Node{constructor(v){this.v=v;this.c=RED;this.l=null;this.r=null;this.p=null}}",
        "let root=null;",
        "function rotL(x){const y=x.r;x.r=y.l;if(y.l)y.l.p=x;y.p=x.p;if(!x.p)root=y;else if(x===x.p.l)x.p.l=y;else x.p.r=y;y.l=x;x.p=y}",
        "function rotR(x){const y=x.l;x.l=y.r;if(y.r)y.r.p=x;y.p=x.p;if(!x.p)root=y;else if(x===x.p.r)x.p.r=y;else x.p.l=y;y.r=x;x.p=y}",
        "function fix(z){while(z.p&&z.p.c===RED){if(z.p===z.p.p.l){let y=z.p.p.r;if(y&&y.c===RED){z.p.c=BLACK;y.c=BLACK;z.p.p.c=RED;z=z.p.p}else{if(z===z.p.r){z=z.p;rotL(z)}z.p.c=BLACK;z.p.p.c=RED;rotR(z.p.p)}}else{let y=z.p.p.l;if(y&&y.c===RED){z.p.c=BLACK;y.c=BLACK;z.p.p.c=RED;z=z.p.p}else{if(z===z.p.l){z=z.p;rotR(z)}z.p.c=BLACK;z.p.p.c=RED;rotL(z.p.p)}}}root.c=BLACK}",
        "function insert(v){let z=new Node(v),y=null,x=root;while(x){y=x;x=z.v<x.v?x.l:x.r}z.p=y;if(!y)root=z;else if(z.v<y.v)y.l=z;else y.r=z;fix(z)}",
        "function find(n,v){if(!n)return false; if(v===n.v)return true; return v<n.v?find(n.l,v):find(n.r,v)}",
        "insert(10);insert(5);insert(15);insert(7);console.log(find(root,7))"
    });
}
