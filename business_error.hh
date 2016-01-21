#ifndef _GSB_BUSINESS_ERROR_HH_
#define _GSB_BUSINESS_ERROR_HH_

#include <exception>

class business_error : std::exception {
  const char * _msg;
public:
  business_error(const char *s) : _msg(s) {}
  virtual const char * what() const noexcept { return _msg; }
};

#endif /* end of include guard: _GSB_BUSINESS_ERROR_HH_ */
