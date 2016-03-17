#ifndef KERNEL_BTERM_HPP
#define KERNEL_BTERM_HPP

#include <functional>
#include <map>
#include <vector>
#include <net/inet4>

struct Command
{
  using main_func = std::function<int(const std::vector<std::string>&)>;
  
  Command(main_func func) : main(func) {}
  
  main_func main;
};

class Terminal
{
public:
  using Connection_ptr = std::shared_ptr<net::TCP::Connection>;
  enum
  {
    NUL  = 0,
    BELL = 7,
    BS   = 8,
    HTAB = 9,
    LF   = 10,
    VTAB = 11,
    FF   = 12,
    CR   = 13
  };
  
  using on_read_func  = std::function<void(std::string)>;
  using on_write_func = std::function<void(const char*, size_t)>;
  
  Terminal(Connection_ptr);
  
  void set_on_read(on_read_func callback)
  {
    on_read = callback;
  }
  
  template <typename... Args>
  void add_cmd(const std::string& command, Args&&... args)
  {
    commands.emplace(std::piecewise_construct,
                     std::forward_as_tuple(command),
                     std::forward_as_tuple(args...));
  }
  
  template <typename... Args>
  void write(const char* str, Args&&... args)
  {
    char buffer[1024];
    int bytes = snprintf(buffer, 1024, str, args...);
    
    on_write(buffer, bytes);
  }
  
private:
  void command(uint8_t cmd);
  void read(const char* buf, size_t len);
  void run(const std::string& cmd);
  
  on_read_func  on_read;
  on_write_func on_write;
  
  bool iac;
  std::string buffer;
  std::map<std::string, Command> commands;
};

#endif
