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
bool visited[10000]; // �ж��ڵ�visited[i]�Ƿ񱻷��ʹ�
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
	//���ڴ�������
	int v, e; // v�����������e����߸���
	inputStream >> v;
	inputStream >> e;
	int *line1 = new int[v+1]; // ������һ��
	for (int i=0; i<(v+1); i++)
		inputStream >> line1[i];
	int *line2 = new int[e]; // �����ڶ���
	for (int i=0; i<e; i++)
		inputStream >> line2[i];
	int **Adjacencymatrix = new int *[v]; // v*v���ڽӾ���Adjacencymatrix
	int **Pathmatrix = new int *[v]; // v*v�ĵ�·����Pathmatrix
	for (int i=0; i<v; i++)
	{
	   Adjacencymatrix[i] = new int[v];
	   Pathmatrix[i] = new int[v];
	}
	for	(int i=0; i<v; i++) // �����ʼ��
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
	/*for (int m=0; m<v; m++) // ���������ת�ڽӾ���
	{
		outputStream << endl;
		for (int n=0; n<v; n++)
		{
			outputStream << Adjacencymatrix[m][n] << " ";
		}
		
	}*/
	for (int i0=0; i0<v; i0++) // Warshall�㷨ʵ���ڽӾ���ת��·����
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
	/*for (int m=0; m<v; m++) // �����ڽӾ���ת��·����
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
	int *record = new int[10000]; // ���ڼ�¼·��
	outputStream << endl;
	for (int I=0; I<n; I++) // ����������㷨�ҳ���head��tail������·��
	{
		inputStream >> head >> tail;
		if(Pathmatrix[head][tail] == 0) // ��·����Ϊ0˵������֮�䲻��ͨ,����Ϊ��
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
	//���ڴ˸��Ĳ����ļ�
    std::string fileName = "test101.txt"; 
    USSolver unofficialSolver(fileName);
    const std::string unofficialAnswer = unofficialSolver.GetAnswer();
    std::cout << "Your Answer:" << unofficialAnswer;
}