#include <stdio.h>
#include <stdlib.h>

#define INF 0x3f3f3f3f
#define MaxVertexNum 10

typedef struct Graph
{
	int AdjMatrix[MaxVertexNum][MaxVertexNum];
	int	VertexNum,ArcNum;
}Net;
/*
 * Vertex node in linkedlist(path)
 */
typedef struct s
{
	int Vertex;
	struct s *Next;
}VertexNode;
/*
 * EndingVertex i information in CurShortestP[i].
 *  PathWeight is current shortest path value of EndingVertex i.
 *  b is make sure if the EndingVertex i already in ShortestPathV[]
 *       if in b=0,if not in b=1.
 */
typedef struct 
{
	int PathWeight;
	int b;
	VertexNode* v0_ad;
}ShortestPath;

void CreateNet(Net* NetAd);
void dijkstra(Net* NetAd,int SourceVertex);
int GetSmallestPathV(ShortestPath* a,int n);
int GetSmallestPathWehght(int* a,int n);
void UpdateShortestPath(int m,int i,ShortestPath* D,int* S,int n);
int ListLength(VertexNode* L,int j);
void PrintPath(ShortestPath* D,int i);

int main(){
	Net N;
	Net* NetAd;
	NetAd=&N;
	CreateNet(NetAd);
	dijkstra(NetAd,0);
	return 1;
}
/*
 *  dijkstra-Dijkstra algorithm finding the shortest path.
 *    NetAd pointer point the undirected network.
 */
void dijkstra(Net* NetAd,int SourceVertex){
	int ShortestPathV[NetAd->VertexNum-1];
	ShortestPath CurShortestP[NetAd->VertexNum-1];
	int i,j,k;
	int m;
	int* a;
	for(i=0;i<NetAd->VertexNum-1;i++){ //Initialization
		ShortestPathV[i]=0;
		CurShortestP[i].b=1;
		CurShortestP[i].PathWeight=NetAd->AdjMatrix[SourceVertex][i+1];
		CurShortestP[i].v0_ad=(VertexNode*)malloc(sizeof(VertexNode));
		CurShortestP[i].v0_ad->Vertex=-1;
		CurShortestP[i].v0_ad->Next=(VertexNode*)malloc(sizeof(VertexNode));
	    CurShortestP[i].v0_ad->Next->Vertex=i;
	}
	int v;
	v=GetSmallestPathV(CurShortestP,NetAd->VertexNum-1);
	ShortestPathV[v]=1;
	k=1;
	CurShortestP[v].b=0;
	while(k<5){
		for(i=0;i<NetAd->VertexNum-1;i++){//Update the current shortest path values in CurShortestP[i].PathWeight.
			if(ShortestPathV[i]) continue;
			int small_index;
			a=(int*)malloc((k+1)*sizeof(int));//a[] stored the different path weight for i.
			a[0]=CurShortestP[i].PathWeight;
			j=1;
			for(m=0;m<NetAd->VertexNum-1;m++){
				if(ShortestPathV[m]){
					a[j]=CurShortestP[m].PathWeight+NetAd->AdjMatrix[i+1][m+1];
					j++;
				}
			}
			small_index=GetSmallestPathWehght(a,k+1);
			CurShortestP[i].PathWeight=a[small_index];
			if(small_index!=0) UpdateShortestPath(small_index,i,CurShortestP,ShortestPathV,NetAd->VertexNum-1);
		}
		v=GetSmallestPathV(CurShortestP,NetAd->VertexNum-1);
		ShortestPathV[v]=1;
		CurShortestP[v].b=0;
		k++;
	}
	for(i=0;i<NetAd->VertexNum-1;i++){
		printf("v%d shortest path value is %d.\n",i+1,CurShortestP[i].PathWeight );
		PrintPath(CurShortestP,i);
	}
}
/*
 *  PrintPath-Print the shortest path for each ending vertex.
 *    D[i](CurShortestP[i]) stored the imformation for the ending vertex i.
 */
void PrintPath(ShortestPath* D,int i){
	VertexNode* L;
	L=D[i].v0_ad;
	printf("And the shortest path from v0 to v%d is: \n",i+1);
	do{
		printf("v%d  ",L->Vertex+1);
		if(L->Vertex==i) break;
		L=L->Next;
	}while(1);
	printf("\n");
}
/*
 *  UpdateShortestPath-After updating CurShortestP[i].Pathweight,update linkedlist CurShortestP[i].v0_ad accordingly.
 *    m is the smallest path weight coordinate in a[].
 */
void UpdateShortestPath(int m,int i,ShortestPath* D,int* S,int n){
	int j,k=0;
	int PathLength;
	int x;
	VertexNode* L1;
	VertexNode* L2;
	for(j=0;j<=n;j++){
		if(S[j]) k++;
		if(k==m) break;
	}
	PathLength=ListLength(D[j].v0_ad,j);
	L1=D[i].v0_ad;
	L2=D[j].v0_ad;
	for(x=1;x<=PathLength;x++){
		L1->Next=(VertexNode*)malloc(sizeof(VertexNode));
		L1->Next->Vertex=L2->Next->Vertex;
		if(x==PathLength){
			L1->Next->Next=(VertexNode*)malloc(sizeof(VertexNode));
			L1->Next->Next->Vertex=i;
			break;
		}
		L1=L1->Next;
		L2=L2->Next;
	}
}
/*
 *  ListLength-Count the number of vertex in the path from v0 to vj.
 */
int ListLength(VertexNode* L,int j)
{
	int k=0;
	while(L->Vertex!=j){
		L=L->Next;
		k++;
	}
	return k;
}
/*
 *  GetSmallestPathV-Get the vertex which path weight is smallest in a[](CurShortestP[]).
 *    n is the vertex number in the network(except source vertex).
 */
int GetSmallestPathV(ShortestPath* a,int n){
	int i=0,j;
	for(i=0;i<n;i++){
		if(a[i].b==0) continue;
		for(j=0;j<n;j++){
			if(a[i].PathWeight>a[j].PathWeight&&a[j].b!=0) i=j;
		}
		if(j==n) break;
	}
	return i;
}
/*
 *  GetSmallestPathWehght-Get the smallest path weight to update the CurShortestP[i].Pathweight.
 *    a[] stored the different path weight.
 *      eg:If there are v1 in ShortestPathV[] and i=3.
 *         a[] stored : CurShortestP[3].Pathweight,CurShortestP[1].Pathweight+AdjMatrix[1][3]
 */
int GetSmallestPathWehght(int* a,int n){
	int i=0,j;
	for(j=0;j<n;j++){
		if(a[i]>a[j]) i=j;
	}
	return i;
}
/*
 *  CreateNet-Use adjacency matrix to create undirected network.
 *    NetAd is a pointer point the network.
 */
void CreateNet(Net* NetAd){
	int i,j,k,w;
	printf("Please input the vertex number and the arc number(Vnum,Anum): \n");
	scanf("%d,%d",&(NetAd->VertexNum),&(NetAd->ArcNum));
	for(i=0;i<NetAd->VertexNum;i++){
		for(j=i;j<NetAd->VertexNum;j++){
			NetAd->AdjMatrix[i][j]=INF;
			NetAd->AdjMatrix[j][i]=INF;
		}
	}
	for(k=1;k<=NetAd->ArcNum;k++){
		printf("Please input the edge(v,w) and the weight: \n");
		scanf("%d,%d %d",&i,&j,&w);
		NetAd->AdjMatrix[i][j]=w;
		NetAd->AdjMatrix[j][i]=w;
	}
	printf("\n");
}

