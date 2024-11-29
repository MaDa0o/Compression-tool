//Function to build the prefix table
void buildPrefTable(BaseNode* root,std::string pref,std::map<char,std::string>& table){
	if(root->isleaf()){
		table[((LeafNode*)root)->value()] = pref;
		return;
	}

	buildPrefTable(((InternalNode*)root)->left(),pref+"0",table);
	buildPrefTable(((InternalNode*)root)->right(),pref+"1",table);

}

BaseNode* BuildTree(std::map<char,int>& mp){
	std::priority_queue<BaseNode*,std::vector<BaseNode*>,Compare> cont;			//Have to make a custom comparator

	for(auto it:mp){
		char el = it.first;
		int freq = it.second;
		LeafNode* l = new LeafNode(el,freq);
		cont.push(l);
	}

	BaseNode* root = NULL;

	while(cont.size()>=2){
		BaseNode* a = cont.top();
		cont.pop();
		BaseNode* b = cont.top();
		cont.pop();
		int nweight = (a->weight())+(b->weight());
		InternalNode* nd = new InternalNode(a,b,nweight);
		root = nd;
		cont.push(nd);
	}
	return root;
}