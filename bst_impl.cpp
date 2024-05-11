/*
*	DKU Operating System Lab
*	    Lab2 (Concurrent Data Structure)
*	    Student id : 32213947
*	    Student name : Kyungmin Jeon
*/

#include "bst_impl.h"

void BST::insert(int key, int value){
	Node* new_node = new Node{key, value, 0, nullptr, nullptr};

	if (root == nullptr) {
		root = new_node;
	}else {
		Node* current_node = root;
		Node* parent_node;

		while (current_node != nullptr) {
			parent_node = current_node;

			if (key < current_node->key) {
				current_node = current_node->left;
			}else if (key > current_node->key) {
				current_node = current_node->right;
			}else {
				current_node->value += value;
				current_node->upd_cnt++;
				return;
			}
		}

		if (key < parent_node->key) {
			parent_node->left = new_node;
		}else {
			parent_node->right = new_node;
		}

	}
}

int BST::lookup(int key) {
	Node* current_node = root;

	while (current_node != nullptr) {
		if (key < current_node->key) {
			current_node = current_node->left;
		}else if (key > current_node->key) {
			current_node = current_node->right;
		}else {
			return current_node->value;
		}
	}
	return 0;
} 

void BST::remove(int key){
	Node* current_node = root;
	Node* parent_node = nullptr;

	while (current_node != nullptr && current_node->key != key) {
		parent_node = current_node;
		if (key < current_node->key) {
			current_node = current_node->left;
		}else {
			current_node=current_node->right;
		}
	}

	if (current_node == nullptr) {
		return;
	}

	if (current_node->left == nullptr && current_node->right == nullptr) {
		if (parent_node == nullptr) {
			root = nullptr;
		}else if (parent_node->left == current_node) {
			parent_node->left = nullptr;
		}else {
			parent_node->right = nullptr;
		}
	}else if (current_node->left == nullptr) {
		if (parent_node == nullptr) {
			root = current_node->right;
		}else if (parent_node->left == current_node) {
			parent_node->left = current_node->right;
		}else {
			parent_node->right = current_node->right;
		}
	}else if (current_node->right == nullptr) {
		if (parent_node == nullptr) {
			root = current_node->left;
		}else if (parent_node->left == current_node) {
			parent_node->left = current_node->left;
		}else {
			parent_node->right = current_node->left;
		}
	}else {
		Node* right_smallest_node = current_node->right;
		Node* smallest_parent_node = current_node;

		while (right_smallest_node->left != nullptr) {
			smallest_parent_node = right_smallest_node;
			right_smallest_node = right_smallest_node->left;
		}

		if (smallest_parent_node->left == right_smallest_node) {
			smallest_parent_node->left = right_smallest_node->right;
		}else {
			smallest_parent_node->right = right_smallest_node->right;
		}

		current_node->key = right_smallest_node->key;
		current_node->value = right_smallest_node->value;
		current_node->upd_cnt = right_smallest_node->upd_cnt;
	}
}

void BST::traversal(KVC* traverse_arr) {
    inorder_algorithm(traverse_arr, root);
}

void CoarseBST::insert(int key, int value){
	pthread_mutex_lock(&mutex_lock);
	Node* new_node = new Node{key, value, 0, nullptr, nullptr};

	if (root == nullptr) {
		root = new_node;
	}else {
		Node* current_node = root;
		Node* parent_node;

		while (current_node != nullptr) {
			parent_node = current_node;

			if (key < current_node->key) {
				current_node = current_node->left;
			}else if (key > current_node->key) {
				current_node = current_node->right;
			}else {
				current_node->value += value;
				current_node->upd_cnt++;
				break;
			}
		}

		if (current_node == nullptr) {
			if (key < parent_node->key) {
				parent_node->left = new_node;
			}else {
				parent_node->right = new_node;
			}
		}
	}

	pthread_mutex_unlock(&mutex_lock);
}

int CoarseBST::lookup(int key) {
	pthread_mutex_lock(&mutex_lock);
	Node* current_node = root;
	int return_value = 0;

	while (current_node != nullptr) {
		if (key < current_node->key) {
			current_node = current_node->left;
		}else if (key > current_node->key) {
			current_node = current_node->right;
		}else {
			return_value = current_node->value;
			break;
		}
	}
	pthread_mutex_unlock(&mutex_lock);
	return return_value;
} 

void CoarseBST::remove(int key){
	pthread_mutex_lock(&mutex_lock);
	Node* current_node = root;
	Node* parent_node = nullptr;

	while (current_node != nullptr && current_node->key != key) {
		parent_node = current_node;
		if (key < current_node->key) {
			current_node = current_node->left;
		}else {
			current_node=current_node->right;
		}
	}

	if (current_node != nullptr) {
		if (current_node->left == nullptr && current_node->right == nullptr) {
			if (parent_node == nullptr) {
				root = nullptr;
			}else if (parent_node->left == current_node) {
				parent_node->left = nullptr;
			}else {
				parent_node->right = nullptr;
			}
		}else if (current_node->left == nullptr) {
			if (parent_node == nullptr) {
				root = current_node->right;
			}else if (parent_node->left == current_node) {
				parent_node->left = current_node->right;
			}else {
				parent_node->right = current_node->right;
			}
		}else if (current_node->right == nullptr) {
			if (parent_node == nullptr) {
				root = current_node->left;
			}else if (parent_node->left == current_node) {
				parent_node->left = current_node->left;
			}else {
				parent_node->right = current_node->left;
			}
		}else {
			Node* right_smallest_node = current_node->right;
			Node* smallest_parent_node = current_node;

			while (right_smallest_node->left != nullptr) {
				smallest_parent_node = right_smallest_node;
				right_smallest_node = right_smallest_node->left;
			}

			if (smallest_parent_node->left == right_smallest_node) {
				smallest_parent_node->left = right_smallest_node->right;
			}else {
				smallest_parent_node->right = right_smallest_node->right;
			}

			current_node->key = right_smallest_node->key;
			current_node->value = right_smallest_node->value;
			current_node->upd_cnt = right_smallest_node->upd_cnt;
		}
	}
	pthread_mutex_unlock(&mutex_lock);
}

void CoarseBST::traversal(KVC* traverse_arr) {
    inorder_algorithm(traverse_arr, root);
}

void FineBST::insert(int key, int value){
	// Todo
}

int FineBST::lookup(int key) {
	// Todo
	return 0;
} 

void FineBST::remove(int key){
	// Todo
}

void FineBST::traversal(KVC* traverse_arr) {
    // Todo
}