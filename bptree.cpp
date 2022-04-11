#include <bits/stdc++.h>
using namespace std;

 
struct DataNode {
    vector <pair<int, DataNode*>> a;
	int size;
	bool leaf;
	DataNode* rightchild;
};   

struct BplusTree{
    DataNode* root = new DataNode;
    int indexcount, datacount; 
};

int t,d;
BplusTree btree;

void CreateBplusTree (int d, int t){
    btree.root->leaf = true;
    btree.datacount = 0;
    btree.indexcount = 0;
    btree.root->size = 0;
}

DataNode* findParent (DataNode* cursor, DataNode* child){
	DataNode* parent;
	if (cursor->leaf || (cursor->a[0].second)->leaf) {
		return NULL;
	}
	for (int i = 0; i < cursor->size; i++) {
		if (cursor->a[i].second == child) {
			parent = cursor;
			return parent;
		}
		else if(cursor->rightchild == child){
			parent = cursor;
			return parent;
		}
		else {
			parent = findParent(cursor->a[i].second, child);
			if (parent != NULL){
				return parent;
			}
		}
	}
	return parent;
}
void insertInternal(int x, DataNode* cursor, DataNode* newLeaf){
	if(cursor->size < 2*t + 1){
		int i = 0;
		while(x > cursor->a[i].first && i< cursor->size){
			i++;
		}
		if(i < cursor->size){
			
			cursor->a.push_back(pair<int,DataNode*>(x,cursor->a[i].second));
			cursor->a[i].second = newLeaf ;
			sort(cursor->a.begin(),cursor->a.end());
			cursor->size++;
		}
		else if(i == cursor->size){
			cursor->a.push_back(pair<int,DataNode*>(x, cursor->rightchild));
			cursor->rightchild = newLeaf;
			sort(cursor->a.begin(),cursor->a.end());
			cursor->size++;
		}
	}
	else{
		DataNode* newIndex = new DataNode;
		btree.indexcount++;
		vector<pair<int, DataNode*>> virtualNode;
		int j=0;
		for(int j=0;j<cursor->a.size();j++){
			if(cursor->a[j].first > x){
				break;
			}
		}
		if(j == cursor->size -1){
			virtualNode.push_back(pair<int,DataNode*>(x,cursor->rightchild));
			cursor->rightchild = newLeaf;

		}
		else{
			virtualNode.push_back(pair<int, DataNode*>(x,cursor->a[j].second));
			cursor->a[j].second = newLeaf;
		}
		for(int i=0; i<cursor->a.size();i++){
			virtualNode.push_back(cursor->a[i]);
		}
		sort(virtualNode.begin(),virtualNode.end());
		cursor->a.clear();
		cursor->size = t;
		newIndex->leaf = false;
		newLeaf->size = t + 1;
		newIndex->a.clear();

		for(int i =0;i <= t-1;i++){
			cursor->a.push_back(virtualNode[i]);
		}
		cursor->rightchild = virtualNode[t].second;
		int y = virtualNode[t].first;
		for(int i= t+1 ; i <= 2*t+1;i++)
		{
			cursor->a.push_back(virtualNode[i]);
		}
		virtualNode.clear();
		sort(cursor->a.begin(),cursor->a.end());
		sort(newLeaf->a.begin(),newLeaf->a.end());

		if(cursor == btree.root){

			DataNode* newroot = new DataNode;
			btree.indexcount++;
			newroot->a.push_back(pair<int, DataNode*>(y,cursor));
			newroot->leaf = false;
			newroot->rightchild = newIndex;
			newroot->size =1;
			btree.root = newroot;
		}
		else{
			insertInternal(y,findParent(btree.root,cursor),newIndex);
		}
		
	}
}

void insert (int n){
    if(btree.root->leaf && btree.root->size < 2*d){
       
            btree.root->a.push_back(pair<int,DataNode*>(n,nullptr));
			btree.root->size++;
			sort(btree.root->a.begin(),btree.root->a.end());

       
     }
    else{
        DataNode* cursor = btree.root;
        DataNode* parent ;
		while(cursor->leaf == false){
			parent = cursor;
			for(int i=0; i< cursor->size ;i++){
				if(n<cursor->a[i].first){
					cursor = cursor->a[i].second;
					break;
				}
				else if(i == cursor->size-1){
					cursor = cursor->rightchild;
					break;
				}
			}
		}
		if(cursor->size<2*d){
			cursor->a.push_back(pair<int,DataNode*>(n,nullptr));
			sort(cursor->a.begin(), cursor->a.end());
			cursor->size++;
		}
		else{
			DataNode* newLeaf = new DataNode;
			btree.datacount++;
			vector<int> virtualNode;
			//update cursor to virtual node created
			for(int i=0;i<cursor->a.size();i++){
				virtualNode.push_back(cursor->a[i].first);
			}
			// cursor->a.clear();
			virtualNode.push_back(n);

			sort(virtualNode.begin() , virtualNode.end());
			newLeaf->leaf = true;

			cursor->a.clear();
			newLeaf->a.clear();
			
			newLeaf->size = d +1;
			cursor->size = d;

			// for(int i=0;i<=d-1;i++){
			// 	cursor->a.push_back(pair<int,DataNode*>(virtualNode[i],nullptr));
			// }
			// for(int i=d;i<=2*d;i++){
			// 	newLeaf->a.push_back(pair<int,DataNode*>(virtualNode[i],nullptr));
			// }

			// virtualNode.clear();
			for(int i = 2*d; i >= d; i--){
				newLeaf->a.push_back(pair<int,DataNode*>(virtualNode[i],nullptr));
				virtualNode.pop_back();
			}
            for(int i = d - 1; i >= 0; i--){
                cursor->a.push_back(pair<int,DataNode*>(virtualNode[i],nullptr));
				virtualNode.pop_back();
			}

			sort(cursor->a.begin(),cursor->a.end());
			sort(newLeaf->a.begin(),newLeaf->a.end());

			//IF CURSOR IS THE ROOT NODE
			if(cursor == btree.root){
				DataNode* newRoot = new DataNode;
				btree.indexcount++;
				newRoot->a.push_back(pair<int, DataNode*>(newLeaf->a[0].first,cursor));
				newRoot->rightchild=newLeaf;
				newRoot->leaf==  false;
				newRoot->size = 1;
				btree.root = newRoot;
			}
			else{
				insertInternal(newLeaf->a[0].first,parent,newLeaf);
			}
		}
	}
	
}
 
void display(DataNode* cursor){
 
	cout<<btree.indexcount<<" "<<btree.datacount<<" ";
	for(int i = 0;i < btree.root->size; i++){
		cout<<btree.root->a[i].first<<" ";
	} 
}

int main(){
    cin>>d>>t;
   
    int choice;
	int n; 
	CreateBplusTree(d,t);
	while(1)
	{
		cin>>choice;
		switch(choice)
		{
			case 1:
				cin>>n;
				insert(n);
				break;
			case 2:
				display(btree.root);
				break;
			case 3:
				exit(0);
			default:
				cout<<"Invalid Input"<<endl;
		}
	}
}