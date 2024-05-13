/*
*	DKU Operating System Lab
*	    Lab2 (Concurrent Data Structure)
*	    Student id : 32213947
*	    Student name : Kyungmin Jeon
*/

#include "bst_impl.h"

void BST::insert(int key, int value){
	// 새 node 생성
	Node* new_node = new Node{key, value, 0, nullptr, nullptr};

	// root가 존재하지 않을 경우 새 node를 root로 설정
	if (root == nullptr) {
		root = new_node;
	}else {
		Node* current_node = root;
		Node* parent_node;

		// 새 node를 삽입할 위치 탐색
		while (current_node != nullptr) {
			parent_node = current_node;

			if (key < current_node->key) {
				// 현재 탐색하고 있는 node의 key 값보다 작다면 node의 왼쪽 자식 node로 이동
				current_node = current_node->left;
			}else if (key > current_node->key) {
				// 현재 탐색하고 있는 node의 key 값보다 크다면 node의 오른쪽 자식 node로 이동
				current_node = current_node->right;
			}else {
				// 현재 탐색하고 있는 node의 key 값과 같다면 node의 값 수정 후 함수 종료
				current_node->value += value;
				current_node->upd_cnt++;
				return;
			}
		}

		// 삽입할 위치에 도착하였다면 key 값 비교 후 적절한 위치에 삽입
		if (key < parent_node->key) {
			parent_node->left = new_node;
		}else {
			parent_node->right = new_node;
		}

	}
}

int BST::lookup(int key) {
	Node* current_node = root;

	// node가 존재하지 않을 때까지 탐색
	while (current_node != nullptr) {
		if (key < current_node->key) {
			// 현재 탐색하고 있는 node의 key 값보다 작다면 node의 왼쪽 자식 node로 이동
			current_node = current_node->left;
		}else if (key > current_node->key) {
			// 현재 탐색하고 있는 node의 key 값보다 크다면 node의 오른쪽 자식 node로 이동
			current_node = current_node->right;
		}else {
			// 현재 node의 키 값과 같다면 node의 value 반환 후 종료
			return current_node->value;
		}
	}
	return 0;
} 

void BST::remove(int key){
	Node* current_node = root;
	Node* parent_node = nullptr;

	// node 탐색 : 원하는 node를 찾거나, 존재하지 않음을 확인할 때까지
	while (current_node != nullptr && current_node->key != key) {
		parent_node = current_node;
		if (key < current_node->key) {
			current_node = current_node->left;
		}else {
			current_node = current_node->right;
		}
	}

	// node 없으면 함수 종료
	if (current_node == nullptr) {
		return;
	}

	// 삭제하려는 노드가 자식 노드가 없는 경우
	if (current_node->left == nullptr && current_node->right == nullptr) {
		// 동시에 부모 노드가 없는 경우 (본인이 root인 경우)
		if (parent_node == nullptr) {
			root = nullptr;
		// 본인이 부모 노드의 왼쪽 자식 노드인 경우 부모 노드의 왼쪽 자식 노드의 링크 삭제
		}else if (parent_node->left == current_node) {
			parent_node->left = nullptr;
		// 본인이 부모 노드의 오른쪽 자식 노드인 경우 부모 노드의 오른쪽 자식 노드의 링크 삭제
		}else {
			parent_node->right = nullptr;
		}
	// 삭제하려는 노드가 왼쪽 자식 노드가 없는 경우
	}else if (current_node->left == nullptr) {
		// 동시에 부모 노드가 없는 경우 (본인이 root인 경우) root를 본인의 오른쪽 자식 노드로 대체
		if (parent_node == nullptr) {
			root = current_node->right;
		// 본인이 부모 노드의 왼쪽 자식 노드인 경우 부모 노드의 왼쪽 자식 노드를 자신의 오른쪽 자식 노드로 대체
		}else if (parent_node->left == current_node) {
			parent_node->left = current_node->right;
		// 본인이 부모 노드의 오른쪽 자식 노드인 경우 부모 노드의 오른쪽 자식 노드를 자신의 오른쪽 자식 노드로 대체
		}else {
			parent_node->right = current_node->right;
		}
	// 삭제하려는 노드가 오른쪽 자식 노드가 없는 경우
	}else if (current_node->right == nullptr) {
		// 동시에 부모 노드가 없는 경우 (본인이 root인 경우) root를 본인의 왼쪽 자식 노드로 대체
		if (parent_node == nullptr) {
			root = current_node->left;
		// 본인이 부모 노드의 왼쪽 자식 노드인 경우 부모 노드의 왼쪽 자식 노드를 자신의 왼쪽 자식 노드로 대체
		}else if (parent_node->left == current_node) {
			parent_node->left = current_node->left;
		// 본인이 부모 노드의 오른쪽 자식 노드인 경우 부모 노드의 오른쪽 자식 노드를 자신의 왼쪽 자식 노드로 대체
		}else {
			parent_node->right = current_node->left;
		}
	// 삭제하려는 노드에 양쪽 자식 노드가 모두 있는 경우
	}else {
		Node* right_smallest_node = current_node->right;
		Node* smallest_parent_node = current_node;

		// 삭제하려는 노드의 하위 레벨 노드들 중 가장 작은 키 값을 가지고 있는 노드 탐색
		while (right_smallest_node->left != nullptr) {
			smallest_parent_node = right_smallest_node;
			right_smallest_node = right_smallest_node->left;
		}

		// 삭제하려는 노드의 하위 레벨 노드들 중 가장 작은 키 값을 가지고 있는 노드를 삭제
		if (smallest_parent_node->left == right_smallest_node) {
			smallest_parent_node->left = right_smallest_node->right;
		}else {
			smallest_parent_node->right = right_smallest_node->right;
		}

		// 삭제하려는 노드의 값을 삭제하려는 노드의 하위 레벨 노드들 중 가장 작은 키 값을 가지고 있는 노드의 값으로 대체
		current_node->key = right_smallest_node->key;
		current_node->value = right_smallest_node->value;
		current_node->upd_cnt = right_smallest_node->upd_cnt;
	}
}

void BST::traversal(KVC* traverse_arr) {
    inorder_algorithm(traverse_arr, root);
}

void CoarseBST::insert(int key, int value){
	// Coarse-grained lock
	pthread_mutex_lock(&mutex_lock);

	// 새 node 생성
	Node* new_node = new Node{key, value, 0, nullptr, nullptr};

	// root가 존재하지 않을 경우 새 node를 root로 설정
	if (root == nullptr) {
		root = new_node;
	}else {
		Node* current_node = root;
		Node* parent_node;

		// 새 node를 삽입할 위치 탐색
		while (current_node != nullptr) {
			parent_node = current_node;

			if (key < current_node->key) {
				// 현재 탐색하고 있는 node의 key 값보다 작다면 node의 왼쪽 자식 node로 이동
				current_node = current_node->left;
			}else if (key > current_node->key) {
				// 현재 탐색하고 있는 node의 key 값보다 크다면 node의 오른쪽 자식 node로 이동
				current_node = current_node->right;
			}else {
				// 현재 탐색하고 있는 node의 key 값과 같다면 node의 값 수정 후 반복문 탈출
				current_node->value += value;
				current_node->upd_cnt++;
				break;
			}
		}

		// 삽입할 위치에 도착하였다면 key 값 비교 후 적절한 위치에 삽입
		if (current_node == nullptr) {
			if (key < parent_node->key) {
				parent_node->left = new_node;
			}else {
				parent_node->right = new_node;
			}
		}
	}
	// Coarse-grained unlock
	pthread_mutex_unlock(&mutex_lock);
}

int CoarseBST::lookup(int key) {
	// Coarse-grained lock
	pthread_mutex_lock(&mutex_lock);

	Node* current_node = root;
	int return_value = 0;

	// node가 존재하지 않을 때까지 탐색
	while (current_node != nullptr) {
		if (key < current_node->key) {
			// 현재 탐색하고 있는 node의 key 값보다 작다면 node의 왼쪽 자식 node로 이동
			current_node = current_node->left;
		}else if (key > current_node->key) {
			// 현재 탐색하고 있는 node의 key 값보다 크다면 node의 오른쪽 자식 node로 이동
			current_node = current_node->right;
		}else {
			// 현재 node의 키 값과 같다면 node의 value 반환 후 종료
			return_value = current_node->value;
			break;
		}
	}
	// Coarse-grained unlock
	pthread_mutex_unlock(&mutex_lock);
	return return_value;
} 

void CoarseBST::remove(int key){
	// Coarse-grained lock
	pthread_mutex_lock(&mutex_lock);

	Node* current_node = root;
	Node* parent_node = nullptr;

	while (current_node != nullptr && current_node->key != key) {
		parent_node = current_node;
		if (key < current_node->key) {
			current_node = current_node->left;
		}else {
			current_node = current_node->right;
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
	// Coarse-grained unlock
	pthread_mutex_unlock(&mutex_lock);
}

void CoarseBST::traversal(KVC* traverse_arr) {
    inorder_algorithm(traverse_arr, root);
}

void FineBST::insert(int key, int value){
	FineNode* new_node = new FineNode{key, value, 0, nullptr, nullptr};

	// fine-grained lock for root
	pthread_mutex_lock(&root_nullptr_lock);
	if (root == nullptr) {
		root = new_node;
		// fine-grained unlock for root
		pthread_mutex_unlock(&root_nullptr_lock);
	}else {
		// fine-grained unlock for root
		pthread_mutex_unlock(&root_nullptr_lock);

		FineNode* current_node = root;
		FineNode* parent_node = root;

		// fine-grained lock for parent_node
		pthread_mutex_lock(&parent_node->mutex_lock);

		while (current_node != nullptr) {
			// fine-grained unlock for parent_node
			pthread_mutex_unlock(&parent_node->mutex_lock);

			parent_node = current_node;

			// fine-grained lock for parent_node
			pthread_mutex_lock(&parent_node->mutex_lock);
			
			if (key < current_node->key) {
				current_node = current_node->left;
			}else if (key > current_node->key) {
				current_node = current_node->right;
			}else {
				current_node->value += value;
				current_node->upd_cnt++;

				// fine-grained unlock for parent_node
				pthread_mutex_unlock(&parent_node->mutex_lock);
				break;
			}
		}

		if (current_node == nullptr) {
			if (key < parent_node->key) {
				parent_node->left = new_node;
			}else {
				parent_node->right = new_node;
			}

			// fine-grained unlock for parent_node
			pthread_mutex_unlock(&parent_node->mutex_lock);
		}
	}
}

int FineBST::lookup(int key) {
	FineNode* current_node = root;
	int return_value = 0;

	while (current_node != nullptr) {
		// fine-grained lock for current_node
		pthread_mutex_lock(&current_node->mutex_lock);
		if (key < current_node->key) {
			// fine-grained unlock for current_node
			pthread_mutex_unlock(&current_node->mutex_lock);
			current_node = current_node->left;
		}else if (key > current_node->key) {
			// fine-grained unlock for current_node
			pthread_mutex_unlock(&current_node->mutex_lock);
			current_node = current_node->right;
		}else {
			return_value = current_node->value;
			// fine-grained unlock for current_node
			pthread_mutex_unlock(&current_node->mutex_lock);
			break;
		}
	}
	return return_value;
} 

void FineBST::remove(int key){
	FineNode* current_node = root;
	FineNode* parent_node = nullptr;

	while (current_node != nullptr && current_node->key != key) {
		parent_node = current_node;
		if (key < current_node->key) {
			current_node = current_node->left;
		}else {
			current_node = current_node->right;
		}
	}

	if (current_node != nullptr) {
		// fine-grained lock for current_node
		pthread_mutex_lock(&current_node->mutex_lock);
		if (current_node->left == nullptr && current_node->right == nullptr) {
			if (parent_node == nullptr) {
				root = nullptr;
			}else if (parent_node->left == current_node) {
				parent_node->left = nullptr;
			}else {
				parent_node->right = nullptr;
			}
			// fine-grained unlock for current_node
			pthread_mutex_unlock(&current_node->mutex_lock);
		}else if (current_node->left == nullptr) {
			if (parent_node == nullptr) {
				root = current_node->right;
			}else if (parent_node->left == current_node) {
				parent_node->left = current_node->right;
			}else {
				parent_node->right = current_node->right;
			}
			// fine-grained unlock for current_node
			pthread_mutex_unlock(&current_node->mutex_lock);
		}else if (current_node->right == nullptr) {
			if (parent_node == nullptr) {
				root = current_node->left;
			}else if (parent_node->left == current_node) {
				parent_node->left = current_node->left;
			}else {
				parent_node->right = current_node->left;
			}
			// fine-grained unlock for current_node
			pthread_mutex_unlock(&current_node->mutex_lock);
		}else {
			FineNode* right_smallest_node = current_node->right;
			FineNode* smallest_parent_node = current_node;

			while (right_smallest_node->left != nullptr) {
				smallest_parent_node = right_smallest_node;
				right_smallest_node = right_smallest_node->left;
			}

			if (smallest_parent_node->left == right_smallest_node) {
				smallest_parent_node->left = right_smallest_node->right;
			}else {
				smallest_parent_node->right = right_smallest_node->right;
			}

			// fine-grained lock for right_smallest_node
			pthread_mutex_lock(&right_smallest_node->mutex_lock);
			current_node->key = right_smallest_node->key;
			current_node->value = right_smallest_node->value;
			current_node->upd_cnt = right_smallest_node->upd_cnt;
			// fine-grained unlock for right_smallest_node
			pthread_mutex_unlock(&right_smallest_node->mutex_lock);
			// fine-grained unlock for current_node
			pthread_mutex_unlock(&current_node->mutex_lock);
		}
	}
}

void FineBST::traversal(KVC* traverse_arr) {
    inorder_algorithm(traverse_arr, root);
}