/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2012, Institute of CG & CAD, Tsinghua University.
 * All Rights Reserved.
 * 
 * @file    main.cpp
 * @brief   Solver01 class declaration.
 * 
 * This file declares ...
 * 
 * @version 1.0
 * @author  Jackie Pang
 * @e-mail: 15pengyi@gmail.com
 * @date    2012/10/11
 */

#include <iostream>
#include "USSolver.h"



#include<iostream>
#include<stack>
#include<iomanip>
using namespace std;

int pathlenth = 1;
bool visited[10000]; // 判定节点visited[i]是否被访问过
void DFS(int **Adjacencymatrix, int now, int tail, int v, int record[], std::ostream &outputStream, bool visited[])
{
	int i, j;
	for (i=0; i<v; i++)
	{
		if (Adjacencymatrix[now][i] == 1)
		{
			if (i == tail)
			{
				outputStream << '[';
				for(j=0; j<pathlenth; j++)
					outputStream << record[j] << " " << '-' << '>' << " ";
				outputStream << tail << ']';
			}
			else if(visited[i] == false)
			{
				Adjacencymatrix[now][i] = 0;
				record[pathlenth] = i;
				visited[i] = true;
				pathlenth++;
				DFS(Adjacencymatrix, i, tail, v, record, outputStream, visited);
				visited[i] = false;
				Adjacencymatrix[now][i] = 1;
				pathlenth--;
			}
		}
	}
	
	
}

void USSolver::Solve(std::istream &inputStream, std::ostream &outputStream)
{
	//请在此填充代码
	int v, e; // v代表定点个数，e代表边个数
	inputStream >> v;
	inputStream >> e;
	int *line1 = new int[v+1]; // 正向表第一行
	for (int i=0; i<(v+1); i++)
		inputStream >> line1[i];
	int *line2 = new int[e]; // 正向表第二行
	for (int i=0; i<e; i++)
		inputStream >> line2[i];
	int **Adjacencymatrix = new int *[v]; // v*v的邻接矩阵Adjacencymatrix
	int **Pathmatrix = new int *[v]; // v*v的道路矩阵Pathmatrix
	for (int i=0; i<v; i++)
	{
	   Adjacencymatrix[i] = new int[v];
	   Pathmatrix[i] = new int[v];
	}
	for	(int i=0; i<v; i++) // 矩阵初始化
	{
		for (int j=0; j<v; j++)
		{
			Adjacencymatrix[i][j] = 0;
			Pathmatrix[i][j] = 0;
		}
	}
	int temp = 0, next = 0;
	for (int k=0; k<v; k++)
	{
		temp = line1[k+1] - line1[k];
		for (int l=0; l<temp; l++)
		{
			Adjacencymatrix[k][line2[next]] = 1;
			Pathmatrix[k][line2[next]] = 1;
			next++;
		}
	}
	/*for (int m=0; m<v; m++) // 测试正向表转邻接矩阵
	{
		outputStream << endl;
		for (int n=0; n<v; n++)
		{
			outputStream << Adjacencymatrix[m][n] << " ";
		}
		
	}*/
	for (int i0=0; i0<v; i0++) // Warshall算法实现邻接矩阵转道路矩阵
	{
		for (int j0=0; j0<v; j0++)
		{
			for (int k0=0; k0<v; k0++)
			{
				if (Pathmatrix[j0][k0]==1 || (Pathmatrix[j0][i0]==1 && Pathmatrix[i0][k0]==1))
				{
					Pathmatrix[j0][k0] = 1;
				}
			}
		}
	}
	/*for (int m=0; m<v; m++) // 测试邻接矩阵转道路矩阵
	{
		outputStream << endl;
		for (int n=0; n<v; n++)
		{
			outputStream << Pathmatrix[m][n] << " ";
		}

	}*/
	int n = 0;
	inputStream >> n;
	int head, tail;
	int *record = new int[10000]; // 用于记录路径
	outputStream << endl;
	for (int I=0; I<n; I++) // 用深度搜索算法找出由head到tail的所有路径
	{
		inputStream >> head >> tail;
		if(Pathmatrix[head][tail] == 0) // 道路矩阵为0说明两点之间不连通,或者为环
			outputStream << "NO PATH" << endl;
		else
		{
			record[0] = head;
			visited[0] = true;
			DFS(Adjacencymatrix, head, tail, v, record, outputStream, visited);
			outputStream << endl;
		}
	}
}



int main(int argc, char *argv[])
{
	//可在此更改测试文件
    std::string fileName = "test101.txt"; 
    USSolver unofficialSolver(fileName);
    const std::string unofficialAnswer = unofficialSolver.GetAnswer();
    std::cout << "Your Answer:" << unofficialAnswer;
}