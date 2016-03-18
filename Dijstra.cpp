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

#define maxint 99999999 // 设置不邻接结点间距
void Dijkstra(std::ostream &outputStream, int **Weightmatrix, int *&prenode, int *&distance, int head, int v) // 实现Dijkstra算法
{
	bool *visited = new bool[v]; // 记录结点i是否被访问过
	for (int i=0; i<v; i++) // 初始化
	{
		distance[i] = Weightmatrix[head][i];
		visited[i] = false;
		if(distance[i] == maxint)
			prenode[i] = -1;
		else
			prenode[i] = head;
	}
	visited[head] = true;
	distance[head] = 0;
	int min = 0, temp = 0;
	for (int i=1; i<v; i++)
	{
		min = maxint;
		temp = head;
		for (int j=0; j<v; j++)
		{
			if (visited[j] == false && distance[j] < min)
			{
				min = distance[j];
				temp = j;
			}
		}
		visited[temp] = true;
		for (int k=0; k<v; k++)
		{
			if (visited[k] == false && Weightmatrix[temp][k] < maxint && (distance[temp]+Weightmatrix[temp][k] <distance[k]))
			{
				distance[k] = distance[temp] + Weightmatrix[temp][k];
				prenode[k] = temp;
			}
		}
	}
}

void PrintPath(std::ostream &outputStream, int *prenode, int *distance, int head, int tail, int v)
{
	int *print = new int[v];
	int a = 1, b = 0;
	print[0] = tail;
	b = prenode[tail];
	while (b != head)
	{
		print[a] = b;
		a++;
		b = prenode[b];
	}
	print[a] = head;
	outputStream << "[";
	for (int i=a; i>0; i--)
		outputStream << print[i] << " " << '-' << '>' << " ";
	outputStream << print[0] << " " << distance[tail] << "]" << endl;
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
	int *r = new int[e];
	for (int i=0; i<e; i++) // 输入权重r[i]
		inputStream >> r[i];
	int **Adjacencymatrix = new int *[v]; // v*v的邻接矩阵Adjacencymatrix
	int **Pathmatrix = new int *[v]; // v*v的道路矩阵Pathmatrix
	int **Weightmatrix = new int *[v]; // v*v的权值矩阵Weightmatrix
	for (int i=0; i<v; i++)
	{
	   Adjacencymatrix[i] = new int[v];
	   Pathmatrix[i] = new int[v];
	   Weightmatrix[i] = new int[v];
	}
	for	(int i=0; i<v; i++) // 矩阵初始化
	{
		for (int j=0; j<v; j++)
		{
			Adjacencymatrix[i][j] = 0;
			Pathmatrix[i][j] = 0;
			Weightmatrix[i][j] = maxint;
		}
	}
	int temp = 0, next = 0;
	for (int k=0, g=0; k<v; k++)
	{
		temp = line1[k+1] - line1[k];
		for (int l=0; l<temp; l++)
		{
			Weightmatrix[k][line2[next]] = r[g];
			Adjacencymatrix[k][line2[next]] = 1;
			Pathmatrix[k][line2[next]] = 1;
			next++;
			g++;
		}
	}
	/*for (int i=0; i<v; i++) // 验证矩阵值的正确性
	{
		for (int j=0; j<v; j++)
			cout << Weightmatrix[i][j] << " ";
		cout << endl;
	}*/
	for (int i0=0; i0<v; i0++) // Warshall算法实现邻接矩阵转道路矩阵，判断两点之间是否有通路
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
	int n, head, tail; // n为测试点数量，head为测试首结点，tail
	int *prenode = new int[v]; // 用于记录路径
	int *distance = new int[v]; // distance[i]用于存储结点head到i的最短路长度
	inputStream >> n;
	for (int I=0; I<n; I++)
	{
		inputStream >> head >> tail;
		if(head == tail || Pathmatrix[head][tail] == 0) // 两点之间无通路则输出NO PATH
			outputStream << "NO PATH" << endl;
		else // 两点之间有通路则实现D算法
		{
			Dijkstra(outputStream, Weightmatrix, prenode, distance, head, v);
			PrintPath(outputStream, prenode, distance, head, tail, v);
		}
	}
}



int main(int argc, char *argv[])
{
	//可在此更改测试文件
    std::string fileName = "test101.txt"; 
    
    USSolver unofficialSolver(fileName);
    const std::string unofficialAnswer = unofficialSolver.GetAnswer();

    std::cout << "Your Answer:" << endl << unofficialAnswer;
	return 0;
}