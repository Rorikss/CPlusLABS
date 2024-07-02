//
// Created by Roriks on 28/02/2024.
//

#pragma once
#include <iterator>
#include <iostream>
#include <type_traits>
#include <memory>
#include <limits>

namespace BST {

    template <typename T>
    struct Node {
        T value;
        Node* left;
        Node* right;
        Node* parent;

        Node(T value, Node* par) : value(value), left(nullptr), right(nullptr), parent(par) {}
        Node(T value, Node* left, Node* right, Node* par) : value(value), left(left), right(right), parent(par) {}
    };

    struct InOrderTag {};
    struct PreOrderTag {};
    struct PostOrderTag {};

// this thing is aimed to check whether presented tag is mine
    template <typename T>
    constexpr bool IsMyTag = std::is_same_v<T, InOrderTag> || std::is_same_v<T, PreOrderTag> || std::is_same_v<T, PostOrderTag>;


    template<typename Key, typename TraversalTag, typename Compare = std::less<Key>, typename Allocator = std::allocator<Key>>
    class BinarySearchTree {
    public:
        template <bool IsConst>
        class Iterator {
        public:
            using key_type = Key;
            using value_type = Node<Key>;
            using pointer = value_type*;
            using const_pointer = const value_type*;
            using reference = value_type&;
            using const_reference = const value_type&;
            using difference_type = std::ptrdiff_t;
            using size_type = unsigned int;
            using iterator_category = std::bidirectional_iterator_tag;
            using traversal_tag = TraversalTag;

            using conditional_ptr = std::conditional_t<IsConst, const_pointer, pointer>;
            using conditional_key_ref = std::conditional_t<IsConst, const key_type&, key_type&>;
            using conditional_ref = std::conditional_t<IsConst, const_reference, reference>;


            // constructors
            Iterator(pointer ptr, size_type size, bool is_end, pointer end, PostOrderTag) : ptr_(ptr), max_iter_(size), end_ptr_(end) {
                count_ = is_end ? max_iter_ : 0;
                while (ptr_ && ptr_->left) {
                    ptr_ = ptr_->left;
                }
            }

            Iterator(pointer ptr, size_type size, bool is_end, pointer end, PreOrderTag) : ptr_(ptr), max_iter_(size), end_ptr_(end) {
                count_ = is_end ? max_iter_ : 0;
            };

            Iterator(pointer ptr, size_type size, pointer end) : ptr_(ptr), max_iter_(size), end_ptr_(end) {}

            Iterator(pointer ptr, size_type size, bool is_end, pointer end, InOrderTag) : ptr_(ptr), max_iter_(size), end_ptr_(end) {
                count_ = is_end ? max_iter_ : 0;
                while (ptr_ && ptr_->left) {
                    ptr_ = ptr_->left;
                }
            }

            Iterator(const Iterator& other) : ptr_(other.ptr_), end_ptr_(other.end_ptr_),
                                              max_iter_(other.max_iter_), count_(other.count_) {};

            Iterator& operator=(const Iterator& other) {
                Iterator(other).SwapValues(*this);
                return *this;
            }

            ~Iterator() = default;

            // get the value by
            conditional_key_ref operator*() {
                CheckValidity();
                return ptr_->value;
            }

            conditional_ptr operator->() {
                CheckValidity();
                return ptr_;
            }

            // block of comparison functions
            bool operator==(const Iterator& other) const {
                return ptr_ == other.ptr_;
            }

            bool operator!=(const Iterator& other) const {
                return !(*this == other);
            }

            // block of arithmetic functions
            Iterator& operator++() {
                ++count_;
                if (count_ >= max_iter_ || end_ptr_->parent == ptr_ || end_ptr_->right == ptr_) {
                    ptr_ = end_ptr_;
                    return *this;
                }
                Increment(tag_);

                return *this;
            }

            Iterator operator++(int) {
                auto copy = *this;
                operator++();
                return copy;
            }

            Iterator& operator--() {
                --count_;
                Decrement(tag_);

                return *this;
            }

            Iterator operator--(int) {
                auto copy = *this;
                operator--();
                return copy;
            }

        private:
            void CheckValidity() {
                if (!ptr_) {
                    throw std::runtime_error("Invalidated iterator!");
                }
            }

            void Increment(InOrderTag tag) {
                if (ptr_->right) {
                    ptr_ = ptr_->right;
                    while (ptr_->left) {
                        ptr_ = ptr_->left;
                    }
                } else {
                    auto temp = ptr_;
                    while (temp->parent && temp == temp->parent->right) {
                        temp = temp->parent;
                    }
                    ptr_ = temp->parent;
                }
            }

            void Increment(PreOrderTag tag) {
                if (ptr_->left) {
                    ptr_ = ptr_->left;
                } else if (ptr_->right) {
                    ptr_ = ptr_->right;
                } else {
                    while (ptr_->parent && ptr_ == ptr_->parent->right) {
                        ptr_ = ptr_->parent;
                    }
                    if (ptr_->parent) {
                        ptr_ = ptr_->parent->right;
                    }
                }
            }

            void Increment(PostOrderTag tag) {
                if (!ptr_->parent) {
                    ptr_ = end_ptr_;
                    return;
                }
                if (ptr_->parent->right == ptr_ || !ptr_->parent->right) {
                    ptr_ = ptr_->parent;
                    return;
                }
                ptr_ = ptr_->parent->right;
                while(ptr_ && (ptr_->left || ptr_->right)) {
                    ptr_ = ptr_->left ? ptr_->left : ptr_->right;
                }
            }

            void Decrement(InOrderTag tag) {
                if (ptr_->left) {
                    ptr_ = ptr_->left;
                    while (ptr_->right != nullptr) {
                        ptr_ = ptr_->right;
                    }
                } else {
                    auto prev = ptr_;
                    ptr_ = ptr_->parent;
                    while (ptr_ != nullptr && prev == ptr_->left) {
                        prev = ptr_;
                        ptr_ = ptr_->parent;
                    }
                }
            }

            void Decrement(PreOrderTag tag) {
                if (ptr_->parent == nullptr) {
                    ptr_ = ptr_->left;
                    while (ptr_->right != nullptr) {
                        ptr_ = ptr_->right;
                    }
                    return;
                }
                if (ptr_->parent->left == nullptr || ptr_->parent->left == ptr_) {
                    ptr_ = ptr_->parent;
                    return;
                }
                ptr_ = ptr_->parent->left;
                while (ptr_->right != nullptr) {
                    ptr_ = ptr_->right;
                }

            }

            void Decrement(PostOrderTag tag) {
                if (ptr_ == end_ptr_) {
                    ptr_ = end_ptr_->right;
                    return;
                }
                if (ptr_->right) {
                    ptr_ = ptr_->right;
                    return;
                }
                while (ptr_->parent->left == ptr_ || !ptr_->parent->left) {
                    ptr_ = ptr_->parent;
                }
                ptr_ = ptr_->parent->left;
            }

            void SwapValues(Iterator& other) noexcept {
                std::swap(ptr_, other.ptr_);
            }


            size_t count_ = 0;
            size_t max_iter_;
            conditional_ptr end_ptr_;
            conditional_ptr ptr_;
            TraversalTag tag_;
        };


        using value_type = Node<Key>;
        using allocator_type = Allocator;
        using allocator_traits = std::allocator_traits<Allocator>;
        using size_type = typename allocator_traits::size_type;
        using difference_type = typename allocator_traits::difference_type;

        using pointer = typename allocator_traits::pointer;
        using const_pointer = typename Allocator::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;

        using key_compare = Compare;
        using value_compare = Compare;
        using key_type = Key;
        using node_type = Node<Key>;
        using tag = TraversalTag;

        using iterator = Iterator<false>;
        using const_iterator = Iterator<true>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        BinarySearchTree() : root_(nullptr), size_(0) {
            if (!IsMyTag<TraversalTag>) { // TODO function for throw
                throw std::runtime_error("Unknown tag entered, supports only: InOrderTag, PreOrderTag, PostOrderTag");
            }
            value_type* new_node = allocator_.allocate(1); // добавляем элемент пустышку
            allocator_.construct(new_node, Node<Key>(Key{}, nullptr));
            end_ = new_node;
        }

        BinarySearchTree(std::initializer_list<key_type> values) : root_(nullptr), size_(0) { // from initializer list
            if (!IsMyTag<TraversalTag>) { // this tag is mine
                throw std::runtime_error("Unknown tag entered, supports only: InOrderTag, PreOrderTag, PostOrderTag");
            }
            end_ = GenerateNewNode(Key{}, nullptr);
            for (key_type value : values) {
                insert(value);
            }
        }

        BinarySearchTree(const BinarySearchTree& other) : comp_(other.comp_), allocator_(other.allocator_), tag_(other.tag_) {
            root_ = CopyTree(other.root_);
            value_type* new_node = allocator_.allocate(1); // TODO copy node method
            allocator_.construct(new_node, *other.end_);
            end_ = new_node;
            EndNodePosition(tag_);
            MakeParentsForCopy(root_);
            size_ = other.size_;
        }

        BinarySearchTree& operator=(BinarySearchTree other) {
            BinarySearchTree copied(other);
            swap(*this, copied);
            return *this;
        }

        ~BinarySearchTree() {
            ClearRecursive(root_);
            DeleteNode(end_);
            root_ = nullptr;
            end_ = nullptr;
        }

        iterator insert(Key value) {
            value_type* temp = nullptr;
            root_ = InsertHelper(root_, value, nullptr, temp);
            EndNodePosition(tag_);
            ++size_;

            return iterator(temp, size_, end_);
        }

        void insert(size_t i, size_t j) {
            for (size_t item = i; item < j; ++item) {
                insert(item);
            }
        }

        void extract(Key key) {
            if (!contains(key)) {
                return;
            }
            size_--;
            root_ = ExtractHelper(root_, key);
            EndNodePosition(tag_);
        }

        void erase(key_type key) {
            extract(key);
        }

        void clear() {
            ClearRecursive(root_);
            root_ = nullptr;
        }

        template <typename... Args>
        iterator emplace(Args&&... args) {
            key_type value = Key(std::forward<Args>(args)...);
            insert(value);

            return begin();
        }

        Compare value_comp() const {
            return comp_;
        }

        Compare key_comp() const {
            return comp_;
        }

        iterator begin() {
            return iterator(root_, size_, false, end_, tag_);
        }

        const_iterator cbegin() const {
            return const_iterator(root_, size_, false, end_, tag_);
        }

        iterator end() {
            return iterator(end_, size_, true, end_, tag_);
        }

        const_iterator cend() const {
            return const_iterator(end_, size_, true, end_, tag_);
        }

        reverse_iterator rbegin() { return std::reverse_iterator(end()); }
        reverse_iterator rend() { return std::reverse_iterator(begin()); }

        const_reverse_iterator crbegin() const { return  const_reverse_iterator(end()); }
        const_reverse_iterator crend() const { return  const_reverse_iterator(begin()); }

        bool contains(Key k) {
            return find(k) != end();
        }

        iterator find(Key key) {
            auto search_result = FindHelper(root_, key);
            return (!search_result) ? end() : iterator(search_result, size_, end_);
        }

        const_iterator find(Key key) const {
            auto search_result = FindHelper(root_, key);
            return (!search_result) ? cend() : const_iterator(search_result, size_, end_);
        }

        [[nodiscard]] size_t size() const {
            return size_;
        }

        [[nodiscard]] size_t max_size() const {
            return std::numeric_limits<size_type>::max();;
        }

        [[nodiscard]] bool empty() const {
            return size_ == 0;
        }

        void merge(BinarySearchTree& other) {
            for (auto it = other.begin(); it != other.end(); ++it) {
                ++size_;
                insert(it->value);
            }
        }

        size_t count(key_type key) const {
            return CountHelper(root_, key, 0);
        }

        iterator upper_bound(key_type key) {
            auto upper = UpperBoundHelper(root_, key);
            return (upper) ? iterator(upper, size_, end_) : end();
        }

        const_iterator upper_bound(key_type key) const {
            auto upper = UpperBoundHelper(root_, key);
            return (upper) ? const_iterator(upper, size_, end_) : cend();
        }

        iterator lower_bound(key_type key) {
            auto lower = LowerBoundHelper(root_, key);
            return (lower) ? iterator(lower, size_, end_) : end();
        }

        const_iterator lower_bound(key_type key) const {
            auto lower = LowerBoundHelper(root_, key);
            return (lower) ? const_iterator(lower, size_, end_) : cend();
        }

        std::pair<iterator, iterator> equal_range(key_type key) {
            return std::make_pair(lower_bound(key), upper_bound(key));
        }

        std::pair<const_iterator, const_iterator> equal_range(key_type key) const {
            return std::make_pair(lower_bound(key), upper_bound(key));
        }

        bool operator==(BinarySearchTree& other) {
            auto it_1 = begin();
            auto it_2 = other.begin();
            for (int i = 0; i < std::min(size_, other.size()) - 1; ++i) {
                if (it_1->value != it_2->value) return false;
                ++it_1;
                ++it_2;
            }

            return (it_1->value == it_2->value);
        }

        bool operator<(BinarySearchTree& other) {
            auto it_1 = begin();
            auto it_2 = other.begin();
            for (int i = 0; i < std::min(size_, other.size()) - 1; ++i) {
                if (it_1->value >= it_2->value) return false;
                ++it_1;
                ++it_2;
            }

            return (it_1->value < it_2->value);
        }

        bool operator>(BinarySearchTree& other) {
            return (other < *this);
        }

        bool operator>=(BinarySearchTree& other) {
            return !(other < *this);
        }

        bool operator<=(BinarySearchTree& other) {
            return !(other > *this);
        }

        bool operator!=(BinarySearchTree& other) {
            return !(*this == other);
        }

        friend void swap(BinarySearchTree& first, BinarySearchTree& second) {
            first.swap(second);
        }

        void swap(BinarySearchTree& other) {
            using std::swap;
            swap(root_, other.root_);
            swap(end_, other.end_);
            swap(size_, other.size_);
            swap(comp_, other.comp_);
            swap(allocator_, other.allocator_);
            swap(tag_, other.tag_);
        }

    private:
        void MakeParentsForCopy(value_type* node) {
            if (node->left) {
                node->left->parent = node;
                MakeParentsForCopy(node->left);
            }
            if (node->right) {
                node->right->parent = node;
                MakeParentsForCopy(node->right);
            }
        }

        void EndNodePosition(InOrderTag tag) {
            value_type* right_most = GetRightest(root_);
            end_->parent = right_most;
        }

        void EndNodePosition(PreOrderTag tag) {
            value_type* deepest = GetDeepest();
            end_->parent = deepest;
        }

        void EndNodePosition(PostOrderTag tag) {
            if (size_ == 0) {
                end_->right = root_;
            }
        }

        value_type* GenerateNewNode(Key value, value_type* parent) {
            value_type* new_node = allocator_.allocate(1);
            allocator_.construct(new_node, Node<Key>(value, parent));
            return new_node;
        }

        value_type* CopyTree(value_type* node) {
            if (node == nullptr) {
                return nullptr;
            }
            value_type* new_node = allocator_.allocate(1);
            allocator_.construct(new_node, *node);
            new_node->left = CopyTree(node->left);
            new_node->right = CopyTree(node->right);

            return new_node;
        }

        void ClearRecursive(value_type* node) {
            if (node != nullptr) {
                ClearRecursive(node->left);
                ClearRecursive(node->right);
                DeleteNode(node);
            }
        }

        void DeleteNode(value_type* node) {
            allocator_.destroy(node);
            allocator_.deallocate(node, 1);
        }

        value_type* InsertHelper(value_type* node, Key value, value_type* parent, value_type*& current) {
            if (node == nullptr) {
                current = GenerateNewNode(value, parent);
                return current;
            }

            if (comp_(value, node->value)) {
                node->left = InsertHelper(node->left, value, node, current);
            } else {
                node->right = InsertHelper(node->right, value, node, current);
            }

            return node;
        }

        node_type* ExtractHelper(node_type* root, Key key) {
            if (root == nullptr) {
                return root;
            } else if (comp_(key, root->value)) {
                root->left = ExtractHelper(root->left, key);
            } else if (comp_(root->value, key)) {
                root->right = ExtractHelper(root->right, key);
            } else {
                if (root->left == nullptr) {
                    value_type* temp = root->right;
                    DeleteNode(root);
                    if (temp) {
                        temp->parent = root->parent;
                    }
                    return temp;
                } else if (root->right == nullptr) {
                    value_type* temp = root->left;
                    DeleteNode(root);
                    if (temp) {
                        temp->parent = root->parent;
                    }
                    return temp;
                }
                value_type* temp = GetLeftest(root->right);
                root->value = temp->value;
                root->right = ExtractHelper(root->right, temp->value);
            }

            return root;
        }

        value_type* GetLeftest(value_type* root) {
            while (root->left != nullptr) {
                root = root->left;
            }

            return root;
        }

        value_type* GetRightest(value_type* root) {
            while (root->right != nullptr) {
                root = root->right;
            }

            return root;
        }

        value_type* GetDeepest() {
            value_type* node = nullptr;
            int max_lvl = 0;
            bool right = false;
            FindDeepestHelper(root_, 0, max_lvl, node, right);

            return node;
        }

        void FindDeepestHelper(value_type* root, int cur_lvl, int& max_lvl, value_type*& deepest_node, bool& from_right) {
            if (root != nullptr) {
                FindDeepestHelper(root->left, cur_lvl + 1, max_lvl, deepest_node, from_right);
                if (cur_lvl > max_lvl || (cur_lvl == max_lvl && !from_right)) {
                    deepest_node = root;
                    max_lvl = cur_lvl;
                    from_right = false;
                }
                if (cur_lvl == max_lvl && root->right != nullptr) {
                    deepest_node = root->right;
                    from_right = true;
                }

                FindDeepestHelper(root->right, cur_lvl + 1, max_lvl, deepest_node, from_right);
            }
        }

        value_type* FindHelper(value_type* node, Key key) const {
            if (!node) {
                return nullptr;
            }

            if (comp_(key, node->value)) {
                return FindHelper(node->left, key);
            } else if (comp_(node->value, key)) {
                return FindHelper(node->right, key);
            }

            return node;
        }

        size_t CountHelper(value_type* node, Key key, size_t cnt = 0) const {
            if (!node) {
                return cnt;
            }

            if (comp_(key, node->value)) {
                return CountHelper(node->left, key, cnt);
            } else if (comp_(node->value, key)) {
                return CountHelper(node->right, key);
            }

            return CountHelper(node->right, key, cnt + 1);
        }

        value_type* UpperBoundHelper(value_type* root, key_type value) {
            value_type* upper = nullptr;
            while (root) {
                if (comp_(value, root->value)) {
                    upper = root;
                    root = root->left;
                } else {
                    root = root->right;
                }
            }

            return upper;
        }

        value_type* LowerBoundHelper(value_type* root, key_type value) {
            value_type* lower = nullptr;
            while (root) {
                if (comp_(root->value, value)) {
                    lower = root;
                    root = root->right;
                } else {
                    root = root->left;
                }
            }

            return lower;
        }


        value_type* root_;
        value_type* end_;
        Compare comp_;
        typename std::allocator_traits<Allocator>::template rebind_alloc<Node<Key>> allocator_;
        size_t size_;
        TraversalTag tag_;
    };
} // end of namespace BST

