

```c++
class IOManager::Node
```

说明：

​	内存池的组成部分，每一个实例代表一个缓存页。

成员变量：

- `int index`

  缓存页对应外存位置

- `int value`

  缓存页对应内存位置

- `int age`

  缓存页对应加入时间

- `short isEdited`

  缓存页在内存中是否被修改过

  若已经修改过则需要在缓存页被替换掉时将修改刷回外存。

```c++
class IOManager
```

全局变量：

- `const int pageSize`

  内存池中缓存页的大小

- `const int bufferSize`

  内存池中缓存页的数量

成员变量：

方法：

- `Node* getBufferId(int index, int flag)`

  参数：

  ​	`index`：外存地址

  ​	`flag`：是否将访问过的节点标记为最新

  为外存地址`index`的缓存页寻找内存位置，若已在内存中，则返回对应节点；否则先执行`getAvailableMemory`创建节点后返回。

- `Node* getAvailableMemory(int forIndex)`