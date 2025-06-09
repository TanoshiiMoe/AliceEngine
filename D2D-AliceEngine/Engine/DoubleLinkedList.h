#pragma once
/*
* 	@brief Iterator를 지원하는 이중 연결 리스트 클래스입니다.
*   @details 이 클래스는 이중 연결 리스트를 구현하며, 노드를 순회할 수 있는 반복자(iterator)가 있습니다.
*   @templte param T 저장할 데이터의 타입을 지정합니다.
* 	@note 이 클래스는 C++ 표준 라이브러리의 std::list와 유사한 기능을 제공합니다.
*/

/*
*  제공하는 메서드
* @ Insert() 메서드를 사용하여 특정 위치에 데이터를 삽입
* @ push_back() 메서드를 사용하여 리스트의 끝에 데이터를 추가
* @ push_front() 메서드를 사용하여 리스트의 시작에 데이터를 추가
* @ pop_front() 메서드를 사용하여 리스트의 시작에서 데이터를 제거하고 반환
* @ pop_back() 메서드를 사용하여 리스트의 끝에서 데이터를 제거하고 반환
* @ find() 메서드를 사용하여 특정 데이터를 찾고 해당 노드를 반환
* @ erase() 메서드를 사용하여 특정 노드를 제거
* @ remove() 메서드를 사용하여 특정 데이터를 제거
* @ clear() 메서드를 사용하여 리스트의 모든 데이터를 제거
* @ size() 메서드를 사용하여 리스트의 크기를 반환
* @ front() 메서드를 사용하여 리스트의 첫 번째 데이터를 반환
* @ back() 메서드를 사용하여 리스트의 마지막 데이터를 반환
* @ empty() 메서드를 사용하여 리스트가 비어있는지 확인
*/
template<typename T>
class DoubleLinkedList
{
public:
	DoubleLinkedList() {}
	~DoubleLinkedList() {}

	/*
	* 	@brief 	이중 연결 리스트의 노드를 순회할 수 있는 반복자(iterator) 구조체입니다.
	* 	@details 이 구조체는 이중 연결 리스트의 노드를 순회할 수 있도록 구현되어 있습니다.
	*/
	struct Iterator
	{
		using NodePtr = typename DoubleLinkedList<T>::Node*;
		NodePtr current;
		Iterator(NodePtr node) : current(node) {}
		T& operator*()
		{
			return current->data;
		}
		Iterator& operator++()
		{
			current = current->next;
			return *this;
		}
		Iterator operator++(int)
		{
			Iterator temp = *this;
			++(*this);
			return temp;
		}
		bool operator!=(const Iterator& other) const
		{
			return current != other.current;
		}
		void operator =(NodePtr node)
		{
			current = node;
		}
	};

	Iterator begin() { return Iterator(m_head); }
	Iterator end() { if (m_tail == nullptr) return nullptr;  return Iterator(m_tail->next); }

	/*
	* 	@brief 	이중 연결 리스트의 노드를 순회할 수 있는 반복자(Reverse_Iterator) 구조체입니다.
	* 	@details 이 구조체는 이중 연결 리스트의 노드를 거꾸로 순회할 수 있도록 구현되어 있습니다.
	*/
	struct Reverse_Iterator
	{
		using NodePtr = typename DoubleLinkedList<T>::Node*;
		NodePtr current;
		Reverse_Iterator(NodePtr node) : current(node) {}
		T& operator*()
		{
			return current->data;
		}
		Reverse_Iterator& operator++()
		{
			current = current->prev;
			return *this;
		}
		Reverse_Iterator operator++(int)
		{
			Reverse_Iterator temp = *this;
			++(*this);
			return temp;
		}
		bool operator!=(const Reverse_Iterator& other) const
		{
			return current != other.current;
		}
		void operator =(NodePtr node)
		{
			current = node;
		}
	};

	Reverse_Iterator rbegin() { return Reverse_Iterator(m_tail); }
	Reverse_Iterator rend() { return Reverse_Iterator(m_head->prev); }

	/*
	* 	@brief 데이터가 저장될 노드입니다. 이전, 이후 포인터를 저장합니다.
	*/
	struct Node
	{
		T data;
		Node* next;
		Node* prev;
		Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
	};

	Node* m_head = nullptr;
	Node* m_tail = nullptr;
	int m_size = 0;

	Node* Insert(Node* prevNode, const T& value)
	{
		Node* newNode = new Node(value);
		// prevNode가 비어있는지 아닌지에 따라 다르게 처리합니다.
		if (prevNode == nullptr)
		{
			newNode->next = m_head;
			if (m_head != nullptr) m_head->prev = newNode;
			m_head = newNode;
			if (m_tail == nullptr) m_tail = newNode;
		}
		else
		{
			newNode->next = prevNode->next;
			newNode->prev = prevNode;
			prevNode->next = newNode;
			if (newNode->next != nullptr)
			{
				newNode->next->prev = newNode;
			}
			if (m_tail == prevNode)
			{
				m_tail = newNode;
			}
		}
		return newNode;
	}

	// 리스트가 비어있는지 아닌지에 따라 다르게 처리합니다.
	void push_back(const T& value)
	{
		if (m_tail == nullptr)
		{
			m_head = m_tail = new Node(value);
		}
		else
		{
			m_tail = Insert(m_tail, value);
		}
		m_size++;
	}

	// 찾지 못했다면 end()를 반환합니다.
	Node* find(const T& value)
	{
		Node* current = m_head;
		while (current != nullptr)
		{
			if (current->data == value)
			{
				return current;
			}
			current = current->next;
		}
		return m_tail->next;
	}

	Node* erase(Node* node)
	{
		if (node == nullptr)
		{
			return nullptr;
		}
		if (node->prev != nullptr)
		{
			node->prev->next = node->next;
		}
		if (node->next != nullptr)
		{
			node->next->prev = node->prev;
		}
		if (m_head == node)
		{
			m_head = node->next;
		}
		if (m_tail == node)
		{
			m_tail = node->prev;
		}
		delete node;
		m_size--;
		return m_head;
	}

	void remove(const T& value)
	{
		Node* node = find(value);
		if (node != nullptr)
		{
			erase(node);
		}
	}

	void clear()
	{
		Node* current = m_head;
		while (current != nullptr)
		{
			Node* nextNode = current->next;
			delete current;
			current = nextNode;
		}
		m_size = 0;
		m_head = m_tail = nullptr;
	}

	int size() const
	{
		return m_size;
	}

	T* front() const
	{
		if (m_head == nullptr)
		{
			return nullptr;
		}
		return &(m_head->data);
	}

	T* back() const
	{
		if (m_tail == nullptr)
		{
			return nullptr;
		}
		return &(m_tail->data);
	}

	void push_front(const T& value)
	{
		if (m_head == nullptr)
		{
			m_head = m_tail = new Node(value);
		}
		else
		{
			Node* newNode = new Node(value);
			newNode->next = m_head;
			m_head->prev = newNode;
			m_head = newNode;
		}
		m_size++;
	}

	void push_back(Node* node)
	{
		if (node == nullptr)
		{
			return; // nullptr인 경우 아무것도 하지 않습니다.
		}
		if (m_tail == nullptr)
		{
			m_head = m_tail = node;
			node->next = nullptr;
			node->prev = nullptr;
		}
		else
		{
			m_tail->next = node;
			node->prev = m_tail;
			node->next = nullptr;
			m_tail = node;
		}
		m_size++;
	}

	Node* pop_front()
	{
		if (m_head == nullptr)
		{
			return m_tail->next; // 리스트가 비어있으면 아무것도 하지 않습니다.
		}
		Node* temp = m_head;
		m_head = m_head->next;
		if (m_head != nullptr)
		{
			m_head->prev = nullptr;
		}
		else
		{
			m_tail = nullptr; // 리스트가 비어있게 되면 tail도 nullptr로 설정합니다.
		}
		m_size--;
		return temp;
	}

	Node* pop_back()
	{
		if (m_tail == nullptr)
		{
			return nullptr; // 리스트가 비어있으면 아무것도 하지 않습니다.
		}
		Node* temp = m_tail;
		m_tail = m_tail->prev;
		if (m_tail != nullptr)
		{
			m_tail->next = nullptr;
		}
		else
		{
			m_head = nullptr; // 리스트가 비어있게 되면 head도 nullptr로 설정합니다.
		}
		m_size--;
		return temp;
	}

	bool empty() const
	{
		return m_size == 0;
	}

	void splice(Iterator& it, const DoubleLinkedList<T>& list)
	{
		if (list.empty()) return; // 리스트가 비어있으면 아무것도 하지 않습니다.
		Node* node = it.current;
		Node* listHead = list.m_head;
		Node* listTail = list.m_tail;
		if (node == nullptr) // it가 end()인 경우
		{
			if (m_tail != nullptr)
			{
				m_tail->next = listHead;
				listHead->prev = m_tail;
			}
			else
			{
				m_head = listHead;
			}
			m_tail = listTail;
		}
		else // it가 유효한 노드를 가리키는 경우
		{
			if (node->prev != nullptr)
			{
				node->prev->next = listHead;
				listHead->prev = node->prev;
			}
			else
			{
				m_head = listHead; // it가 head인 경우
			}
			listTail->next = node;
			node->prev = listTail;
		}
		list.m_head = list.m_tail = nullptr; // 리스트를 비웁니다.
		m_size += list.size();
	}
};