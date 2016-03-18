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
 * @author  Liang Yi
 * @e-mail: liangyi19941213@gmail.com
 * @date    2014/10/11
 */

#include <iostream>
#include "USSolver.h"
#include<iostream>
#include<stack>
#include<iomanip>
using namespace std;

struct HNode // 存储结点
{
	int verdata; // 结点编号
	int in; // 入度
	struct Node *link;
};

struct Node // 存储边权
{
	int adjdata; // 结点编号
	int weight; // 权值
	struct Node *next;
};

struct Info // 将题目中的输入形式转化为起点、终点、权值的输入形式
{
	int num; // 结点编号
	int dut; // 起点到终点的权值
	int pre[500]; // 此结点的前驱结点
	int prenum; // 前驱结点数量
};

struct Input // 以起点、终点、权值的形式存储
{
	int head; // 起点
	int tail; // 终点
	int len; // 权值
};

void CreateGraph(std::istream &inputStream, HNode *&Adjlist, int n) // 建立图
{
	int temp = 0, first = 0, time = 0, flag = 0;
	int *judge = new int[n]; // 判断结点是否为尾结点
	for (int i=0; i<n; i++) // 初始化
		judge[i] = i;
	char ch;
	struct Info *q = new struct Info[n];
	for (int i=0; i<n; i++)
	{
		inputStream >> q[i].num >> q[i].dut;
		inputStream.get(ch);
		if(ch == '\n')
			first = i;
		else
		{
			q[i].prenum = 0;
			inputStream >> q[i].pre[q[i].prenum];
			for (int k=0; k<n; k++) // 不是尾结点则将其值赋为-1
			{
				if(k == q[i].pre[q[i].prenum])
					judge[k] = -1;
			}
			q[i].prenum++;
			inputStream.get(ch);
			while (ch == ',')
			{
				inputStream >> q[i].pre[q[i].prenum];
				for (int k=0; k<n; k++)
				{
					if(k == q[i].pre[q[i].prenum])
						judge[k] = -1;
				}
				q[i].prenum++;
				inputStream.get(ch);
			}
		}
	}
	// 假定第i行输入为工序i-1
	struct Input k[1000];
	struct Input kk;
	for (int i=0; i<n; i++)
	{
		if (i != first)
		{
			for (int j=0; j<q[i].prenum; j++)
			{
				k[time].head = q[i].pre[j];
				k[time].tail = q[i].num;
				k[time].len = q[q[i].pre[j]].dut;
				time++;
			}
		}
	}
	for (int l=0; l<n; l++)
	{
		if (judge[l] != -1)
		{
			k[time].head = l;
			k[time].tail = n;
			k[time].len = q[l].dut;
			time++;
		}
	}
	for (int i=0; i<time; i++)
	{
		for (int j=i; j<time; j++)
		{
			if (k[i].head > k[j].head)
			{
				kk.head = k[i].head;
				kk.tail = k[i].tail;
				kk.len = k[i].len;
				k[i].head = k[j].head;
				k[i].tail = k[j].tail;
				k[i].len = k[j].len;
				k[j].head = kk.head;
				k[j].tail = kk.tail;
				k[j].len = kk.len;
			}
		}
	}
	/*测试结构体Input是否转化成功
	for (int m=0; m<time; m++)
		cout << k[m].head << " " << k[m].tail << " " << k[m].len << endl;
	//*/
	struct Node *p;
	for (int i=0; i<=n; i++)
	{
		Adjlist[i].verdata = i;
		Adjlist[i].in = 0;
		Adjlist[i].link = NULL;
	}
	for (int j=0; j<time; j++)
	{
		p = new struct Node; // 生成新的边表结点
		p->adjdata = k[j].tail;
		p->weight = k[j].len;
		p->next = Adjlist[k[j].head].link;
		Adjlist[k[j].head].link = p;
	}
	for (int l=0; l<=n; l++) // 计算入度
	{
		p = Adjlist[l].link;
		while (p != NULL)
		{
			temp = p->adjdata;
			Adjlist[temp].in++;
			p = p->next;
		}
	}
	/*结点入度测试
	for (int i=1; i<=n; i++)
		cout << Adjlist[i].in << " ";
	*/
}

void PrintPath(std::ostream &outputStream, HNode *Adjlist, int *ve, int *vl, int *&path, int head, int tail)
{
	int i = 0, temp = 0;
	Node *p = NULL;
	temp = path[head-1];
	if (temp == tail) // 按要求输出
	{
		outputStream << "[" << path[0];
		while (path[i] != tail)
		{
			outputStream << " " << "->" << path[i] << " ";
			i++;
		}
		outputStream << "-> "<< tail << "-> end]" << endl;
	}
	for (p=Adjlist[temp].link; p!=NULL; p=p->next)
	{
		if (ve[p->adjdata] == vl[p->adjdata])
		{
			path[head] = p->adjdata; // p->adjdata入栈
			head++;
			PrintPath(outputStream, Adjlist, ve, vl, path, head, tail); // 深搜路径
			head--; // 出栈
		}
	}
}

void CriticalPath(std::ostream &outputStream, HNode *Adjlist, int n) // 找到关键路径
{
	n++;
	int front = 0, rear = 0, flag = 0; // front和rear用于栈操作，flag判断是否有回路
	int j, k, l;
	int *ve = new int[n];
	int *vl = new int[n];
	int *stack = new int[n];
	struct Node *p;
	for (int i=0; i<n; i++)
	{
		ve[i] = 0;
		vl[i] = 0;
		if (Adjlist[i].in == 0) // 寻找入度为0的结点入栈
		{
			stack[rear] = i;
			rear++;
		}
	}
	while (front != rear) // 栈操作开始
	{
		j = stack[front];
		front++;
		flag++;
		p = Adjlist[j].link; // p指向第一个邻接结点
		while (p != NULL)
		{
			k = p->adjdata;
			Adjlist[k].in--; // 入度减1
			if(ve[j]+p->weight > ve[k])
				ve[k] = ve[j] + p->weight; // 最长路径更新
			if (Adjlist[k].in == 0) // 入度为0则入栈
			{
				stack[rear] = k;
				rear++;
			}
			p = p->next;
		}
	}
	if(flag < n)
	{
		outputStream << "Error！有回路,请重新输入数据" << endl;
		return;
	}
	for (int i=0; i<n; i++)
		vl[i] = ve[n-1];
	for (int i=n-2; i>=0; i--)
	{
		j = stack[i];
		p = Adjlist[j].link; // p指向下一邻接节点
		while (p != NULL)
		{
			k = p->adjdata;
			if(vl[k]-p->weight < vl[j]) // 最短路长度更新
				vl[j] = vl[k] - p->weight;
			p = p->next;
		}
	}
	int *record = new int[n]; // record存储关键路径结点
	int *delay = new int[n]; // delay存储可拖延时间
	int *path = new int[n]; // path用于关键路径的输出
	int m = 0, t = 0;
	outputStream << ve[n-1] << endl;
	for (int i=0; i<n; i++)
	{
		delay[i] = vl[i] - ve[i];
		if (delay[i] == 0)
		{
			record[m] = i;
			m++;
		}
	}
	m = m - 2; // 去除最后一个临时结点
	path[0] = record[0];
	PrintPath(outputStream, Adjlist, ve, vl, path, 1, record[m]);
	for (int i=0; i<n-1; i++)
		outputStream << delay[i] << endl;
}

void USSolver::Solve(std::istream &inputStream, std::ostream &outputStream)
{
	outputStream << "Your Answer:" << endl;
	int n; // n为结点数量
	inputStream >> n;
	struct HNode *Adjlist = new struct HNode[n+1];
	CreateGraph(inputStream, Adjlist, n);
	CriticalPath(outputStream, Adjlist, n);
}

int main(int argc, char *argv[])
{
	//可在此更改测试文件
    std::string fileName = "test101.txt"; 
    USSolver unofficialSolver(fileName);
    const std::string unofficialAnswer = unofficialSolver.GetAnswer();
    std::cout << unofficialAnswer;
	return 0;
}
