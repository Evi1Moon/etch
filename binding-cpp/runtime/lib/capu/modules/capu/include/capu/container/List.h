/* $Id$
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to you under the Apache License, Version
 * 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __LIST_H__
#define __LIST_H__
#include "capu/Error.h"
#include "capu/Config.h"
#include "capu/container/Comparator.h"

namespace capu {

  template <class T, class C = Comparator<T> >
  class List {
  private:

    class ListNode {
    public:

      ListNode() {
        mNext = NULL;
        mPrev = NULL;
      }

      ListNode(T data) {
        mNext = NULL;
        mPrev = NULL;
        this->mData = data;
      }

      T mData;
      ListNode * mNext;
      ListNode * mPrev;

    };

    class ListIterator {
    public:

      /**
       * Default Constructor
       */
      ListIterator();

      /**
       * constructor
       * @param initial_node where the iterator will be pointing
       */
      ListIterator(ListNode *initialNode);

      /**
       * destructor
       */
      ~ListIterator();

      /**
       * Check if iterator has next element.
       * @return false if the next of current node that is pointed, is null otherwise true
       */
      bool_t hasNext();

      /**
       * Get next iterator element.
       * @param element point to the next element
       * @return CAPU_ERANGE if the next of current node that is pointed, is null
       *         CAPU_EINVAL if the value is NULL
       *         CAPU_OK if the next element has been gotten
       *
       */
      status_t next(T* element);

    private:
      ListNode *mNextPosition;
    };

    ListNode *mHead;
    ListNode *mTail;
    int32_t mSize;

  public:

    typedef typename List<T, C>::ListIterator Iterator;

    /**
     * Default Constructor
     */
    List();

    /**
     * Destructor
     */
    virtual ~List();

    /**
     * Add element to the end of list
     * @param element element that will be added
     * @return CAPU_ENO_MEMORY if allocation of element is failed
     *         CAPU_OK if the element is successfully added
     */
    status_t add(const T element);

    /**
     * It will insert new element to specified position only the element at specified index with given element
     *
     * @param index index of element which will be inserted
     * @param element new value that will replace the old value
     *
     * @return CAPU_EINVAL if given index is invalid.
     *         CAPU_ENO_MEMORY if allocation of element is failed
     *         CAPU_OK if the element is successfully added
     *         CAPU_ERROR otherwise
     */
    status_t add(int32_t index, const T element);

    /**
     * remove the element in the specified index and if the element_old
     * parameter is not NULL, the removed element will be put to element_old
     * @param index index of element that will be removed
     * @param elementOld the buffer which will keep the copy of the removed element
     * @return CAPU_EINVAL invalid index
     *         CAPU_OK if the element is successfully removed
     */
    status_t removeAt(int32_t index, T* elementOld = NULL);

    /**
     * get a single element on specified index
     * @param index index of element that will be get
     * @param result the buffer that the retrieved element will be stored
     * @return CAPU_EINVAL invalid index
     *         CAPU_OK otherwise
     */
    status_t get(int32_t index, T* result);

    /**
     * return size of list
     * @return return the size of list
     */
    int32_t size();

    /**
     * check the list is empty or not
     * @return true if empty
     *         false otherwise
     */
    bool_t isEmpty();

    /**
     * returns an iterator pointing to the beginning of list
     * @return iterator
     */
    Iterator begin();

    /**
     * finds the index of given element in the link list
     * if you are using an object you need to overload == operator
     *
     * @param element the value that will be searched
     * @return -1 if the value either does not exist or given value is NULL
     *          otherwise index of value on linked list
     */
    int32_t find(const T element);

    /**
     *
     * @param index the index of element that will be replaced
     * @param element element that will be overwritten to existing place
     * @param elementOld the buffer to keep the existing
     * @return CAPU_EINVAL if the index is not valid
     *         CAPU_OK otherwise
     */
    status_t set(int32_t index, T element, T* elementOld = NULL);

    /**
     * check that if the list contains the given parameter or not
     * if you are using an object you need to overload == operator
     * @param element element that will be checked
     * @return true list contains it
     *         false otherwise
     */
    bool_t contains(const T element);

    /**
     * removes all elements from linked list
     *
     * @return CAPU_OK if all of the elements are deleted
     *
     */
    status_t clear();
  };

  template <class T, class C>
  List<T, C>::List()
  : mHead(NULL),
  mTail(NULL),
  mSize(0) {
    //init linked list
  }

  template <class T, class C>
  List<T, C>::~List() {
    clear();
  }

  template <class T, class C>
  status_t List<T, C>::clear() {
    //on the deallocation delete all elements in the list
    ListNode *cursor = mHead;
    ListNode *tmp = NULL;

    while (cursor != NULL) {
      tmp = cursor;
      cursor = cursor->mNext;
      delete tmp;
    }
    mSize = 0;
    mHead = NULL;
    mTail = NULL;
    return CAPU_OK;
  }

  template <class T, class C>
  int32_t List<T, C>::find(const T element) {
    int32_t counter = 0;
    ListNode * cursor = mHead;
    C comparator;
    while (cursor != NULL) {
      if (comparator(cursor->mData, element)) {
        return counter;
      }
      cursor = cursor->mNext;
      ++counter;
    }
    return -1;
  }

  template <class T, class C>
  bool_t List<T, C>::contains(const T element) {
    return (find(element) != -1);
  }

  //add elements to the end of list

  template <class T, class C>
  status_t List<T, C>::add(const T element) {
    ListNode *listElem = NULL;
    listElem = new ListNode(element);
    //NOT ALLOCATED
    if (listElem == NULL) {
      return CAPU_ENO_MEMORY;
    }
    //list is empty
    if (mHead == NULL) {
      mHead = listElem;
      mTail = mHead;
    } else {
      //list contains some elements so add it to the end of list
      mTail->mNext = listElem;
      listElem->mPrev = mTail;
      mTail = listElem;
      mTail->mNext = NULL;
    }
    ++mSize;
    return CAPU_OK;
  }

  template <class T, class C>
  status_t List<T, C>::add(int32_t index, const T element) {
    if ((index > mSize) || (index < 0)) {
      return CAPU_EINVAL;
    }

    ListNode *listElem = NULL;
    listElem = new ListNode(element);
    //NOT ALLOCATED
    if (listElem == NULL) {
      return CAPU_ENO_MEMORY;
    }

    int32_t counter = 0;
    ListNode * cursor = mHead;
    if (cursor == NULL) {
      //empty list
      mHead = listElem;
      mTail = listElem;
      mHead->mPrev = NULL;
      mTail->mNext = NULL;
      ++mSize;
      return CAPU_OK;
    } else if (index == mSize) {
      //list contains some elements so add it to the end of list
      mTail->mNext = listElem;
      listElem->mPrev = mTail;
      mTail = listElem;
      mTail->mNext = NULL;
      ++mSize;
      return CAPU_OK;
    }
    while (cursor != NULL) {
      if (index == counter) {

        if (cursor == mHead) // add to the front of list
        {
          listElem->mNext = mHead;
          mHead->mPrev = listElem;
          listElem->mPrev = NULL;
          mHead = listElem;
        } else {
          listElem->mNext = cursor;
          listElem->mPrev = cursor->mPrev;
          if (cursor->mPrev != NULL)
            cursor->mPrev->mNext = listElem;
          cursor->mPrev = listElem;
        }
        ++mSize;
        return CAPU_OK;
      }
      ++counter;
      cursor = cursor->mNext;
    }
    return CAPU_ERROR;
  }

  //remove the specific element indicated by the index in the list

  template <class T, class C>
  status_t List<T, C>::removeAt(int32_t index, T* elementOld) {
    if ((index < 0) || (index >= mSize)) {
      return CAPU_EINVAL;
    }
    ListNode * tmp = NULL;
    if (mHead == mTail) {
      tmp = mHead;
      mHead = NULL;
      mTail = NULL;
    } else if (index == 0) {

      tmp = mHead;
      mHead = mHead->mNext;
      mHead->mPrev = NULL;
    } else if (index == mSize - 1) {
      tmp = mTail;
      mTail = mTail->mPrev;
      mTail->mNext = NULL;
    } else {
      tmp = mHead;
      for (int32_t i = 0; i < index; ++i) {
        tmp = tmp->mNext;
      }
      tmp->mNext->mPrev = tmp->mPrev;
      tmp->mPrev->mNext = tmp->mNext;
    }
    if (elementOld != NULL)
      *elementOld = tmp->mData;
    delete tmp;
    mSize--;
    return CAPU_OK;
  }

  //get the specified element from list

  template <class T, class C>
  status_t List<T, C>::get(int32_t index, T* result) {
    if (((index < 0) || (index >= mSize)) || (result == NULL)) {
      return CAPU_EINVAL;
    }

    if (index == 0) {
      *result = mHead->mData;
      return CAPU_OK;
    } else if (index == mSize - 1) {
      *result = mTail->mData;
      return CAPU_OK;
    }

    ListNode *cursor = mHead;
    for (int32_t i = 0; i < index; ++i) {
      cursor = cursor->mNext;
    }
    *result = cursor->mData;
    return CAPU_OK;
  }

  template <class T, class C>
  status_t List<T, C>::set(int32_t index, T element, T* elementOld) {
    if ((index < 0) || (index >= mSize))
      return CAPU_EINVAL;

    ListNode *cursor = mHead;
    for (int32_t i = 0; i < index; ++i) {
      cursor = cursor->mNext;
    }
    if (elementOld != NULL)
      *elementOld = cursor->mData;
    cursor->mData = element;
    return CAPU_OK;
  }

  //Return element count of list

  template <class T, class C>
  int32_t List<T, C>::size() {
    return mSize;
  }

  //checks if the list is empty or not

  template <class T, class C>
  bool_t List<T, C>::isEmpty() {
    if (mSize == 0)
      return true;
    else
      return false;
  }

  template <class T, class C>
  typename List<T, C>::Iterator List<T, C>::begin() {
    return ListIterator(mHead);
  }

  template <class T, class C>
  List<T, C>::ListIterator::ListIterator()
  : mNextPosition(NULL) {

  }

  template <class T, class C>
  List<T, C>::ListIterator::~ListIterator() {

  }

  template <class T, class C>
  List<T, C>::ListIterator::ListIterator(ListNode *initialNode) {
    mNextPosition = initialNode;
  }

  template <class T, class C>
  bool_t List<T, C>::ListIterator::hasNext() {
    if (mNextPosition == NULL) {
      return false;
    }
    return true;
  }

  template <class T, class C>
  status_t List<T, C>::ListIterator::next(T* element) {
    if (mNextPosition == NULL) {
      return CAPU_ERANGE;
    } else if (element == NULL) {
      return CAPU_EINVAL;
    } else {
      *element = mNextPosition->mData;
      mNextPosition = mNextPosition->mNext;
      return CAPU_OK;
    }
  }
}

#endif /* DOUBLELINKEDLIST_H */
