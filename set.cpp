#include "set.h"
#include "node.h"

int Set::Node::count_nodes = 0;

/*****************************************************
 * Implementation of the member functions             *
 ******************************************************/

/*
 *  Default constructor :create an empty Set
 */
int Set::get_count_nodes() {
    return Set::Node::count_nodes;
}

/*
 *  Default constructor :create an empty Set
 *     // IMPLEMENT before Lab2 HA
 */
Set::Set() : head{ new Node{} }, tail{ new Node{} }, counter { 0 } {
    head->next = tail;
    tail->prev = head;
}


/*
 *  Conversion constructor: convert val into a singleton {val}
 *     // IMPLEMENT before Lab2 HA
 */
Set::Set(int val) : Set{} {  // create an empty list
    insert_node(head, val);
}

/*
 * Constructor to create a Set from a sorted vector of unique ints
 * \param list_of_values is an increasingly sorted vector of unique ints
 *     // IMPLEMENT before Lab2 HA
 */
Set::Set(const std::vector<int>& list_of_values) : Set{} {  // create an empty list
    for (int i : list_of_values) {
        Node* current = head;
        while (current->next != tail) {
            current = current->next;
        }
        insert_node(current, i);
    }
}

/*
 * Copy constructor: create a new Set as a copy of Set S
 * \param S Set to copied
 * Function does not modify Set S in any way
 *     // IMPLEMENT before Lab2 HA
 */
Set::Set(const Set& rhs) : Set{} {  // create an empty list
    if (rhs.head->next == rhs.tail) {
        return; // Nothing to copy
    }
    Node* current = head;
    Node* ptr_rhs = rhs.head->next;
    while (ptr_rhs != rhs.tail) {
        insert_node(current, ptr_rhs->value);
        ptr_rhs = ptr_rhs->next;
        current = current->next;
    }
}

/*
 * Transform the Set into an empty set
 * Remove all nodes from the list, except the dummy nodes
 *     // IMPLEMENT before Lab2 HA
 */
void Set::make_empty() {
    Node* current = head->next;
    while (current != tail) {
        remove_node(current);
        current = head->next;
    }
}

/*
 * Destructor: deallocate all memory (Nodes) allocated for the list
 *     // IMPLEMENT before Lab2 HA
 */
Set::~Set() {
    make_empty();
    delete head; // Tar bort objektet som head pekar på
    head = nullptr; // Slänger pointern i soptunnan. Kraschar annars för att man försöker peka på ett objekt som inte existerar.
    delete tail;
    tail = nullptr;
}

/*
 * Assignment operator: assign new contents to the *this Set, replacing its current content
 * \param S Set to be copied into Set *this
 * Use copy-and swap idiom -- TNG033: lecture 5
 */
Set& Set::operator=(Set rhs) {
    // IMPLEMENT before Lab2 HA
    std::swap(head, rhs.head);
    std::swap(counter, rhs.counter);
    std::swap(tail, rhs.tail);
    return *this;
}

/*
 * Test whether val belongs to the Set
 * Return true if val belongs to the set, otherwise false
 * This function does not modify the Set in any way
 */
bool Set::is_member(int val) const {
    // IMPLEMENT before Lab2 HA
    Node* current = head->next;
    while (current != tail) {
        if (val == current->value) {
            return true;
        }
        current = current->next;
    }
    return false;  // remove this line
}

/*
 * Three-way comparison operator: to test whether *this == S, *this < S, *this > S
 * Return std::partial_ordering::equivalent, if *this == S
 * Return std::partial_ordering::less, if *this < S
 * Return std::partial_ordering::greater, if *this > S
 * Return std::partial_ordering::unordered, otherwise
 * 
 * Requirement: must iterate through each set no more than once
 *     // IMPLEMENT before Lab2 HA
 */
std::partial_ordering Set::operator<=>(const Set& rhs) const {


    Node* rhs_current = rhs.head->next;
    Node* current = head->next;

    // Subset, *this < rhs
    if (counter < rhs.counter) {
        while (current != tail) {
            if (current->value == rhs_current->value) {
                current = current->next;
                rhs_current = rhs_current->next;
            }
            else if (current->value > rhs_current->value) {
                rhs_current = rhs_current->next;
            }
            else {
                return std::partial_ordering::unordered;
            }
        }
        return std::partial_ordering::less;
    }
    // Subset, rhs < *this
    else if (counter > rhs.counter) {
        while (rhs_current != rhs.tail) {
            if (rhs_current->value == current->value) {
                current = current->next;
                rhs_current = rhs_current->next;
            }
            else if (rhs_current->value > current->value) {
                current = current->next;
            }
            else {
                return std::partial_ordering::unordered;
            }
        }
        return std::partial_ordering::greater;
    }
    // Equivalent, rhs == *this
    else {
        while (current != tail) {
            if (current->value == rhs_current->value) {
                current = current->next;
                rhs_current = rhs_current->next;
            }
            else {
                return std::partial_ordering::unordered;
            }
        }
        return std::partial_ordering::equivalent;
    }
}

/*
 * Test whether Set *this and S represent the same set
 * Return true, if *this has same elemnts as set S
 * Return false, otherwise
 * 
 * Requirement: must iterate through each set no more than once
 */
bool Set::operator==(const Set& rhs) const {
    // IMPLEMENT before Lab2 HA
    if (counter != rhs.counter) {
        return false;
    }
    Node* current = head->next;
    Node* rhs_current = rhs.head->next;
    while (current != tail) {
        if (current->value == rhs_current->value) {
            current = current->next;
            rhs_current = rhs_current->next;
        }
        else {
            return false;
        }
    }
    return true;
}

/*
 * Modify Set *this such that it becomes the union of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator+=(const Set& rhs) {
    if (this == &rhs) return *this;
    // IMPLEMENT
    Set result{};
    Node* res_current = result.head;
    Node* current = head->next;
    Node* rhs_current = rhs.head->next;
    while (current != tail && rhs_current != rhs.tail) {
        if (current->value == rhs_current->value) {
            insert_node(res_current, current->value);
            current = current->next;
            rhs_current = rhs_current->next;
            res_current = res_current->next;
        }
        else if(current->value < rhs_current->value){
            insert_node(res_current, current->value);
            current = current->next;
            res_current = res_current->next;
        }
        else {
            insert_node(res_current, rhs_current->value);
            rhs_current = rhs_current->next;
            res_current = res_current->next;
        }
    }
    while (current != tail) {
        insert_node(res_current, current->value);
        current = current->next;
        res_current = res_current->next;
    }
    while (rhs_current != rhs.tail) {
        insert_node(res_current, rhs_current->value);
        rhs_current = rhs_current->next;
        res_current = res_current->next;
    }
    *this = result;
    return *this;
}

/*
 * Modify Set *this such that it becomes the intersection of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator*=(const Set& rhs) {
    // IMPLEMENT
    Set result{};
    Node* res_current = result.head;
    Node* current = head->next;
    Node* rhs_current = rhs.head->next;
    while (current != tail && rhs_current != rhs.tail) {
        if (current->value == rhs_current->value) {
            insert_node(res_current, current->value);
            current = current->next;
            rhs_current = rhs_current->next;
            res_current = res_current->next;
        }
        else if (current->value < rhs_current->value) {
            current = current->next;
        }
        else {
            rhs_current = rhs_current->next;
        }
    }

    *this = result;
    return *this;
}

/*
 * Modify Set *this such that it becomes the Set difference between Set *this and Set S
 * Set *this is modified and then returned
 */
Set& Set::operator-=(const Set& rhs) {
    // IMPLEMENT
    Set result{};
    Node* res_current = result.head;
    Node* current = head->next;
    Node* rhs_current = rhs.head->next;
    while (current != tail && rhs_current != rhs.tail) {
        if (current->value == rhs_current->value) {
            current = current->next;
            rhs_current = rhs_current->next;
        }
        else if (current->value < rhs_current->value) {
            insert_node(res_current, current->value);
            current = current->next;
            res_current = res_current->next;
        }
        else {
            rhs_current = rhs_current->next;
        }
    }
    while (current != tail) {
        insert_node(res_current, current->value);
        current = current->next;
        res_current = res_current->next;
    }
    *this = result;
    return *this;
}


/* ******************************************** *
 * Private Member Functions -- Implementation   *
 * ******************************************** */

/*
 * Insert a new Node storing val after the Node pointed by p
 * \param p pointer to a Node
 * \param val value to be inserted  after position p
 */
void Set::insert_node(Node* p, int val) {
    Node* newNode = new Node(val, p->next, p);
    p->next = newNode;
    newNode->next->prev = newNode;
    ++counter;
}

/*
 * Remove the Node pointed by p
 * \param p pointer to a Node
 */
void Set::remove_node(Node* p) {
    p->next->prev = p->prev;
    p->prev->next = p->next;
    delete p;
    --counter;
}

/*
 * Write Set *this to stream os
 */
void Set::write_to_stream(std::ostream& os) const {
    if (is_empty()) {
        os << "Set is empty!";
    } else {
        Set::Node* ptr{head->next};

        os << "{ ";
        while (ptr != tail) {
            os << ptr->value << " ";
            ptr = ptr->next;
        }
        os << "}";
    }
}

