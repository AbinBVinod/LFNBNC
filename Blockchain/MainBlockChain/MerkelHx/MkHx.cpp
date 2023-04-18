#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <sstream>

// Function to calculate the hash of a given string
std::string sha256(const std::string& input) {
    // simplicity
    // proper cryptographic library for real-world usage
    return "hash(" + input + ")";
}

// Merkle Tree Node class  (upgrade)
class MerkleTreeNode {
public:
    MerkleTreeNode(const std::string& data) : data_(data) {}
    std::string getData() const { return data_; }
    std::string getHash() const { return hash_; }
    void setHash(const std::string& hash) { hash_ = hash; }
private:
    std::string data_;
    std::string hash_;
};  //root public

// Merkle Tree class
class MerkleTree {
public:
    MerkleTree(const std::vector<std::string>& data) {
        // Build the Merkle Tree  ( )
        root_ = buildTree(data);
    }

    std::string getRootHash() const { return root_->getHash(); }

private:
    MerkleTreeNode* buildTree(const std::vector<std::string>& data) {
        // If there's only one data element, create a leaf node for it
        if (data.size() == 1) {
            MerkleTreeNode* leaf = new MerkleTreeNode(data[0]);
            leaf->setHash(sha256(data[0]));
            return leaf;
        }

        // Create child nodes by recursively building subtrees
        std::vector<MerkleTreeNode*> children;
        for (const std::string& d : data) {
            MerkleTreeNode* leaf = new MerkleTreeNode(d);
            leaf->setHash(sha256(d));
            children.push_back(leaf);
        }

        //  child nodes is odd, duplicate the last node to make it even
        if (children.size() % 2 != 0) {
            MerkleTreeNode* last_node = children.back();
            children.push_back(new MerkleTreeNode(last_node->getData()));
        }

        //  parent nodes by combining pairs of child nodes
        std::vector<MerkleTreeNode*> parents;
        for (size_t i = 0; i < children.size(); i += 2) {
            MerkleTreeNode* left = children[i];
            MerkleTreeNode* right = children[i + 1];
            std::string combined_data = left->getHash() + right->getHash();
            MerkleTreeNode* parent = new MerkleTreeNode(combined_data);
            parent->setHash(sha256(combined_data));
            parent->left_ = left;
            parent->right_ = right;
            parents.push_back(parent);
        }

        //  the Merkle Tree with the parent nodes (Zk needtoo import / Graph)
        return buildTree(parents);
    }

    MerkleTreeNode* root_;
};

int main() {
    // usage
    std::vector<std::string> data = { "data1", "data2", "data3", "data4" };
    MerkleTree merkleTree(data);
    std::cout << "Root hash: " << merkleTree.getRootHash() << std::endl;

    return 0;
}
