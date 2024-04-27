#ifndef CS10123F_PA1_HASHTABLE
#define CS10123F_PA1_HASHTABLE

#include <cstdint>
#include <cstdio>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "hash_types.hpp"

template <>
struct std::hash<Website>
{
  static const uint32_t base = 1009u; // 1009 is a prime number.
  uint32_t operator()(const Website &S) const noexcept
  {
    // ------------------------START YOUR CODE----------------
    const std::string &str = S.name;
    uint32_t hashValue = 0;
    for (char ch : str)
    {
      hashValue = hashValue * base + ch;
    }
    return hashValue;
    // ------------------------END OF YOUR CODE----------------
  }
};

template <class Key = Website, class T = IPv4> // template for key type class
                                               // Key and value type class T
class HashTable
{
private:
  enum entrystate
  {
    Unused,
    Used,
    Erased
  };
  const size_t Mod = 1000037u; // 1000037 is a prime number
  std::vector<T> Table;        // main table stores value
  std::vector<Key> Stored_Key; // used for storing original key
  std::vector<entrystate>
      State;                    // keeps the state of every table entry : {Unused, Used or Erased};
  std::vector<size_t> Used_pos; // hint: used for function clear()

public:
  static const std::size_t npos = -1;
  HashTable() : Table(Mod), Stored_Key(Mod), State(Mod, entrystate::Unused) {}

  const T &operator[](const Key &x) const
  {
    std::size_t pos = find(x);
    if (pos == npos)
      throw std::invalid_argument("Key does not exist");
    return Table[pos];
  }

  // ------------------------START YOUR CODE----------------

  std::size_t Search(const Key &x) const
  {
    std::size_t pos = std::hash<Key>{}(x) % Mod;
    auto tmp = find(x);
    if (tmp != npos)
    {
      return tmp;
    }
    while (State[pos] == entrystate::Used )
    {
      pos = pos + 1;
    }
    return pos;
  }


  std::size_t find(const Key &x) const
  {
    std::size_t pos = std::hash<Key>{}(x) % Mod;
    while (State[pos] != entrystate::Unused)
    {
      if (State[pos] == entrystate::Used && Stored_Key[pos] == x)
      {
        return pos;
      }
      pos = pos + 1;
    }
    return npos;
  }

  void insert(const Key &x, const T &value)
  {
    std::size_t pos = Search(x);
    if (State[pos] == entrystate::Unused || State[pos] == entrystate::Erased)
    {
      Stored_Key[pos] = x;
      Table[pos] = value;
      State[pos] = entrystate::Used;
    }
    else if (State[pos] == entrystate::Used )
    {
      Table[pos] = value;
      return;
    }
    Used_pos.push_back(pos);
  } 

  bool erase(const Key &x)
  {
    std::size_t pos = find(x);
    if (pos != npos)
    { 
      State[pos] = entrystate::Erased;
      return true;
    }
    return false;
  }

  void clear()
  {
    for (std::size_t i = 0; i < this->Used_pos.size(); i++)
    {
      State[Used_pos[i]] = entrystate::Unused;
    }
    Used_pos.clear();
  }
};

#endif // CS10123F_PA1_HASHTABLE





  // std::size_t find(const Key &x) const
  // {
  //   for (std::size_t i = 0; i < Mod; ++i)
  //   {
  //     std::size_t pos = (std::hash<Key>{}(x) % Mod + i) % Mod;
  //     if (State[pos] == entrystate::Unused)
  //     {
  //       return npos;
  //     }
  //     else if (State[pos] == entrystate::Used && Stored_Key[pos] == x)
  //     {
  //       return pos;
  //     }
  //   }
  //   return npos;
  // }