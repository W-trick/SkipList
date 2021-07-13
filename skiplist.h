/*
 * @Author: Whao 
 * @Date: 2021-07-03 00:42:31 
 * @Last Modified by: Whao
 * @Last Modified time: 2021-07-03 02:49:29
 */
#ifndef SKIPLIST
#define SKIPLIST
#include "skipnode.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
//跳表

#define STORE "store/dump"
std::string delimiter = ":";


template<typename K, typename V>
class SkipList
{
public:
    SkipList(int);
    ~SkipList();
    int getRadomLevel();    //获取随机索引
    Node<K,V>* CreateNode(K key, V value, int);
    int InsertNode(K,V);     //跳表 节点插入
    bool SerchNode(K);       //数据查找
    void DeleteNode(K);      //数据删除
    int  SkipSize() const;   //跳表元素个数

    void DisplayList();       //打印跳表

    void dumpFile();          //存取数据到硬盘
    void loadFile();          //加载文件数据
    bool isValidString(const std::string&);   //判断字符串是否合法
    void getKeyValue(const std::string& ,std::string*, std::string*);        //

private:
    Node<K,V> * head;           //跳表头指针
    int max_level;              //最大索引层数
    int current_level;          //当前索引高度
    int element_count;          //跳表中元素个数

    std::ofstream dumpfile;       //存取文件
    std::ifstream loadfile;      //加载文件
};

template<typename K, typename V>
SkipList<K,V>::SkipList(int MaxLevel)
{
    max_level = MaxLevel;
    current_level = 0;
    element_count = 0;
    K k;
    V v;
    this->head = new Node<K,V>(k,v,max_level);

}

template<typename K, typename V>
SkipList<K,V>::~SkipList()
{
    delete head;
}


template<typename K, typename V>
Node<K,V>* SkipList<K,V>::CreateNode(K key, V value, int level)
{
    Node<K, V> * node= new Node<K,V>(key, value, level);
    return node;
}

/*
 * 跳表的插入
*/
template<typename K,typename V>
int SkipList<K,V>::InsertNode(K key, V value)
{
    Node<K, V>* current = this->head;     //跳表头指针

    Node<K,V>* upDateNode[max_level + 1];   //保存需要更新索引位置的指针数组

    for(int i = current_level; i>=0; --i)     //找到每级索引需要更新的位置
    {
        //  如果下一个节点不为空，并且下一个节点值小于想要插入的节点的值
        while(current->forward[i] != NULL && current->forward[i]->getKey()<key)
        {
            current = current->forward[i];
        }

        upDateNode[i] = current;     //保存需要插入节点的位置（应该插入到此节点后）
    }

    current = current->forward[0];     //取无索引状态下节点的下一个值，为了好比较值
    if(current!= NULL&&current->getKey() == key)   //当前节点存在无需插入
    {
        std::cout<<"key:"<<key<<"exists"<<std::endl;
        return 1;
    }

    //如果插入位置为链表尾或key值不等情况
    if(current == NULL || current->getKey() != key)
    {
        int random_level = getRadomLevel();    //获取随机需要插入几级索引

        std::cout<<"random_level"<<random_level<<std::endl;
        
        if(random_level > current_level)
        {
            //如果随机索引大于当前索引，更新索引指针数组，保存头指针
             for(int i = current_level + 1; i < random_level + 1; ++i)
             {
                 upDateNode[i] = head;
             }
             current_level = random_level;   //更新当前索引值
        }//如果获取的索引大于当前索引

        //开始写了current_level
        Node<K, V>* insert_node = CreateNode(key, value,random_level);   //创建插入节点


        //从最高层索引插入节点指针,开始写了current_level
        for(int i = random_level; i >= 0;--i)    //此处发生段错误,因为错写成++i
        {
            insert_node->forward[i] = upDateNode[i]->forward[i];    //
            upDateNode[i]->forward[i] = insert_node;
        }

        std::cout<<"insert succed"<<"key = "<<key<<"value"<<value<<std::endl;
        element_count ++;

        return 0;

    }
}

//跳表查询
template<typename K, typename V>
bool SkipList<K,V>::SerchNode(K key)
{
    Node<K,V>* currentNode = head;

    //从最高层开始查找
    for(int i = current_level; i >= 0; --i)
    {
        //当前层节点遍历
        while(currentNode->forward[i] != NULL && currentNode->forward[i]->getKey() < key)
        {
            currentNode = currentNode->forward[i];
        }
    }

    currentNode = currentNode->forward[0]; //返回到第一层
    if(currentNode != NULL && currentNode->getKey() == key)
    {
        std::cout<<"found key succed"<<"value = "<<currentNode->getValue()<<std::endl;
        return true;
    }

    std::cout<<"not found"<<std::endl;

    return false;
}


template<typename K, typename V>
void SkipList<K,V>::DeleteNode(K key)
{
    Node<K,V> *currentNode = head;

    Node<K,V> *update[max_level + 1];

    memset(update, 0, sizeof(Node<K,V>*) * (max_level + 1));

    //从底层遍历找到每层需要删除的索引节点
    for(int i = 0; i < current_level; ++i)
    {
        while(currentNode->forward[i] != NULL && currentNode->forward[i]->getKey() < key)    //大意了！ 开始写了个等于==
        {
            currentNode = currentNode->forward[i];
        }

        update[i] = currentNode;
    }

    currentNode = currentNode->forward[0];  //目标节点
    if(currentNode != NULL && currentNode->getKey() == key)
    {
        //遍历索引，删除在索引中存在的节点
        for(int i = 0; i < current_level ;++i)
        {
            //如果当前层索引没有key节点,跳过此层
            if(update[i]->forward[i] != currentNode)
			{
				break;
			}

            update[i]->forward[i] = currentNode->forward[i];
        }

        //如果当前索引存在，索引内无节点，则删除该层索引
        while(current_level > 0 && head->forward[current_level] == NULL)
        {
            --current_level;
        }

        std::cout<<"succed delete"<<std::endl;

        --element_count;
    }
    return;

}


//元素个数
template<typename K, typename V>
int SkipList<K,V>::SkipSize()const
{
    return element_count;
}

/*
 * 跳表中为保证一级索引占元素的1/2,二级索引占1/4...
 * 确保每次不会重新重建索引，保证每次建立一级索引概率为1/2,..
 * 取整数对2取余，第一次为1/2,第二次为1/4..
 * 且每次在二级索引插入时，一级也会重建索引，依次类推
*/
template<typename K, typename V>
int SkipList<K,V>::getRadomLevel()
{
    int k = 1;   //测试时没有给k赋值，每次均返回level 10 发生错误

    while(rand() % 2)
    {
        ++k;
    }

    k = (k < max_level) ? k : max_level;

    return k;      //返回重建索引
}

template<typename K, typename V>
void SkipList<K,V>::DisplayList()
{
    std::cout<<"****displaySkiplist****"<<std::endl;
    
    for(int i = current_level; i>=0; --i)
    {
        Node<K,V> *currentNode = head->forward[i]; //当前层的头结点
        std::cout<<"level"<<i<<std::endl;
        while(currentNode != NULL)
        {
            std::cout<<"key = "<<currentNode->getKey()<<"vallue = "<<currentNode->getValue()<<std::endl;
            currentNode = currentNode->forward[i];
        }
    
    }
}


// 存取数据到硬盘

template<typename K, typename V>
void SkipList<K, V>::dumpFile()
{
    std::cout<<"----------dumpFile-----------"<<std::endl;
    dumpfile.open(STORE);
    Node<K,V>* node = this->head->forward[0];

    while(node)
    {
        dumpfile<<node->getKey()<<":"<<node->getValue()<<"\n";
        std::cout << node->getKey() << ":" << node->getValue() << ";\n";
        node = node->forward[0];
    }

    dumpfile.flush();
    dumpfile.close();
}

template<typename K, typename V>
void SkipList<K,V>::loadFile()
{
    loadfile.open(STORE);
    std::cout<<"-----------loadFile-------------"<<std::endl;

    std::string line;
    std::string* key = new std::string();
    std::string* value = new std::string();

    while(getline(loadfile, line))
    {
       getKeyValue(line, key, value);
        if (key->empty() || value->empty()) {
            continue;
        }
        InsertNode(*key, *value);
        std::cout << "key:" << *key << "value:" << *value << std::endl;
    }

    loadfile.close();

}


template<typename K, typename V>
void SkipList<K,V>::getKeyValue(const std::string& str,std::string* key, std::string* value)
{
    if(!isValidString(str))
    {
        return;
    }

    *key = str.substr(0, str.find(delimiter));
    *value = str.substr(str.find(delimiter) + 1, str.length());
}

template<typename K, typename V>
bool SkipList<K,V>::isValidString(const std::string & str)   //判断字符串是否合法
{
    if(str.empty())       //字符串为空
    {
        return false;
    }
    if(str.find(delimiter) == std::string::npos)   //find 没有找到
    {
        return false;
    }

    return true;
}


#endif // SKIPLIST

