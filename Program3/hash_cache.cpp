#include "hash_cache.h"
#include <iostream>
#include <algorithm>
//#include <hash_map>
#include <regex>

using namespace std;
bool file_accessed[DISK_SIZE] = { false };

class HashCache::Node {
public:
    std::string key_;
    std::string value_;
    class Page* page_;
};


class HashCache::Page {
public:
    int file_num_;
    bool lock_;
    bool dirty_;
    bool is_putpape;
    class Node data_[PAGE_SIZE];
    class Page* next;
    class Page* prev;
    Page() {
        lock_ = false;
        dirty_ = false;
        is_putpape = false;
    }
};

HashCache::HashCache(int capacity) {
    new_file_index_ = load_new_file_index();
    load_key_file_map();
    put_page = NULL;
    put_page_existed = false;
    entries_ = new Page[capacity];
    for (int i = 0; i < capacity; i++) {
        for (int j = 0; j < PAGE_SIZE; j++)
            (entries_ + i)->data_[j].page_ = entries_ + i;
        free_entries_.push_back(entries_ + i);
    }
    
    head_ = new Page;
    tail_ = new Page;
    head_->prev = NULL;
    head_->next = tail_;
    tail_->prev = head_;
    tail_->next = NULL;
}
HashCache::~HashCache() {
    save_cache();
    delete head_;
    delete tail_;
}

HashCache::Node* HashCache::search(string key) {
    Node* node = hash_map[key];
    Page* page = NULL;
    if (node) {
        page = node->page_;
        detach(page);
        attach(page);
    }
    else {
    	int file_num = key_file_map[key];
    	if (file_num == 0)
    		return node;
        page = get_new_page();
        if (load_file_to_page(file_num, page, hash_map) == FAILED) {
            free_entries_.push_back(page);
        }
        if (node = hash_map[key]) {
            attach(page);
        }
    }
    return node;
}

string HashCache::where(string regexkey){
    std::regex express(regexkey);
    unordered_map<string, Node*>::iterator it=hash_map.begin();
    string a;
    string abc;
    int i=0;
    for(;it!=hash_map.end();++it){
        if (std::regex_match(it->first, express)){
            cout<<"key:"<<it->first<<" value:"<<it->second->value_<<endl;
            a = "key:"+it->first+" value:"+it->second->value_+"\n";
            if(i>0)
                abc = abc + a;
            else
                abc=a;
            i++;
        }
    }
    return abc;
}

string HashCache::get(string key) {
    Node* node = search(key);
    if (node)
        return node->value_;
    return string();
}


void HashCache::del(string key){
    Node* node = search(key);
    if(node)
        hash_map.erase(key);
    return;
}

void HashCache::put(string key, string value) {
    Node* node = NULL;
    node = search(key);
    if (node) {
        if (value != node->value_) {
            node->value_ = value;
            node->page_->dirty_ = true;
        }
        return;
    }
    else {
        static int data_index = 0;
        if (data_index == PAGE_SIZE || put_page_existed == false) {
        	push_key_to_keymap(put_page);
            data_index = 0;
        }
        if (data_index == 0 || put_page_existed == false) {
            put_page = get_new_page();
            put_page_existed = true;
            put_page->file_num_ = ++new_file_index_;
            put_page->dirty_ = true;
            for (int i = 0; i < PAGE_SIZE; i++) {
                put_page->data_[i].key_ = "";
                put_page->data_[i].value_ = "";
            }
            attach(put_page);
        }
        put_page->data_[data_index].key_ = key;
        put_page->data_[data_index].value_ = value;
        hash_map[key] = put_page->data_ + data_index;
        data_index++;
        detach(put_page);
        attach(put_page);
    }
}

int HashCache::load_file_to_page(const int &file_num, Page* &page, unordered_map<string, Node* > &hash_map) {
    ifstream input;
    string file_name = to_string(file_num) + ".dat";
    input.open(file_name, ios::in);
    if (!input || page == NULL)
        return FAILED;
    file_accessed[file_num] = true;
    page->file_num_ = file_num;
    Node* data = page->data_;
    for (int i = 0; i < PAGE_SIZE; i++) {
        if (!(input >> data[i].key_ >> data[i].value_))
            data[i].key_ = "", data[i].value_ = "";
        hash_map[data[i].key_] = data + i;
    }
    input.close();
    return 0;
}
int HashCache::save_page_to_file(Page* &page) {
    page->dirty_ = false;
    if (page == NULL)
        return FAILED;
    ofstream output;
    string file_name = to_string(page->file_num_) + ".dat";
    output.open(file_name, ios::out);
    Node* data = page->data_;
    for (int i = 0; i < PAGE_SIZE; i++) {
        output << data[i].key_ << " " << data[i].value_ << endl;
    }
    output.close();
    return 0;
}

int HashCache::load_new_file_index(const char* save_file_index) {
    ifstream input;
    input.open(save_file_index, ios::in);
    if (!input) {
        return 0;
    }
    int index;
    input >> index;
    input.close();
    return index;
}
void HashCache::save_new_file_index(const char* save_file_index) {
    ofstream output;
    output.open(save_file_index, ios::out);
    if (!(output << new_file_index_)) {
        cout << "fail to save ：write <" << save_file_index << "> failed." << endl;
        exit(-1);
    }
    output.close();
}

void HashCache::erase_page_of_cache(Page* &page, unordered_map<string, Node* > &hash_map) {
    file_accessed[page->file_num_] = false;
    for (int i = 0; i < PAGE_SIZE; i++) {
        hash_map.erase(page->data_[i].key_);
    }
}

void HashCache::save_cache() {
    Page* page = head_->next;
    while (page != tail_) {
        if (page->dirty_ == true) {
            save_page_to_file(page);
        }
        page = page->next;
    }
    save_new_file_index();
    push_key_to_keymap(put_page);
}

void HashCache::detach(Page* page) {
    page->prev->next = page->next;
    page->next->prev = page->prev;
}
void HashCache::attach(Page* page) {
    page->prev = head_;
    page->next = head_->next;
    head_->next->prev = page;
    head_->next = page;
}

HashCache::Page* HashCache::get_new_page() {
    Page* page;
    if (free_entries_.empty() == true) {
        page = tail_->prev;
        if (page == put_page) {
            put_page_existed = false;
        	push_key_to_keymap(page);
        }
        if (page->dirty_ == true)
            save_page_to_file(page);
        erase_page_of_cache(page, hash_map);
        detach(page);
        free_entries_.push_back(page);
    }
    page = free_entries_.back();
    free_entries_.pop_back();
    return page;
}
void HashCache::push_key_to_keymap(Page* &page, const char* key_filename) {
	if (page == NULL)
		return;
	ofstream output;
	output.open(key_filename, ios::app);

	Node* data = page->data_;
	int file_num = page->file_num_;
	for (int i = 0; i < PAGE_SIZE; i++) {
		key_file_map[data[i].key_] = file_num;
		output << data[i].key_ << " " << file_num << endl;
	}
	output.close();
}
void HashCache::load_key_file_map(const char* key_filename) {
	ifstream input;
	input.open(key_filename, ios::in);
	if (!input)
		return;
	string key;
	int file_num;
	while (input >> key >> file_num) {
		key_file_map[key] = file_num;
	}
	input.close();
}
