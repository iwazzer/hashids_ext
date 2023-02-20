#ifndef HASHIDS_EXT_H
#define HASHIDS_EXT_H 1

#include "ruby.h"

VALUE _hashids_initialize(int argc, VALUE *argv, VALUE self);
VALUE _hashids_encode(int argc, VALUE *argv, VALUE self);
VALUE _hashids_decode(int argc, VALUE *argv, VALUE self);

#endif /* HASHIDS_EXT_H */
