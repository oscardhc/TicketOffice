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
#include <unistd.h>
#include <mutex>

// 8K per page (There should be NO elements with sizes exceeding 8K)
const int pageSize = 8192;
// Bufferpool size: 8K * 128 = 1M
const int bufferSize = 1450;
const int hotListSize = 100;
const int coldListSize = bufferSize - hotListSize;

enum FOR_FILE{BPT = 0, USER = 1, TRAIN = 2, STATION = 3, TRID = 4, RECORD = 5};

namespace sjtu {

    class IOManager {
    private:
        char tmp[pageSize];
        char pool[bufferSize][pageSize];
        int poolAge;
        int totalQuery, hitQuery;

        int fileCount;
        FILE *file[10];
        int fileSize[10];

        mutex ageLock, idxLock;

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
                truncate(filename, fileSize[i]);
            }
            fprintf(stderr, "TOTAL %u HIT %u RATIO %lf\n", totalQuery, hitQuery, 1.0 * hitQuery / totalQuery);
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
        Node* getBufferId(int index, int flag) {
            totalQuery++;
//            auto it = idxMap.find(index);
            idxLock.lock();
            Node* node = idxMap.search(index);
            idxLock.unlock();
//            if (it != idxMap.end()) {
            if (node != nullptr) {
                hitQuery++;
//                Node* node = it->second;
                ageLock.lock();
                if (flag == 0) moveToFront(node);
                ageLock.unlock();
                return node;
            } else {
                ageLock.lock();
                idxLock.lock();
                Node* node = getAvailableMemory(index);
                ageLock.unlock();
                idxLock.unlock();
                return node;
            }
        }
        void getElement(char *t, int offset, int elementSize, FOR_FILE nFile = BPT) {
            int flag = nFile == USER;
            int beginIndex = offset / pageSize;
            int pagePosition = offset % pageSize;
            int pageLeft = pageSize - pagePosition;
            if (pageLeft >= elementSize) {
                Node *poolid = getBufferId(beginIndex * 10 + nFile, flag);
                memcpy(t, pool[poolid->value] + pagePosition, elementSize);
            } else if (elementSize <= 8192 + pageLeft) {
                Node *poolid1 = getBufferId(beginIndex * 10 + nFile, flag);
                Node *poolid2 = getBufferId((beginIndex + 1) * 10 + nFile, flag);
                memcpy(t, pool[poolid1->value] + pagePosition, pageLeft);
                memcpy(t + pageLeft, pool[poolid2->value], elementSize - pageLeft);
            } else if (elementSize <= 8192 * 2 + pageLeft) {
                Node *poolid1 = getBufferId(beginIndex * 10 + nFile, flag);
                Node *poolid2 = getBufferId((beginIndex + 1) * 10 + nFile, flag);
                Node *poolid3 = getBufferId((beginIndex + 2) * 10 + nFile, flag);
                memcpy(t, pool[poolid1->value] + pagePosition, pageLeft);
                memcpy(t + pageLeft, pool[poolid2->value], 8192);
                memcpy(t + pageLeft + 8192, pool[poolid3->value], elementSize - pageLeft - 8192);
            } else {
                Node *poolid1 = getBufferId(beginIndex * 10 + nFile, flag);
                Node *poolid2 = getBufferId((beginIndex + 1) * 10 + nFile, flag);
                Node *poolid3 = getBufferId((beginIndex + 2) * 10 + nFile, flag);
                Node *poolid4 = getBufferId((beginIndex + 3) * 10 + nFile, flag);
                memcpy(t, pool[poolid1->value] + pagePosition, pageLeft);
                memcpy(t + pageLeft, pool[poolid2->value], 8192);
                memcpy(t + pageLeft + 8192, pool[poolid3->value], 8192);
                memcpy(t + pageLeft + 8192 + 8192, pool[poolid4->value], elementSize - pageLeft - 8192 - 8192);
            }
        }
        int createElement(int elementSize, FOR_FILE nFile = BPT) {
            fileSize[nFile] += elementSize;
            return fileSize[nFile] - elementSize;
        }
        void setElement(char *t, int offset, int elementSize, FOR_FILE nFile = BPT) {
            int flag = nFile == USER;
            int beginIndex = offset / pageSize;
            int pagePosition = offset % pageSize;
            int pageLeft = pageSize - pagePosition;
            if (pageLeft >= elementSize) {
                Node* poolid = getBufferId(beginIndex * 10 + nFile, flag);
                memcpy(pool[poolid->value] + pagePosition, t, elementSize);
                poolid->isEdited = true;
            } else if (elementSize <= 8192 + pageLeft){
                Node *poolid1 = getBufferId(beginIndex * 10 + nFile, flag);
                Node *poolid2 = getBufferId((beginIndex + 1) * 10 + nFile, flag);
                memcpy(pool[poolid1->value] + pagePosition, t, pageLeft);
                memcpy(pool[poolid2->value], t + pageLeft, elementSize - pageLeft);
                poolid1->isEdited = true;
                poolid2->isEdited = true;
            } else  if (elementSize <= 8192 + 8192 + pageLeft) {
                Node *poolid1 = getBufferId(beginIndex * 10 + nFile, flag);
                Node *poolid2 = getBufferId((beginIndex + 1) * 10 + nFile, flag);
                Node *poolid3 = getBufferId((beginIndex + 2) * 10 + nFile, flag);
                memcpy(pool[poolid1->value] + pagePosition, t, pageLeft);
                memcpy(pool[poolid2->value], t + pageLeft, 8192);
                memcpy(pool[poolid3->value], t + pageLeft + 8192, elementSize - pageLeft - 8192);
                poolid1->isEdited = true;
                poolid2->isEdited = true;
                poolid3->isEdited = true;
            } else {
                Node *poolid1 = getBufferId(beginIndex * 10 + nFile, flag);
                Node *poolid2 = getBufferId((beginIndex + 1) * 10 + nFile, flag);
                Node *poolid3 = getBufferId((beginIndex + 2) * 10 + nFile, flag);
                Node *poolid4 = getBufferId((beginIndex + 3) * 10 + nFile, flag);
                memcpy(pool[poolid1->value] + pagePosition, t, pageLeft);
                memcpy(pool[poolid2->value], t + pageLeft, 8192);
                memcpy(pool[poolid3->value], t + pageLeft + 8192, 8192);
                memcpy(pool[poolid4->value], t + pageLeft + 8192 + 8192, elementSize - pageLeft - 8192 - 8192);
                poolid1->isEdited = true;
                poolid2->isEdited = true;
                poolid3->isEdited = true;
                poolid4->isEdited = true;
            }
        }

        int createElementVirt(int elementSize, FOR_FILE nFile = BPT) {
            return createElement(elementSize - 8, nFile);
        }
        void getElementVirt(char *t, int offset, int elementSize, FOR_FILE nFile = BPT) {
            getElement(t + 8, offset, elementSize - 8, nFile);
        }
        void setElementVirt(char *t, int offset, int elementSize, FOR_FILE nFile = BPT) {
            setElement(t + 8, offset, elementSize - 8, nFile);
        }
    };

}

#endif //DEMO1_IOMANAGER_HPP
