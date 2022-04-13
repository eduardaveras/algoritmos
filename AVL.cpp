//submissao com os deletes certos! :)
#include <iostream>

using namespace std;

class Root {
    public:
        int valor;
        int indiceBalan;
        bool avl;
        bool check;
        Root* left;
        Root* right; 

        Root* rotate_left(){
            Root* novaRaiz=new Root;
            Root* RL=new Root;
            novaRaiz=this->right;
            RL=novaRaiz->left;
            novaRaiz->left=this;
            this->right=RL;

            return novaRaiz;
        } 

        Root* rotate_right(){
            Root* novaRaiz=new Root;
            Root* RR=new Root;
            novaRaiz=this->left;
            RR=novaRaiz->right;
            novaRaiz->right=this;
            this->left=RR;

            return novaRaiz;
        } 
};

int altura (Root* raiz){
    int hL=0, hR=0;
    if(raiz==NULL) 
        return 0; // altura da árvore vazia
    else{
        hL=altura (raiz->left);
        hR=altura (raiz->right);
        if(hL<hR) return hR+1;
        else return hL+1;
    }
}

//inserir os nos na arvore:
Root* insert(Root* raiz, int val){
    if(raiz==NULL){ //se minha raiz for nula tenho uma arvore vazia
        Root* novaRaiz=new Root;
        novaRaiz->valor=val;
        novaRaiz->left=NULL;
        novaRaiz->right=NULL;
        novaRaiz->indiceBalan=0;
        novaRaiz->avl=true;
        return novaRaiz;
    }
    if(val<raiz->valor){
        raiz->left=insert(raiz->left, val);
        return raiz; //como nao inseri na raiz, ela n ira mudar (posso retornar a msm raiz)
    }
    else {
        raiz->right=insert(raiz->right, val);
        return raiz;
    }
}

Root* etapa1(Root* root, int* countL){
    Root* aux=NULL;

    while(root->right!=NULL){
        root=root->rotate_left();
        *countL=*countL+1;
    }    

    if(root->right==NULL){
        if(root->left==NULL)
            return root; //se os dois ponteiros forem null significa q cheguei no final da arvore!
        aux=root->left;
        aux=etapa1(aux, countL);
        root->left=aux; //conectando o no q eu girei de volta na arvore
    }
    
    return root;
}

Root* etapa2(Root* raiz_S, Root* raiz_T, int* countL, int* countR){
    Root* auxS=NULL;
    Root* auxT=NULL;

    if(raiz_S==NULL)
        return raiz_S;

    while(raiz_S->valor!=raiz_T->valor){
        if(raiz_S->valor<raiz_T->valor){
            raiz_S=raiz_S->rotate_left();
            *countL=*countL+1;
        }
        else {
            raiz_S=raiz_S->rotate_right();
            *countR=*countR+1;
        }     
    }
    
    //indo p arvore da esquerda
    auxS=raiz_S->left;
    auxT=raiz_T->left;
    auxS=etapa2(auxS, auxT, countL, countR);
    raiz_S->left=auxS;
    
    //indo p arvore da direita
    auxS=raiz_S->right;
    auxT=raiz_T->right;
    auxS=etapa2(auxS, auxT, countL, countR);
    raiz_S->right=auxS;

    return raiz_S;
}

void pos_ordem(Root* raiz_T, bool* avl_check, bool* check, int x){

    if(raiz_T==NULL)
        return ;
    pos_ordem(raiz_T->left, &raiz_T->left->avl, check, x);
    pos_ordem(raiz_T->right, &raiz_T->right->avl, check, x);
    //visitando :
    raiz_T->indiceBalan=altura(raiz_T->right)-altura(raiz_T->left);
    if(raiz_T->indiceBalan==-1 || raiz_T->indiceBalan==0 || raiz_T->indiceBalan==1) raiz_T->avl=true;
    else raiz_T->avl=false;
    cout << raiz_T->valor;
    if(raiz_T->valor!=x) cout << " ";
    if(*avl_check==false) *check=false;
}

void delete_tree(Root* raiz){
    if(raiz->left!=NULL) delete_tree(raiz->left);
    if(raiz->right!=NULL) delete_tree(raiz->right);

    delete(raiz);
}


int main(){
    int tam;
    while(cin >> tam){
        Root* raiz_S=NULL;
        Root* raiz_T=NULL;

        int countL=0, countR=0;
        int valores_S[tam];
        int valores_T[tam];
        for(int i=0; i<tam; i++){
            cin >> valores_S[i];
            raiz_S=insert(raiz_S, valores_S[i]);    
        }
        for(int i=0; i<tam; i++){
            cin >> valores_T[i];
            raiz_T=insert(raiz_T, valores_T[i]);
        }

        //-----------------PARTE 1-----------------
        raiz_S=etapa1(raiz_S, &countL);  

        //-----------------PARTE 2-----------------
        raiz_S=etapa2(raiz_S, raiz_T, &countL, &countR);
        cout << countL << " " << countR << endl;

        //printando a arvore t em pos ordem:
        bool* check=new bool;
        *check=raiz_T->avl;
        pos_ordem(raiz_T, &raiz_T->avl, check, valores_T[0]);
        cout << endl;

        if(*check==true) cout << "true" << endl;
        else cout << "false" << endl;

        cout << endl;

        delete_tree(raiz_S);
        delete_tree(raiz_T);

    }
    
    return 0;

}
