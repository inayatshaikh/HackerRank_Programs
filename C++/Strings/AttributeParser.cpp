#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <map>
#include <queue>

using namespace std;


struct Tag {
	string name;
	map<string, string> attributes;
	map<string, Tag*> children;
	Tag* parent;
};

Tag *parse_for_tag(string str) {
	Tag *t = new Tag;
	stringstream ss(str);
	string tagname, attribute, value, eqlsign;
	ss >> tagname;
	tagname.erase(0, 1);
	t->name = tagname;

	if (tagname.find('>') != string::npos)
	{
		tagname.erase(tagname.size() - 1, 1);
		t->name = tagname;
		return t;
	}

	while (ss >> attribute >> eqlsign >> value) {
		value.erase(0, 1);
		value.erase(value.size() - 1, 1);
		if (value.back() == '"') {
			value.erase(value.size() - 1, 1);
		}
		t->attributes[attribute] = value;
	}

	return t;
}

string get_attr(Tag* curr_tag, string attr) {
	if (curr_tag->attributes.count(attr)) {
		return curr_tag->attributes[attr];
	}
	else {
		return "Not Found!";
	}
}

int split_at(string querystr, char literal, string &a, string &b) {
	if (querystr.find(literal) == string::npos)
	{
		a = querystr;
		return -1;
	}
	else {
		b = querystr.substr(querystr.find(literal) + 1, querystr.size() - querystr.find(literal) - 1);
		a = querystr.substr(0, querystr.find(literal));
		return 1;
	}
}


Tag* traverse_to_tag(Tag* root, string path) {

	Tag* curr_tag = root;
	string parta, partb;
    

	
	while (split_at(path, '.', parta, partb) == 1) {
	
		path = partb;

		
		if (curr_tag->children.count(parta))
		{
			curr_tag = curr_tag->children[parta];
		}
	}
	if (curr_tag->children.count(path)) {
		return curr_tag->children[path];
	}
	else {
		return root;
	}
}


string process_query(Tag* tree, string querystr) {
	string attr, path;

	if (split_at(querystr, '~', path, attr) == -1) {
		return "Not Found!";
	}
	else {
		return get_attr(traverse_to_tag(tree, path), attr);
	}
}


int main() {
	int N, Q;
	string temp;
	cin >> N >> Q;
	getline(cin, temp);

	Tag *ptr, *curr_parent;
	Tag *tree;
	tree = new Tag;
	tree->name = "root";
	curr_parent = tree;

	for(int i = 0; i < N; i++) {
		getline(cin, temp);

     
		if ((temp[1] != '/') & (temp.size() > 0)) {
			ptr = parse_for_tag(temp);
			
			ptr->parent = curr_parent;

			curr_parent->children[ptr->name] = ptr;

		
			curr_parent = ptr;
		}
		
    
		if (temp[1] == '/') {
			curr_parent = curr_parent->parent;
		}
	}


	for (int i = 0; i < Q; i++) {
		getline(cin, temp);
		cout << process_query(tree, temp) << endl;
	}
	return 0;
}