#pragma once

#include <memory>
#include <initializer_list>
#include <iterator>



template <typename Type> class SingleLinkedList {
  // Узел списка
  struct Node {
    Node() = default;
    Node(const Type &val) : value(val) {}
    Node(const Type &val, Node *next) : value(val), next_node(next) {}
    Type value;
    std::unique_ptr<Node> next_node = nullptr;
  };
  template <typename ValueType> class BasicIterator {
    // Класс списка объявляется дружественным, чтобы из методов списка
    // был доступ к приватной области итератора
    friend class SingleLinkedList;

    // Конвертирующий конструктор итератора из указателя на узел списка
    explicit BasicIterator(Node *node) : node_(node) {

      // Реализуйте конструктор самостоятельно
    }

  public:
    // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого
    // итератора

    // Категория итератора — forward iterator
    // (итератор, который поддерживает операции инкремента и многократное
    // разыменование)
    using iterator_category = std::forward_iterator_tag;
    // Тип элементов, по которым перемещается итератор
    using value_type = Type;
    // Тип, используемый для хранения смещения между итераторами
    using difference_type = std::ptrdiff_t;
    // Тип указателя на итерируемое значение
    using pointer = ValueType *;
    // Тип ссылки на итерируемое значение
    using reference = ValueType &;

    BasicIterator() = default;

    // Конвертирующий конструктор/конструктор копирования
    // При ValueType, совпадающем с Type, играет роль копирующего конструктора
    // При ValueType, совпадающем с const Type, играет роль конвертирующего
    // конструктора
    BasicIterator(const BasicIterator<Type> &other) noexcept
        : node_(reinterpret_cast<Node *>(other.node_)) {}

    // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при
    // наличии пользовательского конструктора копирования, явно объявим оператор
    // = и попросим компилятор сгенерировать его за нас
    BasicIterator &operator=(const BasicIterator &rhs) = default;

    // Оператор сравнения итераторов (в роли второго аргумента выступает
    // константный итератор) Два итератора равны, если они ссылаются на один и
    // тот же элемент списка либо на end()
    [[nodiscard]] bool operator==(const BasicIterator<const Type> &rhs) const
        noexcept {
      return node_ == reinterpret_cast<Node *>(rhs.node_);
      // Заглушка. Реализуйте оператор самостоятельно
    }

    // Оператор проверки итераторов на неравенство
    // Противоположен !=
    [[nodiscard]] bool operator!=(const BasicIterator<const Type> &rhs) const
        noexcept {
      return node_ != reinterpret_cast<Node *>(rhs.node_);
      // Заглушка. Реализуйте оператор самостоятельно
    }

    // Оператор сравнения итераторов (в роли второго аргумента итератор)
    // Два итератора равны, если они ссылаются на один и тот же элемент списка
    // либо на end()
    [[nodiscard]] bool operator==(const BasicIterator<Type> &rhs) const
        noexcept {
      return node_ == reinterpret_cast<Node *>(rhs.node_);
      // Заглушка. Реализуйте оператор самостоятельно
    }

    // Оператор проверки итераторов на неравенство
    // Противоположен !=
    [[nodiscard]] bool operator!=(const BasicIterator<Type> &rhs) const
        noexcept {
      return node_ != reinterpret_cast<Node *>(rhs.node_);
      // Заглушка. Реализуйте оператор самостоятельно
    }

    // Оператор прединкремента. После его вызова итератор указывает на следующий
    // элемент списка Возвращает ссылку на самого себя Инкремент итератора, не
    // указывающего на существующий элемент списка, приводит к неопределённому
    // поведению
    BasicIterator &operator++() noexcept {
      node_ = node_->next_node.get();
      return *this;
      // Заглушка. Реализуйте оператор самостоятельно
    }

    // Оператор постинкремента. После его вызова итератор указывает на следующий
    // элемент списка Возвращает прежнее значение итератора Инкремент итератора,
    // не указывающего на существующий элемент списка, приводит к
    // неопределённому поведению
    BasicIterator operator++(int) noexcept {
      auto tmp = node_;
      node_ = node_->next_node.get();
      return BasicIterator(tmp);
      // Заглушка. Реализуйте оператор самостоятельно
    }

    // Операция разыменования. Возвращает ссылку на текущий элемент
    // Вызов этого оператора у итератора, не указывающего на существующий
    // элемент списка, приводит к неопределённому поведению
    [[nodiscard]] reference operator*() const noexcept {
      return (node_->value);
      // Не реализовано
      // Заглушка. Реализуйте оператор самостоятельно
    }

    // Операция доступа к члену класса. Возвращает указатель на текущий элемент
    // списка Вызов этого оператора у итератора, не указывающего на существующий
    // элемент списка, приводит к неопределённому поведению
    [[nodiscard]] pointer operator->() const noexcept {
      return &(node_->value);
      // Заглушка. Реализуйте оператор самостоятельно
    }

  private:
    Node *node_ = nullptr;
  };

public:
  using value_type = Type;
  using reference = value_type &;
  using const_reference = const value_type &;

  // Итератор, допускающий изменение элементов списка
  using Iterator = BasicIterator<Type>;
  // Константный итератор, предоставляющий доступ для чтения к элементам списка
  using ConstIterator = BasicIterator<const Type>;

  // Возвращает итератор, ссылающийся на первый элемент
  // Если список пустой, возвращённый итератор будет равен end()
  [[nodiscard]] Iterator begin() noexcept {

    // Реализуйте самостоятельно
    return Iterator(head_.next_node.get());
  }

  // Возвращает итератор, указывающий на позицию, следующую за последним
  // элементом односвязного списка Разыменовывать этот итератор нельзя — попытка
  // разыменования приведёт к неопределённому поведению
  [[nodiscard]] Iterator end() noexcept {

    // Реализуйте самостоятельно
    return Iterator(nullptr);
  }

  // Возвращает константный итератор, ссылающийся на первый элемент
  // Если список пустой, возвращённый итератор будет равен end()
  // Результат вызова эквивалентен вызову метода cbegin()
  [[nodiscard]] ConstIterator begin() const noexcept {

    // Реализуйте самостоятельно
    return ConstIterator(head_.next_node.get());
  }

  // Возвращает константный итератор, указывающий на позицию, следующую за
  // последним элементом односвязного списка Разыменовывать этот итератор нельзя
  // — попытка разыменования приведёт к неопределённому поведению Результат
  // вызова эквивалентен вызову метода cend()
  [[nodiscard]] ConstIterator end() const noexcept {

    return ConstIterator(nullptr);
  }

  // Возвращает константный итератор, ссылающийся на первый элемент
  // Если список пустой, возвращённый итератор будет равен cend()
  [[nodiscard]] ConstIterator cbegin() const noexcept {

    return ConstIterator(head_.next_node.get());
  }

  // Возвращает константный итератор, указывающий на позицию, следующую за
  // последним элементом односвязного списка Разыменовывать этот итератор нельзя
  // — попытка разыменования приведёт к неопределённому поведению
  [[nodiscard]] ConstIterator cend() const noexcept {

    return ConstIterator(nullptr);
  }

  [[nodiscard]] Iterator before_begin() noexcept { return Iterator(&head_); }

  // Возвращает константный итератор, указывающий на позицию перед первым
  // элементом односвязного списка. Разыменовывать этот итератор нельзя -
  // попытка разыменования приведёт к неопределённому поведению
  [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
    return ConstIterator(&head_);
  }

  // Возвращает константный итератор, указывающий на позицию перед первым
  // элементом односвязного списка. Разыменовывать этот итератор нельзя -
  // попытка разыменования приведёт к неопределённому поведению
  [[nodiscard]] ConstIterator before_begin() const noexcept {
    return ConstIterator(&head_);
  }

  /*
   * Вставляет элемент value после элемента, на который указывает pos.
   * Возвращает итератор на вставленный элемент
   * Если при создании элемента будет выброшено исключение, список останется в
   * прежнем состоянии
   */
  Iterator InsertAfter(ConstIterator pos, const Type &value) {
    auto new_node = std::make_unique<Node>(value);
    new_node->next_node = std ::move(pos.node_->next_node);
    pos.node_->next_node = std ::move(new_node);
    size_++;
    return Iterator(pos.node_->next_node.get());
  }

  void PopFront() noexcept {
    head_.next_node = std::move(head_.next_node->next_node);
    size_--;
  }

  /*
   * Удаляет элемент, следующий за pos.
   * Возвращает итератор на элемент, следующий за удалённым
   */
  Iterator EraseAfter(ConstIterator pos) noexcept {
    pos.node_->next_node = std ::move(pos.node_->next_node->next_node);
    size_--;
    return Iterator(pos.node_->next_node.get());
  }

public:
  void PushFront(const Type &value) {
    std::unique_ptr<Node> new_node = std::make_unique<Node>(value);
    new_node->next_node = std::move(head_.next_node);
    head_.next_node = std::move(new_node);
    size_++;
  }

  // Очищает список за время O(N)
  void Clear() noexcept {
    while (head_.next_node) {
      head_.next_node = std::move(head_.next_node->next_node);
    }
    // head_.next_node.release();
    size_ = 0;
  }
  // Возвращает количество элементов в списке за время O(1)
  [[nodiscard]] size_t GetSize() const noexcept { return size_; }

  // Сообщает, пустой ли список за время O(1)
  [[nodiscard]] bool IsEmpty() const noexcept { return size_ == 0; }
  SingleLinkedList() = default;
  SingleLinkedList(std::initializer_list<Type> values) {
    for (auto it = std::crbegin(values); it != std::crend(values); ++it) {
      PushFront(*it);
    }
    size_ = values.size();
  }

  SingleLinkedList(const SingleLinkedList &other) {
    auto *current = &head_;
    for (auto &value : other) {
      current->next_node = std::make_unique<Node>(value);
      current = current->next_node.get();
    }
    size_ = other.size_;
  }

  SingleLinkedList &operator=(const SingleLinkedList &rhs) {
    if (this == &rhs) {
      return *this;
    }
    Node new_list;
    auto *current = &new_list;
    for (auto &value : rhs) {
      current->next_node = std::make_unique<Node>(value);
      current = current->next_node.get();
    }
    std::swap(head_.next_node, new_list.next_node);
    size_ = rhs.size_;
    return *this;
  }

  // Обменивает содержимое списков за время O(1)
  void swap(SingleLinkedList &other) noexcept {
    std::swap(head_.next_node, other.head_.next_node);
    std::swap(size_, other.size_);
  }

private:
  // Фиктивный узел, используется для вставки "перед первым элементом"
  mutable Node head_ = {};
  size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type> &lhs, SingleLinkedList<Type> &rhs) noexcept {
  lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type> &lhs,
                const SingleLinkedList<Type> &rhs) {
  if (lhs.GetSize() != rhs.GetSize()) {
    return false;
  }
  return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type> &lhs,
                const SingleLinkedList<Type> &rhs) {

  return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type> &lhs,
               const SingleLinkedList<Type> &rhs) {
  return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                 rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type> &lhs,
                const SingleLinkedList<Type> &rhs) {
  return !(lhs > rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type> &lhs,
               const SingleLinkedList<Type> &rhs) {
  return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type> &lhs,
                const SingleLinkedList<Type> &rhs) {
  return !(rhs < lhs);
}
