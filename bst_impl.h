/*
*	DKU Operating System Lab
*	    Lab2 (Concurrent Data Structure)
*	    Student id : 32213947
*	    Student name : Kyungmin Jeon
*/

#include "bst.h"

#ifndef BST_IMPI_H
#define BST_IMPI_H

/**
 * @brief BST without lock
 * DefaultBST의 함수를 오버라이드하여, 클래스를 완성한다.
 * 구현에 필요한 멤버 변수/함수를 추가하여 구현한다.
 */
class BST : public DefaultBST {
	private:
		Node* root = nullptr;
		int idx = 0;

	public:
		void insert(int key, int value) override;
		int lookup(int key) override;
		void remove(int key) override;
		void traversal(KVC* arr) override;

		// 원활한 중위 순회를 위해 사용한 재귀 함수
		void inorder_algorithm(KVC* arr, Node* node) {
			if (node == nullptr) {
				return;
			}

			inorder_algorithm(arr, node->left);
			arr[idx++] = {node->key, node->value, node->upd_cnt};
			inorder_algorithm(arr, node->right);
		}
};

/**
 * @brief BST with coarse-grained lock
 * BST 전체를 critical section으로 가정하여, 하나의 lock으로 이를 관리한다.
 * DefaultBST의 함수를 오버라이드하여, 클래스를 완성한다.
 * 구현에 필요한 멤버 변수/함수를 추가하여 구현한다.
 */
class CoarseBST : public DefaultBST {
	private:
		Node* root = nullptr;
		int idx = 0;
		pthread_mutex_t mutex_lock;
	public:
		CoarseBST() {
			pthread_mutex_init(&mutex_lock, nullptr);
		}

		void insert(int key, int value) override;
		int lookup(int key) override;
		void remove(int key) override;
		void traversal(KVC* arr) override;

		// 원활한 중위 순회를 위해 사용한 재귀 함수
		void inorder_algorithm(KVC* arr, Node* node) {
			if (node == nullptr) {
				return;
			}

			inorder_algorithm(arr, node->left);
			arr[idx++] = {node->key, node->value, node->upd_cnt};
			inorder_algorithm(arr, node->right);
		}
};

/// @brief FineBST는 FineNode를 정의하여 사용하길 권장한다.
struct FineNode : public Node {
	FineNode* left;
	FineNode* right;
    pthread_mutex_t mutex_lock;

	// 구조체 생성자 : Node에 존재하는 mutex_lock의 초기 값 입력 받지 않고 초기화 하고자 선언
	FineNode(int key, int value, int upd_cnt, FineNode* left, FineNode* right) {
		this->key = key;
		this->value = value;
		this->upd_cnt = upd_cnt;
		this->left = left;
		this->right = right;
		pthread_mutex_init(&mutex_lock, nullptr);
	}
};

/**
 * @brief BST with fine-grained lock
 * BST 내부의 critical section을 Node 단위로 lock으로 관리한다.
 * Node를 상속받은 FineNode를 정의하여 사용하길 권장한다.
 * DefaultBST의 함수를 오버라이드하여, 클래스를 완성한다.
 * 구현에 필요한 멤버 변수/함수를 추가하여 구현한다.
 */
class FineBST : public DefaultBST {
	private:
		FineNode* root = nullptr;
		// root가 nullptr인 경우에서 동시 삽입할 때를 위한 lock
		pthread_mutex_t root_nullptr_lock;
		int idx = 0;

	public:
		FineBST() {
			pthread_mutex_init(&root_nullptr_lock, nullptr);
		}

		void insert(int key, int value) override;
		int lookup(int key) override;
		void remove(int key) override;
		void traversal(KVC* arr) override;

		// 원활한 중위 순회를 위해 사용한 재귀 함수
		void inorder_algorithm(KVC* arr, FineNode* node) {
			if (node == nullptr) {
				return;
			}

			inorder_algorithm(arr, node->left);
			arr[idx++] = {node->key, node->value, node->upd_cnt};
			inorder_algorithm(arr, node->right);
		}
};
#endif