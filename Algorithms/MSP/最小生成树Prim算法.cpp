#include<stdio.h>
#include<stdlib.h>
#define M 20
#define MAX 20
bool flag[MAX];
typedef struct
{
    int adj;
    int weight;
} AdjMatrix[MAX][MAX];
typedef struct
{
    AdjMatrix arc;
    int vexnum, arcnum;
} MGraph;
//函数申明
void CreatGraph(MGraph *);
void prim(MGraph *G);

void CreatGraph(MGraph *G)//构造图
{
    int i, j,n, m;
    char n1,m1;
    printf("请输入点数及边数:");
    scanf("%d %d",&G->vexnum,&G->arcnum);
    for (i = 1; i <= G->vexnum; i++)//初始化图
    {
        for ( j = 1; j <= G->vexnum; j++)
        {
            G->arc[i][j].adj = G->arc[j][i].adj = 0;
        }
    }
    printf("请输入有路连通的2个点及它们之间的权值:\n");
    for ( i = 1; i <= G->arcnum; i++)
    {
        getchar();
        scanf("%c %c",&n1,&m1);
        n=n1-'a'+1;
        m=m1-'a'+1;
        G->arc[n][m].adj = G->arc[m][n].adj = 1;
        scanf(" %d",&G->arc[n][m].weight);
    }
    printf("邻接矩阵为:\n");
    for ( i = 1; i <= G->vexnum; i++)
    {
        for ( j = 1; j <= G->vexnum; j++)
        {
            if(G->arc[i][j].adj==1)
                printf("%d ",G->arc[i][j].weight);
            else
                printf("%d ",G->arc[i][j].adj);
            G->arc[j][i].weight=G->arc[i][j].weight;
        }
        printf("\n");
    }
}

void prim(MGraph *G)//最小生成树
{
    printf("最小生成树为:\n");
    int i,j,k,min,ans=0,t;
    struct
    {
        int adjvex;
        int lowcost;
    } closedge[MAX];
    for(int i=0;i<MAX;i++)
    closedge[i].lowcost=0;
    for (i=2; i<=G->vexnum; i++)
    {
        closedge[i].lowcost=G->arc[1][i].weight;
        closedge[i].adjvex=1;
        flag[i]=false;
    }
    flag[1]=true;
    for (i=1; i<G->vexnum; i++)
    {
        min=100;
        k=1;
        for(j=2; j<=G->vexnum; j++)
            if (!flag[j] && closedge[j].lowcost<min && closedge[j].lowcost!=0)
            {
                min=closedge[j].lowcost;
                k=j;
            }
        flag[k]=true;
        printf("< %c, %c > %d\n", closedge[k].adjvex+'a'-1, k+'a'-1, min);
        ans+=min;
        for (j=1; j<=G->vexnum; j++)
            if (!flag[j])
                if(G->arc[k][j].weight!=0 &&G->arc[k][j].weight<closedge[j].lowcost||closedge[j].lowcost==0 )
                {
                    closedge[j].lowcost=G->arc[k][j].weight;
                    closedge[j].adjvex=k;
                }
    }
    printf("最小生成树权值用为：Mincost=%d\n",ans);
}

int main()//主函数
{
    MGraph *G;
    G = (MGraph*)malloc(sizeof(MGraph));
    CreatGraph(G);
    prim(G);
}
