#ifndef SKIPNODE
#define SKIPNODE
#include <cstring>

/*
 * 跳表节点类型
 *
*/
template<typename K, typename V>
class Node
{
public:
    Node();                  //无参构造
    Node(const K k,const V v,int);   //有参构造函数
    ~Node();                 //析构函数

    K getKey()const;
    V getValue()const;

    Node<K, V> ** forward;     //指针数组，保存节点各层索引的所指向的节点
    int node_level;            //索引
private:
    K key;          // 节点存储的key值
    V value;


};


template<typename K, typename V>
Node<K, V>::Node()
{

}

template<typename K, typename V>
Node<K,V>::Node(const K k, const V v, int level):key(k),value(v),node_level(level)
{
    
    this->forward = new Node<K,V>*[level + 1];    //
    //level 没有加1 导致内存越界
    memset(this->forward, 0, sizeof(Node<K,V>*)*(level + 1));   //初始化 链接时错误NOt declar in this scope ,把头文件从string改为cstring

}

template<typename K,typename V>
Node<K,V>::~Node()
{
    delete[] this->forward;
}

template<typename K, typename V>
K Node<K,V>::getKey()const
{
    return key;
}

template<typename K, typename V>
V Node<K,V>::getValue()const
{
    return value;
}


#endif // SKIPNODE

