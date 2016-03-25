#ifndef MULTIVERSO_INCLUDE_DASHBOARD_H_
#define MULTIVERSO_INCLUDE_DASHBOARD_H_

#include <map>
#include <mutex>
#include <string>

#include "multiverso/util/timer.h"

namespace multiverso {

class Monitor;

// Dashboard to record and query system running information
// thread safe
class Dashboard {
public:
  static void AddMonitor(const std::string& name, Monitor* monitor);
  static std::string Watch(const std::string& name);
  static void Display();
private:
  static std::map<std::string, Monitor*> record_;
  static std::mutex m_;
};

class Monitor {
public:
  explicit Monitor(const std::string& name) {
    name_ = name;
    timer_.Start();
    Dashboard::AddMonitor(name_, this);
  }

  void Begin() { timer_.Start(); }

  void End() {
    elapse_ += timer_.elapse();
    ++count_;
  }

  double average() const { return elapse_ / count_; };

  std::string name() const { return name_; }
  double elapse() const { return elapse_; }
  int count() const { return count_; }

  std::string info_string() const;

private:
  // name of the Monitor
  std::string name_;
  // total elapsed time 
  double elapse_;
  // count of monitor
  int count_;
  // a timer util
  Timer timer_;
};

// NOTE(feiga): user shouldn't call this, used in MONITOR_BEGIN as a local var
//              note that static variable under different context may differs
//              please use either only in global scope or only in local scope
#define REGISTER_MONITOR(name)           \
  static Monitor g_##name##_monitor(#name);

// Guard with MONITOR macro in the code to monitor it's excuation
// Usage:
// MONITOR_BEGIN(your_code_short_description)
// your code
// MONITOR_END(your_code_short_description)
#define MONITOR_BEGIN(name)              \
  REGISTER_MONITOR(name)                 \
  g_##name##_monitor.Begin();

#define MONITOR_END(name)                \
  g_##name##_monitor.End();


}

#endif // MULTIVERSO_INCLUDE_DASHBOARD_H_