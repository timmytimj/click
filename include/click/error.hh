// -*- c-basic-offset: 2; related-file-name: "../../lib/error.cc" -*-
#ifndef CLICK_ERROR_HH
#define CLICK_ERROR_HH
#include <click/string.hh>
#if defined(CLICK_USERLEVEL) || defined(CLICK_TOOL)
# include <stdio.h>
#endif
#include <stdarg.h>

class ErrorHandler { public:
  
  enum Seriousness {
    ERR_DEBUG, ERR_MESSAGE, ERR_WARNING, ERR_ERROR, ERR_FATAL
  };
  
  ErrorHandler()			{ }
  virtual ~ErrorHandler()		{ }
  
  static void static_initialize(ErrorHandler *);
  static void static_cleanup();

  static ErrorHandler *default_handler();
  static ErrorHandler *silent_handler();
 
  static bool has_default_handler(); 
  static void set_default_handler(ErrorHandler *);
  
  virtual int nwarnings() const = 0;
  virtual int nerrors() const = 0;
  virtual void reset_counts() = 0;

  // seriousness < ERR_WARNING returns OK_RESULT, which is 0
  // seriousness >= ERR_WARNING returns ERROR_RESULT, which is -EINVAL
  static const int OK_RESULT = 0;
  static const int ERROR_RESULT;

  void debug(const char *format, ...);
  void message(const char *format, ...);
  int warning(const char *format, ...);
  int error(const char *format, ...);
  int fatal(const char *format, ...);

  void ldebug(const String &landmark, const char *format, ...);
  void lmessage(const String &landmark, const char *format, ...);
  int lwarning(const String &landmark, const char *format, ...);
  int lerror(const String &landmark, const char *format, ...);
  int lfatal(const String &landmark, const char *format, ...);

  int verror(Seriousness, const String &landmark, const char *format, va_list);
  int verror_text(Seriousness, const String &landmark, const String &text);
  
  String make_text(Seriousness, const char *, ...);
  virtual String make_text(Seriousness, const char *, va_list);
  virtual String decorate_text(Seriousness, const String &, const String &, const String &);
  virtual void handle_text(Seriousness, const String &) = 0;

  virtual void set_error_code(int);

  static String prepend_lines(const String &, const String &);
  
};

#if defined(CLICK_USERLEVEL) || defined(CLICK_TOOL)
class FileErrorHandler : public ErrorHandler { public:
  
  FileErrorHandler(FILE *, const String & = String());

  int nwarnings() const;
  int nerrors() const;
  void reset_counts();
  
  void handle_text(Seriousness, const String &);
  
 private:
  
  FILE *_f;
  String _context;
  int _nwarnings;
  int _nerrors;
  
};
#endif

class ErrorVeneer : public ErrorHandler { public:

  ErrorVeneer(ErrorHandler *errh)	: _errh(errh) { }

  int nwarnings() const;
  int nerrors() const;
  void reset_counts();

  String make_text(Seriousness, const char *, va_list);
  String decorate_text(Seriousness, const String &, const String &, const String &);
  void handle_text(Seriousness, const String &);

 protected:

  ErrorHandler *_errh;
 
};

class ContextErrorHandler : public ErrorVeneer { public:

  ContextErrorHandler(ErrorHandler *, const String &context,
		      const String &indent = "  ");
  
  String decorate_text(Seriousness, const String &, const String &, const String &);
  
 private:
  
  String _context;
  String _indent;
  
};

class PrefixErrorHandler : public ErrorVeneer { public:

  PrefixErrorHandler(ErrorHandler *, const String &prefix);
  
  String decorate_text(Seriousness, const String &, const String &, const String &);
  
 private:
  
  String _prefix;
  
};

class IndentErrorHandler : public ErrorVeneer { public:

  IndentErrorHandler(ErrorHandler *, const String &indent);
  
  String decorate_text(Seriousness, const String &, const String &, const String &);
  
 private:
  
  String _indent;
  
};

class LandmarkErrorHandler : public ErrorVeneer { public:

  LandmarkErrorHandler(ErrorHandler *, const String &);
  
  void set_landmark(const String &s)	{ _landmark = s; }
  
  String decorate_text(Seriousness, const String &, const String &, const String &);
  
 private:
  
  String _landmark;
  
};

#if defined(CLICK_USERLEVEL) || defined(CLICK_TOOL)
class BailErrorHandler : public ErrorVeneer { public:

  BailErrorHandler(ErrorHandler *, Seriousness = ERR_ERROR);

  void handle_text(Seriousness, const String &);

 private:

  int _exit_seriousness;

};
#endif

#endif
