// declare the iterator classes so the names are available
class iterator;
class const_iterator;

// allow the iterator classes to access the private section
// of MBSTree
friend class iterator;
friend class const_iterator;

class iterator
{
    friend class MBSTree<T>;
    friend class const_iterator;

    public:
        // constructor
        iterator ()
        {}

        // comparison operators. just compare node pointers
        bool operator== (const iterator& rhs) const
        {
            return nodePtr == rhs.nodePtr;
        }

        bool operator!= (const iterator& rhs) const
        {
            return nodePtr != rhs.nodePtr;
        }

        // dereference operator. return a reference to
        // the value pointed to by nodePtr
        T& operator* () const
        {
            if (nodePtr == NULL)
                throw ReferenceError("MBSTree iterator operator* (): NULL reference");

            return nodePtr->nodeValue;
        }

        // preincrement. move forward to next larger value
        iterator& operator++ ()
        {
            BSTNode<T> *p;

            if (nodePtr == NULL)
            {
                // ++ from end(). get the root of the tree
                nodePtr = tree->root;

                // error! ++ requested for an empty tree
                if (nodePtr == NULL)
                    throw UnderflowError("MBSTree iterator operator++ (): tree empty");

                // move to the smallest value in the tree,
                // which is the first node inorder
                while (nodePtr->left != NULL)
                    nodePtr = nodePtr->left;
            }
            else
            if (nodePtr->right != NULL)
            {
                // successor is the furthest left node of
                // right subtree
                nodePtr = nodePtr->right;

                while (nodePtr->left != NULL)
                    nodePtr = nodePtr->left;
            }
            else
            {
                // have already processed the left subtree, and
                // there is no right subtree. move up the tree,
                // looking for a parent for which nodePtr is a left child,
                // stopping if the parent becomes NULL. a non-NULL parent
                // is the successor. if parent is NULL, the original node
                // was the last node inorder, and its successor
                // is the end of the list
                p = nodePtr->parent;

                while (p != NULL && nodePtr == p->right)
                {
                    nodePtr = p;
                    p = p->parent;
                }

                // if we were previously at the right-most node in
                // the tree, nodePtr = NULL, and the iterator specifies
                // the end of the list
                nodePtr = p;
            }

            return *this;
        }

        // postincrement
        iterator operator++ (int)
        {
            // save current iterator
            iterator tmp = *this;

            // move myself forward to the next tree node
            ++*this;

            // return the previous value
            return tmp;
        }

        // predecrement. move backward to largest value < current value
        iterator& operator-- ()
        {
            BSTNode<T> *p;

            if (nodePtr == NULL)
            {
                // -- from end(). get the root of the tree
                nodePtr = tree->root;

                // error! -- requested for an empty tree
                if (nodePtr == NULL)
                    throw UnderflowError("MBSTree iterator operator--: tree empty");

                // move to the largest value in the tree,
                // which is the last node inorder
                while (nodePtr->right != NULL)
                    nodePtr = nodePtr->right;
            } else if (nodePtr->left != NULL)
            {
                // must have gotten here by processing all the nodes
                // on the left branch. predecessor is the farthest
                // right node of the left subtree
                nodePtr = nodePtr->left;

                while (nodePtr->right != NULL)
                    nodePtr = nodePtr->right;
            }
            else
            {
                // must have gotten here by going right and then
                // far left. move up the tree, looking for a parent
                // for which nodePtr is a right child, stopping if the
                // parent becomes NULL. a non-NULL parent is the
                // predecessor. if parent is NULL, the original node
                // was the first node inorder, and its predecessor
                // is the end of the list
                p = nodePtr->parent;
                while (p != NULL && nodePtr == p->left)
                {
                    nodePtr = p;
                    p = p->parent;
                }

                // if we were previously at the left-most node in
                // the tree, nodePtr = NULL, and the iterator specifies
                // the end of the list
                nodePtr = p;
            }

            return *this;
        }

        // postdecrement
        iterator operator-- (int)
        {
            // save current iterator
            iterator tmp = *this;

            // move myself backward to the previous tree node
            --*this;

            // return the previous value
            return tmp;
        }

    private:

        // nodePtr is the current location in the tree. we can move
        // freely about the tree using left, right, and parent.
        // tree is the address of the MBSTree object associated
        // with this iterator. it is used only to access the
        // root pointer, which is needed for ++ and --
        // when the iterator value is end()
        BSTNode<T> *nodePtr;
        MBSTree<T> *tree;

        // used to construct an iterator return value from
        // an BSTNode pointer
        iterator (BSTNode<T> *p, MBSTree<T> *t) : nodePtr(p), tree(t)
        {}

};

class const_iterator
{
    friend class MBSTree<T>;

    public:

        // constructor
        const_iterator ()
        {}

        // used to convert a const iterator to a const_iterator
        const_iterator (const iterator& pos): nodePtr(pos.nodePtr)
        {}

        // comparison operators. just compare node pointers
        bool operator== (const const_iterator& rhs) const
        {
            return nodePtr == rhs.nodePtr;
        }

        bool operator!= (const const_iterator& rhs) const
        {
            return nodePtr != rhs.nodePtr;
        }

        // dereference operator. return a reference to
        // the value pointed to by nodePtr
        const T& operator* () const
        {
            if (nodePtr == NULL)
                throw ReferenceError("MBSTree const_iterator operator* (): NULL reference");

            return nodePtr->nodeValue;
        }

        // preincrement. move forward to next larger value
        const_iterator& operator++ ()
        {
            BSTNode<T> *p;

            if (nodePtr == NULL)
            {
                // ++ from end(). get the root of the tree
                nodePtr = tree->root;

                // error! ++ requested for an empty tree
                if (nodePtr == NULL)
                    throw UnderflowError("MBSTree const_iterator operator++ (): tree empty");

                // move to the smallest value in the tree,
                // which is the first node inorder
                while (nodePtr->left)
                    nodePtr = nodePtr->left;
            }
            else if (nodePtr->right)
            {
                // successor is the furthest left node of
                // right subtree
                nodePtr = nodePtr->right;

                while (nodePtr->left)
                    nodePtr = nodePtr->left;
            }
            else
            {
                // have already processed the left subtree, and
                // there is no right subtree. move up the tree,
                // looking for a parent for which nodePtr is a left child,
                // stopping if the parent becomes NULL. a non-NULL parent
                // is the successor. if parent is NULL, the original node
                // was the last node inorder, and its successor
                // is the end of the list
                p = nodePtr->parent;

                while (p && nodePtr == p->right)
                {
                    nodePtr = p;
                    p = p->parent;
                }

                // if we were previously at the right-most node in
                // the tree, nodePtr = NULL, and the iterator specifies
                // the end of the list
                nodePtr = p;
            }

            return *this;
        }

        // postincrement
        const_iterator operator++ (int)
        {
            // save current const_iterator
            const_iterator tmp = *this;

            // move myself forward to the next tree node
            ++*this;

            // return the previous value
            return tmp;
        }

        // predecrement. move backward to largest value < current value
        const_iterator& operator-- ()
        {
            BSTNode<T> *p;

            if (nodePtr == NULL)
            {
                // -- from end(). get the root of the tree
                nodePtr = tree->root;

                // error! -- requested for an empty tree
                if (nodePtr == NULL)
                    throw UnderflowError("MBSTree iterator operator--: tree empty");

                // move to the largest value in the tree,
                // which is the last node inorder
                while (nodePtr->right)
                    nodePtr = nodePtr->right;
            } 
            else if (nodePtr->left)
            {
                // must have gotten here by processing all the nodes
                // on the left branch. predecessor is the farthest
                // right node of the left subtree
                nodePtr = nodePtr->left;

                while (nodePtr->right)
                    nodePtr = nodePtr->right;
            }
            else
            {
                // must have gotten here by going right and then
                // far left. move up the tree, looking for a parent
                // for which nodePtr is a right child, stopping if the
                // parent becomes NULL. a non-NULL parent is the
                // predecessor. if parent is NULL, the original node
                // was the first node inorder, and its predecessor
                // is the end of the list
                p = nodePtr->parent;
                while (p && nodePtr == p->left)
                {
                    nodePtr = p;
                    p = p->parent;
                }

                // if we were previously at the left-most node in
                // the tree, nodePtr = NULL, and the iterator specifies
                // the end of the list
                nodePtr = p;
            }

            return *this;
        }

        // postdecrement
        const_iterator operator-- (int)
        {
            // save current const_iterator
            const_iterator tmp = *this;

            // move myself backward to the previous tree node
            --*this;

            // return the previous value
            return tmp;
        }

    private:

        // nodePtr is the current location in the tree. we can move
        // freely about the tree using left, right, and parent.
        // tree is the address of the MBSTree object associated
        // with this iterator. it is used only to access the
        // root pointer, which is needed for ++ and --
        // when the iterator value is end()
        const BSTNode<T> *nodePtr;
        const MBSTree<T> *tree;

        // used to construct a const_iterator return value from
        // an BSTNode pointer
        const_iterator (const BSTNode<T> *p, const MBSTree<T> *t) : nodePtr(p), tree(t)
        {}

};

