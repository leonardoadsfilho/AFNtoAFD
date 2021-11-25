#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <list>
#include <stdlib.h>
#include "rapidxml.hpp"
#include "XML_READ.h"
#include "AFN_TO_AFD.h"

int main(void)
{
    char file_name[20] = "Teste.jff";
    list<Edge> afn_edges;
    Read_file(file_name, &afn_edges);

    list<AFD_Edge> afd_edges;
    AFN_to_AFD(&afn_edges, &afd_edges);
}


