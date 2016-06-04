#include <map>

#include "MinHeap.h"

using namespace std;

#ifndef _HUFFMAN_TREE_
#define _HUFFMAN_TREE_

#define _BLOCK_SIZE_ 1024
#define _ARRAY_SIZE_ 256
#define _CODE_SIZE_ 16

#define BYTE unsigned char  
#define WORD unsigned short
#define DWORD unsigned int

class Alpha{
public:
	BYTE alpha;
	DWORD count;
	Alpha()
	{
		alpha = 0;
		count = 0;
	}
	Alpha(BYTE c, int n)
	{
		alpha = c;
		count = n;
	}
	~Alpha()
	{
	}
	Alpha* operator+(const Alpha &b)const
	{
		return new Alpha('+', count + b.count);
	}
	bool operator >(Alpha &b){
		return count > b.count;
	}
	bool operator <(Alpha &b){
		return count < b.count;
	}
	bool operator >=(Alpha &b){
		return count >= b.count;
	}
	bool operator <=(Alpha &b){
		return count <= b.count;
	}
	bool operator ==(Alpha &b){
		return count == b.count;
	};
	bool operator ()(BYTE c)
	{
		return alpha == c;
	}//alpha与c是否相同
	Alpha& operator =(const Alpha &b)
	{
		alpha = b.alpha;
		count = b.count;
		return *this;
	}
};

class TreeNode{
public:
	Alpha data;
	TreeNode *parent, *left, *right;
	TreeNode()
	{
		parent = nullptr;
		left = nullptr;
		right = nullptr;
	}
	bool operator >(TreeNode &b){
		return data > b.data;
	}
	bool operator <(TreeNode &b){
		return data < b.data;
	}
	bool operator ==(TreeNode &b){
		return data == b.data;
	}
	TreeNode& operator =(const TreeNode &node)
	{
		data = node.data;
		parent = node.parent;
		left = node.left;
		right = node.right;
		return *this;
	}
};

class HuffmanTree{
public:
	HuffmanTree(Alpha weight[], int n)
	{
		MinHeap<TreeNode> heap(n);
		TreeNode *list = new TreeNode[n];
		for (int i = 0; i < n; i++)
		{
			list[i].data = weight[i];
			list[i].parent = list[i].left = list[i].right = nullptr;
			heap.insert(list[i]);
		}
		TreeNode *parent, *left, *right;
		for (int i = 0; i < n - 1; i++)
		{
			parent = new TreeNode();
			left = new TreeNode();
			right = new TreeNode();
			*left = heap.removeMin();
			*right = heap.removeMin();
			MergeTree(*parent, *left, *right);
			heap.insert(*parent);
			root = parent;
		}
		dumpCode(*root, "");
		seek = root;
		delete[]list;
	}
	~HuffmanTree()
	{
	}
	TreeNode* getRoot()
	{
		return root;
	}
	string code(BYTE b){
		if (codec.find(b) == codec.end()){
			return "";
		}
		return codec[b];
	}
	bool read(BYTE b, Alpha &a){
		if (!seek->left&&!seek->right){ 
			a.alpha = seek->data.alpha;
			seek = b & 0x80 ? root->left : root->right;
			return true;
		}
		seek = b & 0x80 ? seek->left : seek->right;
		return false;
	}
private:
	TreeNode *root, *seek;
	map<BYTE, string> codec;
	void dumpCode(TreeNode &root, string code){
		if (!root.left && !root.right){
			codec[root.data.alpha] = code;
		}
		if (root.left){ dumpCode(*root.left, '1' + code); }
		if (root.right){ dumpCode(*root.right, '0' + code); }
	}//生成各叶节点的编码信息
	void MergeTree(TreeNode &parent, TreeNode &left, TreeNode &right)
	{
		parent.data = *(left.data + right.data);
		parent.left = &left;
		parent.right = &right;
	}//合并树
};

#endif