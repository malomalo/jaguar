#include "ruby.h"

#include "http_parser.h"

struct http_parser_settings callbacks;

void JaguarHttpParser_free(void *data) {
  if(data) {
    xfree(data);
  }
}

void JaguarHttpParser_mark(http_parser* hp) {

}

VALUE JaguarHttpParser_alloc(VALUE klass)
{
  http_parser *parser = ALLOC(http_parser);
  
  return Data_Wrap_Struct(klass, JaguarHttpParser_mark, JaguarHttpParser_free, parser);
}


VALUE JaguarHttpParser_init(VALUE self)
{
  http_parser *parser;
  Data_Get_Struct(self, http_parser, parser);
  
  http_parser_init(parser, HTTP_REQUEST);
  parser->data = &self;

  return self;
}

int HttpParser_on_message_begin(http_parser* parser) {
  VALUE cKernel = rb_const_get(rb_cObject, rb_intern("Kernel"));
  rb_funcall(cKernel, rb_intern("puts"), 1, rb_str_new_cstr("Message Begin"));
  return 0;
}

int HttpParser_on_url(http_parser* parser, const char *at, size_t length) {
  VALUE cKernel = rb_const_get(rb_cObject, rb_intern("Kernel"));
  rb_funcall(cKernel, rb_intern("print"), 1, rb_str_new_cstr("URL: "));
  rb_funcall(cKernel, rb_intern("puts"), 1, rb_str_new(at, length));
  return 0;
}

int HttpParser_on_status(http_parser* parser, const char *at, size_t length) {
  VALUE cKernel = rb_const_get(rb_cObject, rb_intern("Kernel"));
  rb_funcall(cKernel, rb_intern("print"), 1, rb_str_new_cstr("Status: "));
  rb_funcall(cKernel, rb_intern("puts"), 1, rb_str_new(at, length));
  return 0;
}

int HttpParser_on_header_field(http_parser* parser, const char *at, size_t length) {
  VALUE cKernel = rb_const_get(rb_cObject, rb_intern("Kernel"));
  rb_funcall(cKernel, rb_intern("print"), 1, rb_str_new_cstr("Header Field: "));
  rb_funcall(cKernel, rb_intern("puts"), 1, rb_str_new(at, length));
  return 0;
}

int HttpParser_on_header_value(http_parser* parser, const char *at, size_t length) {
  VALUE cKernel = rb_const_get(rb_cObject, rb_intern("Kernel"));
  rb_funcall(cKernel, rb_intern("print"), 1, rb_str_new_cstr("Header Value: "));
  rb_funcall(cKernel, rb_intern("puts"), 1, rb_str_new(at, length));
  return 0;
}

int HttpParser_on_headers_complete(http_parser* parser) {
  VALUE cKernel = rb_const_get(rb_cObject, rb_intern("Kernel"));
  rb_funcall(cKernel, rb_intern("puts"), 1, rb_str_new_cstr("Headers Complete"));
  return 0;
}

int HttpParser_on_body(http_parser* parser, const char *at, size_t length) {
  VALUE cKernel = rb_const_get(rb_cObject, rb_intern("Kernel"));
  rb_funcall(cKernel, rb_intern("print"), 1, rb_str_new_cstr("Body: "));
  rb_funcall(cKernel, rb_intern("puts"), 1, rb_str_new(at, length));
  return 0;
}

int HttpParser_on_message_complete(http_parser* parser) {
  VALUE cKernel = rb_const_get(rb_cObject, rb_intern("Kernel"));
  rb_funcall(cKernel, rb_intern("puts"), 1, rb_str_new_cstr("Message Complete"));
  return 0;
}

int HttpParser_on_chunk_header(http_parser* parser) {
  VALUE cKernel = rb_const_get(rb_cObject, rb_intern("Kernel"));
  rb_funcall(cKernel, rb_intern("puts"), 1, rb_str_new_cstr("Chunk Header"));
  return 0;
}

int HttpParser_on_chunk_complete(http_parser* parser) {
  VALUE cKernel = rb_const_get(rb_cObject, rb_intern("Kernel"));
  rb_funcall(cKernel, rb_intern("puts"), 1, rb_str_new_cstr("Chunk Complete"));
  return 0;
}

VALUE JaguarHttpParser_parse(VALUE self, VALUE buffer) {
  http_parser *parser;
  Data_Get_Struct(self, http_parser, parser);
  

  size_t nparsed = http_parser_execute(parser, &callbacks, StringValuePtr(buffer), RSTRING_LEN(buffer));
  
  if (parser->upgrade) {
    VALUE cKernel = rb_const_get(rb_cObject, rb_intern("Kernel"));
    rb_funcall(cKernel, rb_intern("puts"), 1, rb_str_new_cstr("PROTOL UPGRADE"));
  } else if (nparsed != RSTRING_LEN(buffer)) {
    rb_raise(rb_eRuntimeError, "Error code %d %d", nparsed, RSTRING_LEN(buffer));
  }
  
  return self;
}


void Init_parser()
{
  VALUE mJaguar = rb_define_module("Jaguar");
  VALUE mJaguarHttp = rb_define_module_under(mJaguar, "HTTP");
  VALUE cJaguarHttpParser = rb_define_class_under(mJaguarHttp, "Parser", rb_cObject);

  callbacks.on_message_begin = HttpParser_on_message_begin;
  callbacks.on_url = HttpParser_on_url;
  callbacks.on_status = HttpParser_on_status;
  callbacks.on_header_field = HttpParser_on_header_field;
  callbacks.on_header_value = HttpParser_on_header_value;
  callbacks.on_headers_complete = HttpParser_on_headers_complete;
  callbacks.on_body = HttpParser_on_body;
  callbacks.on_message_complete = HttpParser_on_message_complete;
  callbacks.on_chunk_header = HttpParser_on_chunk_header;
  callbacks.on_chunk_complete = HttpParser_on_chunk_complete;

  // eHttpParserError = rb_define_class_under(mPuma, "HttpParserError", rb_eIOError);
  // rb_global_variable(&eHttpParserError);

  rb_define_alloc_func(cJaguarHttpParser, JaguarHttpParser_alloc);
  rb_define_method(cJaguarHttpParser, "initialize", JaguarHttpParser_init, 0);
  rb_define_method(cJaguarHttpParser, "<<", JaguarHttpParser_parse, 1);
}
