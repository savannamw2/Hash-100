/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Authors
 *    Savanna Whittaker, Isabel Weaver, Jenna Ray
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator
 
class TestList;        // forward declaration for unit tests
class TestHash;        // to be used later

namespace custom
{

/**************************************************
 * LIST
 * Just like std::list
 **************************************************/
template <typename T>
class list
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   friend void swap(list& lhs, list& rhs);
public:  
   // 
   // Construct
   //

   list();
   list(list <T> & rhs);
   list(list <T>&& rhs);
   list(size_t num, const T & t);
   list(size_t num);
   list(const std::initializer_list<T>& il);
   template <class Iterator>
   list(Iterator first, Iterator last);
  ~list() 
   {
   }

   // 
   // Assign
   //

   list <T> & operator = (list &  rhs);
   list <T> & operator = (list && rhs);
   list <T> & operator = (const std::initializer_list<T>& il);
   void swap(list <T>& rhs);

   //
   // Iterator
   //

   class  iterator;
   iterator begin() { return iterator(pHead); } //return the head of the list
   iterator rbegin() { return iterator(pTail); } //return the tail of the list
   iterator end() { return iterator(); }//test cases like it when it is nullptr so nullptr it is

   //
   // Access
   //

   T& front();
   T& back();

   //
   // Insert
   //

   void push_front(const T&  data);
   void push_front(      T&& data);
   void push_back (const T&  data);
   void push_back (      T&& data);
   iterator insert(iterator it, const T& data);
   iterator insert(iterator it, T&& data);

   //
   // Remove
   //

   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator& it);

   // 
   // Status
   //

    bool empty()  const { return numElements == 0; }
    size_t size() const { return numElements;   }


private:
   // nested linked list class
   class Node;

   // member variables
   size_t numElements; // though we could count, it is faster to keep a variable
   Node * pHead;    // pointer to the beginning of the list
   Node * pTail;    // pointer to the ending of the list
};

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <typename T>
class list <T> :: Node
{
public:
    // Construct
    Node() : data(0), pNext(nullptr), pPrev(nullptr) {}
    Node(const int& data) : data(data), pNext(nullptr), pPrev(nullptr) {}
    Node(int&& data) : data(std::move(data)), pNext(nullptr), pPrev(nullptr) {}
    
    // Data
    int data;
    Node* pNext;
    Node* pPrev;
};

/*************************************************
 * LIST ITERATOR
 * Iterate through a List, non-constant version
 ************************************************/
template <typename T>
class list <T> :: iterator
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   template <typename TT>
   friend class custom::list;
public:
    iterator(Node* p = nullptr) : p(p) {}
    iterator(const iterator& rhs) : p(rhs.p) {}
    
    iterator& operator=(const iterator& rhs)
    {
       p = rhs.p;
       return *this;
    }
    
    bool operator==(const iterator& rhs) const { return p == rhs.p; }
    bool operator!=(const iterator& rhs) const { return p != rhs.p; }
    
    int& operator*() { return p->data; }
    
    // Prefix increment
    iterator& operator++()
    {
       p = p->pNext;
       return *this;
    }
    
    // Postfix increment
    iterator operator++(int)
    {
       iterator tmp(*this);
       p = p->pNext;
       return tmp;
    }
    
    // Prefix decrement
    iterator& operator--()
    {
       p = p->pPrev;
       return *this;
    }
    
    // Postfix decrement
    iterator operator--(int)
    {
       iterator tmp(*this);
       p = p->pPrev;
       return tmp;
    }
    
 private:
    Node* p;
};

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num, const T & t) 
{
   //make sure its not an empty list
   if (num > 0)
   {
      numElements = num;
      pHead = pTail = new Node(t);
      // set pHead pNext to pTail
      if (num > 1)
         pHead->pNext = pTail;
      //add all the other nodes
      for (size_t i = 1; i < num; i++)
      {
         Node* pNew = new Node(t);
         pTail->pNext = pNew;
         pNew->pPrev = pTail;
         pTail = pNew;
      }
      //nodes before Head and after tail are nullptrs
      pTail->pNext = nullptr;
      pHead->pPrev = nullptr;
   }
   else // if empty then set numElements to 0 and head and tail to nullptr
   {
      numElements = 0;
      pHead = pTail = nullptr;
   }
}

/*****************************************
 * LIST :: ITERATOR constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
template <class Iterator>
list <T> ::list(Iterator first, Iterator last)
{
   //defaults
   numElements = 0;
   pHead = pTail = nullptr;
   // copy the list using the given iterators
   for (auto it = first; it != last; ++it)
   {
      push_back(*it);
   }
}

/*****************************************
 * LIST :: INITIALIZER constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
list <T> ::list(const std::initializer_list<T>& il)
{
   //defaults
   numElements = 0;
   pHead = pTail = nullptr;
   // copy the initializer list to this list
   for (auto it = il.begin(); it != il.end(); ++it)
   {
      push_back(*it);
   }
}

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num)
{
   // make sure its not an empty list
   if (num > 0)
   {
      numElements = num;
      pHead = pTail = new Node();
      // set pHead pNext to pTail
      if (num > 1)
         pHead->pNext = pTail;
      // add all the other nodes
      for (size_t i = 1; i < num; i++)
      {
         Node* pNew = new Node();
         pTail->pNext = pNew;
         pNew->pPrev = pTail;
         pTail = pNew;
      }
      // nodes before Head and after tail are nullptrs
      pTail->pNext = nullptr;
      pHead->pPrev = nullptr;
   }
   else // if empty then set numElements to 0 and head and tail to nullptr
   {
      numElements = 0;
      pHead = pTail = nullptr;
   }
}

/*****************************************
 * LIST :: DEFAULT constructors
 ****************************************/
template <typename T>
list <T> ::list() 
{
   //defaults to empty list
   numElements = 0;
   pHead = pTail = nullptr;
}

/*****************************************
 * LIST :: COPY constructors
 ****************************************/
template <typename T>
list <T> ::list(list& rhs) 
{
   // default
   numElements = 0;
   pHead = pTail = nullptr;
   // copy the rhs list node by node
   for (auto it = rhs.begin(); it != rhs.end(); ++it)
   {
      push_back(*it);

   }
}

/*****************************************
 * LIST :: MOVE constructors
 * Steal the values from the RHS
 ****************************************/
template <typename T>
list <T> ::list(list <T>&& rhs)
{
   // default
   numElements = 0;
   pHead = pTail = nullptr;
   // set all values to the rhs values
   numElements = rhs.numElements;
   pHead = rhs.pHead;
   pTail = rhs.pTail;
   // set the rhs to empty
   rhs.numElements = 0;
   rhs.pHead = nullptr;
   rhs.pTail = nullptr;
   // clear the rhs
   rhs.clear();

}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS 
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (list <T> && rhs)
{
   // check for self-assignment
   if (this != &rhs)
   {
      // clear the current list
      clear();
      // move the values from the rhs to this
      numElements = rhs.numElements;
      pHead = rhs.pHead;
      pTail = rhs.pTail;
      // set the rhs to empty
      rhs.numElements = 0;
      rhs.pHead = nullptr;
      rhs.pTail = nullptr;
   }
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T> & list <T> :: operator = (list <T> & rhs)
{
   // check for self-assignment
   if (this != &rhs)
   {
      *this = list(rhs); // use the copy constructor 
   }
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (const std::initializer_list<T>& rhs)
{
   //use the initializer list constructor
   *this = list(rhs);
   return *this;
}

/**********************************************
 * LIST :: CLEAR
 * Remove all the items currently in the linked list
 *     INPUT  :
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
void list <T> :: clear()
{
   // make a temporary pointer to the head
   Node* pTemp = pHead;
   // loop to remove all the nodes
   while (pTemp != nullptr)
   {
      Node* pNext = pTemp->pNext;
      delete pTemp;
      pTemp = pNext;
   }
   // reset the head and tail
   pHead = pTail = nullptr;
   numElements = 0;
}

/*********************************************
 * LIST :: PUSH BACK
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> :: push_back(const T & data)
{
   // create a new node
   Node* pNew = new Node(data);
   // if the list is empty, set the head and tail to the new node
   if (pHead == nullptr)
   {
      pHead = pTail = pNew;
   }
   else
   {
      // otherwise, add the new node to the end of the list
      pTail->pNext = pNew;
      pNew->pPrev = pTail;
      pTail = pNew;
      pTail->pNext = nullptr;
   }
   // increment the number of elements
   numElements++;
}

template <typename T>
void list <T> ::push_back(T && data)
{
   // create a new node
   Node* pNew = new Node(std::move(data));
   // if the list is empty, set the head and tail to the new node
   if (pHead == nullptr)
   {
      pHead = pTail = pNew;
   }
   else
   {
      // otherwise, add the new node to the end of the list
      pTail->pNext = pNew;
      pNew->pPrev = pTail;
      pTail = pNew;
      pTail->pNext = nullptr;
   }
   // increment the number of elements
   numElements++;
}

/*********************************************
 * LIST :: PUSH FRONT
 * add an item to the head of the list
 *     INPUT  : data to be added to the list
 *     OUTPUT :
 *     COST   : O(1)
 *********************************************/
template <typename T>
void list <T> :: push_front(const T & data)
{
   //create a new node
   Node* pNew = new Node(data);

   pNew->pNext = pHead;
   pNew->pPrev = nullptr;

   if (pHead != nullptr)
   { //The old head must be updated
      pHead->pPrev = pNew;
   }
   else
   { //if the list was empty then the tail is also the head
      pTail = pNew;
   }
   pHead = pNew; //set the new head
   numElements++; //update the number of elements
}

template <typename T>
void list <T> ::push_front(T && data)
{
   //create a new node
   Node* pNew = new Node(std::move(data));

   pNew->pNext = pHead;
   pNew->pPrev = nullptr;
   if (pHead != nullptr)
   { //The old head must be updated
      pHead->pPrev = pNew;
   }
   else
   { //if the list was empty then the tail is also the head
      pTail = pNew;
   }

   pHead = pNew; //set the new head
   numElements++; //update the number of elements
}


/*********************************************
 * LIST :: POP BACK
 * remove an item from the end of the list
 *    INPUT  : 
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_back()
{
   //if the list is empty, do nothing
   if (pTail == nullptr)
      return;

   //if there is only one element, delete it
   if (pHead == pTail)
   {
      delete pHead;
      pHead = pTail = nullptr;
   }
   else
   { //otherwise, remove the last element and set new tail
      Node* pTemp = pTail;
      pTail = pTail->pPrev;
      delete pTemp;
      pTail->pNext = nullptr;
   }
   //decrement the number of elements
   numElements--;
}

/*********************************************
 * LIST :: POP FRONT
 * remove an item from the front of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_front()
{
   //if the list is empty, do nothing
   if (pHead == nullptr)
      return;

   //if there is only one element, delete it
   if (pHead == pTail)
   {
      delete pHead;
      pHead = pTail = nullptr;
   }
   else
   { //otherwise, remove the first element and set new head
      Node* pTemp = pHead;
      pHead = pHead->pNext;
      delete pTemp;
      pHead->pPrev = nullptr;
   }
   //decrement the number of elements
   numElements--;
}

/*********************************************
 * LIST :: FRONT
 * retrieves the first element in the list
 *     INPUT  :
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list<T>::front()
{
   //if the list is empty, return a dummy value
   if (pHead == nullptr)
      return *(new int(99));
   //else return the first element
   return pHead->data;
}

/*********************************************
 * LIST :: BACK
 * retrieves the last element in the list
 *     INPUT  :
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list<T>::back()
{
   //if the list is empty, return a dummy value
   if (pTail == nullptr)
      return *(new int(99));
   //else return the last element
   return pTail->data;
}

/******************************************
 * LIST :: REMOVE
 * remove an item from the middle of the list
 *     INPUT  : an iterator to the item being removed
 *     OUTPUT : iterator to the new location 
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator  list <T> :: erase(const list <T> :: iterator & it)
{
   Node* pDelete = it.p;
   //if the list is empty, do nothing
   if (pDelete == nullptr)
      return end();

   iterator itReturn(pDelete->pNext); //Iterator to return after deletion

   if (pDelete->pPrev)
      pDelete->pPrev->pNext = pDelete->pNext;
   else
      pHead = pDelete->pNext; //If removing the head

   if (pDelete->pNext)
      pDelete->pNext->pPrev = pDelete->pPrev;
   else
      pTail = pDelete->pPrev; //If removing the tail

   delete pDelete; //delete the node
   numElements--; //decrement the number of elements

   return itReturn; //return the iterator to the new location
}

/******************************************
 * LIST :: INSERT
 * add an item to the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
                                                 const T & data) 
{
   //create a new node for it
   Node* pNext = it.p;

   //If inserteing at the end (it == end()), just use push_back
   if (!pNext)
   {
      push_back(data);
      return iterator(pTail);
   }

   //create new node for the data
   Node* pNew = new Node(data);

   //set pointers
   pNew->pNext = pNext; //new node will equal the next data
   pNew->pPrev = pNext->pPrev; //The new nodes pPrev needs to be this nodes pPrev

   if (pNext->pPrev)
      pNext->pPrev->pNext = pNew; //If not the head, set the previous nodes pNext to the new node
   else
      pHead = pNew; //If inserting at the head, set the head to the new node

   pNext->pPrev = pNew; //set the next nodes pPrev to the new node
   numElements++; //increment the number of elements
   return iterator(pNew); //return the new node
}

template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
   T && data)
{
   //Create a new Node for it.
   Node* pNext = it.p;

   // If inserting at the end (it == end()), just use push_back
   if (!pNext)
   {
      push_back(std::move(data));
      return iterator(pTail);
   }

   // Create new node for the data
   Node* pNew = new Node(std::move(data));

   // Set pointers
   pNew->pNext = pNext; //new node will equal the next data
   pNew->pPrev = pNext->pPrev; //The new nodes pPrev needs to be this pPrev.

   if (pNext->pPrev)
      pNext->pPrev->pNext = pNew;
   else
      pHead = pNew; // New node becomes head

   pNext->pPrev = pNew;

   numElements++;
   return iterator(pNew);
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T>
void swap(list <T> & lhs, list <T> & rhs)
{
   // check for self-assignment
   if (&lhs != &rhs)
   {
      // swap the values of the two lists
      std::swap(lhs.numElements, rhs.numElements);
      std::swap(lhs.pHead, rhs.pHead);
      std::swap(lhs.pTail, rhs.pTail);
   }
}

template <typename T>
void list<T>::swap(list <T>& rhs)
{
   // check for self-assignment
   if (this != &rhs)
   {
      // swap the values of the two lists
      std::swap(numElements, rhs.numElements);
      std::swap(pHead, rhs.pHead);
      std::swap(pTail, rhs.pTail);
   }
}

//#endif
}; // namespace custom
