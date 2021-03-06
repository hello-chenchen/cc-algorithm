/*********************************************************************
 * cclib
 *
 * Copyright (c) 2019 cclib contributors:
 *   - hello_chenchen <https://github.com/hello-chenchen>
 *
 * MIT License <https://github.com/cc-libraries/cclib/blob/master/LICENSE>
 * See https://github.com/cc-libraries/cclib for the latest update to this file
 *
 * author: hello_chenchen <https://github.com/hello-chenchen>
 **********************************************************************************/
#ifndef CCLIB_ADT_VECTOR_H
#define CCLIB_ADT_VECTOR_H

#include "./../../cclib-common/inc/base/common_define.h"

#if __cplusplus >= 201103L
#include <initializer_list>
#endif

namespace cclib {
    namespace adt {
        // template<typename T>
        // class vectorIterator {
        //     public:
        //         typedef vectorIterator<T> _Self;

        //     public:
        //         T operator*() {}
        //         _Self operator++() {}
        //         _Self operator++(int) {}

        //         _Self begin() {}
        //         _Self next() {}
        // };

        template<typename T>
        class Vector {
            public:
                typedef const T*    Iterator;

            public:
                explicit Vector(cc_size_t num = 0) : _size(0 == num ? 1 : num), _storage_count(0), _M_array(CC_NULL) {
                    _M_array = new T[_size]();
                 }

                Vector(const Vector<T>& instance) : _size(instance._size), _storage_count(instance._storage_count) {
                    operator=(instance);
                }

                Vector(Iterator begin, Iterator end) : _size(end - begin), _storage_count(_size) {
                    _M_array = new T[_size]();
                    for(int i = 0; i < _size; i++) {
                        _M_array[i] = *(begin + i);
                    }
                }

                //NOTICE: C++11 feature
                #if __cplusplus >= 201103L
                Vector(const std::initializer_list<T>& args) : _size(args.size()), _storage_count(_size) {
                    operator=(args);
                }

                Vector<T>& operator=(const std::initializer_list<T>& args) {
                    _storage_count = _size = args.size();
                    _M_array = new T[_size]();
                    for(int i = 0; i < _size; i++) {
                        _M_array[i] = *(args.begin() + i);
                    }

                    return *this;
                }
                #endif

                ~Vector() {
                    delete[] _M_array;
                    _M_array = CC_NULL;
                }

                Vector<T>& operator=(const Vector<T>& instance) {
                    _size = instance._size;
                    _storage_count = instance._storage_count;

                    //NOTICE: _M_array will be delete when stack pop or heap delete
                    _M_array = new T[_size]();
                    for(int i = 0; i < _storage_count; i++) {
                        _M_array[i] = instance._M_array[i];
                    }

                    return *this;
                }

                T& operator[] (cc_size_t index) {
                    return *(_M_array + index);
                }

                Iterator begin() {
                    return _M_array;
                }

                Iterator end() {
                    return _M_array + _storage_count;
                }

                bool clear() {
                    delete[] _M_array;
                    _M_array = CC_NULL;
                    _size = _storage_count = 0;

                    return true;
                }

                cc_size_t size() const {
                    return _storage_count;
                }

                bool empty() const {
                    return 0 == _storage_count ? true : false;
                }

                bool push_back(const T& data) {
                    if(_size == _storage_count) {
                        doubleExpansion();
                    }

                    _M_array[_storage_count++] = data;
                    return true;
                }

                bool push_back(const T* data) {
                    if(_size == _storage_count) {
                        doubleExpansion();
                    }

                    _M_array[_storage_count++] = *data;
                    return true;
                }

                bool pop_back() {
                    _M_array[--_storage_count] = T();
                    return true;
                }

                bool insert(Iterator position, const T& data) {
                    int pos = position - _M_array;
                    return insert(pos, data);
                }

                bool insert(int index, const T& data) {
                    if(index > _storage_count) return false;

                    if(_size == _storage_count) {
                        doubleExpansion();
                    }

                    T* temp = new T[_size]();
                    for(int i = 0; i < index; i++) {
                        temp[i] = _M_array[i];
                    }

                    temp[index] = data;

                    for(int i = index; i < _storage_count; i++) {
                        temp[i + 1] = _M_array[i];
                    }
                    ++_storage_count;

                    delete[] _M_array;
                    _M_array = temp;

                    return true;
                }

                bool earse(Iterator position) {
                    cc_size_t pos = position - _M_array;
                    return earse(pos);
                }

                bool earse(cc_size_t index) {
                    if(index > _storage_count) return false;

                    for(int i = index; i < _size - 1; i++) {
                        _M_array[i] = _M_array[i + 1];
                    }

                    _M_array[_storage_count - 1] = T();
                    --_storage_count;
                    return true;
                }

            private:
                void doubleExpansion() {
                    _size = 2 * _size + 1;
                    T* temp = new T[_size]();
                    for(int i = 0; i < _storage_count; i++) {    //NOTICE: memcpy is the same time complexity O(n)
                        temp[i] = _M_array[i];
                    }
                    delete[] _M_array;
                    _M_array = temp;
                }
                cc_size_t _size;
                cc_size_t _storage_count;
                T*  _M_array;
        };
    } // namespace adt
} // namespace cclib

#endif  //CCLIB_ADT_VECTOR_H