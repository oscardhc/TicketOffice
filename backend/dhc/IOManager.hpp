//
// Created by Haichen Dong on 2019-05-10.
//

#ifndef DEMO1_IOMANAGER_HPP
#define DEMO1_IOMANAGER_HPP

#include <stdio.h>
#include <string.h>
#include "map.hpp"
#include "utility.hpp"
#include <map>
#include "listMap.hpp"
//#include <filesystem>
#include "../whj/splay_new.hpp"

// 8K per page (There should be NO elements with sizes exceeding 8K)
const int pageSize = 4096;
// Bufferpool size: 8K * 128 = 1M
const int bufferSize = 256;
const int hotListSize = 100;
const int coldListSize = bufferSize - hotListSize;

namespace sjtu {

    class IOManager {
    private:
        char pool[bufferSize][pageSize];
        int poolAge;
        int totalQuery, hitQuery;

        int fileCount;
        FILE *file[10];
        int fileSize[10];

        struct Node {
            int index; // Page id (1,2,3...) in hard drive file
            int value; // Pool of this page
            int age;
            short isEdited;
        };

//        sjtu::map<int, Node*> ageMap, idxMap;
//        void moveToFront(Node* node) {
//            if ((--ageMap.end())->first != node->age) {
//                ageMap.erase(ageMap.find(node->age));
//                node->age = ++poolAge;
//                ageMap.insert({node->age, node});
//            }
//        }

        SplayTree<int, Node*> ageMap, idxMap;

        void moveToFront(Node* node) {
            if (ageMap.getmax() != node->age) {
                ageMap.remove(node->age);
                node->age = ++poolAge;
                ageMap.insert(node->age, node);
            }
        }

    public:
        IOManager(int _cnt) {
            fileCount = _cnt;
            char filename[10];
            for (int i = 0; i < fileCount; i++) {
                sprintf(filename, "data_%d", i);
                FILE *tmp = fopen(filename, "a");
                fclose(tmp);
                file[i] = fopen(filename, "r+");
                int ret = fread((char*)(&fileSize[i]), sizeof(int), 1, file[i]);
                if (ret == 0) {
                    fileSize[i] = sizeof(int);
                    fwrite((char*)(&fileSize[i]), sizeof(int), 1, file[i]);
                }
            }
            totalQuery = hitQuery = 0;
            memset(pool, 0, sizeof(pool));
            for (int i = 0; i < bufferSize; i++) {
                Node *node = new Node();
                node->index = - i - 1;
                node->value = i;
                node->age = ++poolAge;
                node->isEdited = -1;
//                ageMap.insert({node->age, node});
//                idxMap.insert({node->index, node});
                ageMap.insert(node->age, node);
                idxMap.insert(node->index, node);
            }
        }
        ~IOManager() {
//            for (auto it = idxMap.begin(); it != idxMap.end(); it++) {
//                if (it->second->isEdited == 1) {
//                    flushToDisk(it->second);
//                }
//                delete it->second;
//            }
            while (idxMap.size) {
                int key = idxMap.getRootKey();
                Node *node = idxMap.search(key);
                if (node->isEdited == 1) {
                    flushToDisk(node);
                }
                idxMap.remove(key);
                delete node;
            }
            char filename[10];
            for (int i = 0; i < fileCount; i++) {
                sprintf(filename, "data_%d", i);
                fseek(file[i], 0, SEEK_SET);
                fwrite((char*)(&fileSize[i]), sizeof(int), 1, file[i]);
                fprintf(stderr, "END FILESIZE %d: %d\n", i, fileSize[i]);
                fclose(file[i]);
//                std::filesystem::resize_file(filename, fileSize[i]);
            }
            fprintf(stderr, "TOTAL %u HIT %u RATIO %lf\n", totalQuery, hitQuery, 1.0 * hitQuery / totalQuery);
            fprintf(stderr, "time used: %lf s\n", 1. * clock() / 1000000);
        }
        void flushToDisk(Node* node) {
            int nfile = node->index % 10, idx = node->index / 10;
            fseek(file[nfile], idx * pageSize, SEEK_SET);
            fwrite(pool[node->value], pageSize, 1, file[nfile]);
        }
        void readFromDisk(Node* node) {
            fseek(file[node->index % 10], (node->index / 10) * pageSize, SEEK_SET);
            fread(pool[node->value], pageSize, 1, file[node->index % 10]);
        }
        Node* getLastPage() {
//            auto it = ageMap.begin();
            Node* node = ageMap.search(ageMap.getmin());
            if (node->isEdited == 1) {
                flushToDisk(node);
            }
            return node;
        }
        Node* getAvailableMemory(int forIndex) {
            Node* node = getLastPage();
//            ageMap.erase(ageMap.find(node->age));
//            idxMap.erase(idxMap.find(node->index));
            ageMap.remove(node->age);
            idxMap.remove(node->index);
            node->age = ++poolAge;
            node->index = forIndex;
//            ageMap.insert({node->age, node});
//            idxMap.insert({node->index, node});
            ageMap.insert(node->age, node);
            idxMap.insert(node->index, node);
            readFromDisk(node);
            return node;
        }
        Node* getBufferId(int index) {
            totalQuery++;
//            auto it = idxMap.find(index);
            Node* node = idxMap.search(index);
//            if (it != idxMap.end()) {
            if (node != nullptr) {
                hitQuery++;
//                Node* node = it->second;
                moveToFront(node);
                return node;
            } else {
                Node* node = getAvailableMemory(index);
                return node;
            }
        }
        void getElement(char *t, int offset, int elementSize, int nFile = 0) {
            int beginIndex = offset / pageSize;
            int pagePosition = offset % pageSize;
            int pageLeft = pageSize - pagePosition;
            if (pageLeft >= elementSize) {
                Node *poolid = getBufferId(beginIndex * 10 + nFile);
                memcpy(t, pool[poolid->value] + pagePosition, elementSize);
            } else {
                Node *poolid1 = getBufferId(beginIndex * 10 + nFile);
                Node *poolid2 = getBufferId((beginIndex + 1) * 10 + nFile);
                memcpy(t, pool[poolid1->value] + pagePosition, pageLeft);
                memcpy(t + pageLeft, pool[poolid2->value], elementSize - pageLeft);
            }
        }
        int createElement(int elementSize, int nFile = 0) {
            fileSize[nFile] += elementSize;
            return fileSize[nFile] - elementSize;
        }
        void setElement(char *t, int offset, int elementSize, int nFile = 0) {
            int beginIndex = offset / pageSize;
            int pagePosition = offset % pageSize;
            int pageLeft = pageSize - pagePosition;
            if (pageLeft >= elementSize) {
                Node *poolid = getBufferId(beginIndex * 10 + nFile);
                memcpy(pool[poolid->value] + pagePosition, t, elementSize);
                poolid->isEdited = true;
            } else {
                Node *poolid1 = getBufferId(beginIndex * 10 + nFile);
                Node *poolid2 = getBufferId((beginIndex + 1) * 10 + nFile);
                memcpy(pool[poolid1->value] + pagePosition, t, pageLeft);
                memcpy(pool[poolid2->value], t + pageLeft, elementSize - pageLeft);
                poolid1->isEdited = true;
                poolid2->isEdited = true;
            }
        }

        int createElementVirt(int elementSize, int nFile = 0) {
            return createElement(elementSize - 8, nFile);
        }
        void getElementVirt(char *t, int offset, int elementSize, int nFile = 0) {
            getElement(t + 8, offset, elementSize - 8, nFile);
        }
        void setElementVirt(char *t, int offset, int elementSize, int nFile = 0) {
            setElement(t + 8, offset, elementSize - 8, nFile);
        }
    };


    class IOManagerList {
    private:
        FILE *file;
        char pool[bufferSize][pageSize];
        int poolAge;
        int fileSize;
        int totalQuery, hitQuery;

        ListMap list;

    public:
        IOManagerList(const char *filename) {
            init(filename);
        }
        void init(const char *filename) {
            FILE *tmp = fopen(filename, "a");
            fclose(tmp);
            file = fopen(filename, "r+");
            initBuff();
        }
        void initBuff() {
            totalQuery = hitQuery = 0;
            int ret = fread((char*)(&fileSize), sizeof(int), 1, file);

            if (ret == 0) {
                fileSize = sizeof(int);
                fwrite((char*)(&fileSize), sizeof(int), 1, file);
            }

            fprintf(stderr, "INIT FILESIZE %d\n", fileSize);
            memset(pool, 0, sizeof(pool));
            for (int i = 0; i < bufferSize; i++) {
                ListMap::Node* node = new ListMap::Node();

//                std::cerr << "newed " << node << std::endl;

                node->index = - i - 1;
                node->value = i;
                node->isEdited = -1;
                list.insertAtFront(node);
            }
        }

        ~IOManagerList() {
            for (auto it = list.begin; it != list.end; ) {
                if (it->isEdited == 1) {
                    flushToDisk(it);
                }
                auto tmp = it;
                it = it->nxt;
                delete tmp;
            }
            fseek(file, 0, SEEK_SET);
            fwrite((char*)(&fileSize), sizeof(int), 1, file);
            fprintf(stderr, "TOTAL %u HIT %u RATIO %lf\n", totalQuery, hitQuery, 1.0 * hitQuery / totalQuery);
            fprintf(stderr, "time used: %lf s\n", 1. * clock() / 1000000);
        }

        void flushToDisk(ListMap::Node* node) {
            fseek(file, node->index * pageSize, SEEK_SET);
            fwrite(pool[node->value], pageSize, 1, file);
        }
        void readFromDisk(ListMap::Node* node) {
            fseek(file, node->index * pageSize, SEEK_SET);
            fread(pool[node->value], pageSize, 1, file);
        }

        ListMap::Node* getLastPage() {
            auto it = list.getLast();
            if (it->isEdited == 1) {
                flushToDisk(it);
            }
            return it;
        }

        ListMap::Node* getAvailableMemory(int forIndex) {
            ListMap::Node* node = getLastPage();

            node->index = forIndex;
            list.makeYoung(node);

            readFromDisk(node);
            return node;
        }

        ListMap::Node* getBufferId(int index) {
            totalQuery++;
            auto it = list.find(index);
            if (it != list.end) {
                hitQuery++;
                list.makeYoung(it);
                return it;
            } else {
                ListMap::Node* node = getAvailableMemory(index);
                return node;
            }
        }

        void getElement(char *t, int offset, int elementSize, int nFile = 0) {
            int _t = clock();
            int beginIndex = offset / pageSize;
            int pagePosition = offset % pageSize;
            int pageLeft = pageSize - pagePosition;
//            fprintf(stderr, "GET %d %d %d\n", beginIndex, pagePosition, pageLeft);
            if (pageLeft >= elementSize) {
                ListMap::Node *poolid = getBufferId(beginIndex);
                memcpy(t, pool[poolid->value] + pagePosition, elementSize);
            } else {
                ListMap::Node *poolid1 = getBufferId(beginIndex);
                ListMap::Node *poolid2 = getBufferId(beginIndex + 1);
                memcpy(t, pool[poolid1->value] + pagePosition, pageLeft);
                memcpy(t + pageLeft, pool[poolid2->value], elementSize - pageLeft);
            }
        }

        int createElement(int elementSize, int nFile = 0) {
            fileSize += elementSize;
            return fileSize - elementSize;
        }

        void setElement(char *t, int offset, int elementSize, int nFile = 0) {
            int _t = clock();
            int beginIndex = offset / pageSize;
            int pagePosition = offset % pageSize;
            int pageLeft = pageSize - pagePosition;
//            printf("SET %d %d %d\n", beginIndex, pagePosition, pageLeft);
            if (pageLeft >= elementSize) {
                ListMap::Node *poolid = getBufferId(beginIndex);
                memcpy(pool[poolid->value] + pagePosition, t, elementSize);
                poolid->isEdited = true;
            } else {
                ListMap::Node *poolid1 = getBufferId(beginIndex);
                ListMap::Node *poolid2 = getBufferId(beginIndex + 1);
                memcpy(pool[poolid1->value] + pagePosition, t, pageLeft);
                memcpy(pool[poolid2->value], t + pageLeft, elementSize - pageLeft);
                poolid1->isEdited = true;
                poolid2->isEdited = true;
            }
        }

        int createElementVirt(int elementSize, int nFile = 0) {
            return createElement(elementSize - 8);
        }
        void getElementVirt(char *t, int offset, int elementSize, int nFile = 0) {
            getElement(t + 8, offset, elementSize - 8);
        }
        void setElementVirt(char *t, int offset, int elementSize, int nFile = 0) {
            setElement(t + 8, offset, elementSize - 8);
        }

    };
}

#endif //DEMO1_IOMANAGER_HPP
