# SkipList

#### 项目中文件
* main.cpp 包含skiplist.h使用跳表进行数据操作
* skiplist.h 跳表核心实现
* README.md 中文介绍
* store 数据落盘的文件存放在这个文件夹
#### 提供接口
 * int InsertNode(K,V);     //跳表 节点插入
 * bool SerchNode(K);       //数据查找
 * void DeleteNode(K);      //数据删除
 * int  SkipSize() const;   //跳表元素个数
 * void DisplayList();       //打印跳表
 * void dumpFile();          //存取数据到硬盘
 * void loadFile();          //加载文件数据
