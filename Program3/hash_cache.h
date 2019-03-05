#ifndef HASH_CACHE_H
#define HASH_CACHE_H
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
//#include "node.h"
#define PAGE_SIZE 2000
#define CACHE_SIZE 200
#define DISK_SIZE 2000
#define FAILED -1
using namespace std;

class HashCache {
private:
    class Page;
    class Node;
    Page* entries_;
    vector<Page*> free_entries_;
    unordered_map<string, Node* > hash_map;
    unordered_map<string, int> key_file_map;
    Page* get_new_page();
    Page* put_page;
    bool put_page_existed;
    const char**file_list_;
    int new_file_index_;
    Page *head_, *tail_;
    void detach(Page* page);
    void attach(Page* page);

    Node* search(string key);
    int load_file_to_page(const int &file_num, Page* &page, unordered_map<string, Node* > &hash_map);
    int save_page_to_file(Page* &page);
    void erase_page_of_cache(Page* &page, unordered_map<string, Node* > &hash_map);

    int load_new_file_index(const char* save_file_index = "file_index.dat");
    void save_new_file_index(const char* save_file_index = "file_index.dat");

    void load_key_file_map(const char* key_filename = "key_file.dat");
    void push_key_to_keymap(Page* &page, const char* key_filename = "key_file.dat");
public:
    HashCache(int capacity = CACHE_SIZE);
    ~HashCache();
    void save_cache();  
    string get(string key);
    void put(string key, string value);
    void del(string key);
    string where(string regexkey);

};
#endif
