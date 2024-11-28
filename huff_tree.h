class BaseNode{
public:
	virtual bool isleaf() = 0;
	virtual int weight() = 0;
};	

class Compare{
public:
	bool operator()(BaseNode* a, BaseNode* b){
		if(a->weight() > b->weight()){
			return true;
		}
		return false;
	}
};

class LeafNode : public BaseNode{
private:
	char element;
	int w;
public:
	LeafNode(char el, int w){
		element = el;
		this->w = w;
	}

	//getter functions
	char value()
	{return element;}
	int weight() override
	{return w;}

	bool isleaf() override
	{return true;}
};

class InternalNode : public BaseNode{
private:
	int w;
	BaseNode* lft;
	BaseNode* rght;
public:
	InternalNode(BaseNode* l,BaseNode* r, int w){
		lft = l;
		rght = r;
		this->w = w;
	}

	//getter functions
	int weight() override
	{return w;}
	BaseNode* left(){return lft;}
	BaseNode* right(){return rght;}

	bool isleaf() override
	{return false;}

};