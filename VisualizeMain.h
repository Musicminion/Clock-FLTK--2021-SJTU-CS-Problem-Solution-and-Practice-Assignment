#ifndef _VisualizeMain_h
#define _VisualizeMain_h

#include <fstream>
#include <direct.h>
#include "GUI.h"
#include <io.h>
#include "Window.h"
#include "Simple_window.h"
#include "Graph.h"
#include "Point.h"
#include <iostream>
#include "std_lib_facilities.h"
#include <time.h>
#include <string>
#include <cmath>
#include <vector>
#include <map>

using namespace Graph_lib;

class Node
{
	friend class VisualizeMain;
public:
	// 基础数据 节点的名字，节点的左右关系
	string NodeInfo;	// 字符串表示节点的唯一标识名词
	Node* left;			// 指针指向左右节点 初始化为空
	Node* right;

	// 考虑到最终大小要适应屏幕，所以xx yy的规模要做适应性调整，并单独的全部重新计算!
	int xxScale;		
	int yyScale;

};


class VisualizeMain : Window
{
	friend class TreeVisualize;
	friend class Node;


public:
	VisualizeMain(int width, int height);
	
private:
	// ------------------------------------------------------------------------------------------------------------------
	// 基础变量数据 屏幕像素数据
	int ScreenWidth;
	int ScreenHeight;
	
	// 基础控件数据 包括按钮 这些数据的即使在清空按钮点下之后都不会改变，属于背景类型
	// 基础按钮
	Button button_exit;
	Button button_readfileAuto;
	Button button_readfileManu;
	Button button_refresh;
	Button button_nextRead;

	// 基础图形 进度条
	Graph_lib::Rectangle processBar1;
	Graph_lib::Rectangle processBar2;
	Graph_lib::Rectangle processBar3;
	Graph_lib::Rectangle processBar4;

	// 基础图形 线条
	Graph_lib::Line line1;
	Graph_lib::Line line2;

	// 基础图形 输入框和输出框
	In_box inputbox;
	Out_box infoOut;

	// 基础信息 进度条标题
	Text processBar1Title;
	Text processBar2Title;
	Text processBar3Title;
	Text processBar4Title;
	Text inboxTitle;
	Text infoOutTitle;
	Text inputreminderText;
	// ------------------------------------------------------------------------------------------------------------------

	// 特别警告！每次处理一大轮（就是说文件队列清空之后算一大轮）后，下面的所有的变量全部要刷新清空
	// 但是，一小轮结束后，只需要清空画布，两个树相关的执行回收即可
	// 后台变量
	// ------------------------------------------------------------------------------------------------------------------
	string currentfilePlace;
	string ManualfilePlace;
	vector <string> OriginalAllfilePlace;					// 保存原始的所有文件的目录 检索支持子目录
	vector <string> FinalPlaceResult;						// 保存筛选后的最终的 txt，csv，或者其他类型的树文件
	void STSTEM_var_refresh();								// 后台变量全部初始化
	// ------------------------------------------------------------------------------------------------------------------
	
	// 树相关的实现构想，原始的字符串读取 读取文件后写入 Tree1_OriginalData
	// 然后解析 Tree1_OriginalData 的每一行元素 用->为分隔符 建立 原始文件左名字到右名字的映射
	// 然后利用 Tree1_RootToSon_NameRelation（原始文件左名字到右名字的映射） 构建 名字到节点的映射 相当于树的节点的名字空间
	// 上面的部分需要修正 可能有一对多 那就需要两个以上的映射来保存
	// 
	// 下面的部分特别注意！！！！！！！！！
	// 特别注意 构建的过程中 需要用二级指针 Tree1_vitualRootNode 构造一级指针的数组下放，得到一级指针的节点，一级指针的节点再去申请node节点
	// 虚拟根节点不是节点，具体要寻找节点还需要通过函数来实现！！
	// 
	// 在回收的时候，先回收所有的node节点，再回收 一级指针，最后回收二级指针 注意  Tree1_realRootNode 无需回收！
	
	
	
	// tree1
	bool Tree1_iflegal;										// 树的合法 通讯工具
	int Tree1_height;										// 树的高度
	Node** Tree1_vitualRootNode;							// 虚拟根节点，二级指针用来下放，得到一系列的一级指针，这个节点目的就是用来申请节点
	Node* Tree1_realRootNode;								// 真实的树的一级节点
	vector<string> Tree1_OriginalData;						// 原始的字符串读取 读取文件后写入这个数组 再解析
	map<string, string> Tree1_RootToSon_NameRelationLeft;	// 根节点到左儿子映射
	map<string, string> Tree1_RootToSon_NameRelationRight;	// 根节点到右儿子映射
	map<string, Node*> Tree1_NameToNode;					// 由节点名字到该节点指针的映射 key值相当于树的节点的名字空间 
	

	// tree2
	bool Tree2_iflegal;
	int Tree2_height;										// 树的高度
	Node** Tree2_vitualRootNode;							// 虚拟根节点，二级指针用来下放，得到一系列的一级指针，这个节点目的就是用来申请节点
	Node* Tree2_realRootNode;								// 真实的树的一级节点
	vector<string> Tree2_OriginalData;						// 原始的字符串读取 读取文件后写入这个数组 再解析
	map<string, string> Tree2_RootToSon_NameRelationLeft;	// 根节点到左儿子映射
	map<string, string> Tree2_RootToSon_NameRelationRight;	// 根节点到右儿子映射
	map<string, Node*> Tree2_NameToNode;					// 由节点名字到该节点指针的映射 key值相当于树的节点的名字空间 



	// 按钮控件所对应的函数调用！
	void buttonPressed_exit();
	void buttonPressed_readfileAuto();
	void buttonPressed_readfileManu();
	void buttonPressed_refresh();
	void buttonPressed_nextRead();


	// 树相关的所有的功能
	int TreeCenterMaster();				// read and deal with Two tree file!
	
	// tree1的操作函数
	// 特别警告：所有的操作只有在树合法的前提下 才允许进行！  Tree1_iflegal是通讯报错工具！
	void Tree1_OriginalData_Get();			// 函数pop掉队列的一个文件目录字符串，开始读取文件，然后写入OriginalData
	void Tree1_OriginalData_Analysis();		// 原始数据解析 用->为分隔符 建立 原始文件左名字到右名字的映射
	void Tree1_NameToNode_Build();			// 由节点名字到该节点指针的映射 key值相当于树的节点的名字空间 
	void Tree1_Build();						// 建立树的链接形式
	void Tree1_draw();						// 画圆圈图 画按钮 按钮响应点击的效果
	
	void Tree1_drawClear();					// 画布清空
	void Tree1_clear();						// 清理所有的变量，归还申请的内存

	// tree2的操作函数
	// 特别警告：所有的操作只有在树合法的前提下 才允许进行！  Tree2_iflegal是通讯报错工具！
	void Tree2_OriginalData_Get();			// 函数pop掉队列的一个文件目录字符串，开始读取文件，然后写入OriginalData
	void Tree2_OriginalData_Analysis();		// 原始数据解析 用->为分隔符 建立 原始文件左名字到右名字的映射
	void Tree2_NameToNode_Build();			// 由节点名字到该节点指针的映射 key值相当于树的节点的名字空间 
	void Tree2_Build();						// 建立树的链接形式
	void Tree2_draw();						// 画圆圈图 画按钮 按钮响应点击的效果

	void Tree2_drawClear();					// 画布清空
	void Tree2_clear();						// 清理所有的变量，归还申请的内存

	bool if_isomorphism();					// 同构判断函数
	void isomorphismLink();					// 产生一系列的按钮 点击实现匹配	

	void Tree1_free();						// 释放tree1资源
	void Tree2_free();						// 释放tree2资源


	
	// 辅助功能
	void findCurrentPlace();												// 查找当前所在的exe文件的根目录
	void PrimefileFocus(string TargetFolder, vector<string>& files);		// 查找txt，csv，或者其他类型的文件
	void Selector(vector<string>& beforeSel, vector<string>& AfterSel);		// 过滤装置，只保留后缀是txt csv 或者in的文件！
	bool if_Select(string& obj);											// 判断目录的末端是否是合条件的！


	// 控制台的输出检查
	void TmpPrinter_Tree1_RootToSon_NameRelation();
	void TmpPrinter_Tree2_RootToSon_NameRelation();
};

#endif

