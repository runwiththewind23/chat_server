/*
 * @Author: runwiththewind23 19945243142@163.com
 * @Date: 2024-08-09 11:31:12
 * @LastEditors: runwiththewind23 19945243142@163.com
 * @LastEditTime: 2024-08-09 11:31:42
 * @FilePath: /sxd/chat/src/bitset.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include <iostream>
#include <vector>
using namespace std;

//定义了位图数据结构
template <size_t N> class bitset {
public:
  bitset() {
    _bits.resize(N / 8 + 1, 0); //包括0
  }
  void set(size_t x) {
    size_t i = x / 8;
    size_t j = x % 8;
    _bits[i] |= (1 << j);
  }

  void reset(size_t x) {
    size_t i = x / 8;
    size_t j = x % 8;
    _bits[i] &= ~(1 << j);
  }

  bool isExists(size_t x) {
    size_t i = x / 8;
    size_t j = x % 8;
    return _bits[i] & (1 << j);
  }

private:
  vector<char> _bits;
};