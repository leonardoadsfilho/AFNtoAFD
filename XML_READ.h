#ifndef XML_READ_H_INCLUDED
#define XML_READ_H_INCLUDED
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <list>
#include <stdlib.h>
#include "rapidxml.hpp"

using namespace rapidxml;
using namespace std;

typedef struct Transition{
    char *to;
    char *read;
}Transition;

typedef struct Edge{
    char *id;
    list<Transition> transitions;
    int state_type;
}Edge;

void Add_to_List(char *id, Transition transition, list<Edge> *Edges){

    for (list<Edge>::iterator it = Edges->begin(); it != Edges->end(); it++){
         if(strcmp(id, it->id) == 0){
            it->transitions.push_back(transition);
         }
    }

}

void Read_file(char* file_name, list<Edge> *Edges){
    Transition transition;
    Edge edge;
    char *position;
    int a=0;
    char* name;
	xml_document<> doc;
	xml_node<> * root_node;
	xml_node<> * x_node;
	xml_node<> * y_node;
	xml_node<> * type_node;
	xml_node<> * transition_op_node;
	ifstream theFile (file_name);
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);
	root_node = doc.first_node("structure")->first_node("automaton");

	 for(xml_node<>* state_node = root_node->first_node("state"); state_node; state_node = state_node->next_sibling()){
        name = state_node->name();
        if(strcmp(name,"transition") == 0){
            break;
        }
        printf("ID: %s Name: %s.\n",
        state_node->first_attribute("id")->value(),
        state_node->first_attribute("name")->value());
        x_node = state_node->first_node("x");
        printf("X: %s\n",x_node->value());
        y_node = state_node->first_node("y");
        printf("Y: %s \n",y_node->value());
        type_node = state_node->first_node("initial");
        if(type_node){
            printf("Inicial\n");
        }
        type_node = state_node->first_node("final");
        if(type_node){
            printf("Final\n");
        }
        printf("\n\n");
    }

    for(xml_node<>* state_node = root_node->first_node("state"); state_node; state_node = state_node->next_sibling()){
        edge.state_type = 0;
        name=state_node->name();
        if(strcmp(name,"transition") == 0){
            break;
        }
        state_node->first_attribute("id")->value();
        edge.id = state_node->first_attribute("id")->value();
        type_node = state_node->first_node("initial");
        if(type_node){
            edge.state_type += 1;
        }
        type_node = state_node->first_node("final");
        if(type_node){
            edge.state_type += 2;
        }
        Edges->push_back(edge);
    }

    for(xml_node<>* transition_node = root_node->first_node("transition"); transition_node; transition_node = transition_node->next_sibling()){
                transition_op_node = transition_node->first_node("from");
                printf("De: %s\n", transition_op_node->value());
                position = transition_op_node->value();
                transition_op_node = transition_node->first_node("to");
                printf("Para: %s\n", transition_op_node->value());
                transition.to = transition_op_node->value();
                transition_op_node = transition_node->first_node("read");
                printf("Lendo: %s\n\n", transition_op_node->value());
                transition.read = transition_op_node->value();
                Add_to_List(position,transition,Edges);
    }

}

#endif // XML_READ_H_INCLUDED
