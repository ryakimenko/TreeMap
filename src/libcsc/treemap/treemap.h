#pragma once

#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace libcsc {
// TreeMap
template <typename KeyType, typename ValueType>
class TreeMap {
public:
    using key_type = KeyType;
    using mapped_type = ValueType;
    using value_type = std::pair<const key_type, mapped_type>;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;

    class Iterator;
    class ConstIterator;

    using iterator = Iterator;
    using const_iterator = ConstIterator;

private:
    struct Node {
        value_type data_;

        Node* parent_ = nullptr;
        Node* left_ = nullptr;
        Node* right_ = nullptr;
        int height_ = 0;

        Node()
            : data_(nullptr), parent_(nullptr), left_(nullptr), right_(nullptr)
        {
        }

        Node(key_type key, mapped_type value, Node* parent = nullptr)
            : parent_(parent), left_(nullptr), right_(nullptr)
        {
            data_ = value_type(key, value);
            height_ = 0;
        }

        explicit Node(
                value_type data,
                Node* parent = nullptr,
                Node* left = nullptr,
                Node* right = nullptr,
                int height = 0)
            : data_(data),
              parent_(parent),
              left_(left),
              right_(right),
              height_(height)
        {
        }
        bool operator==(const key_type& key)
        {
            return equal(this->data_.first, key);
        }

        bool operator==(const value_type& value)
        {
            return equal(this->data_.first, value.first);
        }

        bool operator==(const Node& node)
        {
            return equal(this->data_.first, node->data_.first);
        }
    };

    Node* root_ = nullptr;
    size_type size_ = 0;

    int height(Node* node)
    {
        return (node != nullptr) ? node->height_ : -1;
    }

    void create(value_type data)
    {
        root_ = new Node(data);
        size_ = 1;
    }

    Node* left_rotate(Node* tree)
    {
        Node* right;
        right = tree->right_;
        tree->right_ = right->left_;
        if (tree->right_ != nullptr) {
            tree->right_->parent_ = tree;
        }
        right->left_ = tree;
        right->parent_ = tree->parent_;
        tree->parent_ = right;
        tree->height_ = std::max(height(tree->left_), height(tree->right_)) + 1;
        right->height_ = std::max(height(right->right_), tree->height_) + 1;
        return right;
    }

    Node* right_rotate(Node* tree)
    {
        Node* left;
        left = tree->left_;
        tree->left_ = left->right_;
        if (tree->left_ != nullptr) {
            tree->left_->parent_ = tree;
        }
        left->right_ = tree;
        left->parent_ = tree->parent_;
        tree->parent_ = left;
        tree->height_ = std::max(height(tree->left_), height(tree->right_)) + 1;
        left->height_ = std::max(height(left->left_), tree->height_) + 1;
        return left;
    }

    Node* leftRight_rotate(Node* tree)
    {
        tree->left_ = left_rotate(tree->left_);
        return right_rotate(tree);
    }

    Node* rightLeft_rotate(Node* tree)
    {
        tree->right_ = right_rotate(tree->right_);
        return left_rotate(tree);
    }

    Node* add(Node* tree, const value_type& data, iterator& iter)
    {
        if (tree->data_.first == data.first) {
            this->size_--;
            iter = tree;
        }
        if (data.first < tree->data_.first) {
            if (tree->left_ == nullptr) {
                tree->left_ = new Node(data, tree);
                iter = tree->left_;
            } else {
                tree->left_ = add(tree->left_, data, iter);
            }
            if (std::abs(height(tree->left_) - height(tree->right_)) == 2) {
                if (data.first < tree->left_->data_.first) {
                    tree = right_rotate(tree);
                } else {
                    tree = leftRight_rotate(tree);
                }
            }
        }

        else if (data.first > tree->data_.first) {
            if (tree->right_ == nullptr) {
                tree->right_ = new Node(data, tree);
                iter = tree->right_;
            } else {
                tree->right_ = add(tree->right_, data, iter);
            }
            if (std::abs(height(tree->left_) - height(tree->right_)) == 2) {
                if (data.first > tree->right_->data_.first) {
                    tree = left_rotate(tree);
                } else {
                    tree = rightLeft_rotate(tree);
                }
            }
        }
        tree->height_ = std::max(height(tree->left_), height(tree->right_)) + 1;
        return tree;
    }

    void delete_tree(Node* root)
    {
        if (root != nullptr) {
            delete_tree(root->left_);
            delete_tree(root->right_);
            delete (root);
            root = nullptr;
        }
        size_ = 0;
    }

    Node* change_data(Node* node, value_type& data)
    {
        if (node->parent_ == nullptr) {
            node = new Node(
                    data,
                    node->parent_,
                    node->left_,
                    node->right_,
                    node->height_);
        } else if (node->parent_->left_ == node) {
            node = new Node(
                    data,
                    node->parent_,
                    node->left_,
                    node->right_,
                    node->height_);
            node->parent_->left_ = node;
        } else {
            node = new Node(
                    data,
                    node->parent_,
                    node->left_,
                    node->right_,
                    node->height_);
            node->parent_->right_ = node;
        }
        if (node->left_) {
            node->left_->parent_ = node;
        }
        if (node->right_) {
            node->right_->parent_ = node;
        }
        return node;
    }

public:
    TreeMap() : root_(nullptr)
    {
    }

    TreeMap(std::initializer_list<value_type> list) : TreeMap()
    {
        for (auto&& it : list) {
            insert(it);
        }
    }

    TreeMap(const TreeMap& other) : TreeMap()
    {
        for (auto it = other.cbegin(); it != other.cend(); ++it) {
            insert(*it);
        }
    }

    TreeMap(TreeMap&& other) noexcept : TreeMap()
    {
        this->root_ = other.root_;
        this->size_ = other.size_;

        other.root_ = nullptr;
        other.size_ = 0;
    }

    ~TreeMap()
    {
        delete_tree(root_);
    }

    TreeMap& operator=(const TreeMap& other)
    {
        delete_tree(root_);
        for (auto it = other.cbegin(); it != other.cend(); ++it) {
            insert(*it);
        }
    }

    TreeMap& operator=(TreeMap&& other) noexcept
    {
        delete_tree(root_);
        this->root_ = other.root_;
        this->size_ = other.size_;

        other.root_ = nullptr;
        other.size_ = 0;
    }

    bool operator==(const TreeMap& other) const
    {
        if (this->size_ != other.size_) {
            return false;
        }
        for (auto it1 = cbegin(), it2 = other.cbegin(); it1 != cend();
             ++it1, ++it2) {
            if (*it1 != *it2) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const TreeMap& other) const
    {
        return !(*this == other);
    }

    mapped_type& operator[](key_type&& key)
    {
        for (auto it = begin(); it != end(); ++it) {
            if (it->first == key) {
                return it->second;
            }
        }

        return insert(std::make_pair(key, mapped_type())).first->second;
    }

    mapped_type& at(const key_type& key)
    {
        auto it = find(key);
        if (it == end()) {
            throw std::out_of_range("at");
        }
        return it->second;
    }

    iterator begin()
    {
        if (root_ == nullptr) {
            return iterator(static_cast<Node*>(nullptr));
        }
        Node* node = root_;
        while (node->left_ != nullptr) {
            node = node->left_;
        }
        return iterator(node);
    }

    iterator end()
    {
        return iterator(static_cast<Node*>(nullptr));
    }

    const_iterator cbegin() const
    {
        if (root_ == nullptr) {
            return const_iterator(static_cast<Node*>(nullptr));
        }
        Node* node = root_;
        while (node->left_ != nullptr) {
            node = node->left_;
        }
        return const_iterator(node);
    }

    const_iterator cend() const
    {
        return const_iterator(static_cast<Node*>(nullptr));
    }

    size_type size() const noexcept
    {
        return size_;
    }

    bool empty() const noexcept
    {
        if (root_ == nullptr && size_ == 0) {
            return true;
        } else {
            return false;
        }
    }

    std::pair<iterator, bool> insert(const value_type& data)
    {
        size_++;
        if (root_ == nullptr) {
            create(data);
            return std::make_pair(iterator(root_), true);

        } else {
            iterator iter;
            auto old_size = size_;
            root_ = add(root_, data, iter);
            if (old_size == size_ + 1) {
                return std::make_pair(iter, false);
            } else {
                return std::make_pair(iter, true);
            }
        }
    }
    void erase(iterator pos);

    void erase(const key_type& key)
    {
        erase(find(key));
    }

    iterator find(const key_type& key)
    {
        Node* node = root_;
        while (node != nullptr) {
            if (key > node->data_.first) {
                node = node->right_;
            } else if (key < node->data_.first) {
                node = node->left_;
            } else {
                break;
            }
        }
        if (node == nullptr) {
            return end();
        } else {
            return iterator(node);
        }
    }
    const_iterator find(const key_type& key) const
    {
        Node* node = root_;
        while (node != nullptr) {
            if (key > node->data_.first) {
                node = node->right_;
            } else if (key < node->data_.first) {
                node = node->left_;
            } else {
                break;
            }
        }
        if (node->data_.first != key) {
            return cend();
        } else {
            return const_iterator(node);
        }
    }

    bool contains(const key_type& key) const
    {
        if (find(key) != cend()) {
            return true;
        } else {
            return false;
        }
    }

    size_type count(const key_type& key) const
    {
        if (find(key) != cend()) {
            return 1;
        } else {
            return 0;
        }
    }
};

// Const_Iterator
template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::ConstIterator {
public:
    using reference = typename TreeMap::const_reference;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = const typename TreeMap::value_type;
    using pointer = const typename TreeMap::value_type*;

private:
    friend class TreeMap<KeyType, ValueType>;
    Node* node_ = nullptr;

    explicit ConstIterator(Node* node) : node_(node)
    {
    }

public:
    explicit ConstIterator(const TreeMap* tree = nullptr)
    {
        if (tree) {
            node_ = tree->root_;
        }
    }

    ConstIterator& operator++()
    {
        if (node_ == nullptr) {
            throw std::out_of_range("operator++ iterator");
        } else if (node_->right_ != nullptr) {
            node_ = node_->right_;
            while (node_->left_ != nullptr) {
                node_ = node_->left_;
            }
        } else {
            while (true) {
                if (node_->parent_ == nullptr) {
                    node_ = nullptr;
                    break;
                }
                if (node_->parent_->left_ == node_) {
                    node_ = node_->parent_;
                    break;
                }
                node_ = node_->parent_;
            }
        }

        return *this;
    }
    ConstIterator operator++(int)
    {
        auto result = *this;
        ConstIterator::operator++();
        return result;
    }

    ConstIterator& operator--()
    {
        if (node_ == nullptr) {
            node_ = node_->parent_;
            while (node_->right_ != nullptr) {
                node_ = node_->right_;
            }
        } else if (node_->left_ != nullptr) {
            node_ = node_->left_;
            while (node_->right_ != nullptr) {
                node_ = node_->right_;
            }
        } else {
            while (true) {
                if (node_->parent_ == nullptr) {
                    throw std::out_of_range("operator--");
                }
                if (node_->parent_->right_ == node_) {
                    node_ = node_->parent_;
                    break;
                }
                node_ = node_->parent_;
            }
        }

        return *this;
    }
    ConstIterator operator--(int)
    {
        auto result = *this;
        ConstIterator::operator--();
        return result;
    }

    reference operator*() const
    {
        if (node_ == nullptr) {
            throw std::out_of_range("operator* iterator");
        }
        return node_->data_;
    }
    pointer operator->() const
    {
        return &this->operator*();
    }

    bool operator==(const ConstIterator& other) const
    {
        return node_ == other.node_;
    }

    bool operator!=(const ConstIterator& other) const
    {
        return !(*this == other);
    }
};

// Iterator
template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator
    : public TreeMap<KeyType, ValueType>::ConstIterator {
private:
    friend class TreeMap<KeyType, ValueType>;
    explicit Iterator(Node* node) : ConstIterator(node)
    {
    }

    Iterator& operator=(Node* node)
    {
        ConstIterator::node_ = node;
        return *this;
    }

public:
    using reference = typename TreeMap::reference;
    using pointer = typename TreeMap::value_type*;
    using iterator_category = std::bidirectional_iterator_tag;

    explicit Iterator(TreeMap* tree = nullptr) : ConstIterator(tree)
    {
    }

    Iterator& operator++()
    {
        ConstIterator::operator++();
        return *this;
    }

    Iterator operator++(int)
    {
        auto result = *this;
        ConstIterator::operator++();
        return result;
    }

    Iterator& operator--()
    {
        ConstIterator::operator--();
        return *this;
    }

    Iterator operator--(int)
    {
        auto result = *this;
        ConstIterator::operator--();
        return result;
    }

    pointer operator->() const
    {
        return &this->operator*();
    }

    reference operator*() const
    {
        return const_cast<reference>(ConstIterator::operator*());
    }

    bool operator==(const Iterator& other) const
    {
        return ConstIterator::node_ == other.node_;
    }

    bool operator!=(const Iterator& other) const
    {
        return !(*this == other);
    }
};
template <typename KeyType, typename ValueType>
void TreeMap<KeyType, ValueType>::erase(TreeMap::iterator pos)
{
    if (pos.node_ == nullptr) {
        return;
    }
    if (pos.node_->parent_ == nullptr && pos.node_->left_ == nullptr
        && pos.node_->right_ == nullptr) {
        delete root_;
        root_ = nullptr;
        size_--;
        return;
    }
    if (pos.node_->left_ == nullptr || pos.node_->right_ == nullptr) {
        Node* parent = pos.node_->parent_;
        Node* temp = pos.node_->left_ ? pos.node_->left_ : pos.node_->right_;
        if (temp == nullptr) {
            if (parent->left_ == pos.node_) {
                delete parent->left_;
                parent->left_ = nullptr;
            } else {
                delete parent->right_;
                parent->right_ = nullptr;
            }

        } else {
            Node* newNode = change_data(pos.node_, temp->data_);
            delete pos.node_;
            pos.node_ = newNode;
            pos.node_->height_ = 0;
            delete pos.node_->left_;
            delete pos.node_->right_;
            pos.node_->left_ = nullptr;
            pos.node_->right_ = nullptr;
        }
        if (parent == nullptr) {
            root_ = pos.node_;
        } else {
            pos.node_ = parent;
        }
    } else {
        Node* temp = pos.node_->left_;
        while (temp->right_ != nullptr) {
            temp = temp->right_;
        }
        Node* newNode = change_data(pos.node_, temp->data_);
        delete pos.node_;
        pos.node_ = newNode;
        erase(iterator(temp));
        size_++;
    }

    pos.node_->height_
            = std::max(height(pos.node_->left_), height(pos.node_->right_)) + 1;

    if (std::abs(height(pos.node_->left_) - height(pos.node_->right_)) == 2) {
        if (height(pos.node_->left_) > height(pos.node_->right_)) {
            if (pos.node_->data_.first > pos.node_->left_->data_.first) {
                pos.node_ = right_rotate(pos.node_);
            } else {
                pos.node_ = leftRight_rotate(pos.node_);
            }

            if (pos.node_->parent_ == nullptr) {
                this->root_ = pos.node_;
                return;
            }
            pos.node_->parent_->right_ = pos.node_;
        } else {
            if (pos.node_->data_.first < pos.node_->right_->data_.first) {
                pos.node_ = left_rotate(pos.node_);
            } else {
                pos.node_ = rightLeft_rotate(pos.node_);
            }

            if (pos.node_->parent_ == nullptr) {
                this->root_ = pos.node_;
                return;
            }
            pos.node_->parent_->left_ = pos.node_;
        }
    }
    size_--;
}

} // namespace libcsc