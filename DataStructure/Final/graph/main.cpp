#include <iostream>
#define MAXVERTEXNUM 100
#define INFINITY 100
using namespace std ;


struct Graph
{
    int VertexNum ;
    char Vertex[MAXVERTEXNUM] ;
    int AdjMatrix[MAXVERTEXNUM][MAXVERTEXNUM] ;
} ;

Graph MGraph ;



void CreateGraph (Graph *G)
{
    int i, j ;

    cin >> G-> VertexNum ;
    for (i = 1 ; i <= G->VertexNum ; i++)
    {
        cin >> G->Vertex[i] ;
    }
    for (i = 1 ; i <= G->VertexNum ; i++)
    {
        for (j = 1 ; j <= G->VertexNum ; j++)
        {
            cin >> G->AdjMatrix[i][j] ;
            if (G->AdjMatrix[i][j] == -1)
            {
                G->AdjMatrix[i][j] = INFINITY ;
            }
        }
    }
}


void ShowGraph (Graph *G)
{
    int i, j ;

    for (i = 1 ; i <= G->VertexNum ; i++)
    {
        cout << G->Vertex[i] << " " ;
    }
    cout << endl ;

    for (i = 1 ; i <= G->VertexNum ; i++)
    {
        for (j = 1 ; j <= G->VertexNum ; j++)
        {
            cout << G->AdjMatrix[i][j] << " " ;
        }
        cout << endl ;
    }
}

char Path[MAXVERTEXNUM][MAXVERTEXNUM] ;
int Dest[MAXVERTEXNUM] ;
void ShortestPath (Graph *G, char StartVexChar)
{
    int i, j, m, StartVex, CurrentVex, MinDest ;
    int Final[MAXVERTEXNUM] ;


    for (i = 1 ; j <= G->VertexNum ; i++)
    {
        if (G->Vertex[i] == StartVexChar)
        {
            StartVex = i ;
            break ;
        }
    }

    for (i = 1 ; i <= G->VertexNum ; i++)
    {
        Path[i][0] = 0 ;
        Dest[i] = INFINITY ;
        if (G->AdjMatrix[StartVex][i] < INFINITY)
        {
            Dest[i] = G->AdjMatrix[StartVex][i] ;
            Path[i][1] = G->Vertex[StartVex] ;
            Path[i][2] = G->Vertex[i] ;
            Path[i][0] = 2 ;
        }
        Final[i] = 'F' ;
    }
    Dest[StartVex] = 0 ;
    Final[StartVex] = 'T' ;
    for (i = 1 ; i <= G->VertexNum ; i++)
    {
        MinDest = INFINITY ;
        for (j = 1 ; j <= G->VertexNum ; j++)
        {
            if (Final[j] == 'F')
            {
                if (Dest[j] < MinDest)
                {
                    CurrentVex = j ;
                    MinDest = Dest[j] ;
                }
            }
        }
        Final[CurrentVex] = 'T' ;
        for (j = 1 ; j <= G->VertexNum ; j++)
        {
            if ((Final[j] == 'F') && (MinDest + G->AdjMatrix[CurrentVex][j] < Dest[j]))
            {
                Dest[j] = MinDest + G->AdjMatrix[CurrentVex][j] ;

                for (m = 0 ; m <= Path[CurrentVex][0] ; m++)
                {
                    Path[j][m] = Path[CurrentVex][m] ;
                }
                Path[j][0]++ ;
                Path[j][Path[j][0]] = G->Vertex[j] ;
            }
        }
    }
}

void ShowPath (Graph *G)
{
    int i, j ;

    for (i = 1 ; i <= G->VertexNum ; i++)
    {
        cout << G->Vertex[i] << "(" << Dest[i] << ") : " ;
        if (Path[i][0] > 0)
        {
            for (j = 1 ; j < G->VertexNum ; j++)
            {
                cout << " " << Path[i][j] ;
            }
        }
        cout << endl ;
    }
}

int main ()
{
    char StartVex ;

    CreateGraph (&MGraph) ;
    ShowGraph (&MGraph) ;

    cin >> StartVex ;
    ShortestPath (&MGraph, StartVex) ;
    ShowPath (&MGraph) ;

    return 0 ;

}
