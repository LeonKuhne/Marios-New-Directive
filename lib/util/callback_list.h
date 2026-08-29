#pragma once

#include <functional>
#include <map>
#include <cstdlib>

class CallbackList
{
  typedef std::function<void()> Callback;
  std::map<int, Callback> callbacks = {};

public:
  int add(Callback callback)
  {
    int id = uniqueId();
    callbacks.emplace(id, callback);
    return id;
  }

  void remove(int id)
  {
    callbacks.erase(id);
  }

  void runAll()
  {
    for (auto &callback : callbacks)
      callback.second();
  }

private:
  int uniqueId()
  {
    int id = rand();
    while (callbacks.find(id) != callbacks.end())
      id = rand();
    return id;
  }
};