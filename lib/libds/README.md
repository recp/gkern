# 🔗 Data Structures and Algorithms

[![Build Status](https://travis-ci.org/recp/ds.svg?branch=master)](https://travis-ci.org/recp/ds)
[![Build status](https://ci.appveyor.com/api/projects/status/yqpyll64woh39a23/branch/master?svg=true)](https://ci.appveyor.com/project/recp/libds/branch/master)
[![Coverage Status](https://coveralls.io/repos/github/recp/libds/badge.svg?branch=master)](https://coveralls.io/github/recp/libds?branch=master)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/50b73a3049b9497a97fe7468a9d79d08)](https://www.codacy.com/app/recp/libds?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=recp/libds&amp;utm_campaign=Badge_Grade)

This library provides common data structures. It will also provide some data structures which needed in render or game engines. In the future I'll try to optimize memmory access. 

There may multiple way to implement a data structure but I tried to implement best way to do that. For instance Red-Black Trees are only implemented as top down insertion/deletion to make it faster. 

There are some convenient constructors for common use cases. For instance `rb_newtree_str()` creates new rb-tree that uses strings as key, `rb_newtree_ptr()` creates new rb-tree that uses pointers as key. When you use these functions to alloc a data structure then you don't need to provide compare or print functions. 

This library prodives allocator api so you can override default allocator. 

## Usage

```C
#include <ds/rb.h> // Red-Black Tree

int
main(int argc, const char * argv[]) {
  RBTree *tree;
  /* ... */

  /* use string keys */
  tree = rb_newtree_str();

  rb_insert(tree, "key", value);

  /* ... */
  value = rb_find(tree, "key");

  rb_destroy(tree);
}

```

## Todo
- [x] rbtree
   - [x] top-down insertion
   - [x] top-down deletion
- [x] forward-list
  - [x] forward-list-separate (reduces FList struct)
- [x] hash-table
   - [x] builtin hash functions e.g. djb2
   - [x] resizing hash table
- [x] quick sort implementation for float, double, i32, u32, i64, u64
- [ ] queue (working on this)
- [ ] stack
- [ ] binary heap / priority queue
- [ ] dynamic array
- [ ] octree
- [ ] quadtree
- [ ] b-tree

## Build

### Unix (Autotools)

```bash
$ sh autogen.sh
$ ./configure
$ make
$ make check # [Optional]
$ [sudo] make install # [Optional]
```

### Windows (MSBuild)
Windows related build, project files are located in `win` folder,
make sure that you are inside in the `libds/win` folder.
Code Analysis are enabled, it may take awhile to finish build

```Powershell
$ cd win
$ .\build.bat
```
if `msbuild` didn't work then you can try to build it with `devenv`:
```Powershell
$ devenv libds.sln /Build Release
```

## License
MIT. check the LICENSE file
