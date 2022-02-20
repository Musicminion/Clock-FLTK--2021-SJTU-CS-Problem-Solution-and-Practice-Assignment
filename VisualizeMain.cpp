#include "VisualizeMain.h"

using namespace Graph_lib;

VisualizeMain::VisualizeMain(int scrW, int scrH)
	:Window{ Point(0 ,0),scrW ,scrH , "Main" },
	button_exit{ Point{int(0.025 * x_max()),int(0.8 * y_max())},int(0.08 * x_max()),40,"退出",
		[](Address,Address pw) {reference_to<VisualizeMain>(pw).buttonPressed_exit(); } },
	button_readfileAuto{ Point{int(0.025 * x_max()),int(0.7 * y_max())},int(0.08 * x_max()),40,"自动读取文件",
		[](Address,Address pw) {reference_to<VisualizeMain>(pw).buttonPressed_readfileAuto(); } },
	button_readfileManu{ Point{int(0.025 * x_max()),int(0.6 * y_max())},int(0.08 * x_max()),40,"手动读取文件",
		[](Address,Address pw) {reference_to<VisualizeMain>(pw).buttonPressed_readfileManu(); } },
	button_refresh{ Point{int(0.025 * x_max()),int(0.5 * y_max())},int(0.08 * x_max()),40,"清空数据",
		[](Address,Address pw) {reference_to<VisualizeMain>(pw).buttonPressed_refresh(); } },
	button_nextRead{ Point{int(0.325 * x_max()),int(0.93 * y_max())},int(0.13 * x_max()),40,"继续读取队列中文件",
		[](Address,Address pw) {reference_to<VisualizeMain>(pw).buttonPressed_nextRead(); } },
	processBar1(Point{ int(0.025 * x_max()),int(0.045 * x_max()) }, Point{ int(0.125 * x_max()),int(0.060 * x_max()) }),
	processBar2(Point{ int(0.025 * x_max()),int(0.085 * x_max()) }, Point{ int(0.125 * x_max()),int(0.100 * x_max()) }),
	processBar3(Point{ int(0.025 * x_max()),int(0.125 * x_max()) }, Point{ int(0.125 * x_max()),int(0.140 * x_max()) }),
	processBar4(Point{ int(0.025 * x_max()),int(0.165 * x_max()) }, Point{ int(0.125 * x_max()),int(0.180 * x_max()) }),
	processBar1Title(Point{ int(0.025 * x_max()),int(0.040 * x_max()) }, "文件读取与数据对接"),
	processBar2Title(Point{ int(0.025 * x_max()),int(0.080 * x_max()) }, "树的构建与可视化"),
	processBar3Title(Point{ int(0.025 * x_max()),int(0.120 * x_max()) }, "同构的判断"),
	processBar4Title(Point{ int(0.025 * x_max()),int(0.160 * x_max()) }, "同构链接的呈现"),
	inputbox(Point{ int(0.025 * x_max()),int(0.390 * y_max()) }, int(0.1 * x_max()), int(0.04 * y_max()), ""),
	inboxTitle(Point{ int(0.025 * x_max()),int(0.385 * y_max()) },"目录输入框：(Ctrl+V)"),
	inputreminderText(Point{ int(0.025 * x_max()),int(0.450 * y_max()) }, "请粘贴目标文件夹的目录"),
	infoOutTitle(Point{ int(0.025 * x_max()),int(0.92 * y_max()) }, "信息框"),
	infoOut(Point{ int(0.025 * x_max()),int(0.93 * y_max()) }, int(0.25 * x_max()), int(0.05 * y_max()), ""),
	line1(Point{ int(0.135 * x_max()),int(0.05 * y_max()) },Point{ int(0.135 * x_max()),int(0.89 * y_max()) }),
	line2(Point{ int(0.5675 * x_max()),int(0.05 * y_max()) }, Point{ int(0.5675 * x_max()),int(0.89 * y_max()) })
{
	


	ScreenWidth = scrW;
	ScreenHeight = scrH;
	

	// 颜色初始化
	//processBar1.set_fill_color
	processBar1.set_color(Color::black);
	processBar1.set_fill_color(Color::white);
	processBar2.set_color(Color::black);
	processBar2.set_fill_color(Color::white);
	processBar3.set_color(Color::black);
	processBar3.set_fill_color(Color::white);
	processBar4.set_color(Color::black);
	processBar4.set_fill_color(Color::white);

	line1.set_color(250);
	line2.set_color(250);

	color(254);
	inputreminderText.set_color(Color::red);

	// 窗口吸附
	attach(button_exit);
	attach(button_readfileAuto);
	attach(button_readfileManu);
	attach(button_refresh);
	attach(button_nextRead);

	attach(processBar1);
	attach(processBar2);
	attach(processBar3);
	attach(processBar4);

	attach(processBar1Title);
	attach(processBar2Title);
	attach(processBar3Title);
	attach(processBar4Title);

	attach(infoOut);
	attach(inputbox);
	attach(inboxTitle);
	attach(infoOutTitle);
	attach(inputreminderText);

	attach(line1);
	attach(line2);
	
	
	// 基础变量在这里初始化
	Tree1_iflegal = true;
	Tree2_iflegal = true;		// 先信任树的合法
	infoOut.put("你好呀!欢迎使用程序 ~ 请准备好相关文件");
	
	
}

void VisualizeMain::buttonPressed_exit()
{
	hide();
}


void VisualizeMain::buttonPressed_readfileAuto()
{
	// 避免用户重复按下按钮 每次开始时刷新一次变量
	STSTEM_var_refresh();

	findCurrentPlace();
	PrimefileFocus(currentfilePlace, OriginalAllfilePlace);
	Selector(OriginalAllfilePlace, FinalPlaceResult);
	
	// 目录筛选完成，控制权全部交给tree master
	//TreeCenterMaster();

	gui_main();
}

void VisualizeMain::buttonPressed_readfileManu()
{
	// 避免用户重复按下按钮 每次开始时刷新一次变量
	STSTEM_var_refresh();

	ManualfilePlace = inputbox.get_string();
	if (ManualfilePlace.size() > 0)
	{
		infoOut.put("【手动读取】目录已加载");
		cout << ManualfilePlace << endl;
		PrimefileFocus(ManualfilePlace, OriginalAllfilePlace);
		Selector(OriginalAllfilePlace, FinalPlaceResult);
		
		// 目录筛选完成，控制权全部交给tree master
		TreeCenterMaster();
	}
	else
	{
		infoOut.put("【手动读取】目录为空，请重新输入");
	}
	
	gui_main();
}

void VisualizeMain::buttonPressed_refresh()
{
	currentfilePlace = "";
	ManualfilePlace = "";
	infoOut.put("【清空数据】数据清空完成");

	gui_main();
}

void VisualizeMain::buttonPressed_nextRead()
{

	gui_main();
}




void VisualizeMain::findCurrentPlace()
{
	char* buffer;
	//也可以将buffer作为输出参数
	if ((buffer = _getcwd(NULL, 0)) == NULL)
	{
		perror("getcwd error");
		infoOut.put("【自动读取】目录获取失败,请检查操作系统权限！");
	}
	else
	{
		//printf("%s\n", buffer);
		currentfilePlace += buffer;
		free(buffer);
	}

	cout << currentfilePlace << endl;
	infoOut.put("【自动读取】文件目录获取成功");

}

// 初步识别文件
void VisualizeMain::PrimefileFocus(string path, vector<string>& files)
{
	//文件句柄  
	long hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				/*
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					PrimefileFocus(p.assign(path).append("\\").append(fileinfo.name), files);
				*/
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}

	for (int i = 0; i < files.size(); i++)
	{
		cout << files[i] << endl;
	}
}

bool VisualizeMain::if_Select(string& obj)
{
	for (int i = 0; i < obj.size() ; i++)
	{
		if (obj[i] == '\0')
		{
			obj.erase(obj.begin() + i);
		}
	}

	int pointPlace = -1;
	for (int i = 1; i < 5; i++)
	{
		if (obj.size() - i>=0 && obj[obj.size() - i] == '.')
		{
			pointPlace = obj.size() - i;
			break;
		}
	}

	if (pointPlace >= 0 && obj.size() - pointPlace > 0)
	{
		string filesort = obj.substr(pointPlace, obj.size() - pointPlace);
		if (filesort == ".in" || filesort == ".csv" || filesort == ".txt")
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		
		return false;
	}
		
}


void VisualizeMain::Selector(vector<string>& beforeSel, vector<string>& AfterSel)
{
	for (int i = 0; i <= beforeSel.size(); i++)
	{
		string tmpstr = beforeSel[i];
		if (if_Select(tmpstr) == true)
		{
			AfterSel.push_back(beforeSel[i]);
		}
		
	}
	string tmp = "【文件过滤】已经筛选出";
	tmp += to_string(AfterSel.size());
	tmp += "个文件,进入队列";
	infoOut.put(tmp);
	cout << "筛选出" << AfterSel.size() << endl;
}


// function deal with tWO fire in the queue
// before function will be executed, if will Auto check if queue size >= 2
int VisualizeMain::TreeCenterMaster()
{
	if (FinalPlaceResult.size() >= 2)
	{
		Tree1_OriginalData_Get();
		Tree2_OriginalData_Get();

		if (Tree1_iflegal == false || Tree2_iflegal == false)
		{
			cout << "【错误】读取文件内容时出现问题" << endl;
			infoOut.put("【错误】读取文件内容时出现问题");
		}

		/*
		Tree1_OriginalData_Analysis();
		Tree2_OriginalData_Analysis();

		if (Tree1_iflegal == false || Tree2_iflegal == false)
		{
			infoOut.put("【错误】文件解析错误，请检查文件内容");
		}

		Tree1_NameToNode_Build();
		Tree2_NameToNode_Build();

		if (Tree1_iflegal == false || Tree2_iflegal == false)
		{
			infoOut.put("【错误】节点名字空间建立失败，请检查合法性");
			
		}

		Tree1_Build();
		Tree2_Build();

		if (Tree1_iflegal == false || Tree2_iflegal == false)
		{
			infoOut.put("【错误】树的链接建立失败");
			
		}

		Tree1_draw();
		Tree2_draw();

		if (Tree1_iflegal == false || Tree2_iflegal == false)
		{
			infoOut.put("【错误】树的可视化失败");
			
		}

		if (Tree1_iflegal == true || Tree2_iflegal == true)
		{
			if (if_isomorphism() == true)
			{
				infoOut.put("【提示】可视化成功，两棵树同构");
				
			}

			else if (if_isomorphism() == false)
			{
				infoOut.put("【提示】可视化成功，两棵树不同构");
				
			}
		}
		*/
	}
	else
	{
		infoOut.put("队列无可读取文件，请单击清空重新读取文件");
	}

	return gui_main();
}


void VisualizeMain::Tree1_OriginalData_Get()
{
	ifstream tree1_ifs;
	tree1_ifs.open(FinalPlaceResult.front(), ios::in);
	if (tree1_ifs.is_open())
	{
		string buf;
		while (getline(tree1_ifs, buf))
		{
			Tree1_OriginalData.push_back(buf);
			cout << buf << endl;
			buf.clear();
		}

		FinalPlaceResult.erase(FinalPlaceResult.begin());
		cout << "读取结束" << endl;
	}
	else
	{
		cout << "打开失败" << endl;
	}
}


void VisualizeMain::Tree2_OriginalData_Get()
{
	ifstream tree2_ifs;
	tree2_ifs.open(FinalPlaceResult.front(), ios::in);
	if (tree2_ifs.is_open())
	{
		string buf;
		while (getline(tree2_ifs, buf))
		{
			Tree2_OriginalData.push_back(buf);
			cout << buf << endl;
			buf.clear();
		}

		FinalPlaceResult.erase(FinalPlaceResult.begin());
		cout << "读取结束" << endl;
	}
	else
	{
		cout << "打开失败" << endl;
	}
}


void VisualizeMain::Tree1_OriginalData_Analysis()
{
	for (int i = 0; i < Tree1_OriginalData.size(); i++)
	{
		int divideNum = -1;
		for (int j = 0; j < Tree1_OriginalData[i].size() - 1; j++)
		{
			if (Tree1_OriginalData[i][j] == '-' && Tree1_OriginalData[i][j + 1] == '>')
			{
				divideNum = j;
				break;
			}
		}

		if (divideNum <= 0 )
		{
			cout << "找不到dividenum" << endl;
			Tree1_iflegal = false;
			return;
		}
		
		else
		{
			string leftname = Tree1_OriginalData[i].substr(0, divideNum);
			string rightname = Tree1_OriginalData[i].substr(divideNum + 2, Tree1_OriginalData[i].size() - divideNum);
			//cout << "lN" << leftname << "RN" << rightname << endl;
			if (Tree1_RootToSon_NameRelationLeft.count(leftname) == 1)
			{
				if(Tree1_RootToSon_NameRelationRight.count(leftname) == 0)
				{
					Tree1_RootToSon_NameRelationRight[leftname] = rightname;
				}
			}
			
			else if (Tree1_RootToSon_NameRelationLeft.count(leftname) == 0)
			{
				Tree1_RootToSon_NameRelationLeft[leftname] = rightname;
			}
		}
	}
	TmpPrinter_Tree1_RootToSon_NameRelation();
}

void VisualizeMain::Tree2_OriginalData_Analysis()
{
	for (int i = 0; i < Tree2_OriginalData.size(); i++)
	{
		int divideNum = -1;
		for (int j = 0; j < Tree2_OriginalData[i].size() - 1; j++)
		{
			if (Tree2_OriginalData[i][j] == '-' && Tree2_OriginalData[i][j + 1] == '>')
			{
				divideNum = j;
				break;
			}
		}

		if (divideNum <= 0 )
		{
			cout << "找不到dividenum" << endl;
			Tree2_iflegal = false;
			return;
		}

		else
		{
			string leftname = Tree2_OriginalData[i].substr(0, divideNum);
			string rightname = Tree2_OriginalData[i].substr(divideNum + 2, Tree1_OriginalData[i].size() - divideNum);
			//cout << "lN" << leftname << "RN" << rightname << endl;
			if (Tree2_RootToSon_NameRelationLeft.count(leftname) == 1)
			{
				if (Tree2_RootToSon_NameRelationRight.count(leftname) == 0)
				{
					Tree2_RootToSon_NameRelationRight[leftname] = rightname;
				}
			}

			else if (Tree2_RootToSon_NameRelationLeft.count(leftname) == 0)
			{
				Tree2_RootToSon_NameRelationLeft[leftname] = rightname;
			}
		}
	}
	TmpPrinter_Tree2_RootToSon_NameRelation();
}


void VisualizeMain::Tree1_NameToNode_Build()
{

}


void VisualizeMain::Tree2_NameToNode_Build()
{

}

void VisualizeMain::Tree1_Build()
{

}


void VisualizeMain::Tree2_Build()
{

}

void VisualizeMain::Tree1_draw()
{

}


void VisualizeMain::Tree2_draw()
{

}

bool VisualizeMain::if_isomorphism()
{
	return true;
}


// 一大轮结束后 才允许调用！
void VisualizeMain::STSTEM_var_refresh()
{
	currentfilePlace.clear();
	ManualfilePlace.clear();
	OriginalAllfilePlace.clear();					// 保存原始的所有文件的目录 检索支持子目录
	FinalPlaceResult.clear();						// 保存筛选后的最终的 txt，csv，或者其他类型的树文件
}













void VisualizeMain::Tree1_free()
{

}


void VisualizeMain::Tree2_free()
{

}



// 后面的函数属于控制台的临时打印输出
// --------------------------------------------------------------------------------------------------------------------------------
void VisualizeMain::TmpPrinter_Tree1_RootToSon_NameRelation()
{
	for (auto iter = Tree1_RootToSon_NameRelationLeft.begin(); iter != Tree1_RootToSon_NameRelationLeft.end(); iter++)
	{
		cout << iter->first << "\t" << iter->second << endl;
	}

	for (auto iter = Tree1_RootToSon_NameRelationRight.begin(); iter != Tree1_RootToSon_NameRelationRight.end(); iter++)
	{
		cout << iter->first << "\t" << iter->second << endl;
	}

}


void VisualizeMain::TmpPrinter_Tree2_RootToSon_NameRelation()
{
	for (auto iter = Tree2_RootToSon_NameRelationLeft.begin(); iter != Tree2_RootToSon_NameRelationLeft.end(); iter++)
	{
		cout << iter->first << "\t" << iter->second << endl;
	}

	for (auto iter = Tree2_RootToSon_NameRelationRight.begin(); iter != Tree2_RootToSon_NameRelationRight.end(); iter++)
	{
		cout << iter->first << "\t" << iter->second << endl;
	}


}