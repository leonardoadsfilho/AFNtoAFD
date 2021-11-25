#ifndef AFN_TO_AFD_H_INCLUDED
#define AFN_TO_AFD_H_INCLUDED

#include <iostream>
#include <string>
#include <list>
#include "XML_READ.h"

typedef struct AFD_Transition{
    list<char*> to;
    char *read;
}AFD_Transition;

typedef struct AFD_Edge{
    char *id;
    int state_type;
    list<AFD_Transition> transitions;
}AFD_Edge;

void Insert_Transition_Read(list<AFD_Transition> *transitions, char *read, char *to){

    if(transitions->size() == 0){

        AFD_Transition transition;
        char *cpy1 = (char*)calloc(20, sizeof(char));
        char *cpy2 = (char*)calloc(20, sizeof(char));
        strcpy(cpy1, read);
        transition.read = cpy1;
        strcpy(cpy2, to);
        transition.to.push_back(cpy2);
        transitions->push_back(transition);
    }else{

        list<AFD_Transition>::iterator it = transitions->begin();
        while(it != transitions->end()){
            if(strcmp(it->read, read) == 0){
                char *cpy = (char*)calloc(20, sizeof(char));
                strcpy(cpy, to);
                it->to.push_back(cpy);
                return;
            }
            it++;
        }
        
        AFD_Transition transition;
        char *cpy1 = (char*)calloc(20, sizeof(char));
        char *cpy2 = (char*)calloc(20, sizeof(char));
        strcpy(cpy1, read);
        transition.read = cpy1;
        strcpy(cpy2, to);
        transition.to.push_back(cpy2);
        transitions->push_back(transition);
    }
}

AFD_Edge Create_First_Edge(list<Edge> *edges){

    AFD_Edge afd_first_edge;

    list<Edge>::iterator it = edges->begin();
    while(it != edges->end()){

        if(it->state_type == 1 || it->state_type == 3){
            
            char *cpy = (char*)calloc(20, sizeof(char));
            strcpy(cpy, it->id);
            afd_first_edge.id = cpy;
            afd_first_edge.state_type = it->state_type;

            list<Transition>::iterator tit = it->transitions.begin(); 
            while(tit != it->transitions.end()){
                Insert_Transition_Read(&afd_first_edge.transitions, tit->read, tit->to);
                tit++;
            }
            it = edges->end();
        }else{
            it++;
        }
    }

    return afd_first_edge;
}

void Print_AFD(list<AFD_Edge> afd_edges){

    for(list<AFD_Edge>::iterator it = afd_edges.begin(); it != afd_edges.end(); it++){
        printf("\n(%s)-<%d", it->id, it->state_type);
        for(list<AFD_Transition>::iterator tit = it->transitions.begin(); tit != it->transitions.end(); tit++){
            cout << "\n\t[" << tit->read << "] -> ";
            for(list<char*>::iterator cit = tit->to.begin(); cit != tit->to.end(); cit++){
                cout << *cit << ", ";
            }
        }
    }
}

void Insert_id_in_ToList(char *to, char *read, list<AFD_Transition> *transitions){
    if(transitions->size() == 0){
        
        AFD_Transition new_transition;
        char *cpy1 = (char*)calloc(20, sizeof(char));
        char *cpy2 = (char*)calloc(20, sizeof(char));
        strcpy(cpy1, read);
        new_transition.read = cpy1;
        strcpy(cpy2, to);
        new_transition.to.push_back(cpy2);
        transitions->push_back(new_transition);
    }else{

        list<AFD_Transition>::iterator i = transitions->begin();

        while(i != transitions->end()){

            if(strcmp(i->read, read) == 0){
                
                list<char*>::iterator a = i->to.begin();
                while(a != i->to.end()){
                    if(strcmp(*a, to) == 0){
                        return;
                    }
                    a++;
                }
                char *cpy = (char*)calloc(20, sizeof(char));
                strcpy(cpy, to);
                i->to.push_back(cpy);
                return;
            }
            i++;
        }

        AFD_Transition new_transition;
        char *cpy1 = (char*)calloc(20, sizeof(char));
        char *cpy2 = (char*)calloc(20, sizeof(char));
        strcpy(cpy1, read);
        new_transition.read = cpy1;
        strcpy(cpy2, to);
        new_transition.to.push_back(cpy2);
        transitions->push_back(new_transition);
    }
}

void Order_Char_List(list<char*> *to){
    list<char*>::iterator i = to->begin();

    while(i != to->end()){

        list<char*>::iterator small = i;
        list<char*>::iterator j = i;

        while(j != to->end()){
            if(strcmp(*small, *j) > 0){
                small = j;
            }
            j++;
        }

        char cpy[] = "";
        strcpy(cpy, *i);
        strcpy(*i, *small);
        strcpy(*small, cpy);
        
        i++;
    }
}

void Create_New_AFD_Edge(list<Edge> *afn_edges, list<AFD_Edge> *afd_edges, list<char*> to){

    if(to.size() > 0){

        char *afd_edge_id = (char *)calloc(20, sizeof(char));
        afd_edge_id[0] = '\0';
        list<char*>::iterator char_it = to.begin();
        while(char_it != to.end()){
            strcat(afd_edge_id, *char_it);
            char_it++;
        }

        bool id_already_exists = false;
        list<AFD_Edge>::iterator afd_it = afd_edges->begin();
        while(afd_it != afd_edges->end()){
            if(strcmp(afd_it->id, afd_edge_id) == 0){
                id_already_exists = true;
            }
            afd_it++;
        }

        if(!id_already_exists){

            AFD_Edge new_edge;
            new_edge.id = afd_edge_id;
            new_edge.state_type = 0;
            list<AFD_Transition> new_transitions;    

            list<char*>::reverse_iterator i = to.rbegin();
            int cont = 0;
            while(i != to.rend()){
                list<Edge>::iterator j = afn_edges->begin();
                while(j != afn_edges->end()){
                    if(strcmp(*i, j->id) == 0){
                        if(j->state_type == 3 || j->state_type == 2){
                            new_edge.state_type = 2;
                        }
                        list<Transition>::iterator k = j->transitions.begin();
                        while(k != j->transitions.end()){
                            Insert_id_in_ToList(k->to, k->read, &new_transitions);
                            k++;
                        }
                    }            
                    j++;
                }
                i++;    
                cont++;
                if(cont == to.size()){
                    break;
                }               
            }
            new_edge.transitions = new_transitions;
            afd_edges->push_back(new_edge);
            return;
        }
        free(afd_edge_id);
    }
}

void Execute_Conversion(list<Edge> *afn_edges, list<AFD_Edge> *afd_edges){

    list<AFD_Edge>::iterator afd_it = afd_edges->begin();
    while (afd_it != afd_edges->end()){

        if(afd_it->transitions.size() > 0){
            list<AFD_Transition>::iterator afd_tit = afd_it->transitions.begin();
            int cont = 0;
            while(afd_tit != afd_it->transitions.end()){

                Order_Char_List(&afd_tit->to);

                Create_New_AFD_Edge(afn_edges, afd_edges, afd_tit->to);

                afd_tit++;
            }
        }
        afd_it++;
    }
}

void ToList_Back_ID(list<AFD_Edge> *afd_edges){
    list<AFD_Edge>::iterator i = afd_edges->begin();
    while (i != afd_edges->end()){
        
        list<AFD_Transition>::iterator j = i->transitions.begin();
        while(j != i->transitions.end()){
            char *afd_edge_id = (char *)calloc(20, sizeof(char));

            list<char*>::iterator char_it = j->to.begin();
            while(char_it != j->to.end()){
                strcat(afd_edge_id, *char_it);
                char_it++;
            }
            j->to.push_back(afd_edge_id);
            j++;
        }
        i++;
    }    
}

void AFN_to_AFD(list<Edge> *afn_edges, list<AFD_Edge> *afd_edges){

    AFD_Edge afd_edge = Create_First_Edge(afn_edges);
    afd_edges->push_back(afd_edge);

    Execute_Conversion(afn_edges, afd_edges);

    Print_AFD(*afd_edges);

    ToList_Back_ID(afd_edges);
}

#endif // AFN_TO_AFD_H_INCLUDED
