
#include <node.h>
#include <nan.h>
#include <sophia.h>
#include "sophist.h"
#include "iterator.h"
#include "iterator_next.h"
#include "iterator_end.h"

using namespace v8;

namespace sophist {

  static Persistent<FunctionTemplate> iterator_constructor;

  Iterator::Iterator() {}
  Iterator::~Iterator() {}

  void Iterator::Init () {
    Local<FunctionTemplate> tpl =
      FunctionTemplate::New(Iterator::New);
    NanAssignPersistent(FunctionTemplate, iterator_constructor, tpl);
    tpl->SetClassName(NanSymbol("Iterator"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    NODE_SET_PROTOTYPE_METHOD(tpl, "next", Iterator::Next);
    NODE_SET_PROTOTYPE_METHOD(tpl, "end", Iterator::End);
  }

  NAN_METHOD(Iterator::New) {
    NanScope();
    void *cursor = NULL;
    Iterator *iterator = new Iterator();
    Sophist *sp = ObjectWrap::Unwrap<Sophist>(args[0]->ToObject());
    if (NULL == sp->db) {
      return NanThrowError("Unable to create an iterator on an unopen database!");
    }

    Local<Object> options = v8::Local<v8::Object>::Cast(args[1]);
    // TODO moar
    bool _reverse = NanBooleanOptionValue(options
      , NanSymbol("reverse")
      , false);

    iterator->Wrap(args.This());
    iterator->reverse = _reverse;
    iterator->wrapper = sp;
    cursor = _reverse
      ? sp_cursor(sp->db, SPLT, NULL, 0)
      : sp_cursor(sp->db, SPGT, NULL, 0);

    if (NULL == cursor) {
      return NanThrowError(sp_error(sp->db));
    }

    iterator->cursor = cursor;
    NanReturnValue(args.This());
  }

  NAN_METHOD(Iterator::Next) {
    NanScope();
    Iterator *iterator = ObjectWrap::Unwrap<Iterator>(args.This());
    Local<Function> cb = args[0].As<Function>();
    sophist::IteratorNext(iterator, new NanCallback(cb));
    NanReturnUndefined();
  }

  NAN_METHOD(Iterator::End) {
    NanScope();
    Iterator *iterator = ObjectWrap::Unwrap<Iterator>(args.This());
    Local<Function> cb = args[0].As<Function>();
    sophist::IteratorEnd(iterator, new NanCallback(cb));
    NanReturnUndefined();
  }

  Local<Object>
  Iterator::NewInstance (Local<Object> sp, Local<Object> options) {
    NanScope();
    Local<Object> instance;
    Local<FunctionTemplate> constructorHandle =
      NanPersistentToLocal(iterator_constructor);
    Handle<Value> argv[2] = {
        sp
      , options
    };
    instance = constructorHandle->GetFunction()->NewInstance(2, argv);
    return instance;
  }
}