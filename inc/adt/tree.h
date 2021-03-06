/*********************************************************************
 * cclib
 *
 * Copyright (c) 2019 cclib contributors:
 *   - hello_chenchen <https://github.com/hello-chenchen>
 *
 * MIT License <https://github.com/cc-libraries/cclib/blob/master/LICENSE>
 * See https://github.com/cc-libraries/cclib for the latest update to this file
 *
 * author: hello_chenchen <https://github.com/hello-chenchen>
 **********************************************************************************/
/************************
 * NOTICE: Binary search tree
 * it is a binary tree;
 * each node contains a value;
 * a total order is defined on these values (every two values can be compared with each other);
 * left subtree of a node contains only values lesser, than the node's value;
 * right subtree of a node contains only values greater, than the node's value.
*************************/
/************************
 * NOTICE: Red Black tree
 * Every node has a color either red or black.
 * Root of tree is always black.
 * There are no two adjacent red nodes (A red node cannot have a red parent or red child).
 * Every path from a node (including root) to any of its descendant NULL node has the same number of black nodes.
************************/
#ifndef CCLIB_ADT_TREE_H
#define CCLIB_ADT_TREE_H
#include "./../../cclib-common/inc/base/common_define.h"
#include <iostream>

namespace cclib
{
    namespace adt
    {
        enum RedBlackColor {EN_Red, EN_Black};
        enum RedBlackDirection{EN_Left, EN_Right};

        template<typename Comparable>
        struct RedBlackNode {
            Comparable _data;
            RedBlackColor _color;
            RedBlackNode* _parent;
            RedBlackNode* _leftChild;
            RedBlackNode* _rightChild;

            RedBlackNode(): _data(Comparable()), _parent(CC_NULL), _leftChild(CC_NULL), _rightChild(CC_NULL), _color(EN_Black) {}
            RedBlackNode(const Comparable& data, RedBlackColor color, RedBlackNode* parent, RedBlackNode* leftChild, RedBlackNode* rightChild):
             _data(data), _color(color), _parent(parent), _leftChild(leftChild), _rightChild(rightChild) {}

            RedBlackNode* grandParent() {
                if(CC_NULL == this->_parent) {
                    return CC_NULL;
                }

                return this->_parent->_parent;
            }

            RedBlackNode* uncle() {
                if(CC_NULL == grandParent()) {
                    return CC_NULL;
                }

                return this->_parent == grandParent()->_rightChild ? grandParent()->_leftChild : grandParent()->_rightChild;
            }

            RedBlackNode* sibling() {
                return this->_parent->_leftChild == this ? this->_parent->_rightChild : this->_parent->_leftChild;
            }

            RedBlackNode* self() {
                return this->_parent->_leftChild == this ? this->_parent->_leftChild : this->_parent->_rightChild;
            }

            RedBlackDirection direction() {
                return this->_parent->_leftChild == this ? EN_Left : EN_Right;
            }

            // bool operator==(RedBlackNode<Comparable>*& node) const {
            //     return (this->_color == node->_color) && (this->_parent == node->_parent) && (this->_data == node->_data)
            //     && (this->_leftChild == node->_leftChild) && (this->_rightChild == node->_rightChild);
            // }

            // bool operator!=(RedBlackNode<Comparable>*& node) const {
            //     return (this->_color != node->_color) || (this->_parent != node->_parent) || (this->_data != node->_data)
            //     || (this->_leftChild != node->_leftChild) || (this->_rightChild != node->_rightChild);
            // }
        };

        template<typename Comparable>
        class RedBlackTreeIterator {
            public:
                typedef RedBlackTreeIterator<Comparable> _Self;

            public:
                RedBlackTreeIterator(RedBlackNode<Comparable>* node, RedBlackNode<Comparable>* nil) {
                    _M_node = node;
                    _M_Nil = nil;
                }

                Comparable operator*() {
                    return  CC_NULL == _M_node ? CC_NULL : _M_node->_data;
                }

                _Self operator++(int) {
                    increment();
                    return *this;
                }

                _Self operator++() {
                    _Self temp = *this;
                    increment();
                    return temp;
                }

                void increment()
                {
                    if (_M_node->_rightChild != _M_Nil) {
                        _M_node = _M_node->_rightChild;
                        while (_M_node->_leftChild != _M_Nil) {
                            _M_node = _M_node->_leftChild;
                        }
                    } else {
                        RedBlackNode<Comparable>* parent = _M_node->_parent;
                        while (_M_node == parent->_rightChild) {
                            _M_node = parent;
                            parent = parent->_parent;
                        }

                        if (_M_node->_rightChild != parent) {
                            _M_node = parent;
                        }
                    }
                }

                _Self operator--(int) {
                    decrement();
                    return *this;
                }

                _Self operator--() {
                    _Self temp = *this;
                    decrement();
                    return temp;
                }

                void decrement()
                {
                    if (_M_node->_color == EN_Red &&
                        _M_node->grandParent() == _M_node) {    //TODO: case 1?
                        _M_node = _M_node->_rightChild;
                    } else if (_M_node->_leftChild != _M_Nil) {
                        RedBlackNode<Comparable>* leftNode = _M_node->_leftChild;
                        while (leftNode->_rightChild != _M_Nil) {
                            leftNode = leftNode->_rightChild;
                        }
                        _M_node = leftNode;
                    } else {
                        RedBlackNode<Comparable>* parent = _M_node->_parent;
                        while (_M_node == parent->_leftChild) {
                            _M_node = parent;
                            parent = parent->_parent;
                        }
                        _M_node = parent;
                    }
                }

            private:
                RedBlackNode<Comparable> *_M_node, *_M_Nil;
        };

        template<typename Comparable>
        class RedBlackTree {
            public:
                typedef RedBlackTreeIterator<Comparable> iterator;

            public:
                RedBlackTree(): _size(0), _M_header(CC_NULL), _M_Nil(CC_NULL) {
                    this->_M_Nil = new RedBlackNode<Comparable>();
                    this->_M_header = _M_Nil;
                    this->_M_header->_parent = _M_Nil;
                    this->_M_header->_leftChild = _M_Nil;
                    this->_M_header->_rightChild = _M_Nil;
                }
                // RedBlackTree(const Comparable& data) {}
                // RedBlackTree(const RedBlackTree& instance) {}
                ~RedBlackTree() {
                    clear();
                }

                // const RedBlackTree& operator=(const RedBlackTree& instance) {}
                RedBlackNode<Comparable>* nil() const {
                    return this->_M_Nil;
                }

                iterator begin() {
                    RedBlackNode<Comparable>* minNode = findMin(_M_header);
                    return iterator(minNode, _M_Nil);
                }

                iterator end() {
                    RedBlackNode<Comparable>* maxNode = findMax(_M_header);
                    return iterator(maxNode, _M_Nil);
                }

                bool empty() const {
                    return 0 == _size ? true : false;
                }

                cc_size_t size() {
                    return _size;
                }

                bool clear() {
                    while(CC_INIT != _size) {
                        remove(this->_M_header);
                    }
                    return true;
                }

                void remove(const Comparable& data) {
                    remove(data, this->_M_header);
                }

                void printHeader() {}

                void remove(RedBlackNode<Comparable>*& node) {
                    RedBlackNode<Comparable>* replaceNode = _M_Nil;
                    RedBlackNode<Comparable>* realRemoveNode = node;
                    RedBlackColor removeColor = realRemoveNode->_color;
                    if(node->_leftChild == _M_Nil) {
                        replaceNode = node->_rightChild;
                        transplant(node, node->_rightChild);
                    } else if(node->_rightChild == _M_Nil) {
                        replaceNode = node->_leftChild;
                        transplant(node, node->_leftChild);
                    } else {
                        realRemoveNode = findMin(node->_rightChild);
                        removeColor = realRemoveNode->_color;
                        if(realRemoveNode->_parent == node) {
                            replaceNode->_parent = realRemoveNode;
                        } else {
                            transplant(realRemoveNode, realRemoveNode->_rightChild);
                            realRemoveNode->_rightChild = node->_rightChild;
                            realRemoveNode->_rightChild->_parent = realRemoveNode;
                        }
                        transplant(node, realRemoveNode);
                        realRemoveNode->_leftChild = node->_leftChild;
                        realRemoveNode->_leftChild->_parent = realRemoveNode;
                        realRemoveNode->_color = node->_color;
                        if(EN_Black == removeColor) {
                            deleteFixUp(replaceNode);
                        }
                    }
                    --_size;
                    return;
                }

                bool insert(const Comparable& data) {
                    RedBlackNode<Comparable>* node = new RedBlackNode<Comparable>(data, EN_Red, _M_Nil, _M_Nil, _M_Nil);
                    bool result = false;
                    result = insert(node);

                    if(result) {
                        insertFixUp(node);
                    }

                    return result;
                }

                RedBlackNode<Comparable>* find(const Comparable& data) {
                    return find(data, this->_M_header);
                }

            private:
                RedBlackNode<Comparable>* find(const Comparable& data, RedBlackNode<Comparable>*& root) {
                    if(_M_Nil == root) {   //not found
                        return _M_Nil;
                    }

                    if(data < root->_data) {
                        return find(data, root->_leftChild);
                    } else if(data > root->_data) {
                        return find(data, root->_rightChild);
                    } else {    //data == root->_data
                        return root;
                    }
                }

                RedBlackNode<Comparable>* findMin(RedBlackNode<Comparable>* root) const {
                    if(_M_Nil == root) {
                        return _M_Nil;
                    }

                    while(root->_leftChild != _M_Nil) {
                        root = root->_leftChild;
                    }

                    return root;
                }

                RedBlackNode<Comparable>* findMax(RedBlackNode<Comparable>* root) const {
                    if(_M_Nil == root) {
                        return _M_Nil;
                    }

                    while(root->_rightChild != _M_Nil) {
                        root = root->_rightChild;
                    }

                    return root;
                }

                void remove(const Comparable& data, RedBlackNode<Comparable>*& root) {
                    RedBlackNode<Comparable>* node = find(data, root);

                    if(_M_Nil == node) return;

                    remove(node);
                }

                void transplant(RedBlackNode<Comparable>* originalNode, RedBlackNode<Comparable>* replaceNode) {
                    if(_M_Nil == originalNode->_parent) {
                        _M_header = replaceNode;
                    } else if(EN_Left == originalNode->direction()) {
                        originalNode->_parent->_leftChild = replaceNode;
                    } else {
                        originalNode->_parent->_rightChild = replaceNode;
                    }

                    replaceNode->_parent = originalNode->_parent;
                }

                void deleteFixUp(RedBlackNode<Comparable>*& node) {
                    while(node != _M_header && node->_color == EN_Black) {
                        if(EN_Left == node->direction()) {    //case 1
                            RedBlackNode<Comparable>* uncle = node->_parent->_rightChild;
                            if(uncle->_color == EN_Red) {
                                uncle->_color = EN_Black;
                                node->_parent->_color = EN_Red;
                                rotateLeft(node->_parent);
                                uncle = node->_parent->_rightChild;
                            }   //NOTICE: to case 2
                            //case 2 FIXME: if node is black and uncle is black, does uncle has black child?
                            if((uncle->_leftChild->_color == EN_Black) &&
                            (uncle->_rightChild->_color == EN_Black)) {   //NOTICE: uncle->_color == EN_Black
                                uncle->_color = EN_Red;
                                node = node->_parent;   //NOTICE: repeat while until node == _M_header
                            } else if(uncle->_rightChild->_color == EN_Black) { //case 3
                                uncle->_leftChild->_color = EN_Black;
                                uncle->_color = EN_Red;
                                rotateRight(uncle);
                                uncle = node->_parent->_rightChild; //NOTICE: repeat while to case 4
                            } else {    //case 4
                                uncle->_color = node->_parent->_color;
                                node->_parent->_color = EN_Black;
                                uncle->_rightChild->_color = EN_Black;
                                rotateLeft(node->_parent);
                                node = _M_header;
                            }
                        } else {    //FIXME: the same with left?
                            RedBlackNode<Comparable>* uncle = node->_parent->_leftChild;
                            if(uncle->_color == EN_Red) {
                                uncle->_color = EN_Black;
                                node->_parent->_color = EN_Red;
                                rotateRight(node->_parent);
                                uncle = node->_parent->_leftChild;
                            }   //NOTICE: to case 2
                            //case 2
                            if((uncle->_leftChild->_color == EN_Black) &&
                            (uncle->_rightChild->_color == EN_Black)) {   //NOTICE: uncle->_color == EN_Black
                                uncle->_color = EN_Red;
                                node = node->_parent;   //NOTICE: repeat while until node == _M_header
                            } else if(uncle->_leftChild->_color == EN_Black) { //case 3
                                uncle->_rightChild->_color = EN_Black;
                                uncle->_color = EN_Red;
                                rotateLeft(uncle);
                                uncle = node->_parent->_rightChild; //NOTICE: repeat while to case 4
                            } else {    //case 4
                                uncle->_color = node->_parent->_color;
                                node->_parent->_color = EN_Black;
                                uncle->_leftChild->_color = EN_Black;
                                rotateRight(node->_parent);
                                node = _M_header;
                            }
                        }
                    }
                    node->_color = EN_Black;
                }

                void insertFixUp(RedBlackNode<Comparable>* node) {
                    while(EN_Red == node->_parent->_color) {
                        if(EN_Left == node->_parent->direction()) {
                            RedBlackNode<Comparable>* uncle = node->grandParent()->_rightChild;
                            if(EN_Red == uncle->_color) {
                                node->_parent->_color = EN_Black;
                                uncle->_color = EN_Black;
                                node->grandParent()->_color = EN_Red;
                                node = node->grandParent();
                            } else if(node == node->_parent->_rightChild) {
                                node = node->_parent;
                                rotateLeft(node);
                            } else{
                                node->_parent->_color = EN_Black;
                                node->grandParent()->_color = EN_Red;
                                rotateRight(node->grandParent());
                            }

                        } else {
                            RedBlackNode<Comparable>* uncle = node->grandParent()->_leftChild;
                            if(EN_Red == uncle->_color) {
                                node->_parent->_color = EN_Black;
                                uncle->_color = EN_Black;
                                node->grandParent()->_color = EN_Red;
                                node = node->grandParent();
                            } else if(node == node->_parent->_leftChild) {
                                node = node->_parent;
                                rotateRight(node);
                            } else {
                                node->_parent->_color = EN_Black;
                                node->grandParent()->_color = EN_Red;
                                rotateLeft(node->grandParent());
                            }
                        }
                    }
                    _M_header->_color = EN_Black;
                }

                bool insert(RedBlackNode<Comparable>* node) {
                    RedBlackNode<Comparable>* insertNode = _M_Nil;
                    RedBlackNode<Comparable>* rootNode = _M_header;

                    while(rootNode != _M_Nil) {
                        insertNode = rootNode;
                        rootNode = node->_data < rootNode->_data ? rootNode->_leftChild : rootNode->_rightChild;
                    }

                    node->_parent = insertNode;
                    ++_size;
                    if(insertNode == _M_Nil) {
                        _M_header = node;
                    } else if(node->_data < insertNode->_data) {
                        insertNode->_leftChild = node;
                    } else if(node->_data > insertNode->_data) {
                        insertNode->_rightChild = node;
                    } else {
                        --_size;
                        return false;
                    }

                    return true;
                }

                void rotateRight(RedBlackNode<Comparable>* y) {
                    RedBlackNode<Comparable>* x = y->_leftChild;
                    y->_leftChild = x->_rightChild; //1. rotate x child

                    if(_M_Nil != x->_rightChild) { //2. rotate child parent
                        x->_rightChild->_parent = y;
                    }

                    x->_parent = y->_parent;    //3. rotate x parent

                    //rotate y parent child
                    if(_M_Nil == y->_parent) {
                        _M_header = x;
                    } else if(y->_parent->_leftChild == y) {
                        y->_parent->_leftChild = x;
                    } else {
                        y->_parent->_rightChild = x;
                    }

                    //5. rotate two node
                    x->_rightChild = y;
                    y->_parent = x;
                }

                void rotateLeft(RedBlackNode<Comparable>* x) {
                    RedBlackNode<Comparable>* y = x->_rightChild;
                    x->_rightChild = y->_leftChild; //1. rotate y child

                    if(_M_Nil != y->_leftChild) {
                        y->_leftChild->_parent = x;    //2. rotate child parent
                    }

                    y->_parent = x->_parent;    //3. rotate y parent

                    if(_M_Nil == x->_parent) { //4. rotate x parent child
                        _M_header = y;
                    } else if(x == x->_parent->_leftChild) {
                        x->_parent->_leftChild = y;
                    } else {
                        x->_parent->_rightChild = y;
                    }

                    //5. rotate two node
                    y->_leftChild = x;  //y child
                    x->_parent = y; //x parent
                }

                //DEPRECATED:
                void deleteNode(RedBlackNode<Comparable>*& node) {
                    // node->_data.~Comparable();  //TODO: destruct?
                    delete node;
                    node = _M_Nil;
                }

                //DEPRECATED:
                void removeFixUp(RedBlackNode<Comparable>*& node) {
                    if(_M_Nil == node) {
                        --_size;
                        return;
                    }

                    //NOTICE:_M_Nil == node->_leftChild
                    if(EN_Red == node->_color) {
                        deleteNode(node);
                        --_size;
                        return;
                    }

                    //EN_Black == node->_color
                    if(_M_Nil != node->_rightChild) {   //EN_Red == node->_rightChild->_color
                        node->_data = node->_rightChild->_data;
                        removeFixUp(node->_rightChild);
                    }

                    if(EN_Left == node->direction()) {
                        RedBlackNode<Comparable>* uncle = node->uncle();
                        if(EN_Red == uncle->_color) {
                            uncle->_color = EN_Black;
                            node->_parent->_color = EN_Red;
                            rotateLeft(node->_parent);
                            uncle = node->_parent->_rightChild;
                        }
                        if(uncle->_leftChild->_color == EN_Black && uncle->_rightChild->_color == EN_Black) {   //TODO: ?
                            uncle->_color = EN_Red;
                            node = node->_parent;
                        }
                    }
                }

            private:
                cc_size_t _size;
                RedBlackNode<Comparable> *_M_header;
                RedBlackNode<Comparable> *_M_Nil;
        };

        template<typename Comparable>
        struct BinaryNode {
            Comparable _data;
            BinaryNode* _leftChild;
            BinaryNode* _rightChild;
            BinaryNode(const Comparable& data, BinaryNode* leftChild, BinaryNode* rightChild)
            : _data(data), _leftChild(leftChild), _rightChild(rightChild) {}
        };

        template<typename Comparable>
        class BinarySearchTree {
            public:
                BinarySearchTree(): _size(0), _M_node(CC_NULL) {}
                // BinarySearchTree(const BinarySearchTree& instance) {}
                ~BinarySearchTree() {
                    clear();
                }

            public:
                bool contains(const Comparable& data) {
                    return preOrderTraversalHandle(data, this->_M_node);
                }

                BinaryNode<Comparable>* begin() {
                    return this->_M_node;
                }

                bool insert(const Comparable& data) {
                    bool result = insert(data, this->_M_node);
                    return result;
                }

                bool remove(const Comparable& data) {
                    remove(data, this->_M_node);
                }

                Comparable find(const Comparable& key) {  //TODO: change to callback
                    return find(key, this->_M_node);
                }

                cc_size_t size() {
                    return _size;
                }

                bool empty() const {
                    return CC_INIT == _size;
                }

                bool clear() {
                    clear(this->_M_node);
                }

            private:
                bool clear(BinaryNode<Comparable>*& root) {
                    if(CC_NULL == root) {
                        return true;
                    }

                    remove(root->_data, root);
                    return clear(root);
                }

                bool remove(const Comparable& data, BinaryNode<Comparable>*& root) {
                    if(CC_NULL == root) {   //not found
                        return false;
                    }

                    if(data < root->_data) {
                        remove(data, root->_leftChild);
                    } else if(data > root->_data) {
                        remove(data, root->_rightChild);
                    } else if(data == root->_data && root->_leftChild != CC_NULL && root->_rightChild != CC_NULL) {
                        // root->_data.~Comparable();  //NOTICE: destruct?
                        root->_data = findMin(root->_rightChild)->_data;
                        remove(root->_data, root->_rightChild);
                    } else {    //data == root->_data && (root->_leftChild != CC_NULL || root->_rightChild != CC_NULL)
                        BinaryNode<Comparable>* temp = root;
                        root = root->_leftChild != CC_NULL ? root->_leftChild : root->_rightChild;
                        delete temp;    //NOTICE: delete?
                        --_size;
                        return true;
                    }
                }

                BinaryNode<Comparable>* findMin(BinaryNode<Comparable>*& root) const {
                    IS_POINT_NULL_POINT(root);

                    while(root->_leftChild != CC_NULL) {
                        root = root->_leftChild;
                    }

                    return root;
                }

                BinaryNode<Comparable>* findMax(BinaryNode<Comparable>*& root) const {
                    IS_POINT_NULL_POINT(root);

                    while(root->_rightChild != CC_NULL) {
                        root = root->_rightChild;
                    }

                    return root;
                }

                Comparable find(const Comparable& key, BinaryNode<Comparable>*& root) {
                    IS_POINT_NULL_POINT(root);

                    if(key == root->_data) {
                        return root->_data;
                    }
                    if(CC_NULL != root->_leftChild) {
                        return find(key, root->_leftChild);
                    }
                    if(CC_NULL != root->_rightChild) {
                        return find(key, root->_rightChild);
                    }

                    return CC_NULL;
                }

                bool preOrderTraversalHandle(const Comparable& data, const BinaryNode<Comparable>* root) {
                    IS_POINT_NULL_POINT(root);

                    if(data == root->_data)
                        return true;
                    if(CC_NULL != root->_leftChild)
                        return preOrderTraversalHandle(data, root->_leftChild);
                    if(CC_NULL != root->_rightChild)
                        return preOrderTraversalHandle(data, root->_rightChild);

                    return false;
                }

                bool inOrderTraversalHandle(const Comparable& data, const BinaryNode<Comparable>* root) {
                    IS_POINT_NULL_POINT(root);

                    if(CC_NULL != root->_leftChild)
                        return inOrderTraversalHandle(data, root->_leftChild);
                    if(data == root->_data)
                        return true;
                    if(CC_NULL != root->_rightChild)
                        return inOrderTraversalHandle(data, root->_rightChild);

                    return false;
                }

                bool postOrderTraversalHandle(const Comparable& data, const BinaryNode<Comparable>* root) {
                    IS_POINT_NULL_POINT(root);

                    if(CC_NULL != root->_leftChild)
                        return postOrderTraversalHandle(data, root->_leftChild);
                    if(CC_NULL != root->_rightChild)
                        return postOrderTraversalHandle(data, root->_rightChild);
                    if(data == root->_data)
                        return true;

                    return false;
                }

                bool insert(const Comparable& data, BinaryNode<Comparable>*& root) {
                    bool result = true;

                    if(CC_NULL == root) {
                        root = new BinaryNode<Comparable>(data, CC_NULL, CC_NULL);
                        ++_size;
                    }
                    else if(data < root->_data) {
                        insert(data, root->_leftChild);
                    }
                    else if(data > root->_data) {
                        insert(data, root->_rightChild);
                    }
                    else    //NOTICE: duplicate data, do nothing
                        result = false;

                    return result;
                }

            private:
                cc_size_t _size;
                BinaryNode<Comparable>* _M_node;
        };
    } // namespace adt
} // namespace cclib


#endif //CCLIB_ADT_TREE_H