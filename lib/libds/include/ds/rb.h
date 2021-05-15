/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

/*
 Types:
   struct RBTree;
   struct RBNode;
   typedef void (*RBNodeFn)(RBTree *tree, RBNode *node);
   typedef void (*RBFoundFn)(RBTree *tree, void *key, bool *replace);

 Functions:
   RBTree* rb_newtree(DsAllocator *allocator, DsCmpFn cmp, DsPrintFn print);
   RBTree* rb_newtree_str(void);
   RBTree* rb_newtree_ptr(void);
   void    rb_insert(RBTree *tree, void *key, void *val);
   void    rb_remove(RBTree *tree, void *key);
   void*   rb_find(RBTree *tree, void *key);
   RBNode* rb_find_node(RBTree *tree, void *key);
   int     rb_parent(RBTree *tree, void *key, RBNode ** dest);
   void    rb_print(RBTree *tree);
   void    rb_walk(RBTree *tree, RBNodeFn walkFn);
   int     rb_assert(RBTree *tree, RBNode *root);
   void    rb_empty(RBTree *tree);
   bool    rb_isempty(RBTree *tree);
   void    rb_destroy(RBTree *tree);
 */

#ifndef ds_redblack_h
#define ds_redblack_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "allocator.h"
#include "util.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

struct RBTree;
struct RBNode;

typedef void (*RBNodeFn)(struct RBTree *tree, struct RBNode *node);
typedef void (*RBFoundFn)(struct RBTree *tree, void *key, bool *replace);

typedef struct RBNode {
  void          *key;
  void          *val;
  struct RBNode *chld[2];
  uint8_t        color:1;
} RBNode;

typedef struct RBTree {
  DsAllocator *alc;
  RBNode      *root;
  RBNode      *nullNode;
  void        *userData;
  DsCmpFn      cmp;
  DsPrintFn    print;
  RBNodeFn     onFreeNode;
  RBFoundFn    onFound; /* fires for duplicates */
  size_t       count;
} RBTree;

/*!
 * @brief creates new tree with specific options, you can sent all parameters
 *        as NULL, in this case tree will str versions like rb_newtree_str
 *
 * @param[in]  allocator    custom allocator or NULL for default
 * @param[in]  cmp    custom comparer, check util.h to pick pre-existed one
 *             if you don't need to specific comparer. or NULL for default (str)
 * @param[in]  print  custom allocator or NULL for default (str)
 *
 * @return new tree
 */
DS_EXPORT
RBTree*
rb_newtree(DsAllocator *allocator,
           DsCmpFn      cmp,
           DsPrintFn    print);

/*!
 * @brief pre-defined ctor, it is same with these:
 *        rb_newtree(NULL, NULL, NULL)
 *        rb_newtree(NULL, ds_cmp_str, ds_print_str)
 *
 * @return new tree
 */
DS_EXPORT
RBTree*
rb_newtree_str(void);

/*!
 * @brief pre-defined ctor, it is same with:
 *        rb_newtree(NULL, ds_cmp_ptr, ds_print_ptr)
 *
 * @return new tree
 */
DS_EXPORT
RBTree*
rb_newtree_ptr(void);

/*!
 * @brief inserts new node by specified key
 *
 * @param[in]  tree tree
 * @param[in]  key  key
 * @param[in]  val  value
 */
DS_EXPORT
void
rb_insert(RBTree *tree,
          void   *key,
          void   *val);

/*!
 * @brief removes item by key and frees found node. You can register an event
 *        for catch remoing event to free resources before node getting freed
 *
 * @param[in] tree rbtree
 * @param[in] key  key
 */
DS_EXPORT
void
rb_remove(RBTree *tree,
          void   *key);

/*!
 * @brief find value by key
 *
 * @param[in] tree rbtree
 * @param[in] key  key
 *
 * @return found value or NULL
 */
DS_EXPORT
void*
rb_find(RBTree *tree,
        void   *key);

/*!
 * @brief find node by key. It returns RBnode you can get value from node
 *
 * @param[in] tree rbtree
 * @param[in] key  key
 *
 * @return found RBNode or NULL
 */
DS_EXPORT
RBNode*
rb_find_node(RBTree *tree,
             void   *key);

/*!
 * @brief get parent node of found node (of key). This will lookup for node
 *        then will get parent. Because this rbtree is topdown and doesn't
 *        has parent link to save space.
 *
 *        Example:
 * @code
 * RBNode *parent, *node;
 * int     side;
 *
 * side = rb_parent(tree, "key", &parent);
 * node = parent->chld[side];
 * @endcode
 *
 *
 * @param[in]  tree tree
 * @param[in]  key  key
 * @param[out] dest found node
 *
 * @return side of found node.
 */
DS_EXPORT
int
rb_parent(RBTree  *tree,
          void    *key,
          RBNode **dest);

/*!
 * @brief print tree's keys
 *
 * @param[in]  tree tree
 */
DS_EXPORT
void
rb_print(RBTree *tree);

/*!
 * @brief walks on tree as ordered recursively
 *
 * @param[in]  tree   tree
 * @param[in]  walkFn callback which will called in every node
 */
DS_EXPORT
void
rb_walk(RBTree *tree, RBNodeFn walkFn);

/*!
 * @brief assertion util
 *
 * @param[in] tree  tree
 * @param[in] root  entry point
 */
DS_EXPORT
int
rb_assert(RBTree *tree, RBNode *root);

/*!
 * @brief releases all nodes in tree except tree. You can insert after empty
 *
 * @param[in]  tree tree
 */
DS_EXPORT
void
rb_empty(RBTree *tree);

/*!
 * @brief return true if rbtree is empty
 *
 * @param[in]  tree tree
 *
 * @return is empty or not
 */
DS_EXPORT
bool
rb_isempty(RBTree *tree);

/*!
 * @brief releases all nodes and tree, you cannot use tree after destroying it.
 *        to use tree again you must re-alloc tree
 *
 * @param[in]  tree tree
 */
DS_EXPORT
void
rb_destroy(RBTree *tree);

#ifdef __cplusplus
}
#endif
#endif /* ds_redblack_h */
